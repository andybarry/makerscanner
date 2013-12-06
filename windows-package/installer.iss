

[Setup]
AppName=MakerScanner
AppVerName=MakerScanner v0.3.1
DefaultDirName={pf}\MakerScanner
DefaultGroupName=MakerScanner
UninstallDisplayIcon={app}\MakerScanner.exe
Compression=lzma
SolidCompression=yes
OutputDir=userdocs:MakerScanner
AppMutex=MakerScanner

[Files]
Source: MakerScanner.exe; DestDir: {app}
Source: README; DestDir: {app}
Source: AUTHORS; DestDir: {app}
Source: LICENSE; DestDir: {app}
Source: laserIcon.ico; DestDir: {app}
Source: cv210.dll; DestDir: {app}
Source: cxcore210.dll; DestDir: {app}
Source: highgui210.dll; DestDir: {app}
Source: mingwm10.dll; DestDir: {app}

[Icons]
Name: {group}\MakerScanner; Filename: {app}\MakerScanner.exe; WorkingDir: {app}
Name: {commonstartup}\MakerScanner; Filename: {app}\MakerScanner.exe; WorkingDir: {app}

[Run]
Filename: {app}\MakerScanner.exe; Description: Run MakerScanner; Flags: postinstall nowait skipifsilent
