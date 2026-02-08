# TOML配置文件使用指南

## 1. 什么是TOML？

TOML（Tom's Obvious, Minimal Language）是一种简洁明了的配置文件格式，专为人类可读性而设计。它易于阅读和编写，同时也易于机器解析。Ecco插件使用TOML文件来存储配置信息，包括HUD设置、路径配置、颜色设置、购买菜单设置等。

## 2. TOML文件结构

Ecco插件的TOML配置文件遵循以下结构：

```toml
[Ecco.Hud]
# HUD相关配置

[Ecco.Path]
# 路径相关配置

[Ecco.Color]
# 颜色相关配置

[Ecco.BuyMenu]
# 购买菜单相关配置

[Ecco]
# 核心配置
```

## 3. 配置详解

### 3.1 HUD配置（[Ecco.Hud]）

| 配置项 | 说明 | 可选值 | 默认值 |
|-------|------|-------|-------|
| ShowMoneyHUD | 显示金钱HUD的方式 | 0（不显示）、1（显示全部）、2（仅显示金钱）、3（仅显示金钱变化） | 1 |
| MainHudPosX | 金钱HUD显示位置X坐标 | 范围（-1，1），-1和0代表居中 | 0.5 |
| MainHudPosY | 金钱HUD显示位置Y坐标 | 范围（-1，1），-1和0代表居中 | 0.9 |
| NoticeHudPosX | 金钱变化HUD显示位置X坐标 | 范围（-1，1），-1和0代表居中 | 0.5 |
| NoticeHudPosY | 金钱变化HUD显示位置Y坐标 | 范围（-1，1），-1和0代表居中 | 0.858 |
| RefreshHudInterv | 刷新HUD间隔（秒） | 任意正数 | 1.0 |
| MoneyChannel | 金钱HUD频道 | 任意整数 | 3 |
| NotifyChannel | 金钱变更HUD频道 | 任意整数 | 4 |

### 3.2 路径配置（[Ecco.Path]）

| 配置项 | 说明 | 示例值 | 默认值 |
|-------|------|-------|-------|
| Data | 数据储存位置 | "storage" | "storage" |
| BanMaps | 禁用地图文件路径 | "config/BannedMaps.txt" | "config/BannedMaps.txt" |
| MoneyIcon | 金币图标 | "dollar.spr" | "dollar.spr" |

### 3.3 颜色配置（[Ecco.Color]）

| 配置项 | 说明 | 格式 | 默认值 |
|-------|------|------|-------|
| Positive | 正数金钱RGBA | 十六进制值，格式为0xRRGGBBAA | 0xFFFF00FF |
| Negative | 负数金钱RGBA | 十六进制值，格式为0xRRGGBBAA | 0xFF0000FF |
| Incresed | 增加金钱RGBA | 十六进制值，格式为0xRRGGBBAA | 0x00FF00FF |
| Decresed | 扣除金钱RGBA | 十六进制值，格式为0xRRGGBBAA | 0xFF0000FF |

### 3.4 购买菜单配置（[Ecco.BuyMenu]）

| 配置项 | 说明 | 示例值 | 默认值 |
|-------|------|-------|-------|
| OpenShopTriggers | 购买指令触发器 | 字符串数组 | ["buy", "shop"] |
| AllowDeathPlayerBuy | 是否允许死亡玩家购买 | true/false | true |
| ReOpenMenuAfterBuy | 购买后是否重新打开菜单 | true/false | true |
| KeepOpenTime | 菜单保持的时间（秒） | 任意正数 | 10.0 |

### 3.5 核心配置（[Ecco]）

| 配置项 | 说明 | 可选值 | 默认值 |
|-------|------|-------|-------|
| ScoreToMoneyMultiplier | 玩家获得的分数转化为金钱的倍数 | 任意正数 | 1.0 |
| StorePlayerScore | 是否储存玩家金钱 | 0（不储存）、1（只有系列地图储存）、2（储存） | 1 |
| PlayerStartScore | 若不储存金钱，设置玩家起始资金 | 任意整数 | 0 |
| MoneyLimitePerMap | 单张地图最大可获得金钱数目，小于0为不限制 | 整数 | -1 |
| SaveDelayTime | 延迟保存时间（秒），小于0为即时保存 | 任意实数 | 30.0 |
| DefaultLang | 默认的语言 | 语言代码 | "english" |
| QueryLangCVar | 查询用于查询客户端语言的cvar，留空为不查询 | 字符串 | "cap_lang" |
| PrecacheOffset | Precache偏移量 | 2（Half-Life系列）、4（Sven Co-op） | 4 |

## 4. 如何修改配置

1. 找到Ecco插件的配置文件，通常位于 `addons/ecco/Config.toml`
2. 使用文本编辑器打开该文件
3. 根据需要修改相应的配置项
4. 保存文件并重启服务器或重载插件以应用更改

## 5. 配置示例

以下是一个完整的Config.toml配置示例：

```toml
[Ecco.Hud]
#显示金钱 HUD 的方式
#0 为不显示
#1 为显示全部
#2 为仅显示金钱不显示金钱变化
#3 为仅显示金钱变化不显示金钱
ShowMoneyHUD=1
#金钱HUD显示位置
#范围（-1，1）
#-1和0代表居中
MainHudPosX=0.5
MainHudPosY=0.9
#加减HUD显示位置
#范围（-1，1）
#-1和0代表居中
NoticeHudPosX=0.5
NoticeHudPosY=0.858
#刷新HUD间隔
RefreshHudInterv=1.0
#金钱HUD频道
MoneyChannel=3
#金钱变更HUD频道
NotifyChannel=4

[Ecco.Path]
#数据储存位置
Data="storage"
#禁用地图文件路径
BanMaps="config/BannedMaps.txt"
#金币图标
MoneyIcon="dollar.spr"

[Ecco.Color]
#正数金钱RGBA
Positive=0xFFFF00FF
#负数金钱RGBA
Negative=0xFF0000FF
#增加金钱RGBA
Incresed=0x00FF00FF
#扣除金钱RGBA
Decresed=0xFF0000FF

[Ecco.BuyMenu]
#购买指令Trigger
OpenShopTriggers=["buy", "shop"]
#是否允许死亡玩家购买
AllowDeathPlayerBuy=true
#购买后重新打开菜单
ReOpenMenuAfterBuy=true
#菜单保持的时间
KeepOpenTime=10.0

[Ecco]
#玩家获得的分数转化为金钱的倍数
ScoreToMoneyMultiplier=1.0
#是否储存玩家金钱
#0 不储存
#1 只有系列地图储存
#2 储存
StorePlayerScore=1
#若不储存金钱，设置玩家起始资金
PlayerStartScore=0
#单张地图最大可获得金钱数目
#小于0为不限制
MoneyLimitePerMap=-1
#延迟保存时间（秒）
#小于0为即时保存
#大于等于0为延迟保存，默认30秒
SaveDelayTime=30.0
#默认的语言
DefaultLang="english"
#查询用于查询客户端语言的cvar，留空为不查询
QueryLangCVar="cap_lang"
#Precache偏移量，用户PrecacheOther
#Half-Life（包括cs,dod）为2
#Sven Co-op 为4
#错误的偏移量会导致游戏崩溃
#如果你不知道你的游戏偏移量是多少
#最好的方法是通过precache变量来进行precache
#而不是选择使用other_precache
PrecacheOffset=4
```

## 6. 常见问题

### 6.1 配置文件不生效

- 确保配置文件路径正确
- 确保配置文件格式正确，特别是TOML语法
- 重启服务器或重载插件以应用更改

### 6.2 颜色配置不正确

- 确保颜色值格式为0xRRGGBBAA
- 确保颜色值在有效范围内（0-255）

### 6.3 PrecacheOffset设置错误

- Half-Life（包括CS、DoD）使用值2
- Sven Co-op使用值4
- 错误的偏移量会导致游戏崩溃，请谨慎设置

## 7. 高级配置

### 7.1 自定义金钱图标

1. 准备一个.spr格式的图标文件
2. 将文件放置在适当的目录中
3. 在配置文件中设置MoneyIcon为图标文件名

### 7.2 多语言支持

1. 在配置文件中设置DefaultLang为默认语言
2. 如果需要根据客户端语言自动调整，设置QueryLangCVar为相应的cvar名称

## 8. 总结

TOML配置文件是Ecco插件的核心配置方式，通过修改TOML文件，你可以：

- 自定义HUD显示方式和位置
- 配置文件路径和图标
- 设置颜色方案
- 调整购买菜单行为
- 配置核心游戏机制

希望本指南能帮助你更好地理解和使用Ecco插件的TOML配置文件。如果有任何问题，请参考插件文档或寻求社区支持。
