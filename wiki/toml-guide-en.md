# TOML Configuration File Guide

## 1. What is TOML?

TOML (Tom's Obvious, Minimal Language) is a concise and clear configuration file format designed for human readability. It is easy to read and write, and also easy for machines to parse. The Ecco plugin uses TOML files to store configuration information, including HUD settings, path configuration, color settings, buy menu settings, etc.

## 2. TOML File Structure

The Ecco plugin's TOML configuration file follows the following structure:

```toml
[Ecco.Hud]
# HUD related configuration

[Ecco.Path]
# Path related configuration

[Ecco.Color]
# Color related configuration

[Ecco.BuyMenu]
# Buy menu related configuration

[Ecco]
# Core configuration
```

## 3. Configuration Details

### 3.1 HUD Configuration ([Ecco.Hud])

| Configuration Item | Description | Optional Values | Default Value |
|-------------------|-------------|-----------------|---------------|
| ShowMoneyHUD | How to display money HUD | 0 (don't display), 1 (display all), 2 (only display money), 3 (only display money changes) | 1 |
| MainHudPosX | Money HUD display position X coordinate | Range (-1, 1), -1 and 0 represent center | 0.5 |
| MainHudPosY | Money HUD display position Y coordinate | Range (-1, 1), -1 and 0 represent center | 0.9 |
| NoticeHudPosX | Money change HUD display position X coordinate | Range (-1, 1), -1 and 0 represent center | 0.5 |
| NoticeHudPosY | Money change HUD display position Y coordinate | Range (-1, 1), -1 and 0 represent center | 0.858 |
| RefreshHudInterv | Refresh HUD interval (seconds) | Any positive number | 1.0 |
| MoneyChannel | Money HUD channel | Any integer | 3 |
| NotifyChannel | Money change HUD channel | Any integer | 4 |

### 3.2 Path Configuration ([Ecco.Path])

| Configuration Item | Description | Example Value | Default Value |
|-------------------|-------------|---------------|---------------|
| Data | Data storage location | "storage" | "storage" |
| BanMaps | Banned maps file path | "config/BannedMaps.txt" | "config/BannedMaps.txt" |
| MoneyIcon | Coin icon | "dollar.spr" | "dollar.spr" |

### 3.3 Color Configuration ([Ecco.Color])

| Configuration Item | Description | Format | Default Value |
|-------------------|-------------|--------|---------------|
| Positive | Positive money RGBA | Hexadecimal value, format 0xRRGGBBAA | 0xFFFF00FF |
| Negative | Negative money RGBA | Hexadecimal value, format 0xRRGGBBAA | 0xFF0000FF |
| Incresed | Increased money RGBA | Hexadecimal value, format 0xRRGGBBAA | 0x00FF00FF |
| Decresed | Decreased money RGBA | Hexadecimal value, format 0xRRGGBBAA | 0xFF0000FF |

### 3.4 Buy Menu Configuration ([Ecco.BuyMenu])

| Configuration Item | Description | Example Value | Default Value |
|-------------------|-------------|---------------|---------------|
| OpenShopTriggers | Buy command triggers | String array | ["buy", "shop"] |
| AllowDeathPlayerBuy | Whether to allow dead players to buy | true/false | true |
| ReOpenMenuAfterBuy | Whether to reopen menu after purchase | true/false | true |
| KeepOpenTime | Time menu stays open (seconds) | Any positive number | 10.0 |

### 3.5 Core Configuration ([Ecco])

| Configuration Item | Description | Optional Values | Default Value |
|-------------------|-------------|-----------------|---------------|
| ScoreToMoneyMultiplier | Multiplier for converting player score to money | Any positive number | 1.0 |
| StorePlayerScore | Whether to store player money | 0 (don't store), 1 (only store for map series), 2 (store) | 1 |
| PlayerStartScore | If not storing money, set player starting funds | Any integer | 0 |
| MoneyLimitePerMap | Maximum money obtainable per map, less than 0 means no limit | Integer | -1 |
| SaveDelayTime | Delay save time (seconds), less than 0 means immediate save | Any real number | 30.0 |
| DefaultLang | Default language | Language code | "english" |
| QueryLangCVar | Cvar used to query client language, leave blank to not query | String | "cap_lang" |
| PrecacheOffset | Precache offset | 2 (Half-Life series), 4 (Sven Co-op) | 4 |

## 4. How to Modify Configuration

1. Find the Ecco plugin configuration file, usually located at `addons/ecco/Config.toml`
2. Open the file with a text editor
3. Modify the corresponding configuration items as needed
4. Save the file and restart the server or reload the plugin to apply changes

## 5. Configuration Example

The following is a complete Config.toml configuration example:

```toml
[Ecco.Hud]
#How to display money HUD
#0 means don't display
#1 means display all
#2 means only display money, don't display money changes
#3 means only display money changes, don't display money
ShowMoneyHUD=1
#Money HUD display position
#Range (-1, 1)
#-1 and 0 represent center
MainHudPosX=0.5
MainHudPosY=0.9
#Add/subtract HUD display position
#Range (-1, 1)
#-1 and 0 represent center
NoticeHudPosX=0.5
NoticeHudPosY=0.858
#Refresh HUD interval
RefreshHudInterv=1.0
#Money HUD channel
MoneyChannel=3
#Money change HUD channel
NotifyChannel=4

[Ecco.Path]
#Data storage location
Data="storage"
#Banned maps file path
BanMaps="config/BannedMaps.txt"
#Coin icon
MoneyIcon="dollar.spr"

[Ecco.Color]
#Positive money RGBA
Positive=0xFFFF00FF
#Negative money RGBA
Negative=0xFF0000FF
#Increased money RGBA
Incresed=0x00FF00FF
#Decreased money RGBA
Decresed=0xFF0000FF

[Ecco.BuyMenu]
#Buy command Trigger
OpenShopTriggers=["buy", "shop"]
#Whether to allow dead players to buy
AllowDeathPlayerBuy=true
#Reopen menu after purchase
ReOpenMenuAfterBuy=true
#Time menu stays open
KeepOpenTime=10.0

[Ecco]
#Multiplier for converting player score to money
ScoreToMoneyMultiplier=1.0
#Whether to store player money
#0 means don't store
#1 means only store for map series
#2 means store
StorePlayerScore=1
#If not storing money, set player starting funds
PlayerStartScore=0
#Maximum money obtainable per map
#Less than 0 means no limit
MoneyLimitePerMap=-1
#Delay save time (seconds)
#Less than 0 means immediate save
#Greater than or equal to 0 means delayed save, default 30 seconds
SaveDelayTime=30.0
#Default language
DefaultLang="english"
#Cvar used to query client language, leave blank to not query
QueryLangCVar="cap_lang"
#Precache offset, used for PrecacheOther
#Half-Life (including cs, dod) is 2
#Sven Co-op is 4
#Wrong offset will cause game crash
#If you don't know what your game offset is
#The best way is to use precache variable for precaching
#Instead of choosing to use other_precache
PrecacheOffset=4
```

## 6. Common Issues

### 6.1 Configuration File Not Taking Effect

- Ensure configuration file path is correct
- Ensure configuration file format is correct, especially TOML syntax
- Restart server or reload plugin to apply changes

### 6.2 Color Configuration Incorrect

- Ensure color value format is 0xRRGGBBAA
- Ensure color values are within valid range (0-255)

### 6.3 PrecacheOffset Setting Incorrect

- Half-Life (including CS, DoD) uses value 2
- Sven Co-op uses value 4
- Wrong offset will cause game crash, please set carefully

## 7. Advanced Configuration

### 7.1 Custom Money Icon

1. Prepare a .spr format icon file
2. Place the file in the appropriate directory
3. Set MoneyIcon to the icon file name in the configuration file

### 7.2 Multi-language Support

1. Set DefaultLang to default language in configuration file
2. If you need to automatically adjust based on client language, set QueryLangCVar to the corresponding cvar name

## 8. Summary

TOML configuration file is the core configuration method of the Ecco plugin. By modifying TOML files, you can:

- Customize HUD display method and position
- Configure file paths and icons
- Set color schemes
- Adjust buy menu behavior
- Configure core game mechanics

I hope this guide helps you better understand and use the Ecco plugin's TOML configuration files. If you have any questions, please refer to the plugin documentation or seek community support.
