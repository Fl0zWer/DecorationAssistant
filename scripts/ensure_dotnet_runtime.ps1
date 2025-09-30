[CmdletBinding()]
param(
    [string]$Version = "8.0.7",
    [string]$InstallDir = "$env:USERPROFILE\.dotnet"
)

$ErrorActionPreference = 'Stop'

function Test-DotnetRuntime {
    param([string]$RequiredVersion)

    $desired = [Version]$RequiredVersion
    $list = @()
    try {
        $list = & dotnet --list-runtimes 2>$null
    } catch {
        return $false
    }
    if ($LASTEXITCODE -ne 0 -or -not $list) {
        return $false
    }

    foreach ($line in $list) {
        if ($line -match '^Microsoft\.NETCore\.App\s+([0-9]+\.[0-9]+\.[0-9]+)') {
            $current = [Version]$Matches[1]
            if ($current.Major -eq $desired.Major -and $current.Minor -eq $desired.Minor -and $current.Build -ge $desired.Build) {
                return $true
            }
        }
    }
    return $false
}

if (Test-DotnetRuntime -RequiredVersion $Version) {
    Write-Host "✔️ .NET runtime $Version ya está instalado."
    return 0
}

$installScript = Join-Path $env:TEMP "dotnet-install.ps1"
$dotnetInstallUri = "https://dot.net/v1/dotnet-install.ps1"

try {
    [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12 -bor [Net.SecurityProtocolType]::Tls13
} catch {
    # Ignorar si no está disponible (PowerShell 7+ ya usa TLS moderno).
}

Write-Host "Descargando script oficial dotnet-install desde $dotnetInstallUri..."
try {
    Invoke-WebRequest -Uri $dotnetInstallUri -OutFile $installScript -UseBasicParsing | Out-Null
} catch {
    throw "No se pudo descargar dotnet-install.ps1. Verifica tu conexión a internet o instala .NET $Version manualmente."
}

Write-Host "Instalando Microsoft.NETCore.App $Version en $InstallDir..."
try {
    & powershell -NoProfile -ExecutionPolicy Bypass -File $installScript -Runtime "dotnet" -Version $Version -InstallDir $InstallDir | ForEach-Object { Write-Host $_ }
    if ($LASTEXITCODE -ne 0) {
        throw "dotnet-install.ps1 falló con código $LASTEXITCODE"
    }
} catch {
    throw "No se pudo instalar el runtime de .NET $Version automáticamente. Ejecuta el instalador manual o vuelve a intentar."
}

[Environment]::SetEnvironmentVariable('DOTNET_ROOT', $InstallDir, 'Process')
[Environment]::SetEnvironmentVariable('DOTNET_ROOT', $InstallDir, 'User')
$pathParts = $env:PATH.Split(';')
if (-not ($pathParts -contains $InstallDir)) {
    $newPath = "$InstallDir;" + $env:PATH
    [Environment]::SetEnvironmentVariable('PATH', $newPath, 'Process')
}

Write-Host "✅ Runtime .NET $Version listo. Reinicia Visual Studio si estaba abierto para usar la nueva instalación."
