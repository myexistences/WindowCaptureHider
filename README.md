# WindowCaptureHider

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Build](https://img.shields.io/github/actions/workflow/status/<USER>/WindowCaptureHider/ci.yml?branch=main)](../../actions)
[![Issues](https://img.shields.io/github/issues/<USER>/WindowCaptureHider)](../../issues)

**WindowCaptureHider** is a lightweight DLL that blocks screen-capture tools (OBS, Snipping Tool, Teams, Zoom, etc.) from capturing your application's windows.

This DLL can be dynamically injected into a running process or linked manually within your application. It uses `SetWindowDisplayAffinity` with `WDA_EXCLUDEFROMCAPTURE` to make windows invisible to most screen recording tools.

---

## Table of Contents
1. [Key Features](#key-features)  
2. [How It Works](#how-it-works)  
3. [Quick Start](#quick-start)  
4. [User Configuration via Local Storage](#user-configuration-via-local-storage)  
5. [Building From Source](#building-from-source)  
6. [Public API](#public-api)  
7. [Security & Limitations](#security--limitations)  
8. [Contributing](#contributing)  
9. [Roadmap](#roadmap)  
10. [License](#license)

---

## Key Features
| Feature | Description |
|---------|-------------|
| **Universal capture-blocking** | Applies `WDA_EXCLUDEFROMCAPTURE` to every top-level window in the target process. |
| **Self-healing loop** | A background thread re-enumerates windows every 1 second to catch new or destroyed windows. |
| **Minimal footprint** | Pure Win32 API; under 150 lines of code; no heavy dependencies. |
| **Automatic lifecycle** | Auto-starts on `DLL_PROCESS_ATTACH` and cleans up on `DLL_PROCESS_DETACH`. |
| **User-specific configuration** | Uses a JSON config file stored in `%LOCALAPPDATA%\WindowCaptureHider\config.json` to define per-user window filtering rules. |
| **x86 & x64 support** | Compatible with both architectures; builds cleanly with MSVC, clang-cl, or MinGW-w64. |

---

## How It Works
1. **Enumerate windows** – Calls `EnumWindows` to get all top-level HWNDs.
2. **Apply protection** – Calls `SetWindowDisplayAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE)` on each window.
3. **Track state** – Caches HWNDs and their affinity to revert or clean them if needed.
4. **Looping mechanism** – A background thread re-runs the logic every second.
5. **User filtering** – Optionally skips certain windows based on per-user rules in local config.

---

## Quick Start

### 1. Pre-built binary
Download `WindowCaptureHider.dll` from the [Releases](../../releases) page.

### 2. Inject or load
#### Option A – DLL Injection
Use any DLL injector of your choice:
```bash
injector.exe --pid 1234 --dll WindowCaptureHider.dll
