# The Input Transfer Project - SendMeBread

## My irrational goal is to make it so that other computers (clients) can input on one computer (the host). Not just typing, but the cursor as well. *Every input will be included.*

---

### Making

### Steps for Windows

Install Git (Desktop or Bash).

To install Git Bash:

Download the correct binary for YOUR operating system and run it: <https://github.com/git-for-windows/git/releases/tag/v2.51.0.windows.2>

---

To install Github Desktop:

Download and install: <https://desktop.github.com/download/>

---

Install a C++ compiler (minGW-w64 recommended for this repository).

To install minGW-w64:

Install MSYS2: <https://www.msys2.org/>

Install minGW-w64 inside the MSYS2 environment:

```bash
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
```

To add C:\msys64\mingw64\bin to PATH (Required):

Press Windows + R (this will open the Run Prompt), then type sysdm, the press enter. Open the "Advanced" tab and click "Environment Variables."

---

Download and install CMake: <https://cmake.org/download/>

Building:

Options for cloning:

Option 1 (Git Bash):

```powershell
git clone https://github.com/SendMeBread/InputTransfer.git && cd InputTransfer
md build && cd build
cmake ../src/win
mingw32-make.exe
```

---

Option 2 (Github Desktop):

Click on "File" (top left corner):

Next, click on "Clone repository..."

On the input screen, type "<https://github.com/SendMeBread/InputTransfer.git>"

Click "Clone."

Open the Command Prompt (cmd.exe)

Enter the following commands:

```powershell
cd OneDrive/Documents/GitHub/InputTransfer/
md build && cd build 
cmake ../src/win
mingw32-make.exe
```

### Steps for Debian/Debian based systems

Install dependencies:

```bash
sudo snap install cmake #DO NOT USE APT!

sudo apt update && sudo apt install git build-essential libx11-dev -y
```

Building:

```bash
git clone https://github.com/SendMeBread/InputTransfer.git && cd InputTransfer
mkdir build && cd build
cmake ../src/linux
make
```

---

### Steps for Arch Linux/Arch based systems

Install dependencies:

```bash
sudo pacman -Syu && sudo pacman -S git gcc libx11 cmake
```

Building:

```bash
git clone https://github.com/SendMeBread/InputTransfer.git && cd InputTransfer
mkdir build && cd build
cmake ../src/linux
make
```

---

### ***WIP***
