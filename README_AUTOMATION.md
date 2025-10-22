# STM32 Modbus Example Project

## T?ng quan

D? án này là ví d? hoàn ch?nh v? xây d?ng, n?p và debug firmware STM32 s? d?ng STM32CubeIDE, tích h?p th? vi?n Modbus RTU (MicroTBX-Modbus). Toàn b? quy trình build, flash, debug ??u t? ??ng hóa qua các script `.bat`.

- **Firmware m?u**: Giao ti?p Modbus RTU trên UART2, t?c ??19200bps, parity EVEN, h? tr? callback ??c input register.
- **Th? vi?n**: S? d?ng MicroTBX và MicroTBX-Modbus (server, client, RTU, callback, memory pool...)
- **T? ??ng hóa**: build.bat, flash.bat, debug.bat giúp build, n?p, debug ch? v?i1 l?nh, không c?n c?u hình th? công.

## C?u trúc th? m?c

| Th? m?c/File | Mô t? |
|--------------|-------|
| `Core/Src/main.c` | Code chính, kh?i t?o Modbus, callback, vòng l?p x? lý |
| `Library/microtbx-modbus/` | Th? vi?n Modbus RTU, server, client, transport, event... |
| `Library/microtbx/` | Th? vi?n MicroTBX: memory, assert, crypto, port... |
| `build.bat` | Script build firmware (t? ??ng clean, build, t?o .elf) |
| `flash.bat` | Script n?p firmware qua ST-Link, t? ??ng tìm .elf |
| `debug.bat` | Script debug v?i OpenOCD + GDB, t? ??ng tìm .elf |

## H??ng d?n s? d?ng code Modbus

1. **Kh?i t?o Modbus RTU và server:**
 ```c
 modbusTp = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
 modbusServer = TbxMbServerCreate(modbusTp);
 ```
2. **??ng ký callback x? lý d? li?u:**
 ```c
 TbxMbServerSetCallbackReadInputReg(modbusServer, ModbusReadInputReg);
 // Có th? ??ng ký thêm các callback khác: ??c/ghi coil, holding reg, custom function...
 ```
3. **Cài ??t callback ví d?:**
 ```c
 tTbxMbServerResult ModbusReadInputReg(tTbxMbServer channel, uint16_t addr, uint16_t *value) {
 switch (addr) {
 case30000U: *value =1234U; break;
 case30001U: *value =5678U; break;
 default: return TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR;
 }
 return TBX_MB_SERVER_OK;
 }
 ```
4. **Vòng l?p x? lý Modbus:**
 ```c
 while (1) {
 TbxMbEventTask();
 }
 ```

## Build, Flash, Debug

- **Build:** Ch?y `build.bat` ?? biên d?ch, t?o file `Debug/*.elf`.
- **Flash:** Ch?y `flash.bat` ?? n?p firmware vào board qua ST-Link.
- **Debug:** Ch?y `debug.bat` ?? m? phiên debug v?i OpenOCD + GDB.

> Các script t? ??ng nh?n di?n project, không c?n s?a tên file.

## L?u ý
- ??m b?o ?ã cài STM32CubeIDE1.16.1 và driver ST-Link.
- Có th? m? r?ng thêm các callback cho Modbus server/client tùy nhu c?u.
- Th? vi?n MicroTBX-Modbus h? tr? nhi?u ch? ??, có th? tham kh?o thêm trong source code.

---

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
