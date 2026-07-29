# bandbox

A lightweight Linux sandbox built from scratch in C, using kernel namespaces
for process isolation. Educational project — work in progress.

## Prerequisites

- Linux (tested on Ubuntu)
- GCC (or another C compiler)
- CMake (version 3.10+)
- `make`

Install prerequisites on Ubuntu/Debian:

```bash
sudo apt update
sudo apt install build-essential cmake
```

## Setup & Build

Clone the repo, then build with CMake:

```bash
git clone <your-repo-url>
cd bandbox
mkdir build && cd build
cmake ..
make
```

This produces a `bandbox` binary inside the `build/` folder.

## Usage

Run it :

```bash
sudo ./bandbox
```

## Project Structure

```
bandbox/
├── CMakeLists.txt      
├── include/           
├── src/                
└── build/              
```

## Rebuilding after changes

- Edited a `.c` or `.h` file only → just run `make` from inside `build/`
- Edited `CMakeLists.txt` (e.g. added a new source file) → run `cmake ..` then `make`
- If something seems broken/stale → wipe and rebuild:
  ```bash
  rm -rf build
  mkdir build && cd build
  cmake ..
  make
  ```

