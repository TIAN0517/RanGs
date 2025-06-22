#pragma once

#include "../../../RanLogic/GLCharDefine.h"
#include "../../../RanLogic/GLTypeDefine.h"
#include <string>
#include <vector>

// 前向聲明
enum EBOUNTY_STATUS;
enum EBOUNTY_TYPE;
enum EBOUNTY_REWARD_TYPE;

/**
 * 追殺令實體類
 * 表示一個追殺令的完整信息
 */
class CBountyOrder
{
public:
    struct BountyData
    {
        DWORD dwBountyID;              // 追殺令ID
        DWORD dwPublisherID;           // 發布者ID
        DWORD dwTargetID;              // 目標ID
        DWORD dwHunterID;              // 接受者ID (0表示未接受)
        EBOUNTY_TYPE eBountyType;      // 追殺令類型
        EBOUNTY_STATUS eStatus;        // 狀態
        EBOUNTY_REWARD_TYPE eRewardType; // 獎勵類型
        LONGLONG llRewardAmount;       // 獎勵數量
        std::string strReason;         // 追殺理由
        __time64_t tCreateTime;        // 創建時間
        __time64_t tAcceptTime;        // 接受時間
        __time64_t tCompleteTime;      // 完成時間
        __time64_t tExpireTime;        // 過期時間
        std::string strPublisherName;  // 發布者名稱
        std::string strTargetName;     // 目標名稱
        std::string strHunterName;     // 獵人名稱
    };

public:
    CBountyOrder();
    CBountyOrder(const BountyData& data);
    virtual ~CBountyOrder();

public:
    // 基本信息獲取
    DWORD GetBountyID() const { return m_Data.dwBountyID; }
    DWORD GetPublisherID() const { return m_Data.dwPublisherID; }
    DWORD GetTargetID() const { return m_Data.dwTargetID; }
    DWORD GetHunterID() const { return m_Data.dwHunterID; }
    EBOUNTY_TYPE GetBountyType() const { return m_Data.eBountyType; }
    EBOUNTY_STATUS GetStatus() const { return m_Data.eStatus; }
    EBOUNTY_REWARD_TYPE GetRewardType() const { return m_Data.eRewardType; }
    LONGLONG GetRewardAmount() const { return m_Data.llRewardAmount; }
    const std::string& GetReason() const { return m_Data.strReason; }
    
    // 時間信息
    __time64_t GetCreateTime() const { return m_Data.tCreateTime; }
    __time64_t GetAcceptTime() const { return m_Data.tAcceptTime; }
    __time64_t GetCompleteTime() const { return m_Data.tCompleteTime; }
    __time64_t GetExpireTime() const { return m_Data.tExpireTime; }
    
    // 名稱信息
    const std::string& GetPublisherName() const { return m_Data.strPublisherName; }
    const std::string& GetTargetName() const { return m_Data.strTargetName; }
    const std::string& GetHunterName() const { return m_Data.strHunterName; }
    
    // 完整數據
    const BountyData& GetData() const { return m_Data; }
    void SetData(const BountyData& data) { m_Data = data; }

public:
    // 狀態操作
    BOOL Accept(DWORD dwHunterID, const std::string& strHunterName);
    BOOL Complete();
    BOOL Cancel();
    BOOL Expire();
    
    // 狀態檢查
    BOOL IsActive() const;
    BOOL IsAccepted() const;
    BOOL IsCompleted() const;
    BOOL IsCancelled() const;
    BOOL IsExpired() const;
    BOOL IsExpiring() const; // 即將過期
    
    // 時間計算
    int GetRemainingHours() const;
    int GetElapsedHours() const;
    BOOL ShouldExpire() const;
    
    // 驗證
    BOOL IsValidTarget(DWORD dwPlayerID) const;
    BOOL CanBeAcceptedBy(DWORD dwPlayerID) const;
    BOOL CanBeCancelledBy(DWORD dwPlayerID) const;
    
    // 格式化
    std::string GetStatusString() const;
    std::string GetTypeString() const;
    std::string GetRewardString() const;
    std::string GetTimeString() const;
    std::string GetFormattedDescription() const;
    
    // 序列化
    BOOL Serialize(std::vector<BYTE>& buffer) const;
    BOOL Deserialize(const std::vector<BYTE>& buffer);
    
    // 比較操作
    bool operator==(const CBountyOrder& other) const;
    bool operator<(const CBountyOrder& other) const;

protected:
    // 內部方法
    void UpdateStatus();
    BOOL ValidateData() const;
    void InitializeDefaults();

private:
    BountyData m_Data;
    
    // 緩存計算結果
    mutable int m_nCachedRemainingHours;
    mutable __time64_t m_tLastCalculationTime;
};

/**
 * 追殺令搜索條件
 */
struct BountySearchCriteria
{
    DWORD dwPublisherID;           // 發布者ID (0 = 任意)
    DWORD dwTargetID;              // 目標ID (0 = 任意)
    DWORD dwHunterID;              // 獵人ID (0 = 任意)
    EBOUNTY_TYPE eBountyType;      // 類型篩選
    EBOUNTY_STATUS eStatus;        // 狀態篩選
    LONGLONG llMinReward;          // 最小獎勵
    LONGLONG llMaxReward;          // 最大獎勵
    __time64_t tStartTime;         // 開始時間
    __time64_t tEndTime;           // 結束時間
    std::string strKeyword;        // 關鍵字搜索
    int nMaxResults;               // 最大結果數
    
    BountySearchCriteria()
        : dwPublisherID(0)
        , dwTargetID(0)
        , dwHunterID(0)
        , eBountyType(EBOUNTY_TYPE(0))
        , eStatus(EBOUNTY_STATUS(0))
        , llMinReward(0)
        , llMaxReward(LLONG_MAX)
        , tStartTime(0)
        , tEndTime(0)
        , nMaxResults(100)
    {
    }
};

/**
 * 追殺令統計信息
 */
struct BountyStatistics
{
    int nTotalBounties;            // 總追殺令數
    int nActiveBounties;           // 活躍追殺令數
    int nCompletedBounties;        // 完成追殺令數
    int nCancelledBounties;        // 取消追殺令數
    int nExpiredBounties;          // 過期追殺令數
    LONGLONG llTotalRewards;       // 總獎勵金額
    LONGLONG llPendingRewards;     // 待發放獎勵
    float fAverageCompletionTime;  // 平均完成時間(小時)
    float fSuccessRate;            // 成功率
    
    BountyStatistics()
        : nTotalBounties(0)
        , nActiveBounties(0)
        , nCompletedBounties(0)
        , nCancelledBounties(0)
        , nExpiredBounties(0)
        , llTotalRewards(0)
        , llPendingRewards(0)
        , fAverageCompletionTime(0.0f)
        , fSuccessRate(0.0f)
    {
    }
};