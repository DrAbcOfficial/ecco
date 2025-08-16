# 定义需要检测的文件夹
$requiredFolders = @("ChaiScript", "tomlplusplus", "metamod", "mmlib")

# 检查文件夹是否存在
$foldersMissing = $false
foreach ($folder in $requiredFolders) {
    if (-not (Test-Path -Path $folder -PathType Container)) {
        Write-Host "缺少必要的文件夹: $folder"
        $foldersMissing = $true
    }
}

# 如果有文件夹缺失，执行git submodule命令
if ($foldersMissing) {
    Write-Host "正在更新git子模块..."
    git submodule update --remote --init
    
    # 验证是否成功获取了所有文件夹
    foreach ($folder in $requiredFolders) {
        if (-not (Test-Path -Path $folder -PathType Container)) {
            Write-Error "获取子模块失败，仍然缺少文件夹: $folder"
            exit 1
        }
    }
    Write-Host "子模块更新完成"
} else {
    Write-Host "所有必要的文件夹都已存在"
}

# 提示用户选择版本
Write-Host "`n请选择版本:"
Write-Host "1. 5:13版本（metamod-p metamod-r）"
Write-Host "2. 5:18版本（metamod-fallguys）"

do {
    $versionChoice = Read-Host "请输入选项 (1 或 2)"
    if ($versionChoice -eq "1") {
        $version = "5:13"
        $metaInclude = "./mmlib/include/metamod"
    } elseif ($versionChoice -eq "2") {
        $version = "5:18"
        $metaInclude = "./metamod/metamod"
    } else {
        Write-Host "无效选项，请重新输入"
    }
} while ([string]::IsNullOrEmpty($version))

# 提示用户选择是否开启svencoop支持
do {
    $svencoopChoice = Read-Host "是否开启svencoop支持? (Y/N)"
    $svencoopChoice = $svencoopChoice.ToUpper()
    if ($svencoopChoice -eq "Y") {
        $enableSvencoop = $true
        $gameDefine = "_GAME_SVENCOOP"
    } elseif ($svencoopChoice -eq "N") {
        $enableSvencoop = $false
        $gameDefine = ""
    } else {
        Write-Host "无效选项，请输入 Y 或 N"
    }
} while ($svencoopChoice -ne "Y" -and $svencoopChoice -ne "N")

# 提示用户输入调试器路径，并确保以斜杠结尾
do {
    $debuggerPath = Read-Host "请输入调试器路径"
    if ([string]::IsNullOrEmpty($debuggerPath)) {
        Write-Host "调试器路径不能为空，请重新输入"
    } else {
        # 检查路径是否以\或/结尾，如果不是则添加/
        $lastChar = $debuggerPath.Substring($debuggerPath.Length - 1)
        if ($lastChar -ne "\" -and $lastChar -ne "/") {
            $debuggerPath += "/"
        }
    }
} while ([string]::IsNullOrEmpty($debuggerPath))

# 提示用户输入调试器名称
do {
    $debuggerName = Read-Host "请输入调试器名称"
    if ([string]::IsNullOrEmpty($debuggerName)) {
        Write-Host "调试器名称不能为空，请重新输入"
    }
} while ([string]::IsNullOrEmpty($debuggerName))

# 显示已存储的变量值
Write-Host "`n已存储的配置信息:"
Write-Host "版本: $version"
Write-Host "开启svencoop支持: $enableSvencoop"
Write-Host "调试器路径: $debuggerPath"
Write-Host "调试器名称: $debuggerName"

# 用户确认设置是否正确
do {
    $confirmChoice = Read-Host "`n以上设置是否正确? (Y/N)"
    $confirmChoice = $confirmChoice.ToUpper()
    
    if ($confirmChoice -eq "N") {
        Write-Host "操作已取消，将退出脚本"
        exit 0
    } elseif ($confirmChoice -ne "Y") {
        Write-Host "无效选项，请输入 Y 或 N"
    }
} while ($confirmChoice -ne "Y" -and $confirmChoice -ne "N")

# 处理Build_template.props文件并生成Build.props
$templatePath = "Build_template.props"
$outputPath = "Build.props"

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
        Write-Host "`n成功生成Build.props文件"
    } else {
        Write-Error "`n生成Build.props文件失败"
        exit 1
    }
} else {
    Write-Error "`n错误: 未找到模板文件Build_template.props"
    exit 1
}
    