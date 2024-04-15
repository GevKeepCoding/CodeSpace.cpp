## To install latest clang

```bash
#Add the key:
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -

sudo tee /etc/apt/sources.list.d/llvm-toolchain-noble.list << EOF  
deb http://apt.llvm.org/noble/ llvm-toolchain-noble main  
EOF
```

---

## Install latest cmake ubuntu:

```
# install prerequisites
sudo apt update
sudo apt install -y apt-transport-https ca-certificates gnupg software-properties-common wget

# add Kitware’s signing key & repo (replace <codename> with your Ubuntu release, e.g. “focal”, “jammy”, “noble”)
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | sudo apt-key add -
sudo apt-add-repository "deb https://apt.kitware.com/ubuntu/ <codename> main"

# install
sudo apt update
sudo apt install -y cmake
```

---