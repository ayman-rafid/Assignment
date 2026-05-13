#define MyAppName "Student Grade Calculator"
#define MyAppVersion "1.0"
#define MyAppPublisher "Ayman Rafid"
#define MyAppExeName "GradeCalcWinForms.exe"

[Setup]
AppId={{6A6C7F7A-35E6-4E73-AF37-2E3A04E2B3F4}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={autopf}\VVK\Ayman_Rafid
DefaultGroupName=VVK\Ayman Rafid
OutputDir=..\dist
OutputBaseFilename=StudentGradeCalculatorSetup
Compression=lzma
SolidCompression=yes
WizardStyle=modern
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible

[Files]
Source: "..\dist\GradeCalcWinForms.exe"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\Student Grade Calculator"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\Student Grade Calculator"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "Create a desktop shortcut"; GroupDescription: "Additional icons:"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "Launch Student Grade Calculator"; Flags: nowait postinstall skipifsilent
