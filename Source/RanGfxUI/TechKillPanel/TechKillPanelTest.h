#pragma once

#include "../../enginelib/GUInterface/UIGroup.h"
#include "./TechKillPanelHook.h"

/**
 * TechKillPanel Test Suite
 * Validates the 4D High-Tech Kill Panel implementation
 * Tests performance, visual effects, and integration
 */
class CTechKillPanelTest
{
public:
    // Test result structure
    struct TEST_RESULT
    {
        BOOL bPassed;
        CString strTestName;
        CString strDescription;
        float fExecutionTime;
        CString strErrorMessage;
    };
    
    // Test categories
    enum TEST_CATEGORY
    {
        TEST_INITIALIZATION = 0,
        TEST_EQUIPMENT_DETECTION,
        TEST_PARTICLE_SYSTEM,
        TEST_PERFORMANCE_MONITOR,
        TEST_VISUAL_EFFECTS,
        TEST_INTEGRATION,
        TEST_MEMORY_MANAGEMENT,
        TEST_CATEGORY_COUNT
    };

public:
    CTechKillPanelTest();
    virtual ~CTechKillPanelTest();

public:
    // Main test interface
    BOOL RunAllTests(EngineDeviceMan* pEngineDevice);
    BOOL RunTestCategory(TEST_CATEGORY category, EngineDeviceMan* pEngineDevice);
    BOOL RunSingleTest(const CString& testName, EngineDeviceMan* pEngineDevice);
    
    // Test results
    const std::vector<TEST_RESULT>& GetTestResults() const { return m_vecTestResults; }
    int GetPassedTestCount() const;
    int GetFailedTestCount() const;
    float GetTotalExecutionTime() const;
    
    // Test configuration
    void SetVerboseOutput(BOOL bVerbose) { m_bVerboseOutput = bVerbose; }
    void SetPerformanceTestDuration(float fSeconds) { m_fPerformanceTestDuration = fSeconds; }

private:
    // Initialization tests
    BOOL TestSystemInitialization(EngineDeviceMan* pEngineDevice);
    BOOL TestComponentCreation(EngineDeviceMan* pEngineDevice);
    BOOL TestConfigurationLoading();
    
    // Equipment detection tests
    BOOL TestTechCardDetection();
    BOOL TestEquipmentCaching();
    BOOL TestEquipmentCallbacks();
    
    // Particle system tests
    BOOL TestParticleInitialization();
    BOOL TestParticlePerformance();
    BOOL TestQualityAdaptation();
    BOOL TestGPUInstancing();
    
    // Performance monitor tests
    BOOL TestFPSMonitoring();
    BOOL TestQualityRecommendation();
    BOOL TestPerformanceStability();
    
    // Visual effects tests
    BOOL TestNeonEffects();
    BOOL TestTextWaveMotion();
    BOOL TestGearRotation();
    BOOL TestMatrixCodeRain();
    BOOL TestEnergyShield();
    
    // Integration tests
    BOOL TestHookInstallation();
    BOOL TestKillEventProcessing();
    BOOL TestFallbackSystem();
    BOOL TestUIRendering();
    
    // Memory management tests
    BOOL TestMemoryLeaks();
    BOOL TestResourceCleanup();
    BOOL TestAssetBundleManagement();
    
    // Helper functions
    void AddTestResult(const CString& testName, BOOL bPassed, const CString& description, float executionTime, const CString& errorMessage = _T(""));
    void LogTestMessage(const CString& message);
    BOOL ValidatePerformanceTarget(float actualTime, float targetTime, float tolerance = 0.1f);
    void SimulateKillEvent(EKILL_TYPE killType, DWORD attackerID, DWORD targetID);

private:
    // Test state
    std::vector<TEST_RESULT> m_vecTestResults;
    CTechKillPanelHook* m_pTestHook;
    EngineDeviceMan* m_pEngineDevice;
    
    // Test configuration
    BOOL m_bVerboseOutput;
    float m_fPerformanceTestDuration;
    
    // Test data
    DWORD m_dwTestStartTime;
    DWORD m_dwCurrentTestStartTime;
};