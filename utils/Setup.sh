#!/bin/bash

# Script to download git, build-tools, clone a GitHub repo, navigate to the directory, and run make command

# Logging setup
LOG_FILE="install.log"
LOG_LEVEL="INFO"

# Function to log messages
log() {
    local level=$1
    local message=$2
    echo -e "$(date +"%Y-%m-%d %H:%M:%S") [$level] - \e[96m$message\e[39m"
    echo "$(date +"%Y-%m-%d %H:%M:%S") [$level] - $message" >> "$LOG_FILE"
}

# Function to check if a package is installed
is_package_installed() {
    local package_name="$1"
    dpkg-query -W -f='${Status}' "$package_name" 2>/dev/null | grep -q "install ok installed"
}

# Function to install packages
install_packages() {
    local packages=("$@")
    for package in "${packages[@]}"; do
        if is_package_installed "$package"; then
            log "INFO" "$package is already installed."
        else
            log "INFO" "Installing $package..."
            apt install -y "$package" >> "$LOG_FILE" 2>&1
            if [ $? -eq 0 ]; then
                log "INFO" "$package installed successfully."
            else
                log "ERROR" "Failed to install $package."
            fi
        fi
    done
}

# Function to clone a GitHub repo, navigate to the directory, and run make command
clone_repo_and_run_make() {
    local repo_url="$1"
    local repo_dir="$2"

    log "INFO"  "Cloning the GitHub repo..."
    git clone --recursive "$repo_url" "$repo_dir" || log "ERROR" "Failed to clone the GitHub repo"

    # Build libs
    log "INFO" "Navigating to the directory..."
    cd "$repo_dir" || log "ERROR" "Failed to navigate to the directory"
    cd "rpi-rgb-led-matrix" || log "ERROR" "Failed to navigate to the directory"

    log "INFO"  "Running make command..."
    make || log "ERROR" "Failed to run make command"

    # Build this application
    log "INFO" "Navigating to the directory..."
    cd "../" || log "ERROR" "Failed to navigate to the directory"

    log "INFO"  "Running make command..."
    make || log "ERROR" "Failed to run make command"
}

# Main function
main() {

    log "INFO" "Starting package installation..."
    packages=("libgraphicsmagick++-dev" "libwebp-dev" "git" "build-essential" "make" "libboost-all-dev")
    install_packages "${packages[@]}"
    log "INFO" "Package installation completed."

    # Clone the GitHub repo, navigate to the directory, and run make command
    clone_repo_and_run_make "https://github.com/Differentunic/advanced-rpi-rgb-led-matrix" "advanced-rpi-rgb-led-matrix"
}

# Example usage
main
