# Ecco脚本文件编写指南

## 1. 脚本系统概述

Ecco插件使用TOML格式的配置文件结合Tcl脚本引擎，允许服务器管理员通过编写脚本来自定义游戏行为、创建购买菜单项目、实现特殊功能等。脚本系统是Ecco插件的核心功能之一，为插件提供了极高的扩展性和定制性。

### 1.1 脚本系统的组成

- **脚本引擎**：基于Tcl实现，负责解析和执行脚本
- **脚本配置**：使用TOML格式的配置文件，包含脚本的基本信息和内容
- **菜单解析器**：根据脚本名称构建层次化的购买菜单
- **资源预加载**：支持预加载模型、声音等资源

## 2. 脚本文件结构

Ecco插件的脚本文件通常位于`addons/ecco/scripts/`目录下，使用`.toml`扩展名。每个脚本文件对应购买菜单中的一个项目。

### 2.1 脚本文件的基本结构

每个脚本文件由以下部分组成：

```toml
# 只有在Ecco节下的内容才会被读取
[Ecco]
# 菜单设置，以.设置菜单层级
# 此处设置为id，在lang/user_setting.toml中设置id对应的翻译
name = "Category.SubCategory.ItemName"
# 花费多少金钱
cost = 1000
# 使用tcl编写的脚本

script = """
# 脚本内容
# 这里编写Tcl脚本代码
"""
flags = 0
# 需要Precache的那些资源
precaches = []
sound_precaches = []
other_precaches = []
```

### 2.2 脚本参数说明

| 参数 | 说明 | 示例值 |
|------|------|--------|
| name | 脚本项名称，用于构建菜单层次结构 | "Items.Health.HealthKit" |
| cost | 购买该项目所需的金钱 | 1000 |
| script | 脚本内容，使用Tcl语法 | """give $caller_index "weapon_9mmhandgun"""" |
| flags | 脚本项标志，用于控制行为 | 0（无特殊标志） |
| precaches | 需要预加载的模型资源 | ["models/weapons/v_9mmhandgun.mdl"] |
| sound_precaches | 需要预加载的声音资源 | ["sound/weapons/9mmhandgun1.wav"] |
| other_precaches | 需要预加载的其他资源 | ["item_healthkit"] |

## 3. 脚本编写基础

### 3.1 Tcl语法基础

Tcl是一种简单易学的脚本语言，具有以下特点：

- 命令格式：`command arg1 arg2 ...`
- 变量：使用`$variable`引用变量，使用`set variable value`设置变量
- 字符串：可以使用双引号或花括号包围
- 注释：以`#`开头的行
- 控制结构：`if`、`for`、`foreach`、`while`等

### 3.2 脚本执行环境

每个脚本在执行时会获得以下环境：

- **玩家信息**：当前执行脚本的玩家，通过`$caller_index`变量访问
- **脚本API**：Ecco提供的一系列函数
- **Tcl标准库**：Tcl内置的命令和函数

## 4. 脚本API参考

### 4.1 玩家相关API

| 函数名 | 说明 | 参数 | 返回值 |
|--------|------|------|--------|
| `Player::GetName` | 获取玩家名称 | 玩家索引 | 玩家名称（字符串） |
| `Player::GetSteamId` | 获取玩家SteamID | 玩家索引 | SteamID（字符串） |
| `give` | 给玩家武器或物品 | 玩家索引, 物品名称 | 无 |
| `saytext` | 让玩家说话 | 玩家索引, 消息内容 | 无 |

### 4.2 游戏相关API

| 函数名 | 说明 | 参数 | 返回值 |
|--------|------|------|--------|
| `server_print` | 在服务器控制台打印消息 | 消息内容 | 无 |
| `client_print` | 向玩家发送消息 | 玩家索引, 消息内容 | 无 |
| `center_print` | 向玩家发送中心消息 | 玩家索引, 消息内容 | 无 |
| `server_cmd` | 执行服务器命令 | 命令内容 | 无 |
| `get_map_name` | 获取当前地图名称 | 无 | 地图名称（字符串） |
| `get_time` | 获取当前游戏时间 | 无 | 时间（浮点数） |
| `rand` | 生成随机数 | 最小值, 最大值 | 随机数（整数） |

### 4.3 脚本控制API

| 函数名 | 说明 | 参数 | 返回值 |
|--------|------|------|--------|
| `abort` | 中止脚本执行 | 无 | 无 |
| `error` | 抛出错误 | 错误消息 | 无 |
| `return` | 返回值并结束脚本 | 返回值（可选） | 返回值 |

## 5. 脚本示例

### 5.1 基本示例

#### 5.1.1 健康包脚本

```toml
[Ecco]
name = "Item.HealthKit"
cost = 10
script = """
give $caller_index "item_healthkit"
"""
flags = 0
precaches = []
sound_precaches = []
other_precaches = ["item_healthkit"]
```

#### 5.1.2 武器购买脚本

```toml
[Ecco]
name = "Weapon.9mmHandgun"
cost = 500
script = """
give $caller_index "weapon_9mmhandgun"
saytext $caller_index "You bought a 9mm handgun!"
"""
flags = 0
precaches = ["models/weapons/v_9mmhandgun.mdl", "models/weapons/w_9mmhandgun.mdl"]
sound_precaches = ["sound/weapons/9mmhandgun1.wav", "sound/weapons/9mmhandgun2.wav"]
other_precaches = []
```

### 5.2 高级示例

#### 5.2.1 玩家信息脚本

```toml
[Ecco]
name = "TestCategory.TestItem"
cost = 0
script = """
give $caller_index "weapon_9mmhandgun"
set player_name [Player::GetName $caller_index]
set player_id [Player::GetSteamId $caller_index]
saytext $caller_index "${player_name} 在小心的说悄悄话 ${player_id}"
"""
flags = 0
precaches = []
sound_precaches = []
other_precaches = []
```

#### 5.2.2 随机效果脚本

```toml
[Ecco]
name = "Items.Special.RandomEffect"
cost = 1000
script = """
# 生成随机数（1-5）
set random [rand 1 5]

# 根据随机数执行不同效果
switch $random {
    1 {
        # 效果1：给玩家健康包
        give $caller_index "item_healthkit"
        saytext $caller_index "Lucky! You got a health kit!"
    }
    2 {
        # 效果2：给玩家护甲
        give $caller_index "item_battery"
        saytext $caller_index "Nice! You got a battery!"
    }
    3 {
        # 效果3：给玩家武器
        give $caller_index "weapon_9mmhandgun"
        saytext $caller_index "Good! You got a 9mm handgun!"
    }
    4 {
        # 效果4：给玩家霰弹枪
        give $caller_index "weapon_shotgun"
        saytext $caller_index "Great! You got a shotgun!"
    }
    5 {
        # 效果5：给玩家手榴弹
        give $caller_index "weapon_handgrenade"
        saytext $caller_index "Unlucky! You got a grenade!"
    }
}
"""
flags = 0
precaches = ["models/weapons/v_9mmhandgun.mdl", "models/weapons/v_shotgun.mdl", "models/weapons/v_handgrenade.mdl"]
sound_precaches = []
other_precaches = ["item_healthkit", "item_battery"]
```

## 6. 菜单系统集成

### 6.1 菜单层次结构

脚本名称的格式决定了它在购买菜单中的位置。使用点（.）分隔的名称会被解析为层次结构：

- `Items.Health.HealthKit` → 物品 → 健康 → 健康包
- `Items.Weapons.9mmHandgun` → 物品 → 武器 → 9mm手枪
- `Items.Special.RandomEffect` → 物品 → 特殊 → 随机效果

### 6.2 菜单显示

- 菜单会自动按字母顺序排序
- 每个菜单页面最多显示7个项目
- 超过7个项目会自动创建分页
- 购买菜单可以通过`buy`或`shop`命令打开

## 7. 脚本最佳实践

### 7.1 性能优化

- 避免在脚本中执行耗时操作
- 合理使用变量，避免重复计算
- 对于复杂逻辑，考虑拆分为多个小脚本

### 7.2 错误处理

- 使用`if`语句检查参数有效性
- 捕获可能的错误情况
- 向玩家提供清晰的错误信息

### 7.3 安全性

- 避免使用未验证的用户输入
- 限制脚本的执行权限
- 定期检查脚本是否存在安全漏洞

### 7.4 代码风格

- 使用一致的缩进（推荐4个空格）
- 添加注释说明复杂逻辑
- 使用有意义的变量和函数名
- 保持脚本简洁明了

## 8. 常见问题

### 8.1 脚本不执行

- 检查脚本语法是否正确
- 确保脚本文件格式正确（.toml）
- 检查脚本文件权限
- 查看服务器控制台是否有错误信息

### 8.2 菜单项目不显示

- 检查脚本名称格式是否正确
- 确保脚本文件包含所有必要参数
- 检查购买菜单是否已正确加载
- 检查lang/user_setting.toml中是否有对应的翻译

### 8.3 API函数调用失败

- 检查函数名是否正确
- 确保参数类型和数量正确
- 查看服务器控制台的错误信息

## 9. 高级功能

### 9.1 资源预加载

Ecco插件支持预加载模型、声音和其他资源，确保脚本执行时资源已经加载完成：

```toml
[Ecco]
name = "Items.Weapons.AK47"
cost = 2500
script = """
give $caller_index "weapon_ak47"
"""
flags = 0
precaches = ["models/weapons/v_ak47.mdl", "models/weapons/w_ak47.mdl"]
sound_precaches = ["sound/weapons/ak47-1.wav", "sound/weapons/ak47-2.wav"]
other_precaches = []
```

### 9.2 多语言支持

脚本可以支持多语言，通过在lang/user_setting.toml中设置id对应的翻译：

```toml
# lang/user_setting.toml
[en]
"Items.Health.HealthKit" = "Health Kit"

[zh]
"Items.Health.HealthKit" = "健康包"
```

### 9.3 脚本事件处理

你可以编写脚本响应游戏事件，例如玩家死亡、地图切换等。具体实现方法请参考插件文档。

## 10. 脚本文件管理

### 10.1 脚本文件组织

- 将相关脚本放在同一个目录中
- 使用有意义的文件名
- 定期备份脚本文件

### 10.2 脚本重载

- 可以通过服务器命令`ecco_reload_scripts`重载脚本
- 重载脚本会重新解析所有脚本文件
- 重载过程中可能会暂时影响游戏体验

## 11. 总结

Ecco插件的脚本系统为服务器管理员提供了强大的工具，可以：

- 自定义购买菜单项目
- 实现特殊游戏机制
- 创建独特的游戏体验
- 快速原型和测试新功能

通过本指南的学习，你应该能够编写基本的Ecco脚本，并逐渐掌握更高级的功能。如果有任何问题，请参考插件文档或寻求社区支持。

## 12. 参考资源

- [Tcl官方文档](https://www.tcl.tk/man/tcl8.6/) - 了解Tcl语言的详细语法和特性
- [Ecco插件示例脚本](https://github.com/example/ecco/tree/master/package/scripts) - 更多脚本示例
- [TOML官方文档](https://toml.io/en/) - 了解TOML格式的详细规范

希望本指南能帮助你更好地理解和使用Ecco插件的脚本系统。祝你编写愉快！
