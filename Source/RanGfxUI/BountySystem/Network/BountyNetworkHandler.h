#pragma once

#include "../../../RanLogicClient/Network/s_NetClient.h"
#include "../Core/BountyOrder.h"

// 前向聲明
class CBountyManager;
struct NET_MSG_GENERIC;

// 網絡消息ID定義
enum BOUNTY_MESSAGE_TYPE
{
    NET_MSG_BOUNTY_CREATE_REQ = 0x8000,
    NET_MSG_BOUNTY_CREATE_ACK,
    NET_MSG_BOUNTY_ACCEPT_REQ,
    NET_MSG_BOUNTY_ACCEPT_ACK,
    NET_MSG_BOUNTY_COMPLETE_REQ,
    NET_MSG_BOUNTY_COMPLETE_ACK,
    NET_MSG_BOUNTY_CANCEL_REQ,
    NET_MSG_BOUNTY_CANCEL_ACK,
    NET_MSG_BOUNTY_LIST_REQ,
    NET_MSG_BOUNTY_LIST_ACK,
    NET_MSG_BOUNTY_NOTIFICATION,
    NET_MSG_BOUNTY_STATUS_UPDATE,
    NET_MSG_BOUNTY_RANKING_REQ,
    NET_MSG_BOUNTY_RANKING_ACK
};

// 網絡消息結構
#pragma pack(1)

struct NET_MSG_BOUNTY_CREATE_REQ : public NET_MSG_GENERIC
{
    DWORD dwTargetID;
    EBOUNTY_TYPE eBountyType;
    EBOUNTY_REWARD_TYPE eRewardType;
    LONGLONG llRewardAmount;
    char szReason[128];
    
    NET_MSG_BOUNTY_CREATE_REQ()
        : dwTargetID(0)
        , eBountyType(BOUNTY_TYPE_NORMAL)
        , eRewardType(BOUNTY_REWARD_MONEY)
        , llRewardAmount(0)
    {
        dwSize = sizeof(NET_MSG_BOUNTY_CREATE_REQ);
        nType = NET_MSG_BOUNTY_CREATE_REQ;
        memset(szReason, 0, sizeof(szReason));
    }
};

struct NET_MSG_BOUNTY_CREATE_ACK : public NET_MSG_GENERIC
{
    DWORD dwBountyID;
    DWORD dwResult; // 0: 成功, 其他: 錯誤碼
    
    NET_MSG_BOUNTY_CREATE_ACK()
        : dwBountyID(0)
        , dwResult(0)
    {
        dwSize = sizeof(NET_MSG_BOUNTY_CREATE_ACK);
        nType = NET_MSG_BOUNTY_CREATE_ACK;
    }
};

struct NET_MSG_BOUNTY_ACCEPT_REQ : public NET_MSG_GENERIC
{
    DWORD dwBountyID;
    
    NET_MSG_BOUNTY_ACCEPT_REQ()
        : dwBountyID(0)
    {
        dwSize = sizeof(NET_MSG_BOUNTY_ACCEPT_REQ);
        nType = NET_MSG_BOUNTY_ACCEPT_REQ;
    }
};

struct NET_MSG_BOUNTY_NOTIFICATION : public NET_MSG_GENERIC
{
    enum NOTIFICATION_TYPE
    {
        NOTIFY_BOUNTY_CREATED,
        NOTIFY_BOUNTY_ACCEPTED,
        NOTIFY_BOUNTY_COMPLETED,
        NOTIFY_BOUNTY_CANCELLED,
        NOTIFY_BOUNTY_EXPIRED
    };
    
    NOTIFICATION_TYPE eNotifyType;
    DWORD dwBountyID;
    DWORD dwPlayerID;
    char szPlayerName[32];
    char szMessage[256];
    
    NET_MSG_BOUNTY_NOTIFICATION()
        : eNotifyType(NOTIFY_BOUNTY_CREATED)
        , dwBountyID(0)
        , dwPlayerID(0)
    {
        dwSize = sizeof(NET_MSG_BOUNTY_NOTIFICATION);
        nType = NET_MSG_BOUNTY_NOTIFICATION;
        memset(szPlayerName, 0, sizeof(szPlayerName));
        memset(szMessage, 0, sizeof(szMessage));
    }
};

#pragma pack()

/**
 * 追殺令網絡處理器
 * 負責處理追殺令系統的網絡通信
 */
class CBountyNetworkHandler
{
public:
    CBountyNetworkHandler();
    virtual ~CBountyNetworkHandler();

public:
    // 初始化和清理
    BOOL Initialize();
    void Shutdown();
    
    // 設置管理器
    void SetBountyManager(CBountyManager* pManager) { m_pBountyManager = pManager; }
    
    // 客戶端請求發送
    BOOL SendCreateBountyRequest(DWORD dwTargetID, EBOUNTY_TYPE eType, EBOUNTY_REWARD_TYPE eRewardType, 
                                LONGLONG llAmount, const std::string& strReason);
    BOOL SendAcceptBountyRequest(DWORD dwBountyID);
    BOOL SendCompleteBountyRequest(DWORD dwBountyID);
    BOOL SendCancelBountyRequest(DWORD dwBountyID);
    BOOL SendBountyListRequest();
    BOOL SendRankingRequest();
    
    // 消息處理
    BOOL ProcessMessage(NET_MSG_GENERIC* pMsg);
    
    // 通知發送 (服務器端使用)
    void NotifyBountyCreated(DWORD dwBountyID);
    void NotifyBountyAccepted(DWORD dwBountyID, DWORD dwHunterID);
    void NotifyBountyCompleted(DWORD dwBountyID, DWORD dwHunterID);
    void NotifyBountyCancelled(DWORD dwBountyID);
    void NotifyBountyExpired(DWORD dwBountyID);
    
    // WebSocket支持 (實時通知)
    BOOL InitializeWebSocket();
    void ShutdownWebSocket();
    void SendRealtimeNotification(const std::string& strMessage);

protected:
    // 消息處理函數
    void OnCreateBountyAck(NET_MSG_BOUNTY_CREATE_ACK* pMsg);
    void OnAcceptBountyAck(NET_MSG_GENERIC* pMsg);
    void OnCompleteBountyAck(NET_MSG_GENERIC* pMsg);
    void OnCancelBountyAck(NET_MSG_GENERIC* pMsg);
    void OnBountyListAck(NET_MSG_GENERIC* pMsg);
    void OnBountyNotification(NET_MSG_BOUNTY_NOTIFICATION* pMsg);
    void OnRankingAck(NET_MSG_GENERIC* pMsg);
    
    // 錯誤處理
    void HandleNetworkError(DWORD dwErrorCode, const std::string& strContext);
    void ShowErrorMessage(DWORD dwErrorCode);
    
    // 工具方法
    std::string GetErrorMessage(DWORD dwErrorCode);
    BOOL ValidateMessage(NET_MSG_GENERIC* pMsg);

private:
    CBountyManager* m_pBountyManager;
    BOOL m_bInitialized;
    
    // 網絡統計
    DWORD m_dwMessagesSent;
    DWORD m_dwMessagesReceived;
    DWORD m_dwNetworkErrors;
    
    // WebSocket相關
    BOOL m_bWebSocketEnabled;
    void* m_pWebSocketConnection; // 實際類型待定
    
    // 消息隊列
    std::queue<std::shared_ptr<NET_MSG_GENERIC>> m_queueOutgoingMessages;
    std::queue<std::shared_ptr<NET_MSG_GENERIC>> m_queueIncomingMessages;
    
    // 同步對象
    std::mutex m_mutexMessageQueue;
    
public:
    // 統計信息獲取
    DWORD GetMessagesSent() const { return m_dwMessagesSent; }
    DWORD GetMessagesReceived() const { return m_dwMessagesReceived; }
    DWORD GetNetworkErrors() const { return m_dwNetworkErrors; }
};