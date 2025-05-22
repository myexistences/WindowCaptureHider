# WindowCaptureHider

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Build](https://img.shields.io/github/actions/workflow/status/<USER>/WindowCaptureHider/ci.yml?branch=main)](../../actions)
[![Issues](https://img.shields.io/github/issues/<USER>/WindowCaptureHider)](../../issues)

Hide any top-level window from screen-capture software (OBS, Snipping Tool, Teams, Zoom, etc.).  
Built as a *drop-in* DLL, **WindowCaptureHider** can be injected into an existing process, or loaded explicitly in your own application.

---

## Table of Contents
1. [Key Features](#key-features)  
2. [How It Works](#how-it-works)  
3. [Quick Start](#quick-start)  
4. [Personal Storage (Per-User Rules)](#personal-storage-per-user-rules)  
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
| **Self-healing loop** | A background thread re-enumerates windows every 1 s to catch newly-created windows and to clean up handles for destroyed ones. |
| **Minimal footprint** | Pure Win32 API, < 150 LOC, no STL allocations except `std::unordered_set`. |
| **Automatic start/stop** | Begins hiding on `DLL_PROCESS_ATTACH` and cleans up on `DLL_PROCESS_DETACH`; manual control also exposed. |
| **Optional personal storage** | JSON config in `%LOCALAPPDATA%\WindowCaptureHider\config.json` lets each user whitelist/blacklist windows by title or class. |
| **x86 & x64** | Builds and cross-compiles cleanly with MSVC, clang-cl, or MinGW-w64; tested in CI. |

---

## How It Works
1. **Enumeration** – `EnumWindows` enumerates all top-level HWNDs.  
2. **Capture Exclusion** – Each HWND receives `SetWindowDisplayAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE)`.  
3. **Tracking** – Hidden HWNDs are cached so they can be reset to `WDA_NONE` when destroyed.  
4. **Loop** – A detached thread repeats step 1 every second until `StopHidingWindows` is called or the DLL is unloaded.  
5. **Config Check (optional)** – Before hiding, the code consults *personal storage* rules to see if the window should be ignored.

---

## Quick Start
### 1. Pre-built binary
Grab the latest signed DLL from **[Releases](../../releases)** → `WindowCaptureHider.dll`.

### 2. Inject or load it
#### a) *DLL Injection* (any injector of your choice)
```bash
injector.exe --pid 1234 --dll WindowCaptureHider.dll
