; Скрипт для Inno Setup - создание установщика EKNMusic
; Скачайте Inno Setup: https://jrsoftware.org/isdl.php

[Setup]
AppName=EKNMusic
AppVersion=1.0.0
AppPublisher=EKNMusic Team
DefaultDirName={autopf}\EKNMusic
DefaultGroupName=EKNMusic
OutputDir=output
OutputBaseFilename=EKNMusic_Setup_v1.0.0
Compression=lzma2
SolidCompression=yes
; SetupIconFile=..\client\resources\icon.ico
WizardStyle=modern
PrivilegesRequired=lowest
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible

[Languages]
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Tasks]
Name: "desktopicon"; Description: "Создать ярлык на рабочем столе"; GroupDescription: "Дополнительные значки:"
Name: "quicklaunchicon"; Description: "Создать ярлык на панели быстрого запуска"; GroupDescription: "Дополнительные значки:"; Flags: unchecked
Name: "autostart"; Description: "Запускать при входе в Windows"; GroupDescription: "Дополнительно:"; Flags: unchecked

[Files]
; Главное приложение
Source: "..\client\build-release\EKNMusic.exe"; DestDir: "{app}"; Flags: ignoreversion
; Qt библиотеки (после windeployqt)
Source: "..\client\build-release\*.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs
Source: "..\client\build-release\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs
Source: "..\client\build-release\styles\*"; DestDir: "{app}\styles"; Flags: ignoreversion recursesubdirs
; Другие папки Qt
Source: "..\client\build-release\generic\*"; DestDir: "{app}\generic"; Flags: ignoreversion recursesubdirs
Source: "..\client\build-release\iconengines\*"; DestDir: "{app}\iconengines"; Flags: ignoreversion recursesubdirs
Source: "..\client\build-release\imageformats\*"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs
Source: "..\client\build-release\multimedia\*"; DestDir: "{app}\multimedia"; Flags: ignoreversion recursesubdirs
Source: "..\client\build-release\networkinformation\*"; DestDir: "{app}\networkinformation"; Flags: ignoreversion recursesubdirs
Source: "..\client\build-release\tls\*"; DestDir: "{app}\tls"; Flags: ignoreversion recursesubdirs

[Icons]
Name: "{group}\EKNMusic"; Filename: "{app}\EKNMusic.exe"
Name: "{group}\Удалить EKNMusic"; Filename: "{uninstallexe}"
Name: "{autodesktop}\EKNMusic"; Filename: "{app}\EKNMusic.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\EKNMusic"; Filename: "{app}\EKNMusic.exe"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\EKNMusic.exe"; Description: "Запустить EKNMusic"; Flags: nowait postinstall skipifsilent

[Registry]
; Автозагрузка
Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; ValueType: string; ValueName: "EKNMusic"; ValueData: "{app}\EKNMusic.exe"; Tasks: autostart

[Code]
procedure InitializeWizard;
begin
  WizardForm.LicenseAcceptedRadio.Checked := True;
end;

function InitializeSetup(): Boolean;
var
  ResultCode: Integer;
begin
  Result := True;
  // Можно добавить проверку зависимостей
end;
