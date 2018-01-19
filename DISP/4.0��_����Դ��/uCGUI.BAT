SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;C:\MentorGraphics\9.5PADS\SDD_HOME\common\win32\bin;C:\MentorGraphics\9.5PADS\SDD_HOME\common\win32\lib;C:\MentorGraphics\9.5PADS\MGC_HOME.ixn\bin;C:\MentorGraphics\9.5PADS\MGC_HOME.ixn\lib;C:\windows\system32;C:\windows;C:\windows\System32\Wbem;C:\Program Files\TortoiseGit\bin;C:\Program Files\TortoiseSVN\bin;C:\Program Files (x86)\VisualSVN Server\bin;%JAVA_HOME%\bin;%JAVA_HOME%\jre\bin;D:\Program Files (x86)\Java\jdk1.6.0_43\bin;D:\Program Files (x86)\Java\jre6\bin;C:\MATLAB7\bin\win32;C:\Program Files (x86)\Atmel\sam-ba_2.12\drv\;C:\Program Files (x86)\Atmel\sam-ba_2.12;C:\Program Files (x86)\ATMEL Corporation\AT91-ISP v1.13\Library\;C:\Program Files (x86)\ATMEL Corporation\AT91-ISP v1.13\sam-ba 2.9;C:\Program Files (x86)\Atmel\Flip 3.4.7\bin;D:\MentorGraphics\9.5PADS\SDD_HOME\common\win32\lib
SET CPU_TYPE=STM32F103VE
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=uCGUI
SET CPU_CLOCK=0x00B71B00
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\core_cm3.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\system_stm32f10x.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\output\startup_stm32f10x_hd._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\misc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\main.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_fsmc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_gpio.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_tim.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_rcc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\ili9325.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\Output\Tiky_uCGUI_DEMO.lnp"
"C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" ".\Output\Tiky_uCGUI_DEMO.axf" --i32combined --output ".\Output\Tiky_uCGUI_DEMO.hex"
