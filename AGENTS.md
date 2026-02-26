# Ecco - Half-Life Metamod 插件项目 AI Agent 指南

> **项目概述**: Ecco 是一个基于 Metamod 框架的完整经济系统和购买菜单插件，主要面向 Sven Co-op 5.x，提供高度可定制的游戏行为和商店物品购买功能。

---

## 目录

1. [项目架构](#项目架构)
2. [核心技术栈](#核心技术栈)
3. [核心功能模块](#核心功能模块)
4. [数据结构与存储](#数据结构与存储)
5. [配置系统](#配置系统)
6. [脚本系统](#脚本系统)
7. [扩展开发](#扩展开发)
8. [关键文件说明](#关键文件说明)

---

## 项目架构

### 目录结构

```
ecco/
├── include/api/              # 公共API接口定义
│   ├── IEccoPlugin.h        # 插件主接口
│   ├── IEccoFuncs.h         # 核心功能接口
│   ├── IPlayerStorageItem.h # 玩家存储接口
│   ├── IEccoBaseExcutor.h   # 菜单执行器接口
│   └── HookDef.h            # 钩子定义
│
├── src/                      # 核心源代码
│   ├── command/             # 命令系统
│   │   ├── commands.cpp     # 命令注册
│   │   ├── CEccoClientCommand.h/cpp
│   │   └── CEccoServerCommand.h/cpp
│   │
│   ├── config/              # 配置系统
│   │   ├── CConfig.h/cpp    # 配置解析和管理
│   │   └── CConfigParser.h/cpp
│   │
│   ├── hud/                 # HUD显示系统
│   │   ├── hud.h/cpp        # HUD管理器
│   │   └── CHudNumber.h/cpp # 数字显示组件
│   │
│   ├── lang/                # 多语言系统
│   │   └── lang.h/cpp       # 翻译管理
│   │
│   ├── menu/                # 菜单系统
│   │   ├── MenuParser.h/cpp    # 菜单解析
│   │   └── executor/            # 菜单执行器
│   │       ├── CBaseEccoExecutor.h/cpp
│   │       ├── CEccoTextMenuExecutor.h/cpp
│   │       ├── CEccoScriptExecutor.h/cpp
│   │       └── CEccoBackExecutor.h/cpp
│   │
│   ├── plugin/              # 插件系统
│   │   ├── plugin_system.h/cpp
│   │   └── hook.h/cpp       # 钩子系统
│   │
│   ├── scripts/             # 脚本系统
│   │   ├── script_system.h/cpp
│   │   ├── CEccoScriptSystem.h/cpp
│   │   ├── CEccoScriptItem.h/cpp
│   │   ├── tcl_loader.h/cpp
│   │   └── tcl_dynamic.h    # TCL动态绑定
│   │
│   ├── storage/             # 存储系统
│   │   ├── Storage.h/cpp
│   │   ├── CPlayerStorageItem.h/cpp
│   │   └── ScopedFile.h
│   │
│   ├── timer/               # 定时器系统
│   │   └── Timer.h/cpp
│   │
│   ├── dllapi.cpp           # HL SDK 接口
│   ├── meta_api.cpp         # Metamod 接口
│   ├── engine_api.cpp       # 引擎函数接口
│   ├── CEccoFuncs.h/cpp     # 核心功能实现
│   └── plugin_system.cpp    # 插件系统管理
│
├── mmlib/                   # Metamod 工具库
│   └── src/
│       ├── Scheduler.cpp    # 调度器
│       └── Util.cpp         # 工具函数
│
├── metamod/                 # Metamod 框架头文件
├── tcl/                     # TCL 解释器库
├── tomlplusplus/            # TOML 解析库
│
└── package/                 # 运行时包
    ├── Config.toml          # 主配置文件
    ├── lang/                # 语言文件
    │   ├── english.toml
    │   ├── schinese.toml
    │   └── spanish.toml
    └── scripts/             # 脚本文件
        ├── weapon_crowbar.toml
        └── example.toml
```

### 系统架构图

```
┌─────────────────────────────────────────────────────────┐
│                    Half-Life 引擎                          │
└─────────────────────┬───────────────────────────────────┘
                      │
┌─────────────────────▼───────────────────────────────────┐
│                  Metamod 框架                            │
│  - 插件管理                                             │
│  - 函数Hook                                             │
│  - 事件分发                                             │
└─────────────────────┬───────────────────────────────────┘
                      │
┌─────────────────────▼───────────────────────────────────┐
│                    Ecco 核心系统                         │
│  ┌─────────┬─────────┬─────────┬─────────┬──────────┐ │
│  │配置系统  │菜单系统  │脚本系统  │存储系统  │HUD系统  │ │
│  └─────────┴─────────┴─────────┴─────────┴──────────┘ │
│  ┌─────────┬─────────┬─────────┬─────────┬──────────┐ │
│  │命令系统  │多语言  │插件系统  │定时器   │钩子系统  │ │
│  └─────────┴─────────┴─────────┴─────────┴──────────┘ │
└─────────────────────┬───────────────────────────────────┘
                      │
┌─────────────────────▼───────────────────────────────────┐
│                  TCL 脚本引擎                            │
│  - 命令注册                                             │
│  - 类型转换                                             │
│  - 脚本执行                                             │
└─────────────────────┬───────────────────────────────────┘
                      │
┌─────────────────────▼───────────────────────────────────┐
│              文件系统 (TOML + 二进制数据)                │
└─────────────────────────────────────────────────────────┘
```

---

## 核心技术栈

### 开发语言与框架

- **C++17**: 主要开发语言
- **Metamod**: Half-Life 插件框架
- **HL SDK**: Half-Life 软件开发工具包
- **TCL 9.1**: 脚本语言引擎
- **tomlplusplus**: TOML 配置文件解析库

### 编译系统

- **CMake**: 跨平台构建工具
- **Visual Studio**: Windows 平台主要 IDE
- **GCC/Clang**: Linux 平台编译器

### 依赖库

```
ecco/
├── angelscript/          # AngelScript 脚本引擎 (可选)
├── mmlib/               # Metamod 工具库
├── metamod/             # Metamod 框架头文件
├── tcl/                 # TCL 9.1 解释器库
└── tomlplusplus/        # TOML 解析器
```

---

## 核心功能模块

### 1. Metamod 集成模块

**核心文件**: `meta_api.cpp`, `dllapi.cpp`, `engine_api.cpp`

**主要接口**:

```cpp
// Metamod 插件生命周期
int Meta_Query(char* interfaceVersion, plugin_info_t** pPlugInfo, mutil_funcs_t* pMetaUtilFuncs);
int Meta_Attach(PLUG_LOADTIME now, META_FUNCTIONS* pFunctionTable, ...);
int Meta_Detach(PLUG_LOADTIME now, PL_UNLOAD_REASON reason);

// HL SDK 函数表
int GetEntityAPI2(DLL_FUNCTIONS* pFunctionTable, int* interfaceVersion);
int GetNewDLLFunctions(NEW_DLL_FUNCTIONS* pNewDllFunctionTable, int* interfaceVersion);
```

**Hook 事件**:

| 事件类型 | 阶段 | 触发时机 | 用途 |
|---------|------|---------|------|
| GameInit | Pre | 游戏初始化 | 加载TCL库、初始化系统 |
| ServerActivate | Post | 地图加载 | 加载配置、构建菜单 |
| ServerDeactivate | Post | 地图卸载 | 清理资源、保存数据 |
| ClientCommand | Pre | 客户端命令 | 处理购买、重载等命令 |
| ClientDisconnect | Pre | 玩家离开 | 保存玩家数据 |
| ClientPutInServer | Pre | 玩家加入 | 加载玩家数据 |
| StartFrame | Pre | 每帧调用 | 更新HUD、定时器 |

### 2. 配置系统

**核心文件**: `src/config/CConfig.cpp`, `package/Config.toml`

**配置结构**:

```cpp
struct CEccoConfig {
    CEccoConfigHud Hud;           // HUD显示配置
    CEccoConfigPath Path;         // 路径配置
    CEccoConfigColor Color;       // 颜色配置
    CEccoConfigBuyMenu BuyMenu;   // 购买菜单配置
    float ScoreToMoneyMultiplier;   // 分数转金钱倍数
    int StorePlayerScore;          // 存储策略
    int PlayerStartScore;          // 起始资金
    int MoneyLimitePerMap;        // 单图限制
    float SaveDelayTime;          // 保存延迟
    std::string DefaultLang;       // 默认语言
    int PrecacheOffset;           // 预缓存偏移
};
```

**配置访问方式**:

```cpp
// 点号分隔的路径访问
float posX = g_EccoConfig.Hud.MainHudPosX;
int cost = g_EccoConfig.BuyMenu.Cost;

// 运行时修改
g_EccoConfig.Hud.ShowMoneyHUD = 1;
```

**主要功能**:
- TOML 格式配置文件解析
- 运行时配置修改
- 配置验证和默认值
- 配置保存和重新加载

### 3. 菜单系统

**核心文件**: `src/menu/MenuParser.cpp`, `src/menu/executor/`

**菜单类型**:

1. **CEccoTextMenuExecutor** - 文本菜单容器
   - 最多10个选项
   - 自动分页（7项/页）
   - 支持嵌套层级

2. **CEccoScriptExecutor** - 脚本执行项
   - 绑定TCL脚本
   - 成本检查
   - 权限验证

3. **CEccoBackExecutor** - 返回按钮

**菜单层级结构**:

```
根菜单
├── 武器类别
│   ├── 撬棍 (TCL脚本)
│   ├── 手枪 (TCL脚本)
│   └── ...
├── 道具类别
│   ├── 医疗包 (TCL脚本)
│   └── ...
├── [上一页]
├── [下一页]
├── [返回]
└── [退出]
```

**关键特性**:
- 玩家查看状态追踪
- 自动超时关闭
- 分页导航
- 多语言支持

### 4. 脚本系统

**核心文件**: `src/scripts/CEccoScriptSystem.cpp`, `src/scripts/tcl_loader.cpp`

**TCL 集成架构**:

```
TCL 脚本层 (TCL 9.1)
    ↓
命令注册系统 (Tcl_CreateObjCommand)
    ↓
类型转换层 (Tcl_NewIntObj, Tcl_GetStringFromObj)
    ↓
C++ 回调层 (自定义命令函数)
    ↓
游戏引擎层 (HL SDK, Metamod)
```

**内置 TCL 命令**:

| 命令 | 类型 | 功能 |
|------|------|------|
| `Player::GetName` | 查询 | 获取玩家名称 |
| `Player::GetSteamId` | 查询 | 获取Steam ID |
| `Player::GetCredits` | 查询 | 获取金币数量 |
| `Player::SetCredits` | 设置 | 设置金币数量 |
| `give` | 游戏操作 | 给予玩家物品 |
| `take` | 游戏操作 | 移除玩家物品 |
| `saytext` | 消息操作 | 发送消息给玩家 |
| `centersay` | 消息操作 | 屏幕中央消息 |
| `teleport` | 游戏操作 | 传送玩家 |
| `set` | 变量操作 | 设置变量 |
| `get` | 变量操作 | 获取变量 |
| `Int` | 类型转换 | 创建整数对象 |
| `String` | 类型转换 | 创建字符串对象 |
| `Double` | 类型转换 | 创建浮点对象 |
| `Dict` | 容器操作 | 创建字典 |
| `List` | 容器操作 | 创建列表 |

**脚本示例**:

```tcl
# 给予玩家武器
give $caller_index "weapon_crowbar"

# 获取玩家信息并显示
set player_name [Player::GetName $caller_index]
set player_steamid [Player::GetSteamId $caller_index]
saytext $caller_index "${player_name} (${player_steamid}) 购买了撬棍"
```

**关键特性**:
- UTF-8 编码支持
- 类型安全
- 错误处理
- 性能优化（对象池）

### 5. 存储系统

**核心文件**: `src/storage/Storage.cpp`, `src/storage/CPlayerStorageItem.cpp`

**存储策略**:

| 策略值 | 名称 | 描述 | 适用场景 |
|-------|------|------|---------|
| 0 | 不存储 | 仅内存存储 | 测试环境 |
| 1 | 系列地图存储 | 同系列地图间共享 | 战役模式 |
| 2 | 持久化存储 | 跨地图持久化 | 服务器模式 |

**存储数据结构**:

```cpp
struct save_data_t {
    char SteamId[24];        // Steam ID (ASCII)
    int Credits;            // 金币数量
    char Lang[16];          // 语言代码
    short Admin;            // 管理员等级
    unsigned long long Flags; // 标志位
};
```

**存储位置**:
- 目录: `{GAME}/addons/ecco/storage/`
- 文件名: `{SteamId64}.dat`
- 格式: 原始二进制结构体

**脏标记机制**:

```cpp
class CPlayerStorageItem {
    bool m_bDirty;                   // 脏标记
    float m_fLastSaveTime;           // 上次保存时间

    void MarkDirty() {
        m_bDirty = true;
    }

    void SaveData() {
        if (m_bDirty) {
            // 写入文件
            WriteSaveData();
            m_bDirty = false;
            m_fLastSaveTime = gpGlobals->time;
        }
    }
};
```

**延迟保存**:
- 避免频繁磁盘IO
- 可配置延迟时间 (`SaveDelayTime`)
- 服务器停用时强制保存

### 6. 多语言系统

**核心文件**: `src/lang/lang.cpp`, `package/lang/*.toml`

**支持语言**:
- `english` - 英语
- `schinese` - 简体中文
- `spanish` - 西班牙语

**翻译API**:

```cpp
// 获取翻译字符串
std::string GetTranslation(edict_t* player, std::string key);

// 带参数的翻译 (占位符替换: {0}, {1}, ...)
template <typename... Strings>
void ClientPrintfTranslation(edict_t* target, PRINT_TYPE type,
    std::string text_key, Strings&&... args);

// 示例
ClientPrintfTranslation(pPlayer, PRINT_CHAT, "not_enough_credits",
    item_name, item_cost, player_credits);
```

**语言文件结构** (`package/lang/english.toml`):

```toml
[Lang]
not_enough_credits="{0} needs ${1}, but you only have ${2}."
ecco_menu_back="Back"
ecco_menu_prev_page="Previous"
ecco_menu_next_page="Next"
ecco_menu_exit="Exit"
ecco_menu_root="\\w[\\yEcco Store\\w]\\n\\c============"
```

**占位符替换**:
- UTF-8 编码
- 支持任意数量参数
- 自动检测玩家语言

### 7. HUD 系统

**核心文件**: `src/hud/hud.cpp`, `src/hud/CHudNumber.cpp`

**HUD 功能**:
- 金币实时显示
- 金币变化通知
- 正负数不同颜色
- 自定义位置和颜色
- 渐变效果

**HUD 配置**:

```cpp
struct CEccoConfigHud {
    int ShowMoneyHUD;          // 显示模式 (0-3)
    float MainHudPosX;         // 主HUD X位置 (-1.0 ~ 1.0)
    float MainHudPosY;         // 主HUD Y位置 (-1.0 ~ 1.0)
    float NoticeHudPosX;       // 通知HUD X位置
    float NoticeHudPosY;       // 通知HUD Y位置
    float RefreshHudInterv;    // 刷新间隔 (秒)
    int MoneyChannel;          // 金币HUD频道
    int NotifyChannel;         // 通知HUD频道
};
```

**颜色配置**:

```cpp
struct CEccoConfigColor {
    unsigned int Positive;     // 正数颜色 (RGBA)
    unsigned int Negative;     // 负数颜色
    unsigned int Incresed;     // 增加颜色
    unsigned int Decresed;     // 扣除颜色
};
```

**显示模式**:
- 0: 关闭
- 1: 仅金币数量
- 2: 金币数量 + 变化通知
- 3: 完整HUD

### 8. 命令系统

**核心文件**: `src/command/commands.cpp`

**客户端命令**:

| 命令 | 功能 | 权限 |
|------|------|------|
| `buy` / `shop` | 打开购买菜单 | 所有玩家 |
| `reload` | 重载脚本和配置 | 管理员 |
| `lang` | 设置语言 | 所有玩家 |
| `help` | 显示帮助 | 所有玩家 |

**服务器命令** (控制台):

| 命令 | 功能 | 参数 |
|------|------|------|
| `set_admin` | 设置管理员 | `<steamid> <level>` |
| `listplugins` | 列出插件 | 无 |
| `setconfig` | 设置配置 | `<path> <value>` |
| `saveconfig` | 保存配置 | 无 |
| `dump_tcldocs` | 导出TCL文档 | `<filepath>` |

### 9. 钩子系统

**核心文件**: `src/plugin/hook.cpp`

**钩子类型**:

```cpp
enum class Hook_Type {
    ON_PLAYER_CREDITS_CHANGED,   // 金币变化时
    ON_PLAYER_SCORE_TO_CREDITS,  // 分数转金币时
    ON_MENU_EXCUTE               // 菜单执行时
};
```

**钩子签名**:

```cpp
// 金币变化钩子
using Hook_CreditsChanged = std::function<void(
    IPlayerStorageItem* player,
    int old_credits,
    int& new_credits
)>;

// 菜单执行钩子
using Hook_MenuExecute = std::function<void(
    IEccoBaseExcutor* executor,
    edict_t* player,
    int& cost,
    bool& can_execute
)>;
```

**钩子使用示例**:

```cpp
// 注册钩子 - 双倍金币
void OnCreditsChanged(IPlayerStorageItem* player, int old, int& credits) {
    credits *= 2;
}
AddHook(Hook_Type::ON_PLAYER_CREDITS_CHANGED, OnCreditsChanged);

// 注册钩子 - 菜单折扣
void OnMenuExecute(IEccoBaseExcutor* executor, edict_t* player, int& cost, bool& can_execute) {
    if (cost > 0) {
        cost = int(cost * 0.8); // 8折优惠
    }
}
AddHook(Hook_Type::ON_MENU_EXCUTE, OnMenuExecute);

// 移除钩子
RemoveHook(Hook_Type::ON_PLAYER_CREDITS_CHANGED, OnCreditsChanged);
```

### 10. 定时器系统

**核心文件**: `src/timer/Timer.cpp`, `mmlib/src/Scheduler.cpp`

**定时器类型**:

```cpp
// 一次性定时器
void SetTimer(float delay, std::function<void()> callback);

// 重复定时器
void SetRepeatingTimer(float interval, std::function<void()> callback);

// 移除定时器
void RemoveTimer(int timer_id);
```

**内置定时器**:
- HUD 更新定时器 (每秒刷新)
- 分数转金币定时器 (定期检查)
- 存储保存定时器 (延迟保存)

---

## 数据结构与存储

### 核心数据结构

#### 1. 玩家存储项

```cpp
class CPlayerStorageItem : public IPlayerStorageItem {
private:
    save_data_t m_saveData;          // 保存的数据
    edict_t* m_pPlayer;              // 玩家实体
    int m_iScore;                    // 分数
    int m_iLastCredits;              // 上次金币
    bool m_bDirty;                   // 脏标记
    float m_fLastSaveTime;           // 上次保存时间

public:
    // 金币管理
    int GetCredits() const override;
    void SetCredits(int credits) override;
    void AddCredits(int amount) override;

    // 玩家信息
    const char* GetSteamId() const override;
    const char* GetName() const override;
    const char* GetLang() const override;
    void SetLang(const char* lang) override;

    // 权限管理
    int GetAdminLevel() const override;
    void SetAdminLevel(int level) override;

    // 标志位
    bool HasFlag(unsigned long long flag) const;
    void SetFlag(unsigned long long flag, bool value);

    // 存储
    void MarkDirty();
    void SaveData();
    void LoadData();
};
```

#### 2. 脚本物品定义

```cpp
class CEccoScriptItem {
private:
    std::string m_szName;             // 菜单项名称
    std::string m_szId;               // 唯一标识符
    int m_iFlags;                     // 标志位
    int m_iCost;                      // 成本
    std::string m_szScripts;          // TCL脚本内容

    std::vector<char*> m_aryPrecaches;          // 模型预缓存
    std::vector<char*> m_arySoundPrecaches;     // 声音预缓存
    std::vector<char*> m_aryOtherPrecaches;     // 其他预缓存

public:
    const char* GetName() const;
    const char* GetId() const;
    int GetCost() const;
    const char* GetScript() const;

    // 预缓存
    void Precache();
};
```

#### 3. 菜单执行器基类

```cpp
class CBaseEccoExecutor : public IEccoBaseExcutor {
protected:
    std::string m_szDisplayName;     // 显示名称
    CBaseEccoExecutor* m_pParent;    // 父菜单

public:
    virtual void Execute(edict_t* player) = 0;
    virtual const char* GetDisplayName() const = 0;
    virtual int GetCost() const = 0;

    void SetDisplayName(const char* name);
    void SetParent(CBaseEccoExecutor* parent);
};
```

#### 4. 文本菜单执行器

```cpp
class CEccoTextMenuExecutor : public CBaseEccoExecutor {
private:
    std::vector<IEccoBaseExcutor*> m_vecItems;  // 子菜单项
    std::unordered_set<edict_t*> m_setViewing;  // 查看中的玩家
    int m_iPage;                               // 当前页码

public:
    static constexpr int ITEMS_PER_PAGE = 7;
    static constexpr int MAX_ITEMS = 10;

    void AddItem(IEccoBaseExcutor* item);
    void RemoveItem(IEccoBaseExcutor* item);
    void Clear();

    void Open(edict_t* player);
    void Close(edict_t* player);
    void Execute(edict_t* player) override;
};
```

#### 5. 脚本执行器

```cpp
class CEccoScriptExecutor : public CBaseEccoExecutor {
private:
    std::bitset<32> m_bitFlags;      // 标志位
    std::string m_szScript;          // TCL脚本内容
    int m_iCost;                     // 花费

public:
    void SetScript(const char* script);
    void SetCost(int cost);
    void SetFlag(int index, bool value);

    void Execute(edict_t* player) override;
};
```

### 存储流程

```
玩家加入服务器
    ↓
LoadPlayerData(SteamId)
    ↓
读取 {SteamId}.dat
    ↓
解析 save_data_t 结构
    ↓
创建 CPlayerStorageItem
    ↓
游戏运行
    ↓
数据修改 → MarkDirty()
    ↓
定时检查 → SaveData()
    ↓
写入 {SteamId}.dat
```

---

## 配置系统

### 主配置文件 (Config.toml)

```toml
# HUD 显示配置
[Ecco.Hud]
ShowMoneyHUD=1              # HUD显示模式 (0-3)
MainHudPosX=0.5             # 主HUD X位置 (-1.0 ~ 1.0)
MainHudPosY=0.9             # 主HUD Y位置 (-1.0 ~ 1.0)
NoticeHudPosX=0.5           # 通知HUD X位置
NoticeHudPosY=0.858         # 通知HUD Y位置
RefreshHudInterv=1.0        # 刷新间隔 (秒)
MoneyChannel=3              # 金币HUD频道 (1-4)
NotifyChannel=4             # 通知HUD频道 (1-4)

# 路径配置
[Ecco.Path]
Data="storage"              # 数据存储目录
BanMaps="config/BannedMaps.txt"  # 禁用地图列表
MoneyIcon="dollar.spr"      # 金币图标

# 颜色配置 (RGBA格式)
[Ecco.Color]
Positive=0xFFFF00FF         # 正数颜色 (黄色)
Negative=0xFF0000FF         # 负数颜色 (红色)
Incresed=0x00FF00FF         # 增加颜色 (绿色)
Decresed=0xFF0000FF         # 扣除颜色 (红色)

# 购买菜单配置
[Ecco.BuyMenu]
OpenShopTriggers=["buy", "shop"]  # 购买命令列表
AllowDeathPlayerBuy=true          # 允许死亡玩家购买
ReOpenMenuAfterBuy=true          # 购买后重新打开菜单
KeepOpenTime=10.0                 # 菜单保持打开时间 (秒)

# 核心配置
[Ecco]
ScoreToMoneyMultiplier=1.0         # 分数转金币倍数
StorePlayerScore=1                # 存储策略 (0-2)
PlayerStartScore=0                # 起始资金
MoneyLimitePerMap=-1              # 单图金币限制 (-1=无限制)
SaveDelayTime=30.0                # 保存延迟 (秒)
DefaultLang="english"             # 默认语言
QueryLangCVar="cap_lang"          # 语言CVar名称
PrecacheOffset=4                  # 预缓存偏移值
```

### 脚本配置示例 (weapon_crowbar.toml)

```toml
[Ecco]
name="Weapon.Crowbar"           # 菜单项唯一ID
cost=2                           # 购买花费
script="""
give $caller_index "weapon_crowbar"
"""                              # TCL脚本内容
flags=0                          # 标志位
precaches=[]                     # 模型预缓存列表
sound_precaches=[]               # 声音预缓存列表
other_precaches=["weapon_crowbar"] # 其他预缓存
```

### 语言配置示例 (schinese.toml)

```toml
[Lang]
not_enough_credits="{0} 需要 ${1} 金币，但你只有 ${2} 金币。"
ecco_menu_back="返回"
ecco_menu_prev_page="上一页"
ecco_menu_next_page="下一页"
ecco_menu_exit="退出"
ecco_menu_root="\\w[\\yEcco 商店\\w]\\n\\c============"
```

---

## 脚本系统

### TCL 9.1 集成

**版本**: Tcl 9.1
**编码**: UTF-8
**路径**: `{GAME}/addons/ecco/tcl/`

### 脚本API层次结构

```
┌─────────────────────────────────────┐
│      TCL 脚本层                      │
│  (用户编写的 .tcl 或 .toml 脚本)     │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│      命令解析层                      │
│  (Tcl_CreateObjCommand 注册命令)      │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│      类型转换层                      │
│  (Tcl_NewIntObj, Tcl_GetString等)    │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│      C++ 回调层                      │
│  (自定义命令函数实现)                 │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│      游戏引擎层                      │
│  (HL SDK, Metamod API)              │
└─────────────────────────────────────┘
```

### 内置命令详解

#### 玩家操作命令

```tcl
# 获取玩家信息
set name [Player::GetName $caller_index]
set steamid [Player::GetSteamId $caller_index]
set credits [Player::GetCredits $caller_index]

# 设置玩家金币
Player::SetCredits $caller_index 100
Player::AddCredits $caller_index 50
```

#### 游戏操作命令

```tcl
# 给予物品
give $caller_index "weapon_crowbar"
give $caller_index "item_healthkit"
```

#### 消息操作命令

```tcl
# 聊天消息
saytext $caller_index "这是一条消息"

# 屏幕中央消息
centersay $caller_index "这是一条中央消息"

# 发送给所有玩家
saytext 0 "这是一条广播消息"
```

#### 变量操作

```tcl
# 设置变量
set my_var "Hello"

# 获取变量
set value $my_var

# 数值运算
set result [expr {10 + 20}]
```

### 脚本加载流程

```
ServerActivate (地图加载)
    ↓
LoadEccoScriptItems()
    ↓
遍历 package/scripts/*.toml
    ↓
解析 TOML 文件
    ↓
创建 CEccoScriptItem 对象
    ↓
调用 Precache() 预缓存资源
    ↓
将脚本项添加到菜单树
    ↓
菜单构建完成
```

### 高级脚本示例

```tcl
# 复杂购买逻辑
set cost 10
set player_credits [Player::GetCredits $caller_index]

if {$player_credits >= $cost} {
    # 扣除金币
    Player::SetCredits $caller_index [expr {$player_credits - $cost}]

    # 给予物品
    give $caller_index "weapon_rpg"
    give $caller_index "ammo_rpgclip"

    # 发送消息
    set player_name [Player::GetName $caller_index]
    saytext $caller_index "${player_name} 购买了 RPG 发射器！"

    # 播放声音
    emitsound $caller_index "items/gunpickup2.wav"
} else {
    saytext $caller_index "金币不足！需要 ${cost} 金币，你只有 ${player_credits} 金币。"
}
```

---

## 扩展开发

### 创建新插件

**步骤 1**: 定义插件类，继承 `IEccoPlugin`

```cpp
// MyPlugin.h
#pragma once
#include "IEccoPlugin.h"

class CMyPlugin : public IEccoPlugin {
public:
    const char* GetName() const override {
        return "MyPlugin";
    }

    const char* GetDescription() const override {
        return "我的自定义插件";
    }

    const char* GetVersion() const override {
        return "1.0.0";
    }

    const char* GetAuthor() const override {
        return "YourName";
    }

    void Query(IEccoFuncs* pEccoFuncs) override {
        // 查询插件依赖
    }

    void Initialize() override {
        // 初始化插件
        // 注册钩子
        AddHook(Hook_Type::ON_PLAYER_CREDITS_CHANGED,
            [](IPlayerStorageItem* player, int old, int& credits) {
                // 双倍金币逻辑
                credits *= 2;
            }
        );
    }

    void Shutdown() override {
        // 清理资源
    }
};
```

**步骤 2**: 注册插件

```cpp
// 在适当位置（如 CEccoFuncs::Initialize）
void LoadPlugins() {
    RegisterPlugin(new CMyPlugin());
    // ... 其他插件
}
```

### 注册自定义菜单

```cpp
// 创建菜单项
auto weaponMenu = new CEccoTextMenuExecutor();
weaponMenu->SetDisplayName("武器");

// 添加脚本执行项
auto crowbarItem = new CEccoScriptExecutor();
crowbarItem->SetDisplayName("撬棍");
crowbarItem->SetCost(2);
crowbarItem->SetScript("give $caller_index \"weapon_crowbar\"");
weaponMenu->AddItem(crowbarItem);

// 添加到根菜单
g_pRootMenu->AddItem(weaponMenu);
```

### 添加自定义TCL命令

```cpp
// 在 CEccoScriptSystem 中注册
Tcl_CreateObjCommand(m_pInterp, "MyCommand",
    [](ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[]) -> int {
        // 验证参数数量
        if (objc < 2) {
            Tcl_WrongNumArgs(interp, 1, objv, "arg1 ?arg2 ...?");
            return TCL_ERROR;
        }

        // 获取参数
        const char* arg1 = Tcl_GetString(objv[1]);

        // 执行逻辑
        // ...

        // 返回结果
        Tcl_SetObjResult(interp, Tcl_NewStringObj("Success", -1));
        return TCL_OK;
    },
    nullptr, nullptr
);
```

### 使用钩子系统

```cpp
// 示例：VIP玩家购买折扣
void OnMenuExecute(IEccoBaseExcutor* executor, edict_t* player,
                   int& cost, bool& can_execute) {
    // 获取玩家存储
    auto storage = g_pPlayerStorage->GetPlayerStorage(player);
    if (!storage) return;

    // 检查VIP标志
    if (storage->HasFlag(VIP_FLAG)) {
        cost = int(cost * 0.7); // VIP 7折
    }
}

// 注册钩子
AddHook(Hook_Type::ON_MENU_EXCUTE, OnMenuExecute);
```

---

## 关键文件说明

### 入口文件

| 文件 | 职责 |
|------|------|
| `dllmain.cpp` | DLL 入口点，初始化CRT |
| `meta_api.cpp` | Metamod 插件接口实现 |
| `dllapi.cpp` | HL SDK 函数表注册 |
| `engine_api.cpp` | 引擎函数接口 |

### 核心系统文件

| 文件 | 职责 |
|------|------|
| `CEccoFuncs.h/cpp` | 核心功能实现和导出 |
| `plugin_system.h/cpp` | 插件系统管理 |
| `script_system.h/cpp` | 脚本系统封装 |
| `Storage.h/cpp` | 存储系统管理 |
| `CPlayerStorageItem.h/cpp` | 玩家存储项实现 |
| `CEccoScriptSystem.h/cpp` | TCL脚本系统实现 |

### 辅助库

| 目录 | 职责 |
|------|------|
| `metamod/`  +  `mmlib/` | Metamod 工具库 (调度器、工具函数) |
| `tomlplusplus/` | TOML 配置解析库 |
| `tcl/` | TCL 9.1 解释器库 |

### 构建文件

| 文件 | 职责 |
|------|------|
| `CMakeLists.txt` | CMake 构建配置 |
| `ecco.sln` | Visual Studio 解决方案 |
| `win_build.ps1` | Windows 构建脚本 |
| `linux_build.sh` | Linux 构建脚本 |
| `win_init.ps1` | Windows 初始化脚本 |
| `linux_init.sh` | Linux 初始化脚本 |

---

## 项目特色

### 1. 高度可配置
- **TOML 配置**: 人性化的配置格式
- **运行时修改**: 无需重启即可生效
- **多语言支持**: 内置3种语言，易于扩展

### 2. 强大的脚本系统
- **TCL 9.1 集成**: 稳定高效的脚本引擎
- **丰富的API**: 涵盖游戏操作的各个方面
- **类型安全**: 完善的类型检查和转换

### 3. 插件化架构
- **清晰的接口定义**: 标准化的插件API
- **钩子机制**: 灵活的事件拦截和修改
- **易于扩展**: 最小化侵入性

### 4. 完善的存储系统
- **多策略支持**: 适应不同游戏模式
- **延迟保存**: 优化IO性能
- **脏标记机制**: 智能的数据更新

### 5. 用户友好
- **分层菜单**: 清晰的物品分类
- **HUD 显示**: 实时反馈
- **多命令触发**: 灵活的访问方式

---

## AI Agent 开发指南

### 常见任务

#### 1. 添加新的购买物品

**步骤**:
1. 在 `package/scripts/` 创建 `.toml` 文件
2. 编写 TCL 脚本逻辑
3. 配置成本、预缓存等参数
4. 重新加载配置 (`reload` 命令)

**示例**: `package/scripts/weapon_satchel.toml`

```toml
[Ecco]
name="Weapon.Satchel"
cost=5
script="""
give $caller_index "weapon_satchel"
"""
flags=0
precaches=[]
sound_precaches=[]
other_precaches=["weapon_satchel", "ammo_satchel"]
```

#### 2. 修改HUD显示

**步骤**:
1. 编辑 `package/Config.toml`
2. 调整 `[Ecco.Hud]` 配置项
3. 执行 `reload` 命令

**示例**: 修改HUD位置

```toml
[Ecco.Hud]
MainHudPosX=0.3        # 左移
MainHudPosY=0.95       # 下移
```

#### 3. 添加新语言

**步骤**:
1. 在 `package/lang/` 创建 `newlang.toml`
2. 翻译所有键值
3. 重新加载配置

**示例**: `package/lang/french.toml`

```toml
[Lang]
not_enough_credits="{0} a besoin de ${1}, mais vous n'avez que ${2}."
ecco_menu_back="Retour"
# ... 其他翻译
```

#### 4. 创建自定义插件

**参考**: [扩展开发](#扩展开发) 章节

### 代码修改注意事项

1. **配置修改**: 编辑 `Config.toml` 后执行 `reload` 命令
2. **脚本修改**: 编辑 `.toml` 脚本文件后执行 `reload` 命令
3. **核心代码修改**: 需要重新编译插件
4. **TCL命令修改**: 修改 `CEccoScriptSystem.cpp` 并重新编译

### 调试技巧

1. **启用详细日志**: 检查 `server.log`
2. **使用 TCL 测试**: 通过 `setconfig` 测试配置
3. **单步调试**: 使用 Visual Studio 调试器
4. **检查钩子**: 使用 `dump_tcldocs` 导出文档

---

## 总结

Ecco 是一个设计精良的 Half-Life Metamod 插件，具有以下核心优势：

1. **模块化设计**: 清晰的职责分离，易于理解和维护
2. **扩展性强**: 插件系统和钩子机制提供灵活的扩展点
3. **配置灵活**: TOML 配置和多语言支持适应各种需求
4. **脚本强大**: TCL 集成提供高度定制化能力
5. **存储完善**: 多策略持久化系统适应不同场景

该项目为 Sven Co-op 等 Half-Life 模组提供了完整的经济和商店解决方案，是学习 Metamod 插件开发的优秀参考。

---

## 附录

### A. 完整的钩子类型列表

```cpp
enum class Hook_Type {
    ON_PLAYER_CREDITS_CHANGED,     // 金币变化
    ON_PLAYER_SCORE_TO_CREDITS,    // 分数转金币
    ON_MENU_EXCUTE                 // 菜单执行
};
```

### B. 完整的客户端消息类型

```cpp
enum PRINT_TYPE {
    PRINT_NOTIFY = 1,   // 通知 (右上角)
    PRINT_CONSOLE = 2,  // 控制台
    PRINT_TALK = 3,     // 聊天
    PRINT_CENTER = 4    // 屏幕中央
};
```

### C. 存储策略详细说明

| 策略值 | 名称 | 行为 |
|-------|------|------|
| 0 | 不存储 | 数据仅保存在内存，服务器重启后丢失 |
| 1 | 系列地图存储 | 数据在系列地图间共享，系列结束后清除 |
| 2 | 持久化存储 | 数据永久保存，跨地图和服务器重启 |

### D. TCL 命令快速参考

| 类别 | 命令 |
|------|------|
| 玩家 | `Player::GetName`, `Player::GetSteamId`, `Player::GetCredits`, `Player::SetCredits` |
| 物品 | `give`, `take` |
| 消息 | `saytext`, `centersay` |
| 变量 | `set`, `get` |
| 类型 | `Int`, `String`, `Double` |
| 容器 | `Dict`, `List` |

---

**文档版本**: 1.0
**最后更新**: 2026-02-26
**适用于**: Ecco 1.x (基于 Metamod 的 Half-Life 插件)
