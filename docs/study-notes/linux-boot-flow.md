# Deep Dive: Linux Boot Sequence on Raspberry Pi 4

## 1. Stage 0: The Boot ROM
When power is applied, the ARM CPU is actually **OFF**. A tiny piece of code baked into the hardware (the Boot ROM) starts the **VideoCore GPU**. On a Pi, the GPU is the master; the CPU is a peripheral.

## 2. Stage 1: The GPU Firmware (`start4.elf`)
The GPU searches the SD card (the FAT32 partition) for firmware.
*   It loads `start4.elf` and reads `config.txt`.
*   It initializes the hardware, clocks, and RAM.
*   **Crucial:** It then releases the ARM CPU from "reset" and tells it where to find the Linux Kernel.

## 3. Stage 2: The Linux Kernel (`Image`)
The Kernel is now in control. It performs three main tasks:
1.  **Self-Extraction:** It uncompresses itself into RAM.
2.  **Hardware Discovery:** It reads the **Device Tree Blob (.dtb)**. This is the "map" we built in Buildroot that tells the kernel exactly where the Bluetooth and MQTT hardware registers are.
3.  **Driver Initialization:** It starts the drivers for Disk, Network, and USB.

## 4. Stage 3: The Init Process (`BusyBox`)
Once the Kernel is stable, it looks for the first program to run. This is always **PID 1**.
*   In our Buildroot system, the Kernel runs `/sbin/init` (provided by **BusyBox**).
*   BusyBox reads `/etc/inittab`, which tells it to:
    1.  Mount the filesystems.
    2.  Set up the network.
    3.  Start your Industrial Gateway service.

## 5. Tier-1 Interview Tip: "The Device Tree"
*   **Question:** "How does the Kernel know the address of the UART on an embedded board?"
*   **Answer:** "It doesn't 'discover' it like a PC. We provide a **Device Tree Source (.dts)** file that explicitly defines the memory addresses of all non-discoverable hardware. Buildroot compiles this into a `.dtb` file which the bootloader passes to the Kernel at startup."
