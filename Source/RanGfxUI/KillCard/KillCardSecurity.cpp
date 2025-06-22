#include "../../stdafx.h"
#include "KillCardSecurity.h"
#include "../../../SigmaCore/DebugInclude.h"
#include <psapi.h>
#include <tlhelp32.h>
#include <wincrypt.h>

// 静态实例
CKillCardSecurityManager* CKillCardSecurityManager::s_pInstance = NULL;
CKillCardAntiCheatAdapter* CKillCardAntiCheatAdapter::s_pInstance = NULL;
CKillCardSecurityLogger* CKillCardSecurityLogger::s_pInstance = NULL;

// ==============================================================================
// CKillCardSecurityManager Implementation
// ==============================================================================

CKillCardSecurityManager::CKillCardSecurityManager()
    : m_eSecurityLevel(SECURITY_LEVEL_BASIC)
    , m_dwCheckInterval(5000) // 5秒检查间隔
    , m_dwLastSecurityCheck(0)
    , m_bEnabled(TRUE)
    , m_hCurrentProcess(NULL)
    , m_dwProcessId(0)
{
    memset(&m_sStatus, 0, sizeof(m_sStatus));
    m_sStatus.eLevel = m_eSecurityLevel;
    strcpy(m_sStatus.szStatusMessage, "Security system initialized");
}

CKillCardSecurityManager::~CKillCardSecurityManager()
{
    Shutdown();
}

CKillCardSecurityManager& CKillCardSecurityManager::GetInstance()
{
    if (!s_pInstance)
    {
        s_pInstance = new CKillCardSecurityManager();
    }
    return *s_pInstance;
}

void CKillCardSecurityManager::ReleaseInstance()
{
    if (s_pInstance)
    {
        delete s_pInstance;
        s_pInstance = NULL;
    }
}

BOOL CKillCardSecurityManager::Initialize()
{
    // 获取当前进程信息
    m_hCurrentProcess = GetCurrentProcess();
    m_dwProcessId = GetCurrentProcessId();
    
    // 初始化可信模块列表
    m_vecTrustedModules.clear();
    
    // 添加系统DLL到可信列表
    m_vecAllowedDLLs.push_back("kernel32.dll");
    m_vecAllowedDLLs.push_back("user32.dll");
    m_vecAllowedDLLs.push_back("d3d9.dll");
    m_vecAllowedDLLs.push_back("d3dx9_43.dll");
    m_vecAllowedDLLs.push_back("dsound.dll");
    
    // 执行初始安全检查
    if (m_bEnabled)
    {
        if (!PerformSecurityCheck())
        {
            KILLCARD_LOG_SECURITY("Initial security check failed");
            return FALSE;
        }
    }
    
    KILLCARD_LOG_SECURITY("Security manager initialized successfully");
    return TRUE;
}

void CKillCardSecurityManager::Shutdown()
{
    m_vecTrustedModules.clear();
    m_vecAllowedDLLs.clear();
    
    if (m_hCurrentProcess)
    {
        // 不需要关闭当前进程句柄
        m_hCurrentProcess = NULL;
    }
    
    KILLCARD_LOG_SECURITY("Security manager shutdown");
}

BOOL CKillCardSecurityManager::PerformSecurityCheck()
{
    if (!m_bEnabled)
        return TRUE;
    
    DWORD dwCurrentTime = GetTickCount();
    
    // 检查是否需要执行安全检查
    if (dwCurrentTime - m_dwLastSecurityCheck < m_dwCheckInterval)
        return TRUE;
    
    m_dwLastSecurityCheck = dwCurrentTime;
    m_sStatus.dwLastCheckTime = dwCurrentTime;
    m_sStatus.dwThreatFlags = THREAT_NONE;
    
    BOOL bResult = TRUE;
    
    // 根据安全级别执行不同程度的检查
    switch (m_eSecurityLevel)
    {
    case SECURITY_LEVEL_BASIC:
        bResult &= VerifyProcessIntegrity();
        bResult &= CheckAntiCheatCompatibility();
        break;
        
    case SECURITY_LEVEL_ENHANCED:
        bResult &= VerifyProcessIntegrity();
        bResult &= CheckMemoryIntegrity();
        bResult &= CheckAntiCheatCompatibility();
        bResult &= DetectDebugger();
        break;
        
    case SECURITY_LEVEL_STRICT:
        bResult &= VerifySystemIntegrity();
        bResult &= VerifyProcessIntegrity();
        bResult &= CheckMemoryIntegrity();
        bResult &= DetectCodeModification();
        bResult &= CheckAntiCheatCompatibility();
        bResult &= DetectDebugger();
        bResult &= CheckForHooks();
        break;
        
    default:
        break;
    }
    
    // 更新状态
    m_sStatus.bIntegrityVerified = bResult;
    
    if (bResult)
    {
        strcpy(m_sStatus.szStatusMessage, "All security checks passed");
    }
    else
    {
        strcpy(m_sStatus.szStatusMessage, "Security threats detected");
        KILLCARD_LOG_THREAT(static_cast<EKILLCARD_SECURITY_THREAT>(m_sStatus.dwThreatFlags), 
                           "Security check failed");
    }
    
    return bResult;
}

BOOL CKillCardSecurityManager::VerifySystemIntegrity()
{
    BOOL bResult = TRUE;
    
    // 检查进程列表
    if (!CheckProcessList())
    {
        m_sStatus.dwThreatFlags |= THREAT_PROCESS_HOLLOWING;
        bResult = FALSE;
    }
    
    // 检查加载的模块
    if (!CheckLoadedModules())
    {
        m_sStatus.dwThreatFlags |= THREAT_DLL_INJECTION;
        bResult = FALSE;
    }
    
    // 检查系统文件
    if (!CheckSystemFiles())
    {
        m_sStatus.dwThreatFlags |= THREAT_CODE_MODIFICATION;
        bResult = FALSE;
    }
    
    return bResult;
}

BOOL CKillCardSecurityManager::CheckAntiCheatCompatibility()
{
    // 检查反作弊系统兼容性
    CKillCardAntiCheatAdapter& adapter = CKillCardAntiCheatAdapter::GetInstance();
    
    if (!adapter.DetectAntiCheatSystem())
    {
        m_sStatus.bAntiCheatCompatible = TRUE;
        return TRUE;
    }
    
    // 配置兼容模式
    BOOL bCompatible = adapter.EnableCompatibilityMode(adapter.GetDetectedSystem());
    m_sStatus.bAntiCheatCompatible = bCompatible;
    
    if (!bCompatible)
    {
        KILLCARD_LOG_SECURITY("Anti-cheat compatibility check failed");
    }
    
    return bCompatible;
}

BOOL CKillCardSecurityManager::VerifyProcessIntegrity()
{
    // 检查进程内存完整性
    MEMORY_BASIC_INFORMATION mbi;
    SIZE_T dwAddress = 0;
    
    while (VirtualQuery((LPCVOID)dwAddress, &mbi, sizeof(mbi)) == sizeof(mbi))
    {
        // 检查可执行区域
        if (mbi.State == MEM_COMMIT && (mbi.Protect & PAGE_EXECUTE))
        {
            // 检查是否为已知模块
            HMODULE hModule = (HMODULE)mbi.AllocationBase;
            if (!IsModuleTrusted(hModule))
            {
                char szModuleName[MAX_PATH];
                if (GetModuleFileNameA(hModule, szModuleName, MAX_PATH))
                {
                    // 检查是否在允许列表中
                    std::string strModuleName = szModuleName;
                    size_t pos = strModuleName.find_last_of("\\");
                    if (pos != std::string::npos)
                    {
                        strModuleName = strModuleName.substr(pos + 1);
                    }
                    
                    BOOL bAllowed = FALSE;
                    for (const std::string& allowedDLL : m_vecAllowedDLLs)
                    {
                        if (_stricmp(strModuleName.c_str(), allowedDLL.c_str()) == 0)
                        {
                            bAllowed = TRUE;
                            break;
                        }
                    }
                    
                    if (!bAllowed)
                    {
                        KILLCARD_LOG_THREAT(THREAT_DLL_INJECTION, szModuleName);
                        m_sStatus.dwThreatFlags |= THREAT_DLL_INJECTION;
                    }
                }
            }
        }
        
        dwAddress += mbi.RegionSize;
    }
    
    return (m_sStatus.dwThreatFlags & THREAT_DLL_INJECTION) == 0;
}

BOOL CKillCardSecurityManager::DetectDebugger()
{
    // 检查调试器存在
    if (IsDebuggerPresent())
    {
        m_sStatus.dwThreatFlags |= THREAT_DEBUGGER_PRESENCE;
        KILLCARD_LOG_THREAT(THREAT_DEBUGGER_PRESENCE, "IsDebuggerPresent() returned TRUE");
        return FALSE;
    }
    
    // 检查远程调试器
    BOOL bRemoteDebugger = FALSE;
    if (CheckRemoteDebuggerPresent(m_hCurrentProcess, &bRemoteDebugger) && bRemoteDebugger)
    {
        m_sStatus.dwThreatFlags |= THREAT_DEBUGGER_PRESENCE;
        KILLCARD_LOG_THREAT(THREAT_DEBUGGER_PRESENCE, "Remote debugger detected");
        return FALSE;
    }
    
    // 检查调试标志
    HANDLE hProcess = GetCurrentProcess();
    DWORD dwFlags = 0;
    if (GetProcessInformation && GetProcessInformation(hProcess, ProcessDebugFlags, &dwFlags, sizeof(dwFlags), NULL))
    {
        if (dwFlags == 0) // 调试标志被清除表示有调试器
        {
            m_sStatus.dwThreatFlags |= THREAT_DEBUGGER_PRESENCE;
            KILLCARD_LOG_THREAT(THREAT_DEBUGGER_PRESENCE, "Debug flags indicate debugger presence");
            return FALSE;
        }
    }
    
    return TRUE;
}

BOOL CKillCardSecurityManager::CheckProcessList()
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return FALSE;
    
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    
    // 可疑进程名称列表
    const char* suspiciousProcesses[] = {
        "ollydbg.exe",
        "x64dbg.exe", 
        "x32dbg.exe",
        "windbg.exe",
        "ida.exe",
        "ida64.exe",
        "cheatengine.exe",
        "artmoney.exe",
        "speedhack.exe"
    };
    
    BOOL bResult = TRUE;
    
    if (Process32First(hSnapshot, &pe32))
    {
        do
        {
            // 检查进程名称
            for (int i = 0; i < sizeof(suspiciousProcesses) / sizeof(char*); i++)
            {
                if (_stricmp(pe32.szExeFile, suspiciousProcesses[i]) == 0)
                {
                    KILLCARD_LOG_THREAT(THREAT_DEBUGGER_PRESENCE, pe32.szExeFile);
                    m_sStatus.dwThreatFlags |= THREAT_DEBUGGER_PRESENCE;
                    bResult = FALSE;
                }
            }
        } while (Process32Next(hSnapshot, &pe32));
    }
    
    CloseHandle(hSnapshot);
    return bResult;
}

// ==============================================================================
// CKillCardAntiCheatAdapter Implementation  
// ==============================================================================

CKillCardAntiCheatAdapter::CKillCardAntiCheatAdapter()
    : m_eDetectedSystem(ANTICHEAT_NONE)
    , m_bCompatibilityMode(FALSE)
{
}

CKillCardAntiCheatAdapter::~CKillCardAntiCheatAdapter()
{
}

CKillCardAntiCheatAdapter& CKillCardAntiCheatAdapter::GetInstance()
{
    if (!s_pInstance)
    {
        s_pInstance = new CKillCardAntiCheatAdapter();
    }
    return *s_pInstance;
}

void CKillCardAntiCheatAdapter::ReleaseInstance()
{
    if (s_pInstance)
    {
        delete s_pInstance;
        s_pInstance = NULL;
    }
}

BOOL CKillCardAntiCheatAdapter::DetectAntiCheatSystem()
{
    // 检查XIGNCODE
    if (CheckForXIGNCODE())
    {
        m_eDetectedSystem = ANTICHEAT_XIGNCODE;
        m_strSystemInfo = "XIGNCODE3 detected";
        return TRUE;
    }
    
    // 检查GameGuard
    if (CheckForGameGuard())
    {
        m_eDetectedSystem = ANTICHEAT_GAMEGUARD;
        m_strSystemInfo = "GameGuard detected";
        return TRUE;
    }
    
    // 检查HackShield
    if (CheckForHackShield())
    {
        m_eDetectedSystem = ANTICHEAT_HACKSHIELD;
        m_strSystemInfo = "HackShield detected";
        return TRUE;
    }
    
    // 检查BattlEye
    if (CheckForBattlEye())
    {
        m_eDetectedSystem = ANTICHEAT_BATTLEYE;
        m_strSystemInfo = "BattlEye detected";
        return TRUE;
    }
    
    // 检查EasyAntiCheat
    if (CheckForEAC())
    {
        m_eDetectedSystem = ANTICHEAT_EAC;
        m_strSystemInfo = "EasyAntiCheat detected";
        return TRUE;
    }
    
    m_eDetectedSystem = ANTICHEAT_NONE;
    m_strSystemInfo = "No anti-cheat system detected";
    return FALSE;
}

BOOL CKillCardAntiCheatAdapter::CheckForXIGNCODE()
{
    // 检查XIGNCODE相关模块和进程
    const char* xigncodeModules[] = {
        "xigncode3.dll",
        "xigncode.dll",
        "xi3core.dll"
    };
    
    const char* xigncodeProcesses[] = {
        "xigncode3.exe",
        "xi3.exe"
    };
    
    if (CheckModules(xigncodeModules, 3) || CheckProcesses(xigncodeProcesses, 2))
    {
        KILLCARD_LOG_SECURITY("XIGNCODE3 anti-cheat detected");
        return TRUE;
    }
    
    return FALSE;
}

BOOL CKillCardAntiCheatAdapter::CheckProcesses(const char** processNames, int count)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return FALSE;
    
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    
    BOOL bFound = FALSE;
    
    if (Process32First(hSnapshot, &pe32))
    {
        do
        {
            for (int i = 0; i < count; i++)
            {
                if (_stricmp(pe32.szExeFile, processNames[i]) == 0)
                {
                    bFound = TRUE;
                    break;
                }
            }
        } while (Process32Next(hSnapshot, &pe32) && !bFound);
    }
    
    CloseHandle(hSnapshot);
    return bFound;
}

BOOL CKillCardAntiCheatAdapter::CheckModules(const char** moduleNames, int count)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return FALSE;
    
    MODULEENTRY32 me32;
    me32.dwSize = sizeof(MODULEENTRY32);
    
    BOOL bFound = FALSE;
    
    if (Module32First(hSnapshot, &me32))
    {
        do
        {
            for (int i = 0; i < count; i++)
            {
                if (_stricmp(me32.szModule, moduleNames[i]) == 0)
                {
                    bFound = TRUE;
                    break;
                }
            }
        } while (Module32Next(hSnapshot, &me32) && !bFound);
    }
    
    CloseHandle(hSnapshot);
    return bFound;
}

// ==============================================================================
// CKillCardSecurityLogger Implementation
// ==============================================================================

CKillCardSecurityLogger::CKillCardSecurityLogger()
    : m_pLogFile(NULL)
    , m_bEnabled(TRUE)
{
    InitializeCriticalSection(&m_csLogLock);
}

CKillCardSecurityLogger::~CKillCardSecurityLogger()
{
    Shutdown();
    DeleteCriticalSection(&m_csLogLock);
}

CKillCardSecurityLogger& CKillCardSecurityLogger::GetInstance()
{
    if (!s_pInstance)
    {
        s_pInstance = new CKillCardSecurityLogger();
    }
    return *s_pInstance;
}

void CKillCardSecurityLogger::ReleaseInstance()
{
    if (s_pInstance)
    {
        delete s_pInstance;
        s_pInstance = NULL;
    }
}

BOOL CKillCardSecurityLogger::Initialize(const char* szLogFilePath)
{
    if (!szLogFilePath)
        return FALSE;
    
    m_strLogFilePath = szLogFilePath;
    
    EnterCriticalSection(&m_csLogLock);
    
    if (m_pLogFile)
    {
        fclose(m_pLogFile);
    }
    
    m_pLogFile = fopen(szLogFilePath, "a");
    
    LeaveCriticalSection(&m_csLogLock);
    
    if (!m_pLogFile)
        return FALSE;
    
    LogSecurityEvent("Security logger initialized");
    return TRUE;
}

void CKillCardSecurityLogger::Shutdown()
{
    LogSecurityEvent("Security logger shutdown");
    
    EnterCriticalSection(&m_csLogLock);
    
    if (m_pLogFile)
    {
        fclose(m_pLogFile);
        m_pLogFile = NULL;
    }
    
    LeaveCriticalSection(&m_csLogLock);
}

void CKillCardSecurityLogger::LogSecurityEvent(const char* szEvent, EKILLCARD_SECURITY_THREAT eThreat)
{
    if (!m_bEnabled || !m_pLogFile)
        return;
    
    std::string message = szEvent;
    if (eThreat != THREAT_NONE)
    {
        message += " [" + ThreatTypeToString(eThreat) + "]";
    }
    
    WriteLogEntry("INFO", message.c_str());
}

void CKillCardSecurityLogger::LogThreatDetection(EKILLCARD_SECURITY_THREAT eThreat, const char* szDetails)
{
    if (!m_bEnabled || !m_pLogFile)
        return;
    
    std::string message = "THREAT DETECTED: " + ThreatTypeToString(eThreat);
    if (szDetails)
    {
        message += " - " + std::string(szDetails);
    }
    
    WriteLogEntry("THREAT", message.c_str());
}

void CKillCardSecurityLogger::WriteLogEntry(const char* szLevel, const char* szMessage)
{
    EnterCriticalSection(&m_csLogLock);
    
    if (m_pLogFile)
    {
        fprintf(m_pLogFile, "[%s] %s: %s\n", GetTimestamp().c_str(), szLevel, szMessage);
        fflush(m_pLogFile);
    }
    
    LeaveCriticalSection(&m_csLogLock);
}

std::string CKillCardSecurityLogger::GetTimestamp()
{
    SYSTEMTIME st;
    GetLocalTime(&st);
    
    char buffer[64];
    sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
            st.wYear, st.wMonth, st.wDay,
            st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    
    return std::string(buffer);
}

std::string CKillCardSecurityLogger::ThreatTypeToString(EKILLCARD_SECURITY_THREAT eThreat)
{
    switch (eThreat)
    {
    case THREAT_MEMORY_INJECTION: return "MEMORY_INJECTION";
    case THREAT_CODE_MODIFICATION: return "CODE_MODIFICATION";
    case THREAT_PROCESS_HOLLOWING: return "PROCESS_HOLLOWING";
    case THREAT_DLL_INJECTION: return "DLL_INJECTION";
    case THREAT_HOOK_DETECTION: return "HOOK_DETECTION";
    case THREAT_DEBUGGER_PRESENCE: return "DEBUGGER_PRESENCE";
    default: return "UNKNOWN";
    }
}

// 全局实例
CKillCardSecurityManager& g_KillCardSecurity = CKillCardSecurityManager::GetInstance();
CKillCardAntiCheatAdapter& g_KillCardAntiCheat = CKillCardAntiCheatAdapter::GetInstance();
CKillCardSecurityLogger& g_KillCardSecurityLog = CKillCardSecurityLogger::GetInstance();