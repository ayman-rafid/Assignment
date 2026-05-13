param(
    [string]$InnoVersion = "6.7.1"
)

$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$setupScript = Join-Path $root "installer\GradeCalcWinForms.iss"
$appExe = Join-Path $root "dist\GradeCalcWinForms.exe"
$toolsDir = Join-Path $root "build\tools\inno"
$packageDir = Join-Path $toolsDir "package"
$localIscc = Join-Path $packageDir "tools\ISCC.exe"

if (-not (Test-Path $appExe)) {
    throw "Build the GUI first: powershell -ExecutionPolicy Bypass -File .\build-winforms.ps1"
}

$iscc = (Get-Command ISCC.exe -ErrorAction SilentlyContinue).Source

if (-not $iscc) {
    if (-not (Test-Path $localIscc)) {
        New-Item -ItemType Directory -Force -Path $toolsDir | Out-Null

        $nupkg = Join-Path $toolsDir "Tools.InnoSetup.$InnoVersion.nupkg"
        $zip = Join-Path $toolsDir "Tools.InnoSetup.$InnoVersion.zip"
        $url = "https://www.nuget.org/api/v2/package/Tools.InnoSetup/$InnoVersion"

        Invoke-WebRequest -Uri $url -OutFile $nupkg
        Copy-Item -LiteralPath $nupkg -Destination $zip -Force

        if (Test-Path $packageDir) {
            Remove-Item -LiteralPath $packageDir -Recurse -Force
        }

        Expand-Archive -LiteralPath $zip -DestinationPath $packageDir
    }

    $iscc = $localIscc
}

& $iscc $setupScript

if ($LASTEXITCODE -ne 0) {
    throw "Installer build failed with exit code $LASTEXITCODE"
}

Write-Host "Built $(Join-Path $root 'dist\StudentGradeCalculatorSetup.exe')"
