#include "StdAfx.h"
#include "KillAnimationManager.h"
#include "../../DxTools/DxViewPort.h"
#include "../../../SigmaCore/DebugInclude.h"
#include <iostream>

// Test program for the Six Card Animation System
// 六大卡片動畫系統測試程序

class KillAnimationTester
{
private:
	bool m_bInitialized;
	LPDIRECT3DDEVICEQ m_pd3dDevice;
	DxEffSingleGroup* m_pEffectGroup;
	
	// Test state
	int m_nCurrentTest;
	float m_fTestTimer;
	bool m_bTestRunning;
	
	// Test positions
	D3DXVECTOR3 m_arrTestPositions[6];
	D3DXVECTOR3 m_arrTestDirections[6];
	
public:
	KillAnimationTester() 
		: m_bInitialized(false)
		, m_pd3dDevice(nullptr)
		, m_pEffectGroup(nullptr)
		, m_nCurrentTest(0)
		, m_fTestTimer(0.0f)
		, m_bTestRunning(false)
	{
		// Initialize test positions and directions
		InitializeTestData();
	}
	
	~KillAnimationTester()
	{
		Cleanup();
	}
	
	// Initialize the test system
	bool Initialize(LPDIRECT3DDEVICEQ pd3dDevice, DxEffSingleGroup* pEffectGroup)
	{
		if (!pd3dDevice || !pEffectGroup)
		{
			std::cout << "[Test] Error: Invalid device or effect group!" << std::endl;
			return false;
		}
		
		m_pd3dDevice = pd3dDevice;
		m_pEffectGroup = pEffectGroup;
		
		// Initialize the Kill Animation Manager
		HRESULT hr = KILL_ANIM_MGR.Initialize(pd3dDevice, pEffectGroup);
		if (FAILED(hr))
		{
			std::cout << "[Test] Error: Failed to initialize KillAnimationManager!" << std::endl;
			return false;
		}
		
		m_bInitialized = true;
		std::cout << "[Test] Kill Animation System initialized successfully!" << std::endl;
		return true;
	}
	
	// Cleanup test resources
	void Cleanup()
	{
		if (m_bInitialized)
		{
			KILL_ANIM_MGR.Cleanup();
			m_bInitialized = false;
		}
	}
	
	// Run all animation tests
	void RunAllTests()
	{
		if (!m_bInitialized)
		{
			std::cout << "[Test] Error: Test system not initialized!" << std::endl;
			return;
		}
		
		std::cout << "[Test] Starting comprehensive animation tests..." << std::endl;
		
		// Test each animation type
		for (int i = 0; i < KILL_ANIM_MAX; ++i)
		{
			TestSingleAnimation(static_cast<KILL_ANIMATION_TYPE>(i));
		}
		
		// Test configuration system
		TestConfigurationSystem();
		
		// Test multiple simultaneous animations
		TestMultipleAnimations();
		
		// Test performance with many animations
		TestPerformance();
		
		std::cout << "[Test] All tests completed!" << std::endl;
	}
	
	// Test a single animation type
	void TestSingleAnimation(KILL_ANIMATION_TYPE eType)
	{
		const char* animNames[] = {
			"Slash (劍氣砍殺)",
			"Stab (穿刺致命)",
			"Explosion (爆炸轟殺)",
			"Freeze (冰凍粉碎)",
			"Flame (烈焰焚燒)",
			"Lightning (雷電轟擊)"
		};
		
		std::cout << "[Test] Testing " << animNames[eType] << "..." << std::endl;
		
		// Play the animation
		bool result = KILL_ANIM_MGR.PlayKillAnimation(
			eType,
			m_arrTestPositions[eType],
			m_arrTestDirections[eType]
		);
		
		if (result)
		{
			std::cout << "[Test] " << animNames[eType] << " - SUCCESS" << std::endl;
		}
		else
		{
			std::cout << "[Test] " << animNames[eType] << " - FAILED" << std::endl;
		}
		
		// Wait for animation to complete
		Sleep(2000);
	}
	
	// Test configuration system
	void TestConfigurationSystem()
	{
		std::cout << "[Test] Testing configuration system..." << std::endl;
		
		// Test custom configuration
		KILL_ANIMATION_CONFIG customConfig;
		customConfig.fIntensity = 2.0f;
		customConfig.vColor = D3DXVECTOR3(1.0f, 0.0f, 0.0f); // Red
		customConfig.fDuration = 3.0f;
		customConfig.fScale = 1.5f;
		customConfig.bUseSound = true;
		customConfig.bUseScreenEffect = true;
		
		// Apply custom config to slash animation
		KILL_ANIM_MGR.SetAnimationConfig(KILL_ANIM_SLASH, customConfig);
		
		// Test with custom config
		bool result = KILL_ANIM_MGR.PlayKillAnimation(
			KILL_ANIM_SLASH,
			m_arrTestPositions[0],
			m_arrTestDirections[0],
			&customConfig
		);
		
		if (result)
		{
			std::cout << "[Test] Custom configuration test - SUCCESS" << std::endl;
		}
		else
		{
			std::cout << "[Test] Custom configuration test - FAILED" << std::endl;
		}
		
		Sleep(3000);
	}
	
	// Test multiple simultaneous animations
	void TestMultipleAnimations()
	{
		std::cout << "[Test] Testing multiple simultaneous animations..." << std::endl;
		
		// Play all animations at different positions
		int successCount = 0;
		for (int i = 0; i < KILL_ANIM_MAX; ++i)
		{
			D3DXVECTOR3 offset(static_cast<float>(i * 3), 0.0f, 0.0f);
			D3DXVECTOR3 pos = m_arrTestPositions[i] + offset;
			
			bool result = KILL_ANIM_MGR.PlayKillAnimation(
				static_cast<KILL_ANIMATION_TYPE>(i),
				pos,
				m_arrTestDirections[i]
			);
			
			if (result) successCount++;
		}
		
		std::cout << "[Test] Multiple animations: " << successCount << "/" << KILL_ANIM_MAX << " successful" << std::endl;
		
		// Check active animation count
		int activeCount = KILL_ANIM_MGR.GetActiveAnimationCount();
		std::cout << "[Test] Active animations: " << activeCount << std::endl;
		
		Sleep(4000);
	}
	
	// Test performance with many animations
	void TestPerformance()
	{
		std::cout << "[Test] Testing performance with multiple animations..." << std::endl;
		
		const int TEST_COUNT = 20;
		int successCount = 0;
		
		// Record start time
		DWORD startTime = timeGetTime();
		
		// Create many animations quickly
		for (int i = 0; i < TEST_COUNT; ++i)
		{
			KILL_ANIMATION_TYPE type = static_cast<KILL_ANIMATION_TYPE>(i % KILL_ANIM_MAX);
			D3DXVECTOR3 randomPos(
				static_cast<float>((rand() % 20) - 10),
				0.0f,
				static_cast<float>((rand() % 20) - 10)
			);
			
			bool result = KILL_ANIM_MGR.PlayKillAnimation(type, randomPos);
			if (result) successCount++;
		}
		
		DWORD endTime = timeGetTime();
		DWORD duration = endTime - startTime;
		
		std::cout << "[Test] Performance test: " << successCount << "/" << TEST_COUNT 
				  << " animations created in " << duration << "ms" << std::endl;
		
		// Check memory usage (active animations)
		int activeCount = KILL_ANIM_MGR.GetActiveAnimationCount();
		std::cout << "[Test] Peak active animations: " << activeCount << std::endl;
		
		Sleep(3000);
	}
	
	// Interactive test mode
	void RunInteractiveTest()
	{
		if (!m_bInitialized)
		{
			std::cout << "[Test] Error: Test system not initialized!" << std::endl;
			return;
		}
		
		std::cout << "[Test] Interactive test mode started!" << std::endl;
		std::cout << "[Test] Commands:" << std::endl;
		std::cout << "[Test]   1-6: Play animation type 1-6" << std::endl;
		std::cout << "[Test]   a: Play all animations" << std::endl;
		std::cout << "[Test]   s: Stop all animations" << std::endl;
		std::cout << "[Test]   c: Show active count" << std::endl;
		std::cout << "[Test]   q: Quit interactive mode" << std::endl;
		
		char input;
		while (true)
		{
			std::cout << "[Test] Enter command: ";
			std::cin >> input;
			
			switch (input)
			{
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
				{
					int animIndex = input - '1';
					KILL_ANIMATION_TYPE type = static_cast<KILL_ANIMATION_TYPE>(animIndex);
					bool result = KILL_ANIM_MGR.PlayKillAnimation(
						type, 
						m_arrTestPositions[animIndex], 
						m_arrTestDirections[animIndex]
					);
					std::cout << "[Test] Animation " << (animIndex + 1) 
							  << (result ? " started" : " failed") << std::endl;
				}
				break;
				
			case 'a':
				TestMultipleAnimations();
				break;
				
			case 's':
				KILL_ANIM_MGR.StopAllAnimations();
				std::cout << "[Test] All animations stopped" << std::endl;
				break;
				
			case 'c':
				std::cout << "[Test] Active animations: " 
						  << KILL_ANIM_MGR.GetActiveAnimationCount() << std::endl;
				break;
				
			case 'q':
				std::cout << "[Test] Exiting interactive mode" << std::endl;
				return;
				
			default:
				std::cout << "[Test] Unknown command" << std::endl;
				break;
			}
		}
	}
	
	// Update test system (called each frame)
	void Update(float fElapsedTime)
	{
		if (m_bInitialized)
		{
			KILL_ANIM_MGR.FrameMove(fElapsedTime);
		}
	}
	
	// Render test system (called each frame)
	void Render()
	{
		if (m_bInitialized)
		{
			KILL_ANIM_MGR.Render();
		}
	}
	
private:
	// Initialize test data positions and directions
	void InitializeTestData()
	{
		// Set up test positions in a circle
		const float radius = 5.0f;
		const float angleStep = D3DX_PI * 2.0f / 6.0f;
		
		for (int i = 0; i < 6; ++i)
		{
			float angle = angleStep * i;
			m_arrTestPositions[i] = D3DXVECTOR3(
				cosf(angle) * radius,
				1.0f,
				sinf(angle) * radius
			);
			
			// Directions point toward center
			m_arrTestDirections[i] = D3DXVECTOR3(
				-cosf(angle),
				0.0f,
				-sinf(angle)
			);
		}
	}
};

// Global test instance
static KillAnimationTester g_AnimationTester;

// Test interface functions
namespace KillAnimationTest
{
	bool Initialize(LPDIRECT3DDEVICEQ pd3dDevice, DxEffSingleGroup* pEffectGroup)
	{
		return g_AnimationTester.Initialize(pd3dDevice, pEffectGroup);
	}
	
	void Cleanup()
	{
		g_AnimationTester.Cleanup();
	}
	
	void RunAllTests()
	{
		g_AnimationTester.RunAllTests();
	}
	
	void RunInteractiveTest()
	{
		g_AnimationTester.RunInteractiveTest();
	}
	
	void Update(float fElapsedTime)
	{
		g_AnimationTester.Update(fElapsedTime);
	}
	
	void Render()
	{
		g_AnimationTester.Render();
	}
}

// Example usage in main application:
/*
// In initialization:
KillAnimationTest::Initialize(pd3dDevice, pEffectGroup);

// In main loop:
KillAnimationTest::Update(fElapsedTime);
KillAnimationTest::Render();

// For testing:
KillAnimationTest::RunAllTests();
// or
KillAnimationTest::RunInteractiveTest();

// In cleanup:
KillAnimationTest::Cleanup();
*/