param(
    [string]$Configuration = "Release"
)

$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$dist = Join-Path $root "dist"
$obj = Join-Path $root "build\winforms"
$vsDevCmd = "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"

New-Item -ItemType Directory -Force -Path $dist | Out-Null
New-Item -ItemType Directory -Force -Path $obj | Out-Null

$sources = @(
    "gui\GradeCalcWinForms.cpp",
    "src\Person.cpp",
    "src\Processor.cpp",
    "src\FileManager.cpp"
)

$mscoreeDef = Join-Path $obj "mscoree.def"
Set-Content -Path $mscoreeDef -Value @(
    "LIBRARY MSCOREE.DLL",
    "EXPORTS",
    "_CorExeMain",
    "_CorDllMain"
)

$compileFlags = "/nologo /std:c++17 /EHa /clr /AI`"C:\Windows\Microsoft.NET\Framework64\v4.0.30319`" /I`"$root\include`""

if ($Configuration -ieq "Debug") {
    $compileFlags += " /Zi /Od"
} else {
    $compileFlags += " /O2"
}

$compileSteps = @()
$objectFiles = @()
foreach ($source in $sources) {
    $sourcePath = Join-Path $root $source
    $objectPath = Join-Path $obj ([IO.Path]::GetFileNameWithoutExtension($source) + ".obj")
    $compileSteps += "cl $compileFlags /c `"$sourcePath`" /Fo`"$objectPath`""
    $objectFiles += "`"$objectPath`""
}

$objectArgs = $objectFiles -join " "
$outputExe = Join-Path $dist "GradeCalcWinForms.exe"

$makeMscoreeLib = "lib /nologo /def:`"$mscoreeDef`" /machine:x64 /out:`"$obj\MSCOREE.lib`""
$compileCommand = $compileSteps -join " && "
$command = "`"$vsDevCmd`" -arch=x64 && cd /d `"$root`" && $makeMscoreeLib && $compileCommand && cl $compileFlags $objectArgs /Fe:`"$outputExe`" /link /LIBPATH:`"$obj`" /SUBSYSTEM:WINDOWS"
cmd /c $command
if ($LASTEXITCODE -ne 0) {
    throw "WinForms build failed with exit code $LASTEXITCODE"
}

Write-Host "Built $outputExe"
