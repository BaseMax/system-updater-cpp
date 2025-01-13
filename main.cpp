#include <iostream>
#include <print>
#include <string>
#include <unordered_map>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <span>
#include <format>
#include <expected>
#include <concepts>

namespace fs = std::filesystem;

template <typename T>
concept CommandLike = requires(T t) {
    { std::string(t) } -> std::convertible_to<std::string>;
};

template <CommandLike T>
std::expected<void, std::string> run_command(const T& command) {
    std::string cmd = std::string(command);
    int result = std::system(cmd.c_str());
    if (result != 0) {
        return std::unexpected(std::format("Command failed with exit code: {}", result));
    }
    return {};
}

void print_error(const std::string& message) {
    std::print(stderr, "Error: {}\n", message);
}

void display_system_info() {
    std::print("\nSystem Information:\n");
    std::print("-------------------\n");

#ifdef _WIN32
    if (auto result = run_command("systeminfo"); !result) {
        print_error(result.error());
    }
#elif __APPLE__
    if (auto result = run_command("df -h"); !result) {
        print_error(result.error());
    }
    if (auto result = run_command("top -l 1 -s 0"); !result) {
        print_error(result.error());
    }
#elif __linux__
    if (auto result = run_command("df -h"); !result) {
        print_error(result.error());
    }
    if (auto result = run_command("free -h"); !result) {
        print_error(result.error());
    }
#else
    std::print("Unsupported platform\n");
#endif
}

template <typename DistroMap>
void check_updates_linux(const std::string& distro, const DistroMap& check_commands) {
    if (auto it = check_commands.find(distro); it != check_commands.end()) {
        std::print("Checking for updates using {}...\n", it->second);
        if (auto result = run_command(it->second); !result) {
            print_error(result.error());
            std::exit(1);
        }
    } else {
        print_error("Unsupported distribution: " + distro);
        std::exit(1);
    }
}

void backup_system_files() {
    const std::string backup_dir = "/tmp/system_backup";
    fs::create_directories(backup_dir);
    const std::vector<std::string> critical_files = {"/etc/fstab", "/etc/hostname", "/etc/hosts", "/etc/passwd"};

    for (const auto& file : critical_files) {
        if (fs::exists(file)) {
            try {
                fs::copy(file, backup_dir + "/" + fs::path(file).filename().string(), fs::copy_options::overwrite_existing);
                std::print("Backed up {} to {}\n", file, backup_dir);
            } catch (const fs::filesystem_error& e) {
                print_error(e.what());
                std::exit(1);
            }
        } else {
            std::print("File {} does not exist, skipping backup.\n", file);
        }
    }
}

template <typename DistroMap>
void update_linux(const std::string& distro, const DistroMap& update_commands) {
    if (auto it = update_commands.find(distro); it != update_commands.end()) {
        std::print("Updating system using {}...\n", it->second);
        if (auto result = run_command(it->second); !result) {
            print_error(result.error());
            std::exit(1);
        }
    } else {
        print_error("Unsupported Linux distribution: " + distro);
        std::exit(1);
    }
}

void update_macos() {
    std::print("Updating system using softwareupdate...\n");
    if (auto result = run_command("sudo softwareupdate --install --all"); !result) {
        print_error(result.error());
        std::exit(1);
    }
}

void update_windows() {
    std::print("Updating Windows system...\n");
    if (auto result = run_command("powershell Start-Process ms-settings:windowsupdate"); !result) {
        print_error(result.error());
        std::exit(1);
    }
}

std::string detect_os() {
#ifdef _WIN32
    return "windows";
#elif __APPLE__
    return "macos";
#elif __linux__
    try {
        std::ifstream os_release("/etc/os-release");
        std::string os_info;
        std::getline(os_release, os_info);
        for (const auto& distro : {"ubuntu", "debian", "mint", "centos", "fedora", "redhat", "arch", "manjaro", "opensuse"}) {
            if (os_info.find(distro) != std::string::npos) {
                return distro;
            }
        }
        return "unknown_linux";
    } catch (const std::exception& e) {
        print_error("Could not determine Linux distribution.");
        std::exit(1);
    }
#else
    return "unknown";
#endif
}

int main() {
    std::string os_type = detect_os();

    display_system_info();

    std::print("Do you want to proceed with updating the system? (y/n): ");
    char proceed;
    std::cin >> proceed;
    if (proceed != 'y' && proceed != 'Y') {
        std::print("Update canceled.\n");
        std::exit(0);
    }

    backup_system_files();

    if (os_type == "ubuntu" || os_type == "debian" || os_type == "mint" || os_type == "centos" || os_type == "fedora" || os_type == "redhat" || os_type == "arch" || os_type == "manjaro" || os_type == "opensuse") {
        const std::unordered_map<std::string, std::string> check_commands = {
            {"ubuntu", "sudo apt list --upgradable"},
            {"debian", "sudo apt list --upgradable"},
            {"mint", "sudo apt list --upgradable"},
            {"centos", "sudo dnf check-update"},
            {"fedora", "sudo dnf check-update"},
            {"redhat", "sudo dnf check-update"},
            {"arch", "sudo pacman -Qu"},
            {"manjaro", "sudo pacman -Qu"},
            {"opensuse", "sudo zypper list-updates" }
        };
        check_updates_linux(os_type, check_commands);

        const std::unordered_map<std::string, std::string> update_commands = {
            {"ubuntu", "sudo apt update && sudo apt upgrade -y"},
            {"debian", "sudo apt update && sudo apt upgrade -y"},
            {"mint", "sudo apt update && sudo apt upgrade -y"},
            {"centos", "sudo dnf update -y"},
            {"fedora", "sudo dnf update -y"},
            {"redhat", "sudo dnf update -y"},
            {"arch", "sudo pacman -Syu --noconfirm"},
            {"manjaro", "sudo pacman -Syu --noconfirm"},
            {"opensuse", "sudo zypper update -y"}
        };
        update_linux(os_type, update_commands);
    } else if (os_type == "macos") {
        update_macos();
    } else if (os_type == "windows") {
        update_windows();
    } else {
        std::print("Unsupported operating system.\n");
        std::exit(1);
    }

    return 0;
}
