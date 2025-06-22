#pragma once

#include "../stdafx.h"

/**
 * 击杀卡片安全管理系统
 * 实现反作弊兼容性和安全防护机制
 */

// 安全检查级别
enum EKILLCARD_SECURITY_LEVEL
{
    SECURITY_LEVEL_DISABLED = 0,     // 禁用安全检查
    SECURITY_LEVEL_BASIC,            // 基础安全检查
    SECURITY_LEVEL_ENHANCED,         // 增强安全检查
    SECURITY_LEVEL_STRICT            // 严格安全检查
};

// 安全威胁类型
enum EKILLCARD_SECURITY_THREAT
{
    THREAT_NONE = 0,
    THREAT_MEMORY_INJECTION = 1,     // 内存注入检测
    THREAT_CODE_MODIFICATION = 2,    // 代码修改检测
    THREAT_PROCESS_HOLLOWING = 4,    // 进程挖空检测
    THREAT_DLL_INJECTION = 8,        // DLL注入检测
    THREAT_HOOK_DETECTION = 16,      // Hook检测
    THREAT_DEBUGGER_PRESENCE = 32    // 调试器检测
};

// 安全状态信息
struct SKillCardSecurityStatus
{
    EKILLCARD_SECURITY_LEVEL eLevel;
    DWORD dwThreatFlags;             // 威胁标志位
    BOOL bIntegrityVerified;         // 完整性验证状态
    BOOL bAntiCheatCompatible;       // 反作弊兼容性
    DWORD dwLastCheckTime;           // 上次检查时间
    char szStatusMessage[256];       // 状态消息
};

/**
 * 击杀卡片安全管理器
 * 确保系统安全性和反作弊兼容性
 */
class CKillCardSecurityManager
{
private:
    static CKillCardSecurityManager* s_pInstance;
    
    EKILLCARD_SECURITY_LEVEL m_eSecurityLevel;
    SKillCardSecurityStatus m_sStatus;
    
    // 安全检查相关
    DWORD m_dwCheckInterval;         // 检查间隔
    DWORD m_dwLastSecurityCheck;     // 上次安全检查时间
    BOOL m_bEnabled;                 // 是否启用安全管理
    
    // 进程保护
    HANDLE m_hCurrentProcess;        // 当前进程句柄
    DWORD m_dwProcessId;             // 进程ID
    
    // 模块验证
    std::vector<HMODULE> m_vecTrustedModules;  // 可信模块列表
    std::vector<std::string> m_vecAllowedDLLs; // 允许的DLL列表

public:
    CKillCardSecurityManager();
    ~CKillCardSecurityManager();
    
    static CKillCardSecurityManager& GetInstance();
    static void ReleaseInstance();

public:
    // 初始化和清理
    BOOL Initialize();
    void Shutdown();
    
    // 安全级别设置
    void SetSecurityLevel(EKILLCARD_SECURITY_LEVEL eLevel) { m_eSecurityLevel = eLevel; }
    EKILLCARD_SECURITY_LEVEL GetSecurityLevel() const { return m_eSecurityLevel; }
    
    void SetEnabled(BOOL bEnabled) { m_bEnabled = bEnabled; }
    BOOL IsEnabled() const { return m_bEnabled; }
    
    // 安全检查
    BOOL PerformSecurityCheck();
    BOOL VerifySystemIntegrity();
    BOOL CheckAntiCheatCompatibility();
    const SKillCardSecurityStatus& GetSecurityStatus() const { return m_sStatus; }
    
    // 进程保护
    BOOL VerifyProcessIntegrity();
    BOOL CheckMemoryIntegrity();
    BOOL DetectCodeModification();
    
    // 模块管理
    BOOL RegisterTrustedModule(HMODULE hModule);
    BOOL VerifyModuleIntegrity(HMODULE hModule);
    BOOL IsModuleTrusted(HMODULE hModule) const;
    
    // 反调试保护
    BOOL DetectDebugger();
    BOOL CheckForHooks();
    BOOL VerifyProcessEnvironment();
    
    // 文件完整性
    BOOL VerifyFileIntegrity(const char* szFilePath);
    BOOL CalculateFileHash(const char* szFilePath, std::string& outHash);
    
    // 内存保护
    BOOL ProtectCriticalMemory(void* pAddress, SIZE_T dwSize);
    BOOL UnprotectCriticalMemory(void* pAddress, SIZE_T dwSize);
    BOOL VerifyMemoryRegion(void* pAddress, SIZE_T dwSize);

private:
    // 内部安全检查方法
    BOOL CheckProcessList();
    BOOL CheckLoadedModules();
    BOOL CheckSystemFiles();
    BOOL CheckRegistryEntries();
    
    // 威胁检测
    BOOL DetectMemoryInjection();
    BOOL DetectProcessHollowing();
    BOOL DetectDLLInjection();
    
    // 哈希计算
    BOOL CalculateMD5Hash(const void* pData, SIZE_T dwSize, std::string& outHash);
    BOOL CalculateSHA256Hash(const void* pData, SIZE_T dwSize, std::string& outHash);
    
    // 系统API检查
    BOOL VerifySystemAPIs();
    BOOL CheckAPIHooks();
};

/**
 * 反作弊系统接口适配器
 * 与主流反作弊系统的兼容性接口
 */
class CKillCardAntiCheatAdapter
{
private:
    static CKillCardAntiCheatAdapter* s_pInstance;
    
    // 已知反作弊系统
    enum EANTICHEAT_SYSTEM
    {
        ANTICHEAT_NONE = 0,
        ANTICHEAT_XIGNCODE,          // XIGNCODE
        ANTICHEAT_GAMEGUARD,         // GameGuard  
        ANTICHEAT_HACKSHIELD,        // HackShield
        ANTICHEAT_BATTLEYE,          // BattlEye
        ANTICHEAT_EAC,               // EasyAntiCheat
        ANTICHEAT_VANGUARD,          // Riot Vanguard
        ANTICHEAT_CUSTOM             // 自定义反作弊
    };
    
    EANTICHEAT_SYSTEM m_eDetectedSystem;
    BOOL m_bCompatibilityMode;
    std::string m_strSystemInfo;

public:
    CKillCardAntiCheatAdapter();
    ~CKillCardAntiCheatAdapter();
    
    static CKillCardAntiCheatAdapter& GetInstance();
    static void ReleaseInstance();

public:
    // 反作弊系统检测
    BOOL DetectAntiCheatSystem();
    EANTICHEAT_SYSTEM GetDetectedSystem() const { return m_eDetectedSystem; }
    const char* GetSystemName() const;
    
    // 兼容性管理
    BOOL EnableCompatibilityMode(EANTICHEAT_SYSTEM eSystem);
    void DisableCompatibilityMode();
    BOOL IsCompatibilityModeEnabled() const { return m_bCompatibilityMode; }
    
    // 系统特定适配
    BOOL ConfigureForXIGNCODE();
    BOOL ConfigureForGameGuard();
    BOOL ConfigureForHackShield();
    BOOL ConfigureForBattlEye();
    BOOL ConfigureForEAC();
    BOOL ConfigureForVanguard();
    
    // 安全API使用
    BOOL UseSafeAPI();
    BOOL AvoidSuspiciousOperations();
    BOOL LimitSystemAccess();

private:
    // 系统检测方法
    BOOL CheckForXIGNCODE();
    BOOL CheckForGameGuard();
    BOOL CheckForHackShield();
    BOOL CheckForBattlEye();
    BOOL CheckForEAC();
    BOOL CheckForVanguard();
    
    // 进程和模块检测
    BOOL CheckProcesses(const char** processNames, int count);
    BOOL CheckModules(const char** moduleNames, int count);
    BOOL CheckServices(const char** serviceNames, int count);
};

/**
 * 代码完整性验证器
 * 验证插件代码未被修改
 */
class CKillCardIntegrityVerifier
{
private:
    struct SModuleInfo
    {
        HMODULE hModule;
        std::string strPath;
        std::string strExpectedHash;
        DWORD dwSize;
        DWORD dwTimestamp;
    };
    
    std::vector<SModuleInfo> m_vecModules;
    std::string m_strSignature;

public:
    CKillCardIntegrityVerifier();
    ~CKillCardIntegrityVerifier();
    
    // 模块注册
    BOOL RegisterModule(HMODULE hModule, const char* szExpectedHash = nullptr);
    BOOL UnregisterModule(HMODULE hModule);
    
    // 完整性验证
    BOOL VerifyAllModules();
    BOOL VerifyModule(HMODULE hModule);
    BOOL CalculateModuleHash(HMODULE hModule, std::string& outHash);
    
    // 签名验证
    BOOL SetSignature(const char* szSignature) { m_strSignature = szSignature; return TRUE; }
    BOOL VerifySignature();
    
    // 时间戳检查
    BOOL CheckModuleTimestamp(HMODULE hModule);
    BOOL VerifyModificationTime(const char* szFilePath);

private:
    BOOL GetModuleInfo(HMODULE hModule, SModuleInfo& outInfo);
    BOOL CalculatePEHash(HMODULE hModule, std::string& outHash);
};

/**
 * 安全日志系统
 * 记录安全相关事件和威胁检测
 */
class CKillCardSecurityLogger
{
private:
    static CKillCardSecurityLogger* s_pInstance;
    
    std::string m_strLogFilePath;
    FILE* m_pLogFile;
    CRITICAL_SECTION m_csLogLock;
    BOOL m_bEnabled;

public:
    CKillCardSecurityLogger();
    ~CKillCardSecurityLogger();
    
    static CKillCardSecurityLogger& GetInstance();
    static void ReleaseInstance();

public:
    // 日志管理
    BOOL Initialize(const char* szLogFilePath);
    void Shutdown();
    void SetEnabled(BOOL bEnabled) { m_bEnabled = bEnabled; }
    
    // 日志记录
    void LogSecurityEvent(const char* szEvent, EKILLCARD_SECURITY_THREAT eThreat = THREAT_NONE);
    void LogThreatDetection(EKILLCARD_SECURITY_THREAT eThreat, const char* szDetails);
    void LogSystemInfo(const char* szInfo);
    void LogError(const char* szError);
    
    // 日志分析
    BOOL AnalyzeLogs(std::vector<std::string>& outSuspiciousEvents);
    void ClearOldLogs(DWORD dwDaysToKeep);

private:
    void WriteLogEntry(const char* szLevel, const char* szMessage);
    std::string GetTimestamp();
    std::string ThreatTypeToString(EKILLCARD_SECURITY_THREAT eThreat);
};

// 安全检查宏定义
#define KILLCARD_SECURITY_CHECK() CKillCardSecurityManager::GetInstance().PerformSecurityCheck()
#define KILLCARD_VERIFY_INTEGRITY() CKillCardSecurityManager::GetInstance().VerifySystemIntegrity()
#define KILLCARD_LOG_SECURITY(event) CKillCardSecurityLogger::GetInstance().LogSecurityEvent(event)
#define KILLCARD_LOG_THREAT(threat, details) CKillCardSecurityLogger::GetInstance().LogThreatDetection(threat, details)

// 安全模式执行宏
#define KILLCARD_SECURE_EXECUTION_BEGIN() \
    if (CKillCardSecurityManager::GetInstance().IsEnabled()) { \
        if (!KILLCARD_SECURITY_CHECK()) { \
            KILLCARD_LOG_SECURITY("Security check failed during execution"); \
            return FALSE; \
        } \
    }

#define KILLCARD_SECURE_EXECUTION_END() \
    if (CKillCardSecurityManager::GetInstance().IsEnabled()) { \
        KILLCARD_VERIFY_INTEGRITY(); \
    }

// 反作弊兼容性检查
#define KILLCARD_ANTICHEAT_COMPATIBLE() \
    CKillCardAntiCheatAdapter::GetInstance().EnableCompatibilityMode( \
        CKillCardAntiCheatAdapter::GetInstance().GetDetectedSystem())

// 全局安全管理器访问
extern CKillCardSecurityManager& g_KillCardSecurity;
extern CKillCardAntiCheatAdapter& g_KillCardAntiCheat;
extern CKillCardSecurityLogger& g_KillCardSecurityLog;