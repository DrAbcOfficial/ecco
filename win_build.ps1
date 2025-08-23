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
cd "ecco/tcl/win/"
nmake -f makefile.vc
nmake -f makefile.vc install INSTALLDIR=../build
cd ../../..

msbuild ecco.sln /p:Configuration=Release

mkdir dist/ecco
Copy-Item Release/ecco.dll dist/ecco -Force
$extensionDir = "dist/ecco/extensions"
mkdir $extensionDir
Copy-Item Release/angelscript.dll $extensionDir -Force
Copy-Item Release/base.dll $extensionDir -Force
Copy-Item ecco/package/lang dist/ecco/lang -Recurse -Force
Copy-Item ecco/package/scripts dist/ecco/scripts -Recurse -Force
Copy-Item ecco/package/Config.toml dist/ecco/Config.toml -Force
$tclDir = "dist/ecco/tcl"
mkdir $tclDir
Copy-Item ecco/tcl/build/bin/tcl91.dll $tclDir -Force
Copy-Item ecco/tcl/build/bin/libtommath.dll $tclDir -Force
Copy-Item ecco/tcl/build/bin/zlib1.dll $tclDir -Force
Expand-Archive -Path "ecco/tcl/build/lib/libtcl9.1a1.zip" -DestinationPath "$tclDir/"
Move-Item "$tclDir/tcl_library" "$tclDir/lib" -Force
mkdir "$tclDir/pkg"
Copy-Item "ecco/tcl/build/lib/registry1.4" "$tclDir/pkg/registry1.4" -Recurse -Force -Exclude "*.lib"
Copy-Item "ecco/tcl/build/lib/dde1.5" "$tclDir/pkg/dde1.5" -Recurse -Force -Exclude "*.lib"