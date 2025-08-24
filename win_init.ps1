# 定义需要检测的文件夹
$requiredFolders = @("ecco/tcl", "ecco/tomlplusplus", "ecco/metamod", "ecco/mmlib", "base/mmlib", "angelscript/metamod")

# 检查文件夹是否存在
$foldersMissing = $false
foreach ($folder in $requiredFolders) {
    if (-not (Test-Path -Path $folder -PathType Container)) {
        Write-Host "Missing: $folder"
        $foldersMissing = $true
    }
}

# 如果有文件夹缺失，执行git submodule命令
if ($foldersMissing) {
    git submodule update --init
    
    # 验证是否成功获取了所有文件夹
    foreach ($folder in $requiredFolders) {
        if (-not (Test-Path -Path $folder -PathType Container)) {
            exit 1
        }
    }
}

# 检测vswhere.exe是否存在
if (-not(Test-Path -Path "./vswhere.exe")){
	Write-Host "downloading vswhere.exe"
	Invoke-WebRequest -Uri "https://github.com/microsoft/vswhere/releases/download/3.1.7/vswhere.exe" -OutFile "./vswhere.exe"
}

# 处理版本选择 - 支持命令行参数或交互式输入
$version = $null
$metaInclude = $null
$gameDefine = $null

# 检查是否提供了版本参数
if ($args.Count -ge 1) {
    $versionChoice = $args[0]
    
    # 验证参数有效性
    if ($versionChoice -eq "1") {
        $version = "5:13"
        $metaInclude = "./mmlib/include/metamod"
        $gameDefine = "_META_5_13"
    } elseif ($versionChoice -eq "2") {
        $version = "5:16"
        $metaInclude = "./metamod/metamod"
        $gameDefine = "_META_5_18"
    } else {
        Write-Host "Invalid version parameter: $versionChoice"
        Write-Host "Please use 1 or 2"
        exit 1
    }
} else {
    # 没有参数，使用交互式选择
    Write-Host "`nChose metamod version:"
    Write-Host "1. 5:13（metamod-p metamod-r）"
    Write-Host "2. 5:16（metamod-fallguys）"

    do {
        $versionChoice = Read-Host "Chose (1 or 2)"
        if ($versionChoice -eq "1") {
            $version = "5:13"
            $metaInclude = "./mmlib/include/metamod"
            $gameDefine = "_META_5_13"
        } elseif ($versionChoice -eq "2") {
            $version = "5:16"
            $metaInclude = "./metamod/metamod"
            $gameDefine = "_META_5_18"
        } else {
            Write-Host "Invalid, 1 or 2"
        }
    } while ([string]::IsNullOrEmpty($version))
}

# 提示用户输入调试器名称
do {
    $debuggerName = Read-Host "Debugger name:"
    if ([string]::IsNullOrEmpty($debuggerName)) {
        Write-Host "Invalid, try again"
    }
} while ([string]::IsNullOrEmpty($debuggerName))

# 提示用户输入调试器路径，并确保以斜杠结尾
do {
    $debuggerPath = Read-Host "Debugger path:"
    if ([string]::IsNullOrEmpty($debuggerPath)) {
        Write-Host "Invalid, try again"
    } else {
        # 检查路径是否以\或/结尾，如果不是则添加/
        $lastChar = $debuggerPath.Substring($debuggerPath.Length - 1)
        if ($lastChar -ne "\" -and $lastChar -ne "/") {
            $debuggerPath += "/"
        }
    }
} while ([string]::IsNullOrEmpty($debuggerPath))

# 显示已存储的变量值
Write-Host "`nConfig:"
Write-Host "Metamod Version: $version"
Write-Host "Debugger Name: $debuggerName"
Write-Host "Debugger Path: $debuggerPath"

# 用户确认设置是否正确
do {
    $confirmChoice = Read-Host "`nAll good? (Y/N)"
    $confirmChoice = $confirmChoice.ToUpper()
    
    if ($confirmChoice -eq "N") {
        Write-Host "canceled"
        exit 0
    } elseif ($confirmChoice -ne "Y") {
        Write-Host "Invalid，Y or N"
    }
} while ($confirmChoice -ne "Y" -and $confirmChoice -ne "N")

# 处理Build_template.props文件并生成Build.props
$templatePath = "./ecco/Build_template.props"
$outputPath = "./ecco/Build.props"

if (Test-Path -Path $templatePath -PathType Leaf) {
    # 读取模板文件内容
    $content = Get-Content -Path $templatePath -Raw
    
    # 替换占位符
    $content = $content -replace '{{DEBUG_PROGRAM}}', $debuggerName
    $content = $content -replace '{{DEBUG_PATH}}', $debuggerPath
    $content = $content -replace '{{META_INCLUDE}}', $metaInclude
    $content = $content -replace '{{GAME_DEFINE}}', $gameDefine
    
    # 将替换后的内容保存为新文件
    $content | Set-Content -Path $outputPath -Force
    
    if (Test-Path -Path $outputPath -PathType Leaf) {
        Write-Host "`ngenerated Build.props"
    } else {
        Write-Error "`ngenerating Build.props failed"
        exit 1
    }
} else {
    Write-Error "`nNo Build_template.props"
    exit 1
}
    