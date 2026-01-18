# Installation Guide for EKNMusic - First Iteration

## Step-by-Step Installation

### Step 1: Visual Studio Build Tools ✅ (Installing...)
Visual Studio Build Tools is currently being installed automatically. This will take 10-15 minutes.

### Step 2: Install Qt6 (Manual Installation Required)

1. **Download Qt Online Installer**
   - Go to: https://www.qt.io/download-qt-installer
   - Click "Download the Qt Online Installer"
   - Run the downloaded file: `qt-unified-windows-x64-online.exe`

2. **Create Qt Account** (if you don't have one)
   - You'll need a free Qt account to use the installer
   - Sign up at: https://login.qt.io/register

3. **Installation Selection**
   - Login with your Qt account
   - Choose "Custom Installation"
   - Select the following components:
     - ✅ Qt 6.6.x (or latest 6.x version)
       - ✅ MSVC 2019 64-bit
       - ✅ Qt Multimedia
       - ✅ Sources (optional, for reference)
     - ✅ Developer and Designer Tools
       - ✅ CMake (if not already installed)
       - ✅ Ninja
       - ✅ Qt Creator (optional but recommended)

4. **Installation Path**
   - Default path: `C:\Qt\`
   - You can change it, but remember the path for later
   - Installation size: ~5-8 GB

5. **Wait for Installation**
   - This will take 15-30 minutes depending on your internet speed

### Step 3: Set Up Environment (After Qt Installation)

Add Qt to your PATH:

1. Open System Environment Variables:
   - Press `Win + R`
   - Type `sysdm.cpl`
   - Click "Environment Variables"

2. Add to Path:
   - Under "System variables", find and select "Path"
   - Click "Edit"
   - Click "New"
   - Add: `C:\Qt\6.6.0\msvc2019_64\bin` (adjust version if different)
   - Click OK

### Step 4: Verify Installation

Open a new Command Prompt and run:

```cmd
cmake --version
qmake --version
```

Both should output version information.

### Step 5: Build the Project

1. Navigate to the client directory:
```cmd
cd D:\programs\EKNMusic\client
```

2. Create build directory:
```cmd
mkdir build
cd build
```

3. Configure with CMake:
```cmd
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="C:\Qt\6.6.0\msvc2019_64" ..
```

Note: Adjust the Qt path if you installed to a different location.

4. Build the project:
```cmd
cmake --build . --config Release
```

5. Run the application:
```cmd
.\Release\EKNMusic.exe
```

## Troubleshooting

### Qt not found by CMake
**Error**: `Could not find a package configuration file provided by "Qt6"`

**Solution**: Make sure CMAKE_PREFIX_PATH points to your Qt installation:
```cmd
cmake -DCMAKE_PREFIX_PATH="C:\Qt\6.6.0\msvc2019_64" ..
```

### Missing DLLs when running
**Error**: Application fails to start due to missing Qt DLLs

**Solution**: Use windeployqt to copy required DLLs:
```cmd
C:\Qt\6.6.0\msvc2019_64\bin\windeployqt.exe .\Release\EKNMusic.exe
```

### MSVC compiler not found
**Error**: CMake can't find the C++ compiler

**Solution**:
1. Make sure Visual Studio Build Tools installation completed
2. Open "Developer Command Prompt for VS 2022"
3. Run the cmake commands from there

### Build fails with Qt version mismatch
**Error**: Qt version in CMakeLists.txt doesn't match installed version

**Solution**: Check your Qt version and update if needed:
```cmd
qmake --version
```

## Using Qt Creator (Alternative Method)

If you installed Qt Creator:

1. Open Qt Creator
2. File → Open File or Project
3. Select `D:\programs\EKNMusic\client\CMakeLists.txt`
4. Choose the Qt 6.x MSVC kit
5. Click Configure Project
6. Click the green "Run" button (▶)

This is easier for beginners and handles building automatically!

## Quick Start Script (After Installation)

Save this as `build.bat` in the `client` folder:

```batch
@echo off
echo Building EKNMusic...

if not exist build mkdir build
cd build

cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="C:\Qt\6.6.0\msvc2019_64" ..
if errorlevel 1 goto error

cmake --build . --config Release
if errorlevel 1 goto error

echo.
echo Build successful! Running application...
cd Release
windeployqt.exe EKNMusic.exe
EKNMusic.exe
goto end

:error
echo Build failed! Check the errors above.
pause

:end
```

Then just double-click `build.bat` to build and run!

## Next Steps

Once the application is running, you can:
- ✅ Test the login page (any credentials will work for iteration 1)
- ✅ Browse the Liked Songs page
- ✅ Try the Search functionality
- ✅ Check the Downloaded Songs list
- ✅ Play with the Player controls (UI only, no actual playback yet)

Enjoy your Spotify clone! 🎵
