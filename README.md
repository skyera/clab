# clab

A C99 contact-directory example using
[uthash](https://github.com/troydhanson/uthash) as a Git submodule in
`third_party/uthash`.

The program demonstrates insertion, updating an existing integer key, iteration,
lookup, deletion, and freeing every entry. Names are limited to 63 bytes; longer
names are rejected. uthash uses its default fatal handler for internal allocation
failures.

## Get the dependency

After cloning this repository, initialize the pinned submodule:

```sh
git submodule update --init --recursive
```

Alternatively, clone with `git clone --recurse-submodules <repository-url>`.

## Build on Windows and Linux

Use CMake 3.16 or newer and a C compiler. The same C source builds on both
platforms. Only the uthash header is needed; there is no library to link.

### Quick build scripts

Windows Command Prompt (Visual Studio 2022 C build tools):

```cmd
build.cmd
```

Use `build.cmd -Run` to also run the demo, or `build.cmd -MinGW -Run` for
MinGW GCC. The CMD wrapper uses the PowerShell build script and forwards its
options and exit code.

Windows (PowerShell, Visual Studio 2022 C build tools):

```powershell
.\build.ps1
```

For MinGW GCC, use `.\build.ps1 -MinGW`. Add `-Run` to either command to run
the demo after building, for example `.\build.ps1 -MinGW -Run`.

Linux (Bash, GCC, Make):

```sh
bash build.sh
```

Use `bash build.sh --run` to also run the demo.

The build scripts require CMake on `PATH`, initialize the submodule if missing,
build in Release mode, and run the smoke test. They work from any working
directory and stop if a step fails. The commands below show the manual steps.

### Windows (Visual Studio)

Install Visual Studio 2022 Build Tools with the **Desktop development with C++**
workload (which includes the C compiler and Windows SDK), plus CMake. Run from
the repository root in PowerShell:

```powershell
cmake -S . -B build-windows -G "Visual Studio 17 2022" -A x64
cmake --build build-windows --config Release
ctest --test-dir build-windows -C Release --output-on-failure
.\build-windows\Release\uthash_demo.exe
```

### Windows (MinGW GCC)

With CMake, `gcc`, and `mingw32-make` on `PATH`:

```powershell
cmake -S . -B build-mingw -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build-mingw
ctest --test-dir build-mingw --output-on-failure
.\build-mingw\uthash_demo.exe
```

### Linux (GCC)

Install GCC, Make, and CMake. For example, on Ubuntu/Debian:

```sh
sudo apt-get update
sudo apt-get install build-essential cmake
```

From the repository root:

```sh
cmake -S . -B build-linux -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build-linux
ctest --test-dir build-linux --output-on-failure
./build-linux/uthash_demo
```

Separate build directories let Windows and Linux share a checkout without
mixing CMake caches or compiled files.

### Build directly with GCC

Linux:

```sh
gcc -std=c99 -Wall -Wextra -Wpedantic -Ithird_party/uthash/src src/main.c -o uthash_demo
./uthash_demo
```

Windows (MinGW, PowerShell):

```powershell
gcc -std=c99 -Wall -Wextra -Wpedantic -Ithird_party/uthash/src src/main.c -o uthash_demo.exe
.\uthash_demo.exe
```

GitHub Actions builds and runs the CTest smoke test on Windows (MSVC) and Linux
(GCC) on every push and pull request. The test checks that the demo exits
successfully and reaches cleanup with an empty table.

## Output

Expected output:

```text
Contacts: 3
  1: Ada
  2: Grace Hopper
  3: Linus
Find 2: Grace Hopper
Find 99: not found
Delete 1: deleted
Delete 1 again: not found
Remaining: 2
After cleanup: 0
```
