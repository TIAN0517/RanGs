#include "../../stdafx.h"
#include "KillCardManager.h"
#include "KillCardPlugin.h"
#include "KillCardPostProcess.h"
#include "KillCardPerformanceMonitor.h"
#include "KillCardAssetManager.h"

/**
 * 击杀卡片管理器增强实现
 * 整合插件架构、PostProcess后处理和性能监控
 */

// 添加插件系统支持到现有KillCardManager
void CKillCardManager::TriggerKillCard(EKILL_TYPE killType, DWORD targetID)
{
    if (!m_bEnabled)
        return;
    
    // 检查是否装备了对应的击杀卡片
    CKillCardPluginManager& pluginMgr = CKillCardPluginManager::GetInstance();
    EKILLCARD_EQUIPPED_STATE equippedCards = pluginMgr.GetEquippedCards();
    
    if (equippedCards == CARD_EQUIPPED_NONE)
    {
        // 没有装备击杀卡片，使用原版显示
        return;
    }
    
    // 确定要显示的卡片类型
    EKILL_CARD_TYPE cardType = DetermineCardType(killType);
    
    // 检查卡片是否装备
    BOOL bShouldShow = FALSE;
    switch (cardType)
    {
    case KILL_CARD_TYPE_QUANTUM:
        bShouldShow = (equippedCards & CARD_EQUIPPED_QUANTUM) != 0;
        break;
    case KILL_CARD_TYPE_HOLOGRAM:
        bShouldShow = (equippedCards & CARD_EQUIPPED_HOLOGRAM) != 0;
        break;
    case KILL_CARD_TYPE_SPACETIME:
        bShouldShow = (equippedCards & CARD_EQUIPPED_SPACETIME) != 0;
        break;
    case KILL_CARD_TYPE_PLASMA:
        bShouldShow = (equippedCards & CARD_EQUIPPED_PLASMA) != 0;
        break;
    case KILL_CARD_TYPE_NANO:
        bShouldShow = (equippedCards & CARD_EQUIPPED_NANO) != 0;
        break;
    case KILL_CARD_TYPE_VIRTUAL:
        bShouldShow = (equippedCards & CARD_EQUIPPED_VIRTUAL) != 0;
        break;
    }
    
    if (bShouldShow)
    {
        // 触发科技感击杀卡片效果
        ShowKillCard(cardType);
        
        // 触发PostProcess效果
        D3DXVECTOR3 worldPos(0.0f, 0.0f, 0.0f); // TODO: 获取击杀位置
        pluginMgr.TriggerKillAnimation(killType, worldPos);
        
        // 更新连续击杀计数
        UpdateConsecutiveKills();
        
        // 性能监控
        CKillCardPerformanceMonitor::GetInstance().BeginKillCardRender();
        
        // 触发插件特效
        if (m_pEffects)
        {
            m_pEffects->TriggerEffect(cardType, m_fEffectIntensity);
        }
        
        CKillCardPerformanceMonitor::GetInstance().EndKillCardRender();
    }
}

void CKillCardManager::CreateSubControl()
{
    // 原有的控件创建代码...
    
    // 创建动画控制器
    {
        m_pAnimation = new CKillCardAnimation(m_pEngineDevice);
        m_pAnimation->CreateSub(this, "KILLCARD_ANIMATION_AREA");
        m_pAnimation->CreateSubControl();
        m_pAnimation->SetVisibleSingle(FALSE);
        RegisterControl(m_pAnimation);
    }

    // 创建渲染器
    {
        m_pRenderer = new CKillCardRenderer(m_pEngineDevice);
        m_pRenderer->CreateSub(this, "KILLCARD_RENDERER_AREA");
        m_pRenderer->CreateSubControl();
        m_pRenderer->SetVisibleSingle(FALSE);
        RegisterControl(m_pRenderer);
    }

    // 创建特效系统
    {
        m_pEffects = new CKillCardEffects(m_pEngineDevice);
        m_pEffects->CreateSub(this, "KILLCARD_EFFECTS_AREA");
        m_pEffects->CreateSubControl();
        m_pEffects->SetVisibleSingle(FALSE);
        RegisterControl(m_pEffects);
    }
    
    // 初始化插件系统
    CKillCardPluginManager& pluginMgr = CKillCardPluginManager::GetInstance();
    
    // 尝试加载默认插件DLL
    char szPluginPath[MAX_PATH];
    GetModuleFileNameA(NULL, szPluginPath, MAX_PATH);
    char* pLastSlash = strrchr(szPluginPath, '\\');
    if (pLastSlash)
    {
        strcpy(pLastSlash + 1, "KillCardPlugin.dll");
        pluginMgr.LoadPlugin(szPluginPath);
    }
    
    // 初始化资源管理器
    char szAssetPath[MAX_PATH];
    GetModuleFileNameA(NULL, szAssetPath, MAX_PATH);
    pLastSlash = strrchr(szAssetPath, '\\');
    if (pLastSlash)
    {
        strcpy(pLastSlash + 1, "KillCardAssets\\");
        CKillCardAssetManager::GetInstance().Initialize(m_pEngineDevice->GetDevice(), szAssetPath);
    }
    
    // 初始化PostProcess系统
    CKillCardPostProcessLayer& postProcess = CKillCardPostProcessLayer::GetInstance();
    D3DSURFACE_DESC desc;
    LPDIRECT3DSURFACE9 pBackBuffer = NULL;
    
    if (SUCCEEDED(m_pEngineDevice->GetDevice()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer)))
    {
        if (SUCCEEDED(pBackBuffer->GetDesc(&desc)))
        {
            postProcess.Initialize(m_pEngineDevice->GetDevice(), desc.Width, desc.Height);
        }
        pBackBuffer->Release();
    }
    
    // 初始化性能监控
    CKillCardPerformanceMonitor::GetInstance().Initialize(m_pEngineDevice->GetDevice());
    CKillCardPerformanceMonitor::GetInstance().SetEnabled(TRUE);
    
    // 加载资源预设
    CKillCardAssetPresets presets;
    presets.CreateDefaultPresets();
    presets.PreloadAllPresets();
}

void CKillCardManager::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
    // 性能监控开始
    CKillCardPerformanceMonitor::GetInstance().BeginFrame();
    
    // 更新性能数据
    CKillCardPerformanceMonitor::GetInstance().UpdatePerformanceData(fElapsedTime);
    
    // 原有更新逻辑
    if (!IsVisible())
        return;

    m_fCurrentTime += fElapsedTime;
    
    // 更新动画
    if (m_pAnimation)
        m_pAnimation->Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);
    
    // 更新特效
    if (m_pEffects)
        m_pEffects->Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);
    
    // 更新插件系统
    CKillCardPluginManager& pluginMgr = CKillCardPluginManager::GetInstance();
    pluginMgr.UpdatePerformanceData(fElapsedTime);
    
    // 更新PostProcess系统
    CKillCardPostProcessLayer::GetInstance().Update(fElapsedTime);
    
    // 检查性能阈值
    if (CKillCardPerformanceMonitor::GetInstance().CheckPerformanceWarnings())
    {
        // 自动优化设置
        CKillCardPerformanceMonitor::GetInstance().TriggerAutoOptimization();
    }
    
    // 清理过期动画
    if (m_bActive && m_fCurrentTime > CARD_DISPLAY_TIME)
    {
        m_bActive = FALSE;
        SetVisibleSingle(FALSE);
    }
    
    // 基类更新
    CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);
    
    // 性能监控结束
    CKillCardPerformanceMonitor::GetInstance().EndFrame();
}

// 设备丢失和重置处理
void CKillCardManager::OnDeviceLost()
{
    CKillCardPluginManager::GetInstance().OnDeviceLost();
    CKillCardPostProcessLayer::GetInstance().OnDeviceLost();
    CKillCardPerformanceMonitor::GetInstance().OnDeviceLost();
    CKillCardAssetManager::GetInstance().OnDeviceLost();
}

void CKillCardManager::OnDeviceReset()
{
    CKillCardPluginManager::GetInstance().OnDeviceReset();
    CKillCardPostProcessLayer::GetInstance().OnDeviceReset();
    CKillCardPerformanceMonitor::GetInstance().OnDeviceReset();
    CKillCardAssetManager::GetInstance().OnDeviceReset();
}

// 配置管理
void CKillCardManager::SetEffectIntensity(float fIntensity)
{
    m_fEffectIntensity = fIntensity;
    
    // 更新插件配置
    SKillCardPluginConfig config = CKillCardPluginManager::GetInstance().GetConfiguration();
    config.fEffectIntensity = fIntensity;
    CKillCardPluginManager::GetInstance().SetConfiguration(config);
}

void CKillCardManager::SetEnabled(BOOL bEnabled)
{
    m_bEnabled = bEnabled;
    
    // 更新插件配置
    SKillCardPluginConfig config = CKillCardPluginManager::GetInstance().GetConfiguration();
    config.bEnabled = bEnabled;
    CKillCardPluginManager::GetInstance().SetConfiguration(config);
    
    // 更新PostProcess系统
    CKillCardPostProcessLayer::GetInstance().SetEnabled(bEnabled);
}

// 获取性能数据
SKillCardPerformanceData CKillCardManager::GetPerformanceData() const
{
    return CKillCardPluginManager::GetInstance().GetPerformanceData();
}

// 生成性能报告
void CKillCardManager::GeneratePerformanceReport(std::string& outReport) const
{
    CKillCardPerformanceMonitor::GetInstance().GeneratePerformanceReport(outReport);
}

// 热重载资源
void CKillCardManager::ReloadAssets()
{
    CKillCardAssetManager::GetInstance().CheckForFileChanges();
}

// 保存配置
void CKillCardManager::SaveConfiguration()
{
    CKillCardPluginManager::GetInstance().SaveConfiguration();
}

// 设备管理增强
BOOL CKillCardManager::InitializeAdvancedSystems(LPDIRECT3DDEVICE9 pDevice)
{
    // 初始化所有子系统
    if (!CKillCardPluginManager::GetInstance().InitializeDevice(pDevice))
        return FALSE;
    
    // 获取后备缓冲区信息
    D3DSURFACE_DESC desc;
    LPDIRECT3DSURFACE9 pBackBuffer = NULL;
    
    if (SUCCEEDED(pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer)))
    {
        if (SUCCEEDED(pBackBuffer->GetDesc(&desc)))
        {
            if (!CKillCardPostProcessLayer::GetInstance().Initialize(pDevice, desc.Width, desc.Height))
            {
                pBackBuffer->Release();
                return FALSE;
            }
        }
        pBackBuffer->Release();
    }
    
    if (!CKillCardPerformanceMonitor::GetInstance().Initialize(pDevice))
        return FALSE;
    
    // 获取游戏路径
    char szGamePath[MAX_PATH];
    GetModuleFileNameA(NULL, szGamePath, MAX_PATH);
    char* pLastSlash = strrchr(szGamePath, '\\');
    if (pLastSlash)
    {
        strcpy(pLastSlash + 1, "KillCardAssets\\");
        if (!CKillCardAssetManager::GetInstance().Initialize(pDevice, szGamePath))
            return FALSE;
    }
    
    return TRUE;
}

void CKillCardManager::ShutdownAdvancedSystems()
{
    CKillCardAssetManager::GetInstance().Shutdown();
    CKillCardPerformanceMonitor::GetInstance().Shutdown();
    CKillCardPostProcessLayer::GetInstance().Shutdown();
    CKillCardPluginManager::GetInstance().UnloadPlugin();
    CKillCardPluginManager::ReleaseInstance();
}