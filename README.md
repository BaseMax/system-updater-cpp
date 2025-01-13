# System Updater in C++

A simple cross-platform system updater written in C++ that can be used to check and update various system packages and backup critical system files. This tool is designed to work on **Linux**, **macOS**, and **Windows** operating systems.

## Features

- Checks system information.
- Backs up critical system files.
- Checks for updates and updates packages for supported Linux distributions.
- Updates macOS using the built-in `softwareupdate` command.
- Updates Windows using PowerShell.

## Supported Operating Systems

- **Linux** (Ubuntu, Debian, Mint, CentOS, Fedora, RedHat, Arch, Manjaro, OpenSUSE)
- **macOS**
- **Windows**

## Requirements

- A C++20 or higher compatible compiler.
- CMake (version 3.16 or higher).

## Building the Project

### Using CMake:

1. Clone this repository:

   ```sh
   git clone https://github.com/BaseMax/system-updater-cpp.git
   cd system-updater-cpp
   ```

2. Create a build directory:

  ```sh
  mkdir build
  cd build
  ```

3. Generate the build files with CMake:

```sh
cmake ..
```

4. Build the project:

```sh
cmake --build .
```

5. After building, the executable SystemUpdater will be available in the build directory.

### Install the Program (optional)

You can install the program to your system using the following command:

```sh
sudo cmake --install .
```

This will install the program to the appropriate system directories.

## Usage

Run the program:

```sh
./SystemUpdater
```

The program will detect your operating system and offer to check and update your system.

Follow the prompts to:

- Display system information.
- Backup critical system files.
- Check and update your system.

- For Linux, it will check for updates using the appropriate package manager for your distribution.
- For macOS, it uses softwareupdate to install updates.
- For Windows, it opens the Windows Update settings.

### License

MIT License - see the [LICENSE](LICENSE) file for details.

Copyright 2025 - Kambiz Asadzade & Max Bae
