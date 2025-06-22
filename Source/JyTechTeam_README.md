# RanOnline Kill Animation System + Job Unlocking Configuration
# Jy技術團隊 - TIAN0517

## GM指令使用說明 (GM Command Usage)

### 中文指令 (Chinese Commands):
```
/開啟槍手          - 解鎖槍手職業 (Unlock Gunner class)
/開啟忍者          - 解鎖忍者職業 (Unlock Ninja class)  
/解鎖所有職業      - 解鎖所有隱藏職業 (Unlock all hidden jobs)
/添加擊殺卡片 12001 60  - 添加劍氣砍殺卡片，60分鐘有效期 (Add sword slash kill card, 60 min expiry)
/添加擊殺卡片 12002 30  - 添加穿刺致命卡片，30分鐘有效期 (Add pierce fatal kill card, 30 min expiry)
```

### 英文指令 (English Commands):
```
/unlockgunner      - Unlock Gunner class
/unlockninja       - Unlock Ninja class
/unlockall         - Unlock all hidden jobs
/addkillcard 12001 60  - Add sword slash kill card with 60 min expiry
/addkillcard 12002 30  - Add pierce fatal kill card with 30 min expiry
```

## 擊殺動畫卡片 (Kill Animation Cards)

### 卡片ID對照表:
- 12001: 劍氣砍殺 (Sword Slash)
- 12002: 穿刺致命 (Pierce Fatal)
- 12003: 爆炸轟殺 (Explosion Blast)
- 12004: 冰凍粉碎 (Ice Shatter)
- 12005: 烈焰焚燒 (Flame Burn)
- 12006: 雷電轟擊 (Thunder Strike)

### 使用方式:
1. GM使用指令給玩家添加擊殺卡片
2. 玩家在PVP中擊殺其他玩家時觸發動畫特效
3. 卡片有時效性，過期後自動移除
4. 可同時擁有多種動畫卡片，觸發時隨機選擇

## 職業解鎖系統 (Job Unlocking System)

### 隱藏職業:
- 槍手 (Gunner) - 對應原本的 ARCHER 職業
- 忍者 (Ninja) - 對應原本的 ASSASSIN 職業

### 解鎖機制:
1. 預設狀態下這些職業在角色創建介面中被隱藏
2. 只有GM等級用戶可以使用解鎖指令
3. 解鎖後玩家可以在角色創建時選擇這些職業
4. 支持個別解鎖或批量解鎖

## 技術實現 (Technical Implementation)

### 文件修改清單:
1. `Source/RanGfxUI/OldUI/Interface/KillAnimationSystem.h/cpp` - 擊殺動畫系統
2. `Source/RanGfxUI/OldUI/Interface/CreateCharacterClass.h/cpp` - 職業解鎖支援
3. `Source/RanLogicClient/Command/GLCommand.h/cpp` - 中文指令支援
4. `Source/RanLogicServer/Server/JyGMCommandProcessor.h/cpp` - 服務端GM指令處理
5. `Source/RanLogicServer/FieldServer/GLGaeaServerChat.cpp` - 服務端整合

### 開發團隊:
**Jy技術團隊**
- 主開發者: TIAN0517
- 專案: RanOnline 伺服器功能擴展
- 版本: v1.0

### 測試指令:
```bash
# 在遊戲中測試GM指令 (需要GM3以上權限):
/開啟槍手
/開啟忍者  
/添加擊殺卡片 12001 60
/解鎖所有職業
```