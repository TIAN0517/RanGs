# PowerShell 腳本：創建自簽名數位憑證

param(
    [string]$CertName = "Jy技術團隊",
    [string]$DeveloperID = "TIAN0517"
)

Write-Host "🔐 創建自簽名數位憑證 - $CertName" -ForegroundColor Green
Write-Host "開發者ID: $DeveloperID" -ForegroundColor Yellow

# 設定憑證參數
$CertParams = @{
    Subject = "CN=Jy技術團隊, O=Jy技術團隊工作室, OU=軟體開發部, C=TW"
    FriendlyName = "Jy技術團隊程式碼簽章憑證"
    NotAfter = (Get-Date).AddYears(3)
    CertStoreLocation = "Cert:\CurrentUser\My"
    KeyUsage = "DigitalSignature", "KeyEncipherment"
    KeySpec = "Signature"
    KeyLength = 2048
    KeyAlgorithm = "RSA"
    HashAlgorithm = "SHA256"
    Provider = "Microsoft Enhanced RSA and AES Cryptographic Provider"
    KeyExportPolicy = "Exportable"
}

try {
    # 創建憑證
    Write-Host "正在創建憑證..." -ForegroundColor Yellow
    $Cert = New-SelfSignedCertificate @CertParams
    
    Write-Host "✅ 憑證創建成功！" -ForegroundColor Green
    Write-Host "憑證指紋: $($Cert.Thumbprint)" -ForegroundColor Cyan
    
    # 匯出憑證到檔案
    $CertPath = ".\Certificates\JyTech_CodeSigning.pfx"
    $Password = ConvertTo-SecureString -String "JyTech2025!" -Force -AsPlainText
    
    # 創建憑證目錄
    if (!(Test-Path ".\Certificates")) {
        New-Item -ItemType Directory -Path ".\Certificates"
    }
    
    # 匯出 PFX 檔案
    Export-PfxCertificate -Cert $Cert -FilePath $CertPath -Password $Password
    
    Write-Host "📁 憑證已匯出到: $CertPath" -ForegroundColor Green
    Write-Host "🔑 密碼: JyTech2025!" -ForegroundColor Yellow
    
    # 創建簽名腳本
    $SignScript = @"
@echo off
echo 🔐 RanGs 程式數位簽名 - Jy技術團隊
echo 開發者：TIAN0517
echo ================================

signtool sign /f "Certificates\JyTech_CodeSigning.pfx" /p "JyTech2025!" /t http://timestamp.digicert.com /d "RanGs Kill Animation System" /du "https://github.com/tian0517" "%1"

if %ERRORLEVEL% == 0 (
    echo ✅ 簽名成功！
) else (
    echo ❌ 簽名失敗！
)
pause
"@
    
    $SignScript | Out-File -FilePath ".\Scripts\SignExecutable.bat" -Encoding ASCII
    
    Write-Host "📜 簽名腳本已創建: .\Scripts\SignExecutable.bat" -ForegroundColor Green
    Write-Host ""
    Write-Host "🎯 使用方法：" -ForegroundColor Cyan
    Write-Host "  .\Scripts\SignExecutable.bat RanGs.exe" -ForegroundColor White
    
} catch {
    Write-Host "❌ 憑證創建失敗: $($_.Exception.Message)" -ForegroundColor Red
}