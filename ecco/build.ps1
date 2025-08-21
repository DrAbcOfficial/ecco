# 定义需要检测的文件夹
$requiredFolders = @("tcl", "tomlplusplus", "metamod", "mmlib")

# 检查文件夹是否存在
$missing = $false
foreach ($folder in $requiredFolders) {
    if (-not (Test-Path -Path $folder -PathType Container)) {
        Write-Host "缺少必要的文件夹: $folder"
        $missing = $true
    }
}

# 检测vswhere.exe是否存在
if (-not(Test-Path -Path "./vswhere.exe")){
	Write-Host "缺少必要的工具: vswhere.exe"
	$missing = $true
}

if ($missing) {
	Write-Host "先运行init.ps1再执行build"
	exit 1
}

# 构建tcl
$vsPath = ./vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
$vsPath += "/VC/Auxiliary/Build/"
if ([Environment]::Is64BitOperatingSystem){
	$vsPath += "vcvarsamd64_x86.bat"
}
else{
	$vsPath += "vcvars32.bat"
}
if (-not(Test-Path -Path $vsPath)){
$vsPath
	Write-Host "系统未安装VC工具……"
	exit 1
}
cmd /c "call `"$vsPath`" && set" | foreach {
    $p = $_.IndexOf("=")
    if ($p -gt 0) {
        $name = $_.Substring(0, $p)
        $value = $_.Substring($p + 1)
        [System.Environment]::SetEnvironmentVariable($name, $value, "Process")
    }
}
cd "tcl/win/"
nmake -f makefile.vc
nmake -f makefile.vc install INSTALLDIR=../build
cd ../..