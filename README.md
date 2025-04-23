🛡️ Anti-Cheat System

👤 Developer Information
Name: Arthur Gipson

In 2019, I was contracted to lead development of a fully custom anti-cheat solution for a PC game in production.
The game was ultimately cancelled, but this repo reflects the work done toward that project.

It combines low-level C++ engineering, WinAPI reverse engineering, manual module mapping, and more.

🔐 Core Cybersecurity Concepts Demonstrated

🧬 Anti-Debugging
Direct inspection and validation of memory regions containing DbgBreakPoint, DbgUserBreakPoint functions
Flagging and disconnect logic for any tampered breakpoint instructions
Kernel debugging detection via flag bytes, emulating rootkit defense logic

🔒 Memory Integrity + Remote Thread Analysis
Runtime memory reads from target processes via OpenProcess and ReadProcessMemory
Manual inspection of thread start addresses using GetThreadStartAddress
Simulates EDR-style inspection workflows to detect injected threads or shellcode

🔭 WMI Monitoring & Process Surveillance
Active use of Windows Management Instrumentation (WMI) to monitor:
Win32_Process creation
Win32_Thread spawns
Win32_Service events
Asynchronous notification system for live event-driven detection
Exposes weak points commonly used in malware or cheats for stealth injection

📡 Secure Communication & Encryption
Custom TCP client/server communication protocol
All packet content XOR-encrypted, with option for RC4-based symmetric encryption
Telemetry includes anti-debug flags, HWID fingerprints, and login credentials

📸 Screen Capture Logging
Full screen capture functionality (JPEG/BMP support)
Uses GDI/GDI+ to manually encode screen content
Prepares screenshots for secure upload, validating integrity of remote environment

🆔 Hardware Fingerprinting
Reads disk serial number and device info using DeviceIoControl + IOCTL_STORAGE_QUERY_PROPERTY
Flags inconsistencies and spoofing attempts
Rejects clients with suspiciously short, invalid, or malformed HWIDs

💾 Manual Module Injection (Userland Mapping)
Implements a full PE file loader from scratch (x32ManualMap)
Relocates image, resolves imports, calls entry points
Simulates modern shellcode-style loading behavior as used by malware and red teamers

🚫 Anti-Reversing Protections
Checks PatchGuard, Secure Boot, DSE settings by inspecting BCD configuration
Optional logic to deny access when unsafe kernel flags are active
Mimics boot-chain enforcement logic from modern endpoint security agents
