# Convenient Slow Motion

* Controllable slow motion effect by annotations.
  - Start slow motion effect by sending _SlowMotionStart_ animation event. This will trigger a slow motion that slows down game to 0.2 (20%) speed by 0.3 seconds by default.
  - Manage the slow motion ratio and duration by _SlowMotionRatio_ (ratio) and _SlowMotionResetTimer_ (sec) graph variables.
  - You can manage these variables by using [D7ry](https://github.com/D7ry)'s [Payload Interpreter](https://github.com/D7ry/PayloadInterpreter) _PIE_ event with _@SGVF_ payload.   
    e.g. `PIE.@SGVF|SlowMotionRatio|0.36`
  - Also you can use scripts for managing whole process, papyrus has functions for managing graph variables and sending animation events.
* Compatible with 1.5.97 - 1.6.x game versions.


## Requirements

* CommonLibSSE NG

Because this uses [CommonLibSSE NG](https://github.com/CharmedBaryon/CommonLibSSE-NG), it supports Skyrim SE, AE, GOG, and VR. 

Hook IDs and offsets must still be found manually for each version.


* [Visual Studio 2022](https://visualstudio.microsoft.com/) (_the free Community edition_)
* [`vcpkg`](https://github.com/microsoft/vcpkg)
  - 1. Clone the repository using git OR [download it as a .zip](https://github.com/microsoft/vcpkg/archive/refs/heads/master.zip)
  - 2. Go into the `vcpkg` folder and double-click on `bootstrap-vcpkg.bat`
  - 3. Edit your system or user Environment Variables and add a new one:
    - Name: `VCPKG_ROOT`  
      Value: `C:\path\to\wherever\your\vcpkg\folder\is`
  - The latest version of vcpkg needs a default repository defined in the json. If you're using an older version of vcpkg, simply delete the default repository definition in `vcpkg-configuration.json`

## Project setup

By default, when this project compiles it will output a `.dll` for your SKSE plugin into the `build/` folder.

If you want to configure this project to output your plugin files
into your Skyrim Special Edition's "`Data`" folder:

- Set the `SKYRIM_FOLDER` environment variable to the path of your Skyrim installation  
  e.g. `C:\Program Files (x86)\Steam\steamapps\common\Skyrim Special Edition`

If you want to configure this project to output your plugin files
into your "`mods`" folder:  
(_for Mod Organizer 2 or Vortex_)

- Set the `SKYRIM_MODS_FOLDER` environment variable to the path of your mods folder:  
  e.g. `C:\Users\<user>\AppData\Local\ModOrganizer\Skyrim Special Edition\mods`  
  e.g. `C:\Users\<user>\AppData\Roaming\Vortex\skyrimse\mods`

## Credits
* [CharmedBayron](https://github.com/CharmedBaryon/CommonLibSSE-NG),
[Ryan McKenzie](https://github.com/Ryan-rsm-McKenzie) and
[powerof3](https://github.com/powerof3) for CommonlibSSE-NG.
* [Mrowr Purr](https://github.com/SkyrimScripting) for his guide videos.
* [Monitor144hz](https://github.com/Monitor144hz) for his CommonlibSSE project template.
* [Ersh](https://github.com/ersh1) for Precision API and projectile hook ID's.
