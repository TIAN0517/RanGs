#pragma once

#include "../../../RanLogicServer/Database/DbDefine.h"
#include "../../../RanLogicServer/Database/DBAction/DbAction.h"
#include "BountyOrder.h"
#include <vector>
#include <memory>

class CBountyOrder;
struct BountySearchCriteria;
struct BountyStatistics;

namespace db
{
    class IDbManager;
}

/**
 * 追殺令數據庫操作類
 * 負責所有與追殺令相關的數據庫操作
 */
class CBountyDatabase
{
public:
    CBountyDatabase();
    virtual ~CBountyDatabase();

public:
    // 初始化和清理
    BOOL Initialize(db::IDbManager* pDbManager);
    void Shutdown();
    
    // 追殺令操作
    BOOL CreateBounty(const CBountyOrder::BountyData& data);
    BOOL UpdateBounty(const CBountyOrder::BountyData& data);
    BOOL DeleteBounty(DWORD dwBountyID);
    
    // 查詢操作
    CBountyOrder* GetBountyByID(DWORD dwBountyID);
    std::vector<CBountyOrder*> GetActiveBounties();
    std::vector<CBountyOrder*> GetBountiesByPublisher(DWORD dwPublisherID);
    std::vector<CBountyOrder*> GetBountiesByTarget(DWORD dwTargetID);
    std::vector<CBountyOrder*> GetBountiesByHunter(DWORD dwHunterID);
    std::vector<CBountyOrder*> SearchBounties(const BountySearchCriteria& criteria);
    
    // 統計操作
    BountyStatistics GetGlobalStatistics();
    BountyStatistics GetPlayerStatistics(DWORD dwPlayerID);
    std::vector<std::pair<DWORD, int>> GetHunterRanking(int nTopCount);
    std::vector<std::pair<DWORD, int>> GetTargetRanking(int nTopCount);
    std::vector<std::pair<DWORD, LONGLONG>> GetRewardRanking(int nTopCount);
    
    // 維護操作
    BOOL CleanupExpiredBounties();
    BOOL ArchiveOldBounties(__time64_t tCutoffTime);
    BOOL RecalculateStatistics();
    
    // 事務操作
    BOOL BeginTransaction();
    BOOL CommitTransaction();
    BOOL RollbackTransaction();

protected:
    // 內部輔助方法
    CBountyOrder* CreateBountyFromResultSet(const std::shared_ptr<db::DbAction>& pAction, int nRow);
    BOOL ValidateBountyData(const CBountyOrder::BountyData& data);
    std::string BuildSearchQuery(const BountySearchCriteria& criteria);
    
private:
    db::IDbManager* m_pDbManager;
    BOOL m_bInitialized;
    
    // 緩存
    std::map<DWORD, std::shared_ptr<CBountyOrder>> m_mapBountyCache;
    __time64_t m_tLastCacheUpdate;
    
    // 性能統計
    DWORD m_dwQueryCount;
    DWORD m_dwCacheHits;
    DWORD m_dwCacheMisses;
};

/**
 * 追殺令數據庫操作類 - 創建追殺令
 */
class CDbActionBountyCreate : public db::DbAction
{
public:
    CDbActionBountyCreate(const CBountyOrder::BountyData& data);
    virtual ~CDbActionBountyCreate();

public:
    virtual int Execute(NetServer* pServer) override;

private:
    CBountyOrder::BountyData m_Data;
};

/**
 * 追殺令數據庫操作類 - 更新追殺令
 */
class CDbActionBountyUpdate : public db::DbAction
{
public:
    CDbActionBountyUpdate(const CBountyOrder::BountyData& data);
    virtual ~CDbActionBountyUpdate();

public:
    virtual int Execute(NetServer* pServer) override;

private:
    CBountyOrder::BountyData m_Data;
};

/**
 * 追殺令數據庫操作類 - 查詢追殺令
 */
class CDbActionBountyQuery : public db::DbAction
{
public:
    enum QUERY_TYPE
    {
        QUERY_BY_ID,
        QUERY_ACTIVE,
        QUERY_BY_PUBLISHER,
        QUERY_BY_TARGET,
        QUERY_BY_HUNTER,
        QUERY_SEARCH
    };

public:
    CDbActionBountyQuery(QUERY_TYPE eType, DWORD dwParameter = 0);
    CDbActionBountyQuery(const BountySearchCriteria& criteria);
    virtual ~CDbActionBountyQuery();

public:
    virtual int Execute(NetServer* pServer) override;
    
    // 結果獲取
    const std::vector<CBountyOrder::BountyData>& GetResults() const { return m_vecResults; }

private:
    QUERY_TYPE m_eQueryType;
    DWORD m_dwParameter;
    BountySearchCriteria m_SearchCriteria;
    std::vector<CBountyOrder::BountyData> m_vecResults;
};

/**
 * 追殺令數據庫操作類 - 統計查詢
 */
class CDbActionBountyStatistics : public db::DbAction
{
public:
    enum STATS_TYPE
    {
        STATS_GLOBAL,
        STATS_PLAYER,
        STATS_HUNTER_RANKING,
        STATS_TARGET_RANKING,
        STATS_REWARD_RANKING
    };

public:
    CDbActionBountyStatistics(STATS_TYPE eType, DWORD dwPlayerID = 0, int nTopCount = 10);
    virtual ~CDbActionBountyStatistics();

public:
    virtual int Execute(NetServer* pServer) override;
    
    // 結果獲取
    const BountyStatistics& GetStatistics() const { return m_Statistics; }
    const std::vector<std::pair<DWORD, int>>& GetRanking() const { return m_vecRanking; }
    const std::vector<std::pair<DWORD, LONGLONG>>& GetRewardRanking() const { return m_vecRewardRanking; }

private:
    STATS_TYPE m_eStatsType;
    DWORD m_dwPlayerID;
    int m_nTopCount;
    
    BountyStatistics m_Statistics;
    std::vector<std::pair<DWORD, int>> m_vecRanking;
    std::vector<std::pair<DWORD, LONGLONG>> m_vecRewardRanking;
};

/**
 * 追殺令數據庫操作類 - 維護操作
 */
class CDbActionBountyMaintenance : public db::DbAction
{
public:
    enum MAINTENANCE_TYPE
    {
        MAINTENANCE_CLEANUP_EXPIRED,
        MAINTENANCE_ARCHIVE_OLD,
        MAINTENANCE_RECALC_STATS
    };

public:
    CDbActionBountyMaintenance(MAINTENANCE_TYPE eType, __time64_t tCutoffTime = 0);
    virtual ~CDbActionBountyMaintenance();

public:
    virtual int Execute(NetServer* pServer) override;

private:
    MAINTENANCE_TYPE m_eMaintenanceType;
    __time64_t m_tCutoffTime;
};