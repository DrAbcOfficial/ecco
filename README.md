# `Ecco`: A complete Economy & Buy Menu Plugin <img align="right" src="./resource/logo.png" width="220" height="140" alt="Ecco" />


Ecco is a complete economy & buy menu plugin mainly aimed Sven Co-op 5.x, It is designed to add many commands via toml files to purchase items from different plugins.

---


##  1. <a name='Install'></a>Install

1. install metamod-p or metamod-r or metamod-fallguys
2. grab ecco binary from action or release or build by yourslef
3. edit `(GAME_FOLDER)/addons/metamod/plugins.ini`
4. add 

``` ini

win32 addons/ecco/ecco.dll
linux addons/ecco/ecco.so

```

5. enjoy your new store :3

---

##  2. <a name='Build'></a>Build

If you are using systems that are not covered by automatic builds (yum's and aur's) 

or want to add new hooks yourself, you can follow these steps to configure your build environment.

1. Install everything you need
   
	<img src="https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white"/>

	1. [Git](https://git-scm.com/download/win) or `winget install --id Git.Git -e --source winget`
	2. [Visual Studio with vc143 toolset](https://visualstudio.microsoft.com/) And [C++ desktop development](https://learn.microsoft.com/en-us/cpp/ide/using-the-visual-studio-ide-for-cpp-desktop-development?view=msvc-170)

2. Exec scripts
	<img src="https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white"/>
	
	1. `./win_init.ps1`
	2. `./win_build.ps1`

	<img src="https://img.shields.io/badge/Debian-A81D33?style=for-the-badge&logo=debian&logoColor=white"/>

	1. `./linux_init.sh`
	2. `./linux_build.sh`

	
# Yes, it works on Half-Life, but not very well

<img src="./resource/20250823-191047.jpg" width="1440" height="640"/>

<img src="./resource/20250823-191651.jpg" width="1440" height="640"/>

## 3. <a name='Config'></a>Configuration

The Ecco plugin uses TOML files for configuration. For detailed configuration instructions, please refer to the [TOML Configuration Guide](./wiki/toml-guide.md).

### 3.1 Configuration File Location

The configuration file is located at `addons/ecco/Config.toml`. You can modify the settings according to your needs.

### 3.2 Main Configuration Options

- **HUD Settings**: Control the display method and position of money
- **Path Settings**: Configure data storage location and file paths
- **Color Settings**: Customize money display colors
- **Buy Menu Settings**: Adjust buy menu behavior
- **Core Settings**: Configure game mechanics and language options

For detailed configuration instructions, please see the [TOML Configuration Guide](./wiki/toml-guide.md).

## 4. <a name='Script'></a>Script System

The Ecco plugin uses the Tcl scripting system to implement highly customizable game behaviors and buy menu items. By writing scripts, you can:

- Create custom buy menu items
- Implement special game mechanics
- Customize player behavior
- Add new game features

For detailed script writing instructions, please refer to the [Ecco Script Writing Guide](./wiki/script-guide.md).

### 4.1 Script File Location

Script files are located in the `addons/ecco/scripts/` directory and use the `.tcl` file extension.

### 4.2 Script System Features

- Uses Tcl language, simple and easy to learn
- Rich API function library
- Automatically builds hierarchical buy menus
- Supports complex game logic

For detailed script writing instructions, please see the [Ecco Script Writing Guide](./wiki/script-guide.md).