#include "stdafx.h"
#include "../Core/BountyManager.h"
#include "../Core/BountyOrder.h"
#include "../Core/BountyDatabase.h"
#include "../Core/BountyConfig.h"

// 測試用例基類
class CBountyTestCase
{
public:
    CBountyTestCase(const std::string& strName) : m_strTestName(strName), m_bResult(FALSE) {}
    virtual ~CBountyTestCase() {}
    
    virtual BOOL RunTest() = 0;
    const std::string& GetTestName() const { return m_strTestName; }
    BOOL GetResult() const { return m_bResult; }
    const std::string& GetErrorMessage() const { return m_strErrorMessage; }

protected:
    void SetResult(BOOL bResult, const std::string& strError = "")
    {
        m_bResult = bResult;
        m_strErrorMessage = strError;
    }

private:
    std::string m_strTestName;
    BOOL m_bResult;
    std::string m_strErrorMessage;
};

// 追殺令系統核心測試
class CBountyManagerTest : public CBountyTestCase
{
public:
    CBountyManagerTest() : CBountyTestCase("BountyManager Core Test") {}
    
    virtual BOOL RunTest() override
    {
        try
        {
            // 創建管理器
            CBountyManager* pManager = new CBountyManager(nullptr);
            if (!pManager)
            {
                SetResult(FALSE, "Failed to create BountyManager");
                return FALSE;
            }
            
            // 初始化測試
            if (!pManager->Initialize())
            {
                SetResult(FALSE, "Failed to initialize BountyManager");
                delete pManager;
                return FALSE;
            }
            
            // 創建追殺令測試
            BOOL bCreateResult = pManager->CreateBounty(
                12345,                      // 發布者ID
                67890,                      // 目標ID
                BOUNTY_TYPE_NORMAL,         // 類型
                BOUNTY_REWARD_MONEY,        // 獎勵類型
                100000,                     // 獎勵金額
                "Test bounty creation"      // 理由
            );
            
            if (!bCreateResult)
            {
                SetResult(FALSE, "Failed to create bounty");
                pManager->Shutdown();
                delete pManager;
                return FALSE;
            }
            
            // 獲取活躍追殺令測試
            std::vector<CBountyOrder*> vecBounties = pManager->GetActiveBounties();
            if (vecBounties.empty())
            {
                SetResult(FALSE, "No active bounties found after creation");
                pManager->Shutdown();
                delete pManager;
                return FALSE;
            }
            
            // 清理
            pManager->Shutdown();
            delete pManager;
            
            SetResult(TRUE);
            return TRUE;
        }
        catch (const std::exception& e)
        {
            SetResult(FALSE, std::string("Exception: ") + e.what());
            return FALSE;
        }
    }
};

// 追殺令對象測試
class CBountyOrderTest : public CBountyTestCase
{
public:
    CBountyOrderTest() : CBountyTestCase("BountyOrder Object Test") {}
    
    virtual BOOL RunTest() override
    {
        try
        {
            // 創建測試數據
            CBountyOrder::BountyData data;
            data.dwBountyID = 1001;
            data.dwPublisherID = 12345;
            data.dwTargetID = 67890;
            data.dwHunterID = 0;
            data.eBountyType = BOUNTY_TYPE_NORMAL;
            data.eStatus = BOUNTY_STATUS_ACTIVE;
            data.eRewardType = BOUNTY_REWARD_MONEY;
            data.llRewardAmount = 50000;
            data.strReason = "Test bounty";
            data.tCreateTime = _time64(NULL);
            data.tExpireTime = data.tCreateTime + 86400; // 24小時後過期
            data.strPublisherName = "TestPublisher";
            data.strTargetName = "TestTarget";
            
            // 創建追殺令對象
            CBountyOrder bounty(data);
            
            // 基本信息驗證
            if (bounty.GetBountyID() != 1001)
            {
                SetResult(FALSE, "BountyID mismatch");
                return FALSE;
            }
            
            if (bounty.GetRewardAmount() != 50000)
            {
                SetResult(FALSE, "RewardAmount mismatch");
                return FALSE;
            }
            
            if (!bounty.IsActive())
            {
                SetResult(FALSE, "Bounty should be active");
                return FALSE;
            }
            
            // 接受測試
            if (!bounty.Accept(11111, "TestHunter"))
            {
                SetResult(FALSE, "Failed to accept bounty");
                return FALSE;
            }
            
            if (!bounty.IsAccepted())
            {
                SetResult(FALSE, "Bounty should be accepted");
                return FALSE;
            }
            
            // 完成測試
            if (!bounty.Complete())
            {
                SetResult(FALSE, "Failed to complete bounty");
                return FALSE;
            }
            
            if (!bounty.IsCompleted())
            {
                SetResult(FALSE, "Bounty should be completed");
                return FALSE;
            }
            
            SetResult(TRUE);
            return TRUE;
        }
        catch (const std::exception& e)
        {
            SetResult(FALSE, std::string("Exception: ") + e.what());
            return FALSE;
        }
    }
};

// 配置管理測試
class CBountyConfigTest : public CBountyTestCase
{
public:
    CBountyConfigTest() : CBountyTestCase("BountyConfig Test") {}
    
    virtual BOOL RunTest() override
    {
        try
        {
            CBountyConfig config;
            
            // 使用默認配置初始化
            if (!config.Initialize(""))
            {
                SetResult(FALSE, "Failed to initialize config with defaults");
                return FALSE;
            }
            
            // 測試默認值
            const auto& generalConfig = config.GetGeneralConfig();
            if (generalConfig.nMaxActiveBounties <= 0)
            {
                SetResult(FALSE, "Invalid default MaxActiveBounties");
                return FALSE;
            }
            
            if (generalConfig.llMinRewardAmount <= 0)
            {
                SetResult(FALSE, "Invalid default MinRewardAmount");
                return FALSE;
            }
            
            // 測試驗證方法
            if (!config.ValidateRewardAmount(50000))
            {
                SetResult(FALSE, "ValidateRewardAmount failed for valid amount");
                return FALSE;
            }
            
            if (config.ValidateRewardAmount(-1))
            {
                SetResult(FALSE, "ValidateRewardAmount should fail for negative amount");
                return FALSE;
            }
            
            // 測試動態修改
            config.SetMaxActiveBounties(500);
            if (config.GetGeneralConfig().nMaxActiveBounties != 500)
            {
                SetResult(FALSE, "SetMaxActiveBounties failed");
                return FALSE;
            }
            
            config.Shutdown();
            
            SetResult(TRUE);
            return TRUE;
        }
        catch (const std::exception& e)
        {
            SetResult(FALSE, std::string("Exception: ") + e.what());
            return FALSE;
        }
    }
};

// 性能測試
class CBountyPerformanceTest : public CBountyTestCase
{
public:
    CBountyPerformanceTest() : CBountyTestCase("BountySystem Performance Test") {}
    
    virtual BOOL RunTest() override
    {
        try
        {
            const int TEST_COUNT = 1000;
            
            CBountyManager* pManager = new CBountyManager(nullptr);
            if (!pManager->Initialize())
            {
                SetResult(FALSE, "Failed to initialize manager for performance test");
                delete pManager;
                return FALSE;
            }
            
            // 記錄開始時間
            DWORD dwStartTime = GetTickCount();
            
            // 創建大量追殺令測試性能
            int nSuccessCount = 0;
            for (int i = 0; i < TEST_COUNT; ++i)
            {
                if (pManager->CreateBounty(
                    10000 + i,                // 發布者ID
                    20000 + i,                // 目標ID  
                    BOUNTY_TYPE_NORMAL,       
                    BOUNTY_REWARD_MONEY,      
                    10000 + (i * 100),        // 遞增獎勵
                    "Performance test bounty"
                ))
                {
                    nSuccessCount++;
                }
            }
            
            // 記錄結束時間
            DWORD dwEndTime = GetTickCount();
            DWORD dwElapsed = dwEndTime - dwStartTime;
            
            // 性能檢查：1000個追殺令創建應在10秒內完成
            if (dwElapsed > 10000)
            {
                SetResult(FALSE, sc::string::format("Performance test failed: %d ms for %d bounties", dwElapsed, TEST_COUNT));
                pManager->Shutdown();
                delete pManager;
                return FALSE;
            }
            
            // 成功率檢查：至少90%成功
            float fSuccessRate = static_cast<float>(nSuccessCount) / TEST_COUNT;
            if (fSuccessRate < 0.9f)
            {
                SetResult(FALSE, sc::string::format("Success rate too low: %.2f%%", fSuccessRate * 100));
                pManager->Shutdown();
                delete pManager;
                return FALSE;
            }
            
            // 內存使用檢查
            std::vector<CBountyOrder*> vecBounties = pManager->GetActiveBounties();
            if (vecBounties.size() != static_cast<size_t>(nSuccessCount))
            {
                SetResult(FALSE, "Active bounties count mismatch");
                pManager->Shutdown();
                delete pManager;
                return FALSE;
            }
            
            pManager->Shutdown();
            delete pManager;
            
            SetResult(TRUE);
            return TRUE;
        }
        catch (const std::exception& e)
        {
            SetResult(FALSE, std::string("Exception: ") + e.what());
            return FALSE;
        }
    }
};

// 測試運行器
class CBountySystemTestRunner
{
public:
    static BOOL RunAllTests()
    {
        std::vector<std::unique_ptr<CBountyTestCase>> vecTests;
        
        // 添加所有測試用例
        vecTests.push_back(std::make_unique<CBountyManagerTest>());
        vecTests.push_back(std::make_unique<CBountyOrderTest>());
        vecTests.push_back(std::make_unique<CBountyConfigTest>());
        vecTests.push_back(std::make_unique<CBountyPerformanceTest>());
        
        int nTotalTests = static_cast<int>(vecTests.size());
        int nPassedTests = 0;
        
        sc::writeLogInfo("=== 追殺令系統測試開始 ===");
        
        for (auto& pTest : vecTests)
        {
            sc::writeLogInfo(sc::string::format("運行測試: %s", pTest->GetTestName().c_str()));
            
            BOOL bResult = pTest->RunTest();
            if (bResult)
            {
                nPassedTests++;
                sc::writeLogInfo(sc::string::format("✓ %s - 通過", pTest->GetTestName().c_str()));
            }
            else
            {
                sc::writeLogError(sc::string::format("✗ %s - 失敗: %s", 
                    pTest->GetTestName().c_str(), 
                    pTest->GetErrorMessage().c_str()));
            }
        }
        
        sc::writeLogInfo(sc::string::format("=== 測試完成: %d/%d 通過 ===", nPassedTests, nTotalTests));
        
        return nPassedTests == nTotalTests;
    }
};