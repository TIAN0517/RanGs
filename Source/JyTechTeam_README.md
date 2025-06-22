# RanOnline Complete Chinese GM Command System + Kill Animation + Job Unlocking
# 開發者：Jy技術團隊 - TIAN0517 (Complete Implementation)

## 🎯 **完整系統概述 (Complete System Overview)**

本系統實現了一個完整的中文化GM指令系統，包含：
- **完整中文GM指令系統** (60+ 中文指令支援)
- **Kill Animation 系統** (6種擊殺動畫特效)
- **職業解鎖系統** (槍手/忍者職業)
- **專業編譯配置** (數位簽名支援)

## 📋 **完整中文指令列表 (Complete Chinese Commands)**

### **道具系統 (Item System)**
```bash
# 中文指令                        # 英文對應
/添加道具 [道具ID] [數量] [天數]   # /additem [itemid] [count] [days]
/給道具 [道具ID] [數量] [天數]     # /additem [itemid] [count] [days]
/新增道具 [道具ID] [數量] [天數]   # /additem [itemid] [count] [days]

/刪除道具 [道具ID] [數量]         # /delitem [itemid] [count]
/移除道具 [道具ID] [數量]         # /delitem [itemid] [count]
/刪道具 [道具ID] [數量]           # /delitem [itemid] [count]

/贈送道具 [玩家名] [道具ID] [數量] # /giveitem [player] [itemid] [count]
/送道具 [玩家名] [道具ID] [數量]   # /giveitem [player] [itemid] [count]
```

### **角色屬性 (Character Attributes)**
```bash
/給經驗 [經驗值]                  # /givexp [experience]
/增加經驗 [經驗值]                # /givexp [experience]
/加經驗 [經驗值]                  # /givexp [experience]

/設定等級 [等級]                  # /setlevel [level]
/改等級 [等級]                    # /setlevel [level]
/設等級 [等級]                    # /setlevel [level]

/設定血量 [血量]                  # /sethp [hp]
/改血量 [血量]                    # /sethp [hp]
/滿血                            # /fullhp

/設定魔力 [魔力]                  # /setmp [mp]
/改魔力 [魔力]                    # /setmp [mp]
/滿魔                            # /fullmp
```

### **移動傳送 (Teleportation)**
```bash
/傳送 [地圖ID] [X座標] [Y座標]    # /goto [mapid] [x] [y]
/去 [地圖ID] [X座標] [Y座標]      # /goto [mapid] [x] [y]
/瞬移 [地圖ID] [X座標] [Y座標]    # /goto [mapid] [x] [y]

/召喚 [玩家名]                    # /summon [playername]
/叫來 [玩家名]                    # /summon [playername]
/召喚玩家 [玩家名]                # /summon [playername]

/傳送玩家 [玩家名] [地圖ID] [X] [Y] # /teleport [player] [mapid] [x] [y]
/送玩家 [玩家名] [地圖ID] [X] [Y]   # /teleport [player] [mapid] [x] [y]
```

### **管理功能 (Administration)**
```bash
/踢出 [玩家名]                    # /kick [playername]
/踢人 [玩家名]                    # /kick [playername]
/踢掉 [玩家名]                    # /kick [playername]

/封號 [玩家名] [天數]             # /ban [playername] [days]
/禁止 [玩家名] [天數]             # /ban [playername] [days]
/封帳號 [玩家名] [天數]           # /ban [playername] [days]

/禁言 [玩家名] [分鐘]             # /mute [playername] [minutes]
/閉嘴 [玩家名] [分鐘]             # /mute [playername] [minutes]
```

### **系統功能 (System Functions)**
```bash
/關機 [秒數]                      # /shutdown [seconds]
/關服 [秒數]                      # /shutdown [seconds]
/停機 [秒數]                      # /shutdown [seconds]

/公告 [訊息]                      # /notice [message]
/通知 [訊息]                      # /notice [message]

/廣播 [訊息]                      # /broadcast [message]
/全服廣播 [訊息]                  # /broadcast [message]
```

### **怪物系統 (Monster System)**
```bash
/召喚怪物 [怪物ID] [數量]         # /summonmob [mobid] [count]
/叫怪 [怪物ID] [數量]             # /summonmob [mobid] [count]
/生怪 [怪物ID] [數量]             # /summonmob [mobid] [count]

/清怪                            # /killall
/殺全部                          # /killall
```

### **Kill Animation 系統 (Kill Animation)**
```bash
/添加擊殺卡片 [類型] [天數]       # /addkillcard [type] [days]
/給擊殺特效 [類型] [天數]         # /addkillcard [type] [days]
/擊殺動畫 [類型] [天數]           # /addkillcard [type] [days]
/特效卡 [類型] [天數]             # /addkillcard [type] [days]

# 特效類型：
# 12001 - 劍氣砍殺 (Sword Slash)
# 12002 - 穿刺致命 (Pierce Fatal)
# 12003 - 爆炸轟殺 (Explosion Blast)
# 12004 - 冰凍粉碎 (Ice Shatter)
# 12005 - 烈焰焚燒 (Flame Burn)
# 12006 - 雷電轟擊 (Thunder Strike)
```

### **職業解鎖 (Job Unlocking)**
```bash
/開啟槍手                        # /unlockgunner
/解鎖槍手                        # /unlockgunner

/開啟忍者                        # /unlockninja
/解鎖忍者                        # /unlockninja

/解鎖所有職業                    # /unlockall
/開啟所有職業                    # /unlockall
```

### **資訊查詢 (Information)**
```bash
/查看玩家 [玩家名]                # /viewuser [playername]
/玩家資訊 [玩家名]                # /viewuser [playername]

/線上人數                        # /online
/在線人數                        # /online

/幫助                            # /help
/說明                            # /help
```

## 🏗️ **技術實現架構 (Technical Implementation)**

### **新增檔案架構 (New File Structure)**
```
Source/
├── EngineLib/
│   ├── GMSystem/
│   │   ├── GMCommandChinese.h          # 中文指令映射系統
│   │   └── GMCommandProcessor.cpp      # 指令處理器
│   └── Common/
│       └── ProjectInfo.h               # 專案資訊配置
├── RanGs.rc                           # Windows 資源檔
Scripts/
├── CreateSelfSignedCert.ps1           # 數位憑證產生器
└── SignExecutable.bat                 # 程式簽名腳本
Build_Complete_RanGs.bat               # 完整編譯腳本
Resources/
└── RanGs_Icon.ico                     # 應用程式圖示
```

### **修改檔案清單 (Modified Files)**
```
Source/RanLogicClient/Command/
├── GLCommand.h                        # 新增60+中文指令支援
└── GLCommand.cpp                      # 完整中文指令實現

Source/RanGfxUI/OldUI/Interface/
├── KillAnimationSystem.h/cpp          # 6種擊殺動畫系統
└── CreateCharacterClass.h/cpp         # 職業解鎖整合

Source/RanLogicServer/Server/
├── JyGMCommandProcessor.h/cpp          # 伺服器端GM處理
└── [FieldServer integration]          # 伺服器整合
```

## 🔧 **編譯與部署 (Build & Deployment)**

### **1. 自動編譯**
```bash
# 執行完整編譯腳本
./Build_Complete_RanGs.bat

# 輸出檔案：
# - bin/RanGs_Linux (Linux版本)
# - bin/VERSION.txt (版本資訊)
```

### **2. 數位簽名**
```powershell
# 建立數位憑證
powershell -ExecutionPolicy Bypass -File Scripts/CreateSelfSignedCert.ps1

# 簽名執行檔
Scripts/SignExecutable.bat RanGs.exe
```

### **3. Visual Studio 編譯**
```
開啟：Source/Smtm_2008.sln
新增檔案到專案：
- Source/EngineLib/GMSystem/GMCommandChinese.h
- Source/EngineLib/GMSystem/GMCommandProcessor.cpp
- Source/EngineLib/Common/ProjectInfo.h
- Source/RanGs.rc
```

## 🎮 **使用範例 (Usage Examples)**

### **基本GM操作**
```bash
# 添加道具
/添加道具 1001 10 30           # 添加道具ID 1001，數量10，30天期限

# 角色操作
/給經驗 100000                 # 增加10萬經驗
/設定等級 50                   # 設定等級50
/滿血                         # 補滿血量

# 傳送操作
/傳送 1 100 200               # 傳送到地圖1，座標(100,200)
/召喚 TestPlayer              # 召喚TestPlayer到身邊
```

### **Kill Animation 特效**
```bash
# 添加擊殺特效卡片
/添加擊殺卡片 12001 30         # 劍氣砍殺，30天
/擊殺動畫 12003 -1             # 爆炸轟殺，永久
/特效卡 12006 7               # 雷電轟擊，7天

# 查看所有可用特效
/幫助                         # 顯示完整指令說明
```

### **職業解鎖**
```bash
# 解鎖職業
/開啟槍手                     # 解鎖槍手(Archer)職業
/開啟忍者                     # 解鎖忍者(Assassin)職業
/解鎖所有職業                 # 解鎖所有隱藏職業
```

### **管理功能**
```bash
# 玩家管理
/踢出 BadPlayer               # 踢出違規玩家
/封號 Cheater 7               # 封號作弊玩家7天
/禁言 Spammer 30              # 禁言刷屏玩家30分鐘

# 系統公告
/公告 伺服器將於10分鐘後重啟   # 發送系統公告
/廣播 恭喜XXX獲得傳說裝備      # 全服廣播
```

## 🛡️ **安全與權限 (Security & Permissions)**

### **GM權限等級**
- **USER_GM3+**: 基本指令 (道具、經驗、傳送)
- **USER_GM2+**: 管理指令 (踢人、禁言)
- **USER_GM1+**: 系統指令 (關機、封號)
- **USER_MASTER**: 所有指令 (無限制)

### **指令驗證**
- 伺服器端權限檢查
- 參數範圍驗證
- 指令頻率限制
- 操作日誌記錄

## 📊 **系統特色 (System Features)**

### **✅ 完整中文化**
- 60+ 中文GM指令
- 雙語支援 (中/英文)
- 智能指令映射
- 完整說明文檔

### **✅ 專業架構**
- 模組化設計
- 可擴展架構
- 錯誤處理機制
- 編譯配置完整

### **✅ 企業級功能**
- 數位簽名支援
- 版本管理系統
- 自動化編譯
- 專業文檔

## 👨‍💻 **開發資訊 (Developer Info)**

```
專案名稱：RanGs Kill Animation System
開發者：Jy技術團隊 - TIAN0517
版本：1.0.0 (2025-06-22)
授權：© 2025 Jy技術團隊. All rights reserved.
支援：完整中文化GM系統 + Kill Animation + 職業解鎖
```

### **技術支援**
- GitHub: [TIAN0517/RanGs](https://github.com/TIAN0517/RanGs)
- Email: tian0517@jytech.team
- 團隊：Jy技術團隊工作室

---

**🎉 完整實現！所有功能已整合並可投入生產環境使用！**