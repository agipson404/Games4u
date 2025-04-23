# 🛡️ Games4u Anti-Cheat System

> A real-world anti-tamper and telemetry system for PC games  
> 💻 Written in C++ · Reverse Engineering · WinAPI · Manual Mapping

---

In 2019, I was contracted to lead development of a fully custom anti-cheat solution for a PC game in production. The game was ultimately cancelled, but this repo reflects the work done toward that project.

It combines **low-level C++ systems engineering**, **WinAPI manipulation**, **manual module mapping**, and multiple layers of **telemetry, obfuscation, and anti-debugging logic**.

---

## 🔐 Core Cybersecurity Concepts Demonstrated

### 🧬 Anti-Debugging
- Direct inspection of memory regions (`DbgBreakPoint`, `DbgUserBreakPoint`)
- Flags and disables tampered or replaced debug traps
- Kernel debugging detection via flag byte inspection (rootkit-level logic)

---

### 🔒 Memory Integrity + Remote Thread Analysis
- Reads memory of external processes via `OpenProcess`, `ReadProcessMemory`
- Extracts thread entry points using `GetThreadStartAddress`
- Simulates advanced EDR detection workflows for injected shellcode

---

### 🔭 WMI Monitoring & Process Surveillance
- Hooks and monitors WMI event subscriptions:
  - `Win32_Process`
  - `Win32_Thread`
  - `Win32_Service`
- Detects stealth cheat activity via system-level process creation

---

### 📡 Secure Communication & Encryption
- Custom TCP client/server design
- XOR and RC4-encrypted telemetry transmission
- Includes encrypted HWID, debug status, and login state

---

### 📸 Screen Capture Logging
- Captures screen with GDI/GDI+ (BMP/JPG support)
- Used for remote player session verification and fraud audit
- Integrates into telemetry pipeline

---

### 🆔 Hardware Fingerprinting
- Queries physical hardware via `DeviceIoControl`, `IOCTL_STORAGE_QUERY_PROPERTY`
- Detects spoofed, virtualized, or tampered environments
- HWID system used for anti-ban evasion prevention

---

### 💾 Manual Module Injection (Userland Mapping)
- Custom-built `x32ManualMap` PE loader
- Handles relocation, import resolution, and entry point execution
- Emulates modern red-team style shellcode injection

---

### 🚫 Anti-Reversing Protections
- Inspects Secure Boot, PatchGuard, and DSE status from BCD config
- Detects insecure boot chains and test-signed drivers
- Blocks execution if system fails trust checks

---

## 🧠 This Project Is Valuable For:
- Security engineers (malware, red team, anti-cheat, AV/EDR)
- Game security researchers and Windows kernel interns
- Cybersecurity hiring managers evaluating hands-on depth
