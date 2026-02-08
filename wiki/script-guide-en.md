# Ecco Script Writing Guide

## 1. Script System Overview

The Ecco plugin uses TOML format configuration files combined with the Tcl scripting engine, allowing server administrators to customize game behavior, create buy menu items, implement special features, and more by writing scripts. The script system is one of the core features of the Ecco plugin, providing extremely high extensibility and customizability.

### 1.1 Script System Components

- **Script Engine**: Based on Tcl implementation, responsible for parsing and executing scripts
- **Script Configuration**: Uses TOML format configuration files, containing basic script information and content
- **Menu Parser**: Builds hierarchical buy menus based on script names
- **Resource Preloading**: Supports preloading models, sounds, and other resources

## 2. Script File Structure

Ecco plugin script files are typically located in the `addons/ecco/scripts/` directory and use the `.toml` file extension. Each script file corresponds to an item in the buy menu.

### 2.1 Basic Script File Structure

Each script file consists of the following parts:

```toml
# Only content under the Ecco section will be read
[Ecco]
# Menu settings, use . to set menu hierarchy
# Set to id here, set the corresponding translation for id in lang/user_setting.toml
name = "Category.SubCategory.ItemName"
# How much money to spend
cost = 1000
# Script written in Tcl

script = """
# Script content
# Write Tcl script code here
"""
flags = 0
# Resources that need to be Precached
precaches = []
sound_precaches = []
other_precaches = []
```

### 2.2 Script Parameter Description

| Parameter | Description | Example Value |
|-----------|-------------|---------------|
| name | Script item name, used to build menu hierarchy | "Items.Health.HealthKit" |
| cost | Money required to purchase this item | 1000 |
| script | Script content, using Tcl syntax | """give $caller_index "weapon_9mmhandgun"""" |
| flags | Script item flags, used to control behavior | 0 (no special flags) |
| precaches | Model resources that need to be preloaded | ["models/weapons/v_9mmhandgun.mdl"] |
| sound_precaches | Sound resources that need to be preloaded | ["sound/weapons/9mmhandgun1.wav"] |
| other_precaches | Other resources that need to be preloaded | ["item_healthkit"] |

## 3. Script Writing Basics

### 3.1 Tcl Syntax Basics

Tcl is a simple and easy-to-learn scripting language with the following characteristics:

- Command format: `command arg1 arg2 ...`
- Variables: Use `$variable` to reference variables, use `set variable value` to set variables
- Strings: Can be enclosed in double quotes or curly braces
- Comments: Lines starting with `#`
- Control structures: `if`, `for`, `foreach`, `while`, etc.

### 3.2 Script Execution Environment

Each script gets the following environment when executed:

- **Player Information**: The player currently executing the script, accessed via the `$caller_index` variable
- **Script API**: A series of functions provided by Ecco
- **Tcl Standard Library**: Built-in Tcl commands and functions

## 4. Script API Reference

### 4.1 Player-Related APIs

| Function Name | Description | Parameters | Return Value |
|---------------|-------------|------------|--------------|
| `Player::GetName` | Get player name | Player index | Player name (string) |
| `Player::GetSteamId` | Get player SteamID | Player index | SteamID (string) |
| `give` | Give player weapon or item | Player index, item name | None |
| `saytext` | Make player speak | Player index, message content | None |

### 4.2 Game-Related APIs

| Function Name | Description | Parameters | Return Value |
|---------------|-------------|------------|--------------|
| `server_print` | Print message to server console | Message content | None |
| `client_print` | Send message to player | Player index, message content | None |
| `center_print` | Send center message to player | Player index, message content | None |
| `server_cmd` | Execute server command | Command content | None |
| `get_map_name` | Get current map name | None | Map name (string) |
| `get_time` | Get current game time | None | Time (float) |
| `rand` | Generate random number | Minimum value, maximum value | Random number (integer) |

### 4.3 Script Control APIs

| Function Name | Description | Parameters | Return Value |
|---------------|-------------|------------|--------------|
| `abort` | Abort script execution | None | None |
| `error` | Throw error | Error message | None |
| `return` | Return value and end script | Return value (optional) | Return value |

## 5. Script Examples

### 5.1 Basic Examples

#### 5.1.1 Health Kit Script

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

#### 5.1.2 Weapon Purchase Script

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

### 5.2 Advanced Examples

#### 5.2.1 Player Information Script

```toml
[Ecco]
name = "TestCategory.TestItem"
cost = 0
script = """
give $caller_index "weapon_9mmhandgun"
set player_name [Player::GetName $caller_index]
set player_id [Player::GetSteamId $caller_index]
saytext $caller_index "${player_name} is whispering quietly ${player_id}"
"""
flags = 0
precaches = []
sound_precaches = []
other_precaches = []
```

#### 5.2.2 Random Effect Script

```toml
[Ecco]
name = "Items.Special.RandomEffect"
cost = 1000
script = """
# Generate random number (1-5)
set random [rand 1 5]

# Execute different effects based on random number
switch $random {
    1 {
        # Effect 1: Give player health kit
        give $caller_index "item_healthkit"
        saytext $caller_index "Lucky! You got a health kit!"
    }
    2 {
        # Effect 2: Give player armor
        give $caller_index "item_battery"
        saytext $caller_index "Nice! You got a battery!"
    }
    3 {
        # Effect 3: Give player weapon
        give $caller_index "weapon_9mmhandgun"
        saytext $caller_index "Good! You got a 9mm handgun!"
    }
    4 {
        # Effect 4: Give player shotgun
        give $caller_index "weapon_shotgun"
        saytext $caller_index "Great! You got a shotgun!"
    }
    5 {
        # Effect 5: Give player grenade
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

## 6. Menu System Integration

### 6.1 Menu Hierarchy

The format of the script name determines its position in the buy menu. Names separated by dots (.) are parsed as a hierarchical structure:

- `Items.Health.HealthKit` → Items → Health → Health Kit
- `Items.Weapons.9mmHandgun` → Items → Weapons → 9mm Handgun
- `Items.Special.RandomEffect` → Items → Special → Random Effect

### 6.2 Menu Display

- Menus are automatically sorted alphabetically
- Each menu page displays a maximum of 7 items
- More than 7 items automatically creates pagination
- The buy menu can be opened via `buy` or `shop` commands

## 7. Script Best Practices

### 7.1 Performance Optimization

- Avoid time-consuming operations in scripts
- Use variables reasonably to avoid repeated calculations
- For complex logic, consider splitting into multiple small scripts

### 7.2 Error Handling

- Use `if` statements to check parameter validity
- Catch possible error conditions
- Provide clear error messages to players

### 7.3 Security

- Avoid using unverified user input
- Limit script execution permissions
- Regularly check scripts for security vulnerabilities

### 7.4 Code Style

- Use consistent indentation (4 spaces recommended)
- Add comments to explain complex logic
- Use meaningful variable and function names
- Keep scripts concise and clear

## 8. Common Issues

### 8.1 Script Not Executing

- Check if script syntax is correct
- Ensure script file format is correct (.toml)
- Check script file permissions
- Check server console for error messages

### 8.2 Menu Items Not Displaying

- Check if script name format is correct
- Ensure script file contains all necessary parameters
- Check if buy menu is loaded correctly
- Check if there are corresponding translations in lang/user_setting.toml

### 8.3 API Function Call Failed

- Check if function name is correct
- Ensure parameter types and quantities are correct
- Check server console for error messages

## 9. Advanced Features

### 9.1 Resource Preloading

The Ecco plugin supports preloading models, sounds, and other resources to ensure resources are loaded when scripts execute:

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

### 9.2 Multi-language Support

Scripts can support multiple languages by setting corresponding translations for ids in lang/user_setting.toml:

```toml
# lang/user_setting.toml
[en]
"Items.Health.HealthKit" = "Health Kit"

[zh]
"Items.Health.HealthKit" = "健康包"
```

### 9.3 Script Event Handling

You can write scripts to respond to game events, such as player death, map change, etc. For specific implementation methods, please refer to the plugin documentation.

## 10. Script File Management

### 10.1 Script File Organization

- Place related scripts in the same directory
- Use meaningful file names
- Regularly backup script files

### 10.2 Script Reloading

- Scripts can be reloaded via the server command `ecco_reload_scripts`
- Reloading scripts re-parses all script files
- The reloading process may temporarily affect the game experience

## 11. Summary

The Ecco plugin's script system provides powerful tools for server administrators to:

- Customize buy menu items
- Implement special game mechanics
- Create unique game experiences
- Quickly prototype and test new features

Through this guide, you should be able to write basic Ecco scripts and gradually master more advanced features. If you have any questions, please refer to the plugin documentation or seek community support.

## 12. Reference Resources

- [Tcl Official Documentation](https://www.tcl.tk/man/tcl8.6/) - Learn detailed Tcl syntax and features
- [Ecco Plugin Example Scripts](https://github.com/example/ecco/tree/master/package/scripts) - More script examples
- [TOML Official Documentation](https://toml.io/en/) - Learn detailed TOML format specifications

I hope this guide helps you better understand and use the Ecco plugin's script system. Happy scripting!
