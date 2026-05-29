# Installation Guide - Plazza

## Prerequisites

### System Requirements

- **Operating System**: Linux, macOS, or BSD
- **Architecture**: x86_64 or ARM64
- **C++ Version**: C++17 or later
- **Compiler**: GCC 9.0+ or Clang 10.0+

### Required Dependencies

#### Core Build Tools

- **C++ Compiler** (g++ or clang++)
- **GNU Make** or CMake
- **Git** (for version control)

#### POSIX Libraries

- **POSIX Threads** (libpthread) - Usually included
- **Standard C++ Library** (libstdc++ or libc++)

#### Optional but Recommended

- **CMake** 3.10+ (alternative build system)
- **pkg-config** (for dependency detection)

## Installation Steps

### 1. Install Build Tools

#### On Ubuntu/Debian

```bash
# Update package manager
sudo apt update

# Install essential build tools
sudo apt install -y \
  build-essential \
  g++ \
  gcc \
  make \
  git \
  pkg-config

# Verify installation
g++ --version
make --version
```

#### On Fedora/RHEL

```bash
# Install development tools
sudo dnf groupinstall -y "Development Tools"
sudo dnf install -y \
  gcc-c++ \
  make \
  git

# Verify installation
g++ --version
make --version
```

#### On macOS

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Alternatively, install with Homebrew
brew install gcc make

# Verify installation
g++ --version
make --version
```

### 2. Clone/Download the Project

```bash
# Navigate to your workspace
cd ~/Epitech/tek2/CCP/

# If cloning from repository (if applicable)
git clone <repository-url> G-CCP-400-LIL-4-1-theplazza-23
cd G-CCP-400-LIL-4-1-theplazza-23

# Or if already in the directory
cd G-CCP-400-LIL-4-1-theplazza-23
```

### 3. Build the Project

#### Using Makefile (Primary Method)

```bash
# Standard compilation
make

# This will:
# - Compile all source files in src/
# - Link with necessary libraries (pthread)
# - Generate the 'plazza' executable in the root directory
```

**Build Options:**

```bash
# Full clean rebuild
make fclean && make

# Remove only object files
make clean

# Rebuild from scratch
make re

# Just check if compilation works
make -n  # Dry run
```

#### Using CMake (Alternative)

If CMake configuration is available:

```bash
# Create build directory
mkdir build
cd build

# Configure and compile
cmake ..
make

# Or with optimization flags
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

### 4. Verify Installation

```bash
# Check if binary was created
ls -la plazza

# Verify binary is executable
file plazza

# Test basic functionality (dry run)
./plazza 1 1 1000
# Then type 'exit' to exit
```

## Compiler Configuration

### Compiler Flags

The Makefile typically uses:

```makefile
CXXFLAGS = -std=c++17 -Wall -Wextra -Weffc++ -I./src
LDFLAGS = -pthread
```

### Custom Compilation

To compile with custom flags:

```bash
# With optimization
CXXFLAGS="-std=c++17 -O2" make

# With debug symbols
CXXFLAGS="-std=c++17 -g -O0" make

# With additional warnings
CXXFLAGS="-std=c++17 -Wall -Wextra -Wpedantic" make
```

## Troubleshooting

### Issue: "g++: command not found"

**Solution:** Install GCC development tools

```bash
# Ubuntu/Debian
sudo apt install build-essential g++

# Fedora
sudo dnf install gcc-c++

# macOS
xcode-select --install
```

### Issue: "make: command not found"

**Solution:** Install Make

```bash
# Ubuntu/Debian
sudo apt install make

# Fedora
sudo dnf install make

# macOS
brew install make
```

### Issue: "fatal error: pthread.h: No such file or directory"

**Solution:** Install POSIX thread development files

```bash
# Ubuntu/Debian (usually already installed)
sudo apt install libpthread-stubs0-dev

# Fedora
sudo dnf install glibc-devel
```

### Issue: Compilation with C++17 errors

**Solution:** Update compiler or verify C++17 support

```bash
# Check compiler version
g++ --version

# Update compiler
sudo apt update && sudo apt upgrade

# Verify C++17 support
g++ -std=c++17 --version
```

### Issue: "Too many open files" error at runtime

**Solution:** Increase file descriptor limit

```bash
# Check current limit
ulimit -n

# Temporarily increase (current session only)
ulimit -n 4096

# Permanently increase (add to ~/.bashrc or ~/.zshrc)
echo "ulimit -n 4096" >> ~/.bashrc
source ~/.bashrc
```

### Issue: Linker errors with pthread

**Solution:** Ensure pthread linking in Makefile

```makefile
LDFLAGS = -pthread  # or -lpthread
```

## System Configuration

### Optional: Performance Tuning

For optimal performance:

```bash
# Check and increase system limits
cat /etc/security/limits.conf

# Increase process limits if needed
echo "* soft nofile 4096" | sudo tee -a /etc/security/limits.conf
echo "* hard nofile 4096" | sudo tee -a /etc/security/limits.conf
```

### Recommended: Environment Variables

```bash
# Set C++ compiler (optional)
export CXX=g++
export CC=gcc

# Set optimization flags (optional)
export CXXFLAGS="-std=c++17 -O2"
```

## Development Environment Setup

### VS Code Configuration

Create `.vscode/settings.json`:

```json
{
  "C_Cpp.standard": "c++17",
  "C_Cpp.intelliSenseEngine": "Tag Parser",
  "[cpp]": {
    "editor.formatOnSave": true
  }
}
```

### Debugging Setup

Install debugging tools:

```bash
# Ubuntu/Debian
sudo apt install gdb lldb

# Fedora
sudo dnf install gdb lldb

# macOS
brew install gdb
```

## Next Steps

1. **Read the Main README**: [README.md](README.md)
2. **Check Usage Guide**: [USAGE.md](USAGE.md)
3. **Review Architecture**: [ARCHITECTURE.md](ARCHITECTURE.md)
4. **Run the Program**: `./plazza 2 5 2000`

## Support

If you encounter issues:

1. Check this guide's troubleshooting section
2. Verify all dependencies are installed: `make --version && g++ --version`
3. Try a clean rebuild: `make fclean && make`
4. Check system resources: `ulimit -a`

---

**Last Updated**: May 2026
