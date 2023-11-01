SHELL := cmd.exe
CYGWIN=nontsec
export PATH := C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\GitExtensions\;C:\Program Files\dotnet\;C:\Program Files\Git\cmd;C:\Program Files (x86)\LPM;C:\Temp\hugo_0.119.0_windows-amd64\;C:\Program Files (x86)\LPM;C:\Program Files\nodejs\;C:\Program Files (x86)\LPM;C:\Program Files (x86)\LPM;C:\Program Files (x86)\LPM;C:\Users\Jakeway\AppData\Local\Programs\Python\Python312\Scripts\;C:\Users\Jakeway\AppData\Local\Programs\Python\Python312\;C:\Users\Jakeway\AppData\Local\Programs\Python\Launcher\;C:\Users\Jakeway\AppData\Local\Microsoft\WindowsApps;C:\Users\Jakeway\AppData\Local\Programs\Microsoft VS Code\bin;C:\Program Files (x86)\Common Files\Hilscher GmbH\TLRDecode;C:\Users\Jakeway\AppData\Local\Programs\Python\Python312\Scripts\;C:\Users\Jakeway\AppData\Local\Programs\Python\Python312\;C:\Users\Jakeway\AppData\Local\Programs\Python\Launcher\;C:\Users\Jakeway\AppData\Local\Microsoft\WindowsApps;C:\Users\Jakeway\AppData\Local\Programs\Microsoft VS Code\bin;C:\Program Files (x86)\Common Files\Hilscher GmbH\TLRDecode;C:\BrAutomation\AS411\bin-en\4.11;C:\BrAutomation\AS411\bin-en\4.10;C:\BrAutomation\AS411\bin-en\4.9;C:\BrAutomation\AS411\bin-en\4.8;C:\BrAutomation\AS411\bin-en\4.7;C:\BrAutomation\AS411\bin-en\4.6;C:\BrAutomation\AS411\bin-en\4.5;C:\BrAutomation\AS411\bin-en\4.4;C:\BrAutomation\AS411\bin-en\4.3;C:\BrAutomation\AS411\bin-en\4.2;C:\BrAutomation\AS411\bin-en\4.1;C:\BrAutomation\AS411\bin-en\4.0;C:\BrAutomation\AS411\bin-en
export AS_BUILD_MODE := Build
export AS_VERSION := 4.11.2.75
export AS_WORKINGVERSION := 4.11
export AS_COMPANY_NAME :=  
export AS_USER_NAME := Jakeway
export AS_PATH := C:/BrAutomation/AS411
export AS_BIN_PATH := C:/BrAutomation/AS411/bin-en
export AS_PROJECT_PATH := C:/Projects/Loupe/AxisLib/example/AsProject/ServoMgr
export AS_PROJECT_NAME := ServoMgr
export AS_SYSTEM_PATH := C:/BrAutomation/AS/System
export AS_VC_PATH := C:/BrAutomation/AS411/AS/VC
export AS_TEMP_PATH := C:/Projects/Loupe/AxisLib/example/AsProject/ServoMgr/Temp
export AS_CONFIGURATION := Config1
export AS_BINARIES_PATH := C:/Projects/Loupe/AxisLib/example/AsProject/ServoMgr/Binaries
export AS_GNU_INST_PATH := C:/BrAutomation/AS411/AS/GnuInst/V4.1.2
export AS_GNU_BIN_PATH := C:/BrAutomation/AS411/AS/GnuInst/V4.1.2/4.9/bin
export AS_GNU_INST_PATH_SUB_MAKE := C:/BrAutomation/AS411/AS/GnuInst/V4.1.2
export AS_GNU_BIN_PATH_SUB_MAKE := C:/BrAutomation/AS411/AS/GnuInst/V4.1.2/4.9/bin
export AS_INSTALL_PATH := C:/BrAutomation/AS411
export WIN32_AS_PATH := "C:\BrAutomation\AS411"
export WIN32_AS_BIN_PATH := "C:\BrAutomation\AS411\bin-en"
export WIN32_AS_PROJECT_PATH := "C:\Projects\Loupe\AxisLib\example\AsProject\ServoMgr"
export WIN32_AS_SYSTEM_PATH := "C:\BrAutomation\AS\System"
export WIN32_AS_VC_PATH := "C:\BrAutomation\AS411\AS\VC"
export WIN32_AS_TEMP_PATH := "C:\Projects\Loupe\AxisLib\example\AsProject\ServoMgr\Temp"
export WIN32_AS_BINARIES_PATH := "C:\Projects\Loupe\AxisLib\example\AsProject\ServoMgr\Binaries"
export WIN32_AS_GNU_INST_PATH := "C:\BrAutomation\AS411\AS\GnuInst\V4.1.2"
export WIN32_AS_GNU_BIN_PATH := "$(WIN32_AS_GNU_INST_PATH)\\bin" 
export WIN32_AS_INSTALL_PATH := "C:\BrAutomation\AS411"

.suffixes:

ProjectMakeFile:

	@'$(AS_BIN_PATH)/4.9/BR.AS.AnalyseProject.exe' '$(AS_PROJECT_PATH)/ServoMgr.apj' -t '$(AS_TEMP_PATH)' -c '$(AS_CONFIGURATION)' -o '$(AS_BINARIES_PATH)'   -sfas -buildMode 'Build'   

