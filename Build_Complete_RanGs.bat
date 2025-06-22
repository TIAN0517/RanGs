@echo off
:: 文件：Build_Complete_RanGs.bat
:: RanGs 完整編譯腳本 - Jy技術團隊

echo ==========================================
echo 🚀 RanGs Kill Animation System 完整編譯
echo 開發者：Jy技術團隊 - TIAN0517
echo 版本：1.0.0 (2025-06-22)
echo ==========================================

:: 設定編譯環境
set PROJECT_NAME=RanGs Kill Animation System
set DEVELOPER=Jy技術團隊 - TIAN0517
set BUILD_DATE=%DATE% %TIME%

echo 📅 編譯時間：%BUILD_DATE%
echo 👨‍💻 開發者：%DEVELOPER%
echo.

:: 創建必要目錄
echo 📁 創建編譯目錄...
if not exist "build" mkdir build
if not exist "bin" mkdir bin
if not exist "Resources" mkdir Resources
if not exist "Certificates" mkdir Certificates
if not exist "Scripts" mkdir Scripts

:: 複製資源文件 (圖示等)
echo 🎨 準備資源文件...
if exist "Resources\RanGs_Icon.ico" (
    echo ✅ 找到專案圖示
) else (
    echo ⚠️  請將圖示檔案放置到 Resources\RanGs_Icon.ico
)

:: Linux 編譯
echo.
echo 🐧 Linux 版本編譯...
g++ -c Source/EngineLib/GMSystem/GMCommandProcessor.cpp ^
    -ISource/EngineLib -ISource -std=c++11 -Wall -DLINUX_BUILD ^
    -o build/GMCommandProcessor.o

if %ERRORLEVEL% == 0 (
    echo ✅ GM 指令系統編譯成功
    
    g++ -c Source/RanGfxUI/OldUI/Interface/KillAnimationSystem.cpp ^
        -ISource/EngineLib -ISource -std=c++11 -Wall -DLINUX_BUILD ^
        -o build/KillAnimationSystem.o
        
    if %ERRORLEVEL% == 0 (
        echo ✅ Kill Animation 系統編譯成功
        
        :: 主程式連結
        g++ -o bin/RanGs_Linux ^
            build/GMCommandProcessor.o ^
            build/KillAnimationSystem.o ^
            -ISource/EngineLib -ISource -std=c++11 -Wall -DLINUX_BUILD
            
        if %ERRORLEVEL% == 0 (
            echo ✅ Linux 可執行檔創建成功：bin/RanGs_Linux
        )
    )
) else (
    echo ❌ 編譯失敗
)

:: Windows 編譯 (需要 Visual Studio)
echo.
echo 🪟 Windows 版本編譯...
if exist "Source\Smtm_2008.sln" (
    echo 找到 Visual Studio 專案檔案
    echo 請使用 Visual Studio 2008 開啟 Source\Smtm_2008.sln 進行編譯
    echo 新增的檔案需要加入專案：
    echo   - Source\EngineLib\GMSystem\GMCommandChinese.h
    echo   - Source\EngineLib\GMSystem\GMCommandProcessor.cpp
    echo   - Source\EngineLib\Common\ProjectInfo.h
    echo   - Source\RanGs.rc
) else (
    echo ⚠️  未找到 Visual Studio 專案檔案
)

:: 創建版本資訊檔案
echo.
echo 📋 創建版本資訊...
echo RanGs Kill Animation System > bin\VERSION.txt
echo 版本：1.0.0 >> bin\VERSION.txt
echo 開發者：Jy技術團隊 - TIAN0517 >> bin\VERSION.txt
echo 編譯時間：%BUILD_DATE% >> bin\VERSION.txt
echo 功能：完整中文化GM指令系統 + 六大擊殺動畫 + 職業解鎖 >> bin\VERSION.txt
echo ============================================== >> bin\VERSION.txt
echo. >> bin\VERSION.txt
echo 中文指令支援： >> bin\VERSION.txt
echo   - 道具系統：/添加道具, /刪除道具, /贈送道具 >> bin\VERSION.txt
echo   - 角色屬性：/給經驗, /設定等級, /設定血量, /設定魔力 >> bin\VERSION.txt
echo   - 移動傳送：/傳送, /召喚, /傳送玩家 >> bin\VERSION.txt
echo   - 管理功能：/踢出, /封號, /禁言 >> bin\VERSION.txt
echo   - 系統功能：/公告, /廣播, /關機 >> bin\VERSION.txt
echo   - 怪物系統：/召喚怪物, /清怪 >> bin\VERSION.txt
echo   - Kill Animation：/添加擊殺卡片 >> bin\VERSION.txt
echo   - 職業解鎖：/開啟槍手, /開啟忍者, /解鎖所有職業 >> bin\VERSION.txt
echo   - 資訊查詢：/查看玩家, /線上人數, /幫助 >> bin\VERSION.txt

echo.
echo 🎉 編譯完成！
echo.
echo 📁 輸出文件：
echo   - bin\RanGs_Linux (Linux版本)
echo   - bin\VERSION.txt (版本資訊)
echo.
echo 📋 新增系統檔案：
echo   - Source\EngineLib\GMSystem\GMCommandChinese.h (中文指令系統)
echo   - Source\EngineLib\GMSystem\GMCommandProcessor.cpp (指令處理器)
echo   - Source\EngineLib\Common\ProjectInfo.h (專案資訊)
echo   - Source\RanGs.rc (Windows 資源檔)
echo   - Scripts\CreateSelfSignedCert.ps1 (憑證產生器)
echo.
echo 🔐 數位簽名：
echo   執行 powershell -ExecutionPolicy Bypass -File Scripts\CreateSelfSignedCert.ps1
echo   然後使用 Scripts\SignExecutable.bat 簽名執行檔
echo.
echo 🎯 使用說明：
echo   所有GM指令現在都支援中文，例如：
echo   /添加擊殺卡片 12001 60 (添加劍氣砍殺特效，60天)
echo   /開啟槍手 (解鎖槍手職業)
echo   /幫助 (顯示所有中文指令)
echo.
pause