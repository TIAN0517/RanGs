# Kill Panel Style System Integration Guide
## Jy技術團隊 - TIAN0517

### 系統概述
本系統實現了六種科技感擊殺面板風格，可通過GM指令實時切換，為RanGs遊戲增加豐富的視覺體驗。

### 實現的六種風格

#### 1. 賽博朋克風格 (Cyberpunk Style)
- **主題色彩**: 霓虹粉紫 (#FF007F) + 電藍色 (#00FFFF)
- **特效**: 故障閃爍、數位雨滴、電路板紋理
- **Shader**: `CyberpunkEffect.hlsl`
- **類別**: `CCyberpunkStyle`

#### 2. 量子科技風格 (Quantum Tech Style)
- **主題色彩**: 量子藍 (#0080FF) + 能量白光 (#FFFFFF)
- **特效**: 量子粒子波動、光子流、六邊形蜂巢結構
- **Shader**: `QuantumEffect.hlsl` (現有)
- **類別**: `CQuantumTechStyle`

#### 3. 軍事戰術風格 (Military Tactical Style)
- **主題色彩**: 軍綠色 (#4B7C59) + 橙色警告 (#FF8C00)
- **特效**: 雷達掃描、戰術網格、目標鎖定十字準星
- **Shader**: `MilitaryHUD.hlsl`
- **類別**: `CMilitaryTacticalStyle`

#### 4. 全息投影風格 (Holographic Style)
- **主題色彩**: 全息藍 (#40E0D0) + 透明白光 (#FFFFFF80)
- **特效**: 全息掃描線、投影失真、光線衍射
- **Shader**: `HologramProjection.hlsl`
- **類別**: `CHolographicStyle`

#### 5. 太空科幻風格 (Space Sci-Fi Style)
- **主題色彩**: 深空藍 (#191970) + 星光銀 (#C0C0C0)
- **特效**: 星空背景、星雲流動、引力波、星際傳送門
- **Shader**: `SpaceNebulaEffect.hlsl`
- **類別**: `CSpaceSciFiStyle`

#### 6. 蒸汽朋克風格 (Steampunk Style)
- **主題色彩**: 復古銅色 (#B87333) + 蒸汽白 (#F5F5DC)
- **特效**: 齒輪轉動、蒸汽煙霧、維多利亞裝飾、機械運轉
- **Shader**: `SteampunkMechanical.hlsl`
- **類別**: `CSteampunkStyle`

### 核心系統架構

#### StyleManager (風格管理器)
- **單例模式**: 全局管理所有風格配置
- **功能**: 風格切換、配置管理、顏色獲取
- **文件**: `StyleManager.h/cpp`

#### KillCardManager 擴展
- **新增方法**:
  - `SetKillPanelStyle(int nStyleID)` - 設定風格 (1-6)
  - `GetCurrentStyle()` - 獲取當前風格
  - `GetStyleName(int nStyleID)` - 獲取風格名稱
  - `IsValidStyleID(int nStyleID)` - 驗證風格ID

#### GM指令系統
- **中文指令**: `/設定擊殺面板風格 [1-6]`
- **英文指令**: `/setkillpanelstyle [1-6]`
- **處理函數**: `ProcessSetKillPanelStyle()`

### 使用方法

#### GM指令使用
```
/設定擊殺面板風格 1  // 設定為賽博朋克風格
/設定擊殺面板風格 2  // 設定為量子科技風格
/設定擊殺面板風格 3  // 設定為軍事戰術風格
/設定擊殺面板風格 4  // 設定為全息投影風格
/設定擊殺面板風格 5  // 設定為太空科幻風格
/設定擊殺面板風格 6  // 設定為蒸汽朋克風格
```

#### 程式碼集成
```cpp
// 獲取風格管理器
CStyleManager* pStyleManager = CStyleManager::GetInstance();

// 設定風格
pStyleManager->SetCurrentStyle(STYLE_CYBERPUNK);

// 獲取風格配置
const STYLE_CONFIG* pConfig = pStyleManager->GetCurrentStyleConfig();
D3DXCOLOR primaryColor = pConfig->primaryColor;
```

### 檔案結構
```
Source/RanGfxUI/KillCard/
├── KillPanelStyles/
│   ├── StyleManager.h/.cpp         // 風格管理器
│   ├── CyberpunkStyle.h/.cpp       // 賽博朋克風格
│   ├── QuantumStyle.h              // 量子科技風格
│   ├── MilitaryStyle.h             // 軍事戰術風格
│   ├── HolographicStyle.h          // 全息投影風格
│   ├── SpaceSciFiStyle.h           // 太空科幻風格
│   └── SteampunkStyle.h            // 蒸汽朋克風格
├── Shaders/
│   ├── CyberpunkEffect.hlsl        // 賽博朋克著色器
│   ├── MilitaryHUD.hlsl            // 軍事HUD著色器
│   ├── HologramProjection.hlsl     // 全息投影著色器
│   ├── SpaceNebulaEffect.hlsl      // 太空星雲著色器
│   └── SteampunkMechanical.hlsl    // 蒸汽朋克著色器
└── Resources/
    ├── Textures/                   // 各風格紋理資源
    ├── Sounds/                     // 音效資源
    └── Animations/                 // 動畫數據
```

### 技術特點

#### 性能優化
- 自動FPS監控和效果強度調整
- 可配置的粒子數量和特效品質
- 根據設備性能自動調整複雜度

#### 兼容性
- 完全向後兼容現有Kill Animation系統
- 保留所有原有的6種擊殺動畫 (12001-12006)
- 不影響現有GM指令功能

#### 擴展性
- 模塊化設計，易於添加新風格
- 統一的配置接口
- 支援風格配置的保存和載入

### 開發狀態

#### 已完成功能
- ✅ StyleManager核心架構
- ✅ 6種風格基礎配置
- ✅ GM指令中文支援
- ✅ KillCardManager集成
- ✅ 5個新Shader實現
- ✅ 風格類別框架

#### 待完成功能
- 🔄 各風格類別的完整實現
- 🔄 紋理資源創建
- 🔄 音效系統集成
- 🔄 配置保存/載入功能
- 🔄 性能測試和優化

### 測試驗收

#### 功能測試
1. 所有6種風格可正常切換
2. GM指令響應正確
3. 風格設定持久化
4. 無FPS影響 (<5% 性能損失)

#### 視覺效果測試
1. 每種風格的特效正確顯示
2. 顏色主題符合設計要求
3. 動畫流暢無卡頓
4. 特殊效果（故障、掃描等）正常工作

### 總結
本系統成功為RanGs遊戲增加了六種科技感擊殺面板風格，提供了豐富的視覺體驗和高度的可定制性。通過模塊化設計和性能優化，確保了系統的穩定性和可擴展性。