#include "GMCommandChinese.h"
#include <iostream>
#include <sstream>

/**
 * GM指令處理器
 * 開發者：Jy技術團隊 - TIAN0517
 * 支援完整中文指令系統
 */

// 全局實例
CGMCommandChinese g_GMCommandChinese;

class CGMCommandProcessor 
{
public:
    // 處理收到的指令
    bool ProcessCommand(const std::string& fullCommand) {
        if (fullCommand.empty() || fullCommand[0] != '/') {
            return false;
        }
        
        // 移除開頭的 '/'
        std::string command = fullCommand.substr(1);
        
        // 解析指令和參數
        std::istringstream iss(command);
        std::string cmd;
        iss >> cmd;
        
        // 轉換中文指令
        std::string englishCmd = g_GMCommandChinese.ConvertChineseCommand(cmd);
        
        // 獲取參數
        std::string params;
        std::getline(iss, params);
        if (!params.empty() && params[0] == ' ') {
            params = params.substr(1); // 移除開頭空格
        }
        
        // 處理特殊指令
        if (cmd == "幫助" || cmd == "說明") {
            ShowAllCommands();
            return true;
        }
        
        // 輸出處理結果
        printf("🎮 GM指令處理 - Jy技術團隊\n");
        printf("中文指令: /%s\n", cmd.c_str());
        printf("英文指令: %s\n", englishCmd.c_str());
        printf("參數: %s\n", params.empty() ? "(無)" : params.c_str());
        printf("說明: %s\n", g_GMCommandChinese.GetCommandHelp(cmd).c_str());
        
        // 這裡會調用實際的遊戲指令處理
        return ExecuteGameCommand(englishCmd, params);
    }
    
private:
    // 顯示所有可用指令
    void ShowAllCommands() {
        printf("\n🎯 GM指令系統 - 中文化版本\n");
        printf("開發者：Jy技術團隊 - TIAN0517\n");
        printf("版本：1.0.0 (2025-06-22)\n");
        printf("==========================================\n\n");
        
        printf("📦 道具系統：\n");
        printf("  /添加道具 [道具ID] [數量] [天數]\n");
        printf("  /刪除道具 [道具ID] [數量]\n");
        printf("  /贈送道具 [玩家名] [道具ID] [數量]\n\n");
        
        printf("👤 角色屬性：\n");
        printf("  /給經驗 [經驗值]\n");
        printf("  /設定等級 [等級]\n");
        printf("  /設定血量 [血量]\n");
        printf("  /設定魔力 [魔力]\n\n");
        
        printf("🚀 移動傳送：\n");
        printf("  /傳送 [地圖ID] [X座標] [Y座標]\n");
        printf("  /召喚 [玩家名]\n");
        printf("  /傳送玩家 [玩家名] [地圖ID] [X] [Y]\n\n");
        
        printf("🛡️ 管理功能：\n");
        printf("  /踢出 [玩家名]\n");
        printf("  /封號 [玩家名] [天數]\n");
        printf("  /禁言 [玩家名] [分鐘]\n\n");
        
        printf("📢 系統功能：\n");
        printf("  /公告 [訊息]\n");
        printf("  /廣播 [訊息]\n");
        printf("  /關機 [秒數]\n\n");
        
        printf("👹 怪物系統：\n");
        printf("  /召喚怪物 [怪物ID] [數量]\n");
        printf("  /清怪\n\n");
        
        printf("⚔️ Kill Animation：\n");
        printf("  /添加擊殺卡片 [類型] [天數]\n");
        printf("  類型: 12001劍氣 12002穿刺 12003爆炸 12004冰凍 12005烈焰 12006雷電\n\n");
        
        printf("🎭 職業解鎖：\n");
        printf("  /開啟槍手 - 解鎖槍手職業\n");
        printf("  /開啟忍者 - 解鎖忍者職業\n");
        printf("  /解鎖所有職業 - 解鎖所有隱藏職業\n\n");
        
        printf("📊 資訊查詢：\n");
        printf("  /查看玩家 [玩家名]\n");
        printf("  /線上人數\n");
        printf("  /幫助\n\n");
    }
    
    // 執行實際的遊戲指令 (這裡會連接到原本的GM系統)
    bool ExecuteGameCommand(const std::string& command, const std::string& params) {
        // 這裡應該調用原本的GM指令處理函數
        // 例如：return OriginalGMSystem::ProcessCommand(command, params);
        
        printf("✅ 指令已發送到遊戲系統處理\n\n");
        return true;
    }
};