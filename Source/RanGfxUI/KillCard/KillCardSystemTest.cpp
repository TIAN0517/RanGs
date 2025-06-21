// Kill Card System Test and Validation Script
// This file provides test functions to validate the Kill Card system functionality

#include "KillCardManager.h"
#include "KillCardAnimation.h"
#include "KillCardRenderer.h"
#include "KillCardEffects.h"
#include "KillCardUI.h"

// Test function to validate all kill card types
void TestAllKillCardTypes(CKillCardManager* pManager)
{
    if (!pManager)
        return;
        
    // Test each kill card type with a 2-second delay between each
    const EKILL_TYPE testTypes[6] = {
        KILL_TYPE_CRITICAL,     // Quantum Fission
        KILL_TYPE_SKILL,        // Hologram Disruption  
        KILL_TYPE_CONSECUTIVE,  // Space-Time Distortion
        KILL_TYPE_RANGED,       // Plasma Explosion
        KILL_TYPE_MELEE,        // Nano Dissolution
        KILL_TYPE_HEADSHOT      // Virtual Reconstruction
    };
    
    const char* testNames[6] = {
        "Quantum Fission (Critical)",
        "Hologram Disruption (Skill)",
        "Space-Time Distortion (Consecutive)", 
        "Plasma Explosion (Ranged)",
        "Nano Dissolution (Melee)",
        "Virtual Reconstruction (Headshot)"
    };
    
    for (int i = 0; i < 6; i++)
    {
        // Log test
        printf("Testing Kill Card: %s\n", testNames[i]);
        
        // Trigger the card
        pManager->TriggerKillCard(testTypes[i], 12345); // Test target ID
        
        // Wait for 3 seconds before next test
        Sleep(3000);
    }
}

// Test performance impact
void TestPerformanceImpact(CKillCardManager* pManager)
{
    if (!pManager)
        return;
        
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    
    // Measure time for 100 kill card triggers
    QueryPerformanceCounter(&start);
    
    for (int i = 0; i < 100; i++)
    {
        EKILL_TYPE randomType = (EKILL_TYPE)(1 << (i % 6));
        pManager->TriggerKillCard(randomType);
    }
    
    QueryPerformanceCounter(&end);
    
    double elapsedTime = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    printf("Performance Test: 100 kill cards triggered in %.4f seconds\n", elapsedTime);
    printf("Average time per kill card: %.6f seconds\n", elapsedTime / 100.0);
}

// Test combination triggers
void TestCombinationTriggers(CKillCardManager* pManager)
{
    if (!pManager)
        return;
        
    // Test multiple kill types combined
    printf("Testing combination triggers...\n");
    
    // Critical + Ranged (should show Quantum Fission due to priority)
    pManager->TriggerKillCard(KILL_TYPE_CRITICAL | KILL_TYPE_RANGED);
    Sleep(3000);
    
    // Skill + Melee (should show Hologram Disruption due to priority)
    pManager->TriggerKillCard(KILL_TYPE_SKILL | KILL_TYPE_MELEE);
    Sleep(3000);
    
    // Consecutive + Headshot (should show Virtual Reconstruction due to priority)
    pManager->TriggerKillCard(KILL_TYPE_CONSECUTIVE | KILL_TYPE_HEADSHOT);
    Sleep(3000);
}

// Test rapid succession to check consecutive kill logic
void TestConsecutiveKills(CKillCardManager* pManager)
{
    if (!pManager)
        return;
        
    printf("Testing consecutive kill detection...\n");
    
    // Trigger multiple kills in quick succession
    for (int i = 0; i < 5; i++)
    {
        pManager->TriggerKillCard(KILL_TYPE_CONSECUTIVE);
        Sleep(500); // 0.5 second between kills
    }
}

// Test effect intensity scaling
void TestEffectIntensityScaling(CKillCardManager* pManager)
{
    if (!pManager)
        return;
        
    printf("Testing effect intensity scaling...\n");
    
    // Test with different intensity levels
    float intensities[] = { 0.3f, 0.6f, 1.0f, 1.5f, 2.0f };
    
    for (int i = 0; i < 5; i++)
    {
        pManager->SetEffectIntensity(intensities[i]);
        pManager->TriggerKillCard(KILL_TYPE_CRITICAL);
        printf("Testing with intensity: %.1f\n", intensities[i]);
        Sleep(2000);
    }
    
    // Reset to default
    pManager->SetEffectIntensity(1.0f);
}

// Memory usage test
void TestMemoryUsage(CKillCardManager* pManager)
{
    if (!pManager)
        return;
        
    printf("Testing memory usage...\n");
    
    // Get initial memory usage
    PROCESS_MEMORY_COUNTERS memStart, memEnd;
    GetProcessMemoryInfo(GetCurrentProcess(), &memStart, sizeof(memStart));
    
    // Trigger many kill cards
    for (int i = 0; i < 1000; i++)
    {
        EKILL_TYPE randomType = (EKILL_TYPE)(1 << (i % 6));
        pManager->TriggerKillCard(randomType);
        
        if (i % 100 == 0)
        {
            pManager->Reset(); // Periodic cleanup
        }
    }
    
    // Get final memory usage
    GetProcessMemoryInfo(GetCurrentProcess(), &memEnd, sizeof(memEnd));
    
    SIZE_T memoryIncrease = memEnd.WorkingSetSize - memStart.WorkingSetSize;
    printf("Memory increase after 1000 kill cards: %zu bytes\n", memoryIncrease);
}

// Validate system configuration
bool ValidateKillCardSystem(CKillCardManager* pManager)
{
    if (!pManager)
    {
        printf("ERROR: Kill Card Manager is NULL\n");
        return false;
    }
    
    printf("Validating Kill Card System...\n");
    
    // Check if system is enabled
    if (!pManager->IsEnabled())
    {
        printf("WARNING: Kill Card System is disabled\n");
        return false;
    }
    
    // Test basic functionality
    pManager->TriggerKillCard(KILL_TYPE_CRITICAL);
    
    // Check effect intensity
    float intensity = pManager->GetEffectIntensity();
    if (intensity < 0.1f || intensity > 3.0f)
    {
        printf("WARNING: Effect intensity is out of normal range: %.2f\n", intensity);
    }
    
    printf("Kill Card System validation completed successfully\n");
    return true;
}

// Main test suite
void RunKillCardTestSuite(CKillCardManager* pManager)
{
    printf("=== Kill Card System Test Suite ===\n");
    
    if (!ValidateKillCardSystem(pManager))
    {
        printf("System validation failed, aborting tests\n");
        return;
    }
    
    printf("\n1. Testing all kill card types...\n");
    TestAllKillCardTypes(pManager);
    
    printf("\n2. Testing combination triggers...\n");
    TestCombinationTriggers(pManager);
    
    printf("\n3. Testing consecutive kills...\n");
    TestConsecutiveKills(pManager);
    
    printf("\n4. Testing effect intensity scaling...\n");
    TestEffectIntensityScaling(pManager);
    
    printf("\n5. Testing performance impact...\n");
    TestPerformanceImpact(pManager);
    
    printf("\n6. Testing memory usage...\n");
    TestMemoryUsage(pManager);
    
    printf("\n=== Test Suite Completed ===\n");
}