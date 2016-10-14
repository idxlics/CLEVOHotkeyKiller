# CLEVO Hotkey Killer
##  Why I write this program
I bought a laptop manufactured by CLEVO and I am using Windows 7 on it. Since the laptop has a backlit keyboard, I have to install the CLEVO Control Center(or Hotkey) to control the backlit, otherwise the backlit will keep turned on in the default blue color.

However, Control Center modifies the power plans with its settings occasionally while I want to customize the power plans with my own settings. So I decide to write a program to start Control Center(HkeyTray.exe) automatically after system boots up and user logs in, letting it running for a while and turning off the keyboard backlit, then kills the processes to avoid it modifying the power plans at some time.

## Introduction
Kill CLEVO Hotkey processes including `ComboKeyTray.exe`, `ControlCenter.exe`, `HKClipSvc.exe`, `HkeyTray.exe`, `hkysound.exe`, `HotkeyService.exe`.

## Usage
### Run directly

Kill the processes immediately

### Run with parameter

    CLEVOHotkeyKiller [/r time | /d time | /? | /help]

Options: (case-insensitive)
  
    No options  Kill the processes immediately
  
    /r time     Run HkeyTray.exe, delay <time> seconds, then kill the processes
  
    /d time     Delay <time> seconds, then kill the processes
  
    /? | /help	Display help message
    
Remark:
* The default value of `<time>` is 15. 
* To kill `HKClipSvc.exe` and `HotkeyService.exe`, running the program as administrator may be required.
* If `PowerBiosServer` service is started, `HotkeyService.exe` restarts automatically after you kills it.
  
## Build

The project is created and built in Visual Studio 2015.
