#pragma once
#include <string>
#include <map>
#include <vector>

/**
 * GM指令中文化系統
 * 開發者：Jy技術團隊 - TIAN0517
 * 版本：1.0.0
 * 日期：2025-06-22
 */
class CGMCommandChinese 
{
private:
    std::map<std::string, std::string> m_chineseToEnglish;
    std::map<std::string, std::string> m_commandHelp;
    
public:
    CGMCommandChinese() {
        InitializeChineseCommands();
    }
    
    void InitializeChineseCommands() {
        // ===== 道具系統 =====
        m_chineseToEnglish["添加道具"] = "additem";
        m_chineseToEnglish["給道具"] = "additem";
        m_chineseToEnglish["新增道具"] = "additem";
        m_commandHelp["添加道具"] = "用法：/添加道具 [道具ID] [數量] [天數] - 添加指定道具到背包";
        
        m_chineseToEnglish["刪除道具"] = "delitem";
        m_chineseToEnglish["移除道具"] = "delitem";
        m_chineseToEnglish["刪道具"] = "delitem";
        m_commandHelp["刪除道具"] = "用法：/刪除道具 [道具ID] [數量] - 從背包刪除指定道具";
        
        m_chineseToEnglish["贈送道具"] = "giveitem";
        m_chineseToEnglish["送道具"] = "giveitem";
        m_commandHelp["贈送道具"] = "用法：/贈送道具 [玩家名] [道具ID] [數量] - 給其他玩家道具";
        
        // ===== 角色屬性 =====
        m_chineseToEnglish["給經驗"] = "givexp";
        m_chineseToEnglish["增加經驗"] = "givexp";
        m_chineseToEnglish["加經驗"] = "givexp";
        m_commandHelp["給經驗"] = "用法：/給經驗 [經驗值] - 增加角色經驗值";
        
        m_chineseToEnglish["設定等級"] = "setlevel";
        m_chineseToEnglish["改等級"] = "setlevel";
        m_chineseToEnglish["設等級"] = "setlevel";
        m_commandHelp["設定等級"] = "用法：/設定等級 [等級] - 直接設定角色等級";
        
        m_chineseToEnglish["設定血量"] = "sethp";
        m_chineseToEnglish["改血量"] = "sethp";
        m_chineseToEnglish["滿血"] = "fullhp";
        m_commandHelp["設定血量"] = "用法：/設定血量 [血量] - 修改角色生命值";
        
        m_chineseToEnglish["設定魔力"] = "setmp";
        m_chineseToEnglish["改魔力"] = "setmp";
        m_chineseToEnglish["滿魔"] = "fullmp";
        m_commandHelp["設定魔力"] = "用法：/設定魔力 [魔力] - 修改角色魔力值";
        
        // ===== 移動傳送 =====
        m_chineseToEnglish["傳送"] = "goto";
        m_chineseToEnglish["去"] = "goto";
        m_chineseToEnglish["瞬移"] = "goto";
        m_commandHelp["傳送"] = "用法：/傳送 [地圖ID] [X座標] [Y座標] - 傳送到指定地點";
        
        m_chineseToEnglish["召喚"] = "summon";
        m_chineseToEnglish["叫來"] = "summon";
        m_chineseToEnglish["召喚玩家"] = "summon";
        m_commandHelp["召喚"] = "用法：/召喚 [玩家名] - 召喚玩家到身邊";
        
        m_chineseToEnglish["傳送玩家"] = "teleport";
        m_chineseToEnglish["送玩家"] = "teleport";
        m_commandHelp["傳送玩家"] = "用法：/傳送玩家 [玩家名] [地圖ID] [X] [Y] - 傳送玩家到指定位置";
        
        // ===== 管理功能 =====
        m_chineseToEnglish["踢出"] = "kick";
        m_chineseToEnglish["踢人"] = "kick";
        m_chineseToEnglish["踢掉"] = "kick";
        m_commandHelp["踢出"] = "用法：/踢出 [玩家名] - 踢出指定玩家";
        
        m_chineseToEnglish["封號"] = "ban";
        m_chineseToEnglish["禁止"] = "ban";
        m_chineseToEnglish["封帳號"] = "ban";
        m_commandHelp["封號"] = "用法：/封號 [玩家名] [天數] - 封禁玩家帳號";
        
        m_chineseToEnglish["禁言"] = "mute";
        m_chineseToEnglish["閉嘴"] = "mute";
        m_commandHelp["禁言"] = "用法：/禁言 [玩家名] [分鐘] - 禁止玩家發言";
        
        // ===== 系統功能 =====
        m_chineseToEnglish["關機"] = "shutdown";
        m_chineseToEnglish["關服"] = "shutdown";
        m_chineseToEnglish["停機"] = "shutdown";
        m_commandHelp["關機"] = "用法：/關機 [秒數] - 關閉遊戲服務器";
        
        m_chineseToEnglish["公告"] = "notice";
        m_chineseToEnglish["通知"] = "notice";
        m_commandHelp["公告"] = "用法：/公告 [訊息] - 發送系統公告";
        
        m_chineseToEnglish["廣播"] = "broadcast";
        m_chineseToEnglish["全服廣播"] = "broadcast";
        m_commandHelp["廣播"] = "用法：/廣播 [訊息] - 全服廣播消息";
        
        // ===== 怪物系統 =====
        m_chineseToEnglish["召喚怪物"] = "summonmob";
        m_chineseToEnglish["叫怪"] = "summonmob";
        m_chineseToEnglish["生怪"] = "summonmob";
        m_commandHelp["召喚怪物"] = "用法：/召喚怪物 [怪物ID] [數量] - 召喚指定怪物";
        
        m_chineseToEnglish["清怪"] = "killall";
        m_chineseToEnglish["殺全部"] = "killall";
        m_commandHelp["清怪"] = "用法：/清怪 - 清除場上所有怪物";
        
        // ===== Kill Animation 專用 =====
        m_chineseToEnglish["添加擊殺卡片"] = "addkillcard";
        m_chineseToEnglish["給擊殺特效"] = "addkillcard";
        m_chineseToEnglish["擊殺動畫"] = "addkillcard";
        m_chineseToEnglish["特效卡"] = "addkillcard";
        m_commandHelp["添加擊殺卡片"] = "用法：/添加擊殺卡片 [類型] [天數] - 添加Kill Animation卡片";
        
        // ===== Job Unlocking Commands =====
        m_chineseToEnglish["開啟槍手"] = "unlockgunner";
        m_chineseToEnglish["解鎖槍手"] = "unlockgunner";
        m_commandHelp["開啟槍手"] = "用法：/開啟槍手 - 解鎖槍手職業";
        
        m_chineseToEnglish["開啟忍者"] = "unlockninja";
        m_chineseToEnglish["解鎖忍者"] = "unlockninja";
        m_commandHelp["開啟忍者"] = "用法：/開啟忍者 - 解鎖忍者職業";
        
        m_chineseToEnglish["解鎖所有職業"] = "unlockall";
        m_chineseToEnglish["開啟所有職業"] = "unlockall";
        m_commandHelp["解鎖所有職業"] = "用法：/解鎖所有職業 - 解鎖所有隱藏職業";
        
        // ===== 資訊查詢 =====
        m_chineseToEnglish["查看玩家"] = "viewuser";
        m_chineseToEnglish["玩家資訊"] = "viewuser";
        m_commandHelp["查看玩家"] = "用法：/查看玩家 [玩家名] - 查看玩家詳細資訊";
        
        m_chineseToEnglish["線上人數"] = "online";
        m_chineseToEnglish["在線人數"] = "online";
        m_commandHelp["線上人數"] = "用法：/線上人數 - 查看當前線上人數";
        
        m_chineseToEnglish["幫助"] = "help";
        m_chineseToEnglish["說明"] = "help";
        m_commandHelp["幫助"] = "用法：/幫助 - 顯示所有可用指令";
    }
    
    // 轉換中文指令為英文指令
    std::string ConvertChineseCommand(const std::string& chineseCmd) {
        auto it = m_chineseToEnglish.find(chineseCmd);
        if (it != m_chineseToEnglish.end()) {
            return it->second;
        }
        return chineseCmd;
    }
    
    // 檢查是否為中文指令
    bool IsChineseCommand(const std::string& cmd) {
        return m_chineseToEnglish.find(cmd) != m_chineseToEnglish.end();
    }
    
    // 獲取指令說明
    std::string GetCommandHelp(const std::string& chineseCmd) {
        auto it = m_commandHelp.find(chineseCmd);
        if (it != m_commandHelp.end()) {
            return it->second;
        }
        return "未知指令";
    }
    
    // 獲取所有中文指令列表
    std::vector<std::string> GetAllChineseCommands() {
        std::vector<std::string> commands;
        for (const auto& pair : m_chineseToEnglish) {
            commands.push_back(pair.first);
        }
        return commands;
    }
};

extern CGMCommandChinese g_GMCommandChinese;