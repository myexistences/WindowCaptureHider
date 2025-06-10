# WindowCaptureHider

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Build](https://img.shields.io/github/actions/workflow/status/myexistences/WindowCaptureHider/ci.yml?branch=main)](../../actions)
[![Issues](https://img.shields.io/github/issues/myexistences/WindowCaptureHider)](../../issues)

**WindowCaptureHider** is a lightweight Windows DLL that hides top-level application windows from screen-capturing tools such as OBS, Snipping Tool, Microsoft Teams, Zoom, and others.  
It uses the Windows API `SetWindowDisplayAffinity` to block these windows from being captured or recorded.

> ⚠️ **Note**: This repository serves as **personal GitHub storage** for development backup and archival purposes. While it is public and functional, it is not maintained as a full-scale production library. Users are welcome to fork or reuse the code as needed.

---

## 📌 Table of Contents

1. [Key Features](#key-features)  
2. [How It Works](#how-it-works)  
3. [Quick Start](#quick-start)  
4. [Building From Source](#building-from-source)  
5. [Public API](#public-api)  
6. [Security & Limitations](#security--limitations)  
7. [Contributing](#contributing)  
8. [Roadmap](#roadmap)  
9. [License](#license)  

---

## ✅ Key Features

| Feature                   | Description                                                                 |
|--------------------------|-----------------------------------------------------------------------------|
| Universal capture-blocking | Automatically hides all top-level windows using `WDA_EXCLUDEFROMCAPTURE`.   |
| Self-healing window loop | Re-enumerates windows every second to apply or remove capture-blocking.    |
| Minimal footprint        | No dependencies beyond Win32 API. ~150 lines of C++ code.                   |
| Auto start & stop        | Starts on DLL load (`DLL_PROCESS_ATTACH`) and stops on unload.             |
| Inject or load manually  | Can be injected into any running process or loaded dynamically.            |
| Multi-platform builds    | Works on both x86 and x64. MSVC, Clang, and MinGW supported.               |
| Personal GitHub Storage  | Maintained as a personal development backup; not a user-configurable tool. |

---

## 🛠️ How It Works

1. `EnumWindows` gathers all top-level window handles.
2. Each window is set to `WDA_EXCLUDEFROMCAPTURE` using `SetWindowDisplayAffinity`.
3. Valid handles are stored in a set to track which ones are hidden.
4. Every second, a background thread updates the set: hiding new windows and removing destroyed ones.
5. Cleanup is performed when the DLL is unloaded or `StopHidingWindows()` is called.

---

## ⚡ Quick Start

### Option 1: Use Precompiled DLL
Download the latest version of `WindowCaptureHider.dll` from the [Releases](../../releases) page.

### Option 2: Inject or Load Manually

#### a) Inject via DLL Injector
```bash
injector.exe --pid <target-process-id> --dll WindowCaptureHider.dll
```

#### b) Load in Your Own Application
```cpp
HMODULE hLib = LoadLibrary("WindowCaptureHider.dll");
// Starts automatically on load
```

---

## 🔧 Building From Source

### Requirements

- Visual Studio or MSVC Build Tools  
- CMake (optional, for project generation)

### Build Instructions

```bash
git clone https://github.com/myexistences/WindowCaptureHider.git
cd WindowCaptureHider
cl /LD WindowCaptureHider.cpp /Fe:WindowCaptureHider.dll
```

Or open `WindowCaptureHider.sln` in Visual Studio and build in Release/x64.

---

## 📚 Public API

These two functions are exported by the DLL for manual control:

```cpp
extern "C" __declspec(dllexport) void StartHidingWindows();
extern "C" __declspec(dllexport) void StopHidingWindows();
```

You can call them manually after loading the DLL to start or stop the hiding loop.

---

## 🔒 Security & Limitations

- Windows may override `WDA_EXCLUDEFROMCAPTURE` in some environments (e.g. admin restrictions).
- This technique only hides windows from **software-based** screen capture, not from physical monitors or print screens.
- `SetWindowDisplayAffinity` only works on **Windows 8 and above**.

---

## 🤝 Contributing

This project is primarily for personal use, but improvements or suggestions are welcome via pull requests or issues.

---

## 📈 Roadmap

- [ ] Add window filtering by title/class (optional)
- [ ] GUI interface for selecting specific windows
- [ ] Log hidden windows in debug mode

---

## 📄 License

This project is licensed under the [GNU Affero General Public License v3.0
](LICENSE).
