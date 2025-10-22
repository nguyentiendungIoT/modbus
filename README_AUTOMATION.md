# STM32 Build & Debug Automation Scripts

## ?? Files Overview

| File | Purpose |
|------|---------|
| `build.bat` | Compile project using STM32CubeIDE toolchain |
| `flash.bat` | Flash firmware to STM32 via ST-Link (auto-detects project) |
| `debug.bat` | Start GDB debug session with OpenOCD (auto-detects project) |

---

## ?? Quick Start

### 1. Build Firmware
```batch
build.bat
```
Compiles the project and generates `Debug\*.elf`.

### 2. Flash to Board
```batch
flash.bat
```
Programs the firmware to STM32 via ST-Link (SWD).  
**? Auto-detects project name from .elf file**

### 3. Debug Session
```batch
debug.bat
```
Launches OpenOCD server + GDB for interactive debugging.  
**? Auto-detects project name from .elf file**

---

## ?? Copy to New Project (Zero Configuration!)

1. **Copy all 3 files:**
   ```batch
   copy build.bat flash.bat debug.bat D:\YourNewProjectFolder\
   ```

2. **That's it!** The scripts auto-detect the project name from the `.elf` file.

---

## ??? Tool Requirements

All tools are bundled with **STM32CubeIDE 1.16.1**:

| Tool | Location | Purpose |
|------|----------|---------|
| `make` | `STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.make.win32_*\tools\bin` | Build system |
| `arm-none-eabi-gcc` | `STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.*\tools\bin` | Compiler |
| `STM32_Programmer_CLI` | `STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.*\tools\bin` | Flashing |
| `openocd` | `STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.openocd.*\tools\bin` | Debug server |
| `arm-none-eabi-gdb` | (same as gcc) | Debugger |

### Optional: Standalone STM32CubeProgrammer
Download from: https://www.st.com/en/development-tools/stm32cubeprog.html  
Default install: `C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer`

---

## ?? How Auto-Detection Works

### `flash.bat`
```batch
REM Finds first .elf file in Debug folder
for /f "delims=" %%F in ('dir /b "%BUILD_DIR%\*.elf"') do (
    set "ELF_FILE=%BUILD_DIR%\%%F"
)
REM Extracts name: "MyProject.elf" ? "MyProject"
set "PROJECT_NAME=%ELF_FILE:.elf=%"
```

### `debug.bat`
- Same logic as `flash.bat`
- Automatically loads the found `.elf` file into GDB

**Result:** Copy to any project, and the scripts instantly adapt! ??

---

## ?? GDB Commands Cheat Sheet

Once in `debug.bat` GDB session:

| Command | Action |
|---------|--------|
| `target remote :3333` | Connect to OpenOCD (auto-executed) |
| `load` | Flash firmware to target |
| `monitor reset halt` | Reset CPU and halt at first instruction |
| `break main` | Set breakpoint at `main()` |
| `continue` / `c` | Resume execution |
| `step` / `s` | Step into function |
| `next` / `n` | Step over function |
| `print variable` | Print variable value |
| `info locals` | Show local variables |
| `backtrace` / `bt` | Show call stack |
| `quit` / `q` | Exit GDB (auto-stops OpenOCD) |

---

## ?? Troubleshooting

### Flash fails with "No ST-Link detected"
- Check USB connection
- Install ST-Link drivers: https://www.st.com/en/development-tools/stsw-link009.html
- Try `STM32CubeProgrammer` GUI first to verify hardware

### Debug fails with "Can't connect to OpenOCD"
- Ensure no other debugger is running (close STM32CubeIDE debugger)
- Check firewall isn't blocking port 3333
- Run OpenOCD manually first: `openocd -f interface/stlink.cfg -f target/stm32f4x.cfg`

### Plugin version mismatch errors
Run this to find your actual plugin versions:
```batch
dir /b "C:\ST\STM32CubeIDE_1.16.1\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.*"
```
Update the plugin names in `debug.bat` (if needed).

### "No .elf file found in Debug"
- Run `build.bat` first
- Check that compilation succeeded and `.elf` was generated

---

## ?? If You Need Manual Configuration

Both `flash.bat` and `debug.bat` still support manual project names (backward compatible):

### Option 1: Command-line override for flash.bat
```batch
flash.bat path\to\custom.elf
```

### Option 2: Edit script (if auto-detect fails)
```batch
set "PROJECT_NAME=YourProjectName"
```
Then auto-detection is skipped.

---

## ?? Reusable Template Setup

To create a central template library:

1. **Create template folder:**
 ```
   C:\STM32Tools\templates\
     ?? build.bat
     ?? flash.bat
     ?? debug.bat
   ```

2. **For each new project:**
   ```batch
   copy C:\STM32Tools\templates\*.bat D:\MyProject\
   ```

3. **Zero configuration needed!** ?

---

## ?? Advanced: One-Command Build + Flash + Debug

Create `build_flash_debug.bat`:

```batch
@echo off
call build.bat
if errorlevel 1 exit /b 1

call flash.bat
if errorlevel 1 exit /b 1

echo.
echo [SUCCESS] Build + Flash complete!
echo Starting debugger in 3 seconds...
timeout /t 3 /nobreak
call debug.bat
```

---

## ?? Notes

- Scripts assume ST-Link debugger (update OpenOCD config for J-Link/etc.)
- Default target: STM32F407 (change `target/stm32f4x.cfg` for other MCUs)
- GDB server runs on `localhost:3333` (OpenOCD default)
- Scripts auto-clean before build to ensure fresh compilation
- **Auto-detection is the default - no manual config needed!**

---

**Generated by:** GitHub Copilot  
**Last Updated:** 2025-10-22  
**Feature:** Zero-Configuration Auto-Detection ?
