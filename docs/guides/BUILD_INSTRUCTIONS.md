# Build Instructions for EKNMusic

## Prerequisites

### Windows
- Qt 6.10.1+ with MinGW 64-bit
- CMake 3.16+
- MinGW 64-bit compiler

### macOS
- Qt 6.10.1+
- CMake 3.16+
- Xcode Command Line Tools

### Linux
- Qt 6.10.1+
- CMake 3.16+
- GCC/Clang compiler
- Development libraries: `qt6-base-dev qt6-multimedia-dev`

## Building the Desktop Client

### Method 1: Using Qt Creator (Recommended)
1. Open `client/CMakeLists.txt` in Qt Creator
2. Configure the project with desired kit (Desktop Qt 6.10.1 MinGW 64-bit)
3. Build → Build Project
4. Run → Run

### Method 2: Command Line Build

#### Windows (MinGW)
```bash
cd client
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="D:/programs/6.10.1/mingw_64"
cmake --build . --config Release
```

#### macOS
```bash
cd client
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH="/path/to/Qt/6.10.1/macos"
cmake --build . --config Release
```

#### Linux
```bash
cd client
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH="/path/to/Qt/6.10.1/gcc_64"
cmake --build . --config Release
```

## Common Build Issues

### Issue: "cmake: command not found"
**Solution**: Add CMake to your PATH or use Qt Creator which includes CMake.

### Issue: "Qt6 not found"
**Solution**: Set `CMAKE_PREFIX_PATH` to your Qt installation directory:
```bash
cmake .. -DCMAKE_PREFIX_PATH="path/to/Qt/6.10.1/your_kit"
```

### Issue: "windres.exe not found" (Windows)
**Solution**: Update path in `CMakeLists.txt` line 5:
```cmake
set(CMAKE_RC_COMPILER "path/to/mingw/bin/windres.exe" CACHE FILEPATH "RC Compiler")
```

### Issue: Linking errors with Qt libraries
**Solution**:
1. Ensure all Qt modules are installed: Core, Widgets, Network, Sql, Multimedia
2. Clean build directory and rebuild:
```bash
rm -rf build/*
cmake .. && cmake --build .
```

### Issue: Missing multimedia support
**Solution**: Install Qt Multimedia module:
- Windows: Use Qt Maintenance Tool
- macOS: `brew install qt6-multimedia`
- Linux: `sudo apt install qt6-multimedia-dev`

## Running the Application

### Windows
```bash
cd build
./EKNMusic.exe
```

### macOS
```bash
cd build
open EKNMusic.app
```

### Linux
```bash
cd build
./EKNMusic
```

## Deployment

### Windows Deployment
```bash
cd build
windeployqt EKNMusic.exe --release --no-translations
```

### macOS Deployment
```bash
cd build
macdeployqt EKNMusic.app -dmg
```

### Linux Deployment
Use `linuxdeployqt` or create AppImage:
```bash
linuxdeployqt EKNMusic -appimage
```

## Configuration Notes

- **API Key**: Stored in system settings (Registry/plist/conf)
- **Settings Location**:
  - Windows: `HKEY_CURRENT_USER\Software\EKN\EKNMusic`
  - macOS: `~/Library/Preferences/com.EKN.EKNMusic.plist`
  - Linux: `~/.config/EKN/EKNMusic.conf`

## Clean Build

To perform a clean build:
```bash
cd client
rm -rf build/
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
```

## Debug Build

For debugging with console output:
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```

On Windows, debug builds will show a console window for log output.

## Troubleshooting

If you encounter any issues:
1. Check Qt version: `qmake --version` or check in Qt Creator
2. Verify CMake version: `cmake --version`
3. Clean and rebuild
4. Check `Issues` tab in Qt Creator for detailed errors
5. Ensure all source files are properly added to `CMakeLists.txt`

## Additional Resources

- [Qt Documentation](https://doc.qt.io/qt-6/)
- [CMake Documentation](https://cmake.org/documentation/)
- [Radio API Documentation](./RADIO_API.md)
