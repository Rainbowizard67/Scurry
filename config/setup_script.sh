#!/bin/bash

#============================#
# Start Function declaration #
#============================#

find_config() {
    local env_file="$1"
    export SCURRY_CONFIG_DIR="$(pwd)"
    echo
}

get_distro() {
    distro=$(lsb_release -i | sed 's/^Distributor ID:[[:space:]]*//')
    echo "$distro"
}

debian_depend_install() {
    sudo apt update
    sudo apt install valgrind gdb build-essential libyaml-dev libcyaml-dev libssl-dev cmake -y
}

arch_depend_install() {
    sudo pacman -Syu --noconfirm
    sudo pacman -Sy valgrind gdb base-devel libcyaml openssl cmake --noconfirm
}

depend_switch() {
    local distro="$1"
    case "$distro" in
	Ubuntu|Debian)
	    debian_depend_install
	    ;;
	Fedora|Redhat)
	    ;;
	Arch)
	    arch_depend_install
	    ;;
	*)
	    printf "\e[31mUnsupported distro: $distro"
	    return 1
	    ;;
    esac
}

create_program_env() {
    local ENV_FILE="scurry.env"

    cat > "$ENV_FILE" <<EOL
###Scurry Env###
DISTRO_TYPE="$1"
EOL

    find_config "$ENV_FILE"
}

#==========================#
# End Function Declaration #
#==========================#

#=========================#
# Starting Main of Script #
#=========================#

distro=$(get_distro)

sudo printf "%b\n" "\e[0;34mDownloading and Installing dependencies...\e[0m"

depend_switch "$distro"

sudo printf "%b\n" "\e[0;34mFinished dependency install\e[0m"

echo

sudo printf "%b\n" "\e[0;34mStarting env file initialization...\e[0m"

echo "."
sleep 1
echo "."
sleep 1
echo "."

create_program_env "$distro"

sudo printf "%b\n" "\e[0;34mFinished env file initialization\e[0m"

#=======================#
# Ending Main of Script #
#=======================#
