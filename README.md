# Password Manager v2

A desktop password manager built with React, TypeScript, and Tauri.

## Requirements

Install the following tools before getting started:

- [Node.js](https://nodejs.org/)
- [Rust](https://www.rust-lang.org/tools/install)
- [Tauri prerequisites](https://v2.tauri.app/start/prerequisites/)
- GCC and CMake
- OpenSSL
- [nlohmann/json](https://github.com/nlohmann/json)
- The project's C++ library

> The C++ library must be built and available before starting the Tauri application.

## Getting Started

### 1. Clone the repository

```bash
git clone https://github.com/fotiskosmidis/Password-Manager.v2.git
cd Password-Manager.v2
```

### 2. Build the C++ library

The C++ backend is built as a shared library. CMake chooses the platform format
automatically: `.so` on Linux, `.dll` on Windows, and `.dylib` on macOS.

Install OpenSSL and nlohmann/json first, then run these commands from the
project root:

```bash
cmake -S . -B build
cmake --build build --config Release
```

On Linux, the library will be created at:

```text
build/libpassword_manager_backend.so
```

On Windows, the DLL and its import library will be in:

```text
build/Release/password_manager_backend.dll
build/Release/password_manager_backend.lib
```

To install the library into CMake's installation prefix:

```bash
cmake --install build --config Release
```

### 3. Install frontend dependencies

```bash
npm install
```

### 4. Start the application

```bash
npm run tauri dev
```

## Available Commands

| Command | Description |
| --- | --- |
| `npm run dev` | Start the Vite frontend development server |
| `npm run build` | Type-check and build the frontend |
| `npm run tauri dev` | Start the desktop app in development mode |
| `npm run preview` | Preview the production frontend build |

## Project Structure

```text
src/                React frontend
src-tauri/          Tauri desktop application and bindings with the cpp library
src-tauri/cpplib    The cpp files for the library
public/             Static assets
```