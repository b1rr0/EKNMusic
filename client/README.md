# EKNMusic Desktop Client

Cross-platform desktop application built with Qt C++.

## Technology Stack

- **Language**: C++17/C++20
- **Framework**: Qt 6.x
- **Build System**: CMake 3.16+
- **Platforms**: Windows 10+, macOS 11+, Linux (Ubuntu 20.04+)

## Directory Structure

```
client/
├── src/
│   ├── main.cpp           # Application entry point
│   ├── ui/                # User interface components
│   │   ├── dialogs/       # Dialog windows
│   │   └── widgets/       # Custom widgets
│   ├── api/               # Backend API client
│   │   └── models/        # Data models
│   ├── services/          # Business logic and services
│   ├── utils/             # Utility functions
│   └── resources/         # Icons, images, assets
├── tests/                 # Unit tests
├── platform/              # Platform-specific files
│   ├── windows/           # Windows resources, installer scripts
│   ├── macos/             # macOS bundle, DMG configuration
│   └── linux/             # Linux packaging scripts
├── build/                 # Build output (gitignored)
├── CMakeLists.txt         # CMake build configuration
├── resources.qrc          # Qt resource file
└── README.md              # This file
```

## Prerequisites

### All Platforms
- CMake 3.16 or later
- Qt 6.x SDK
- Git

### Windows
- Visual Studio 2019 or 2022 with C++ workload
- Qt 6.x for MSVC

### macOS
- Xcode 13+ with Command Line Tools
- Qt 6.x for macOS
- Homebrew (recommended)

### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install build-essential cmake git
sudo apt install qt6-base-dev qt6-tools-dev qt6-tools-dev-tools
sudo apt install libgl1-mesa-dev
```

## Building

### Windows

```bash
# Open Qt command prompt or configure environment
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -DCMAKE_PREFIX_PATH=C:/Qt/6.x/msvc2019_64
cmake --build . --config Release
```

Executable will be in `build/Release/EKNMusic.exe`

### macOS

```bash
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=~/Qt/6.x/macos
cmake --build . --config Release
```

Application bundle will be in `build/EKNMusic.app`

### Linux

```bash
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/qt6
cmake --build . --config Release
```

Executable will be in `build/EKNMusic`

## Running

### From Build Directory
```bash
# Windows
.\build\Release\EKNMusic.exe

# macOS
./build/EKNMusic.app/Contents/MacOS/EKNMusic

# Linux
./build/EKNMusic
```

### From Qt Creator
1. Open `CMakeLists.txt` in Qt Creator
2. Configure project with Qt kit
3. Click Run (Ctrl+R / Cmd+R)

## Development

### Qt Creator Setup
1. Install Qt Creator with Qt SDK
2. Open `CMakeLists.txt`
3. Configure kit (Desktop Qt 6.x)
4. Build and run

### Code Style
- Follow Qt coding conventions
- Use Qt naming conventions (camelCase for methods, PascalCase for classes)
- 4 spaces indentation
- Use Qt types where appropriate (QString, QList, etc.)

### Adding UI Components

#### Using Qt Designer
1. Create `.ui` file in Qt Designer
2. Add to `resources.qrc` or CMakeLists.txt
3. Include generated header in C++ code

#### Programmatic UI
```cpp
// Example widget creation
auto* button = new QPushButton("Click Me", this);
connect(button, &QPushButton::clicked, this, &MyClass::onButtonClicked);
```

### API Integration

API client classes in `src/api/` handle backend communication:
- REST API calls using QNetworkAccessManager
- JSON serialization/deserialization
- Authentication token management
- Request/response models in `src/api/models/`

## Testing

### Build with Tests
```bash
cmake .. -DBUILD_TESTS=ON
cmake --build .
```

### Run Tests
```bash
ctest --output-on-failure
# Or run test executable directly
./tests/EKNMusicTests
```

## Packaging and Distribution

### Windows Installer

1. **Bundle Dependencies**
   ```bash
   windeployqt.exe --release build/Release/EKNMusic.exe
   ```

2. **Create Installer**
   - Use NSIS or Inno Setup
   - Script in `platform/windows/installer.nsi`

3. **Sign Executable** (optional but recommended)
   ```bash
   signtool sign /f certificate.pfx EKNMusic.exe
   ```

### macOS DMG

1. **Create App Bundle**
   ```bash
   macdeployqt build/EKNMusic.app -dmg
   ```

2. **Sign and Notarize**
   ```bash
   codesign --deep --force --sign "Developer ID" EKNMusic.app
   xcrun notarytool submit EKNMusic.dmg --apple-id "email" --password "pwd"
   ```

### Linux Packages

#### AppImage (Universal)
```bash
linuxdeployqt EKNMusic -appimage
```

#### Debian Package
```bash
cd platform/linux
./build-deb.sh
```

#### RPM Package
```bash
cd platform/linux
./build-rpm.sh
```

## Configuration

### Application Settings
Settings stored using QSettings:
- Windows: Registry or AppData
- macOS: ~/Library/Preferences
- Linux: ~/.config/EKNMusic/

### Backend API URL
Configure in settings or use environment variable:
```bash
EKNMUSIC_API_URL=https://api.eknmusic.com
```

## Troubleshooting

### Qt Not Found
Ensure `CMAKE_PREFIX_PATH` points to Qt installation:
```bash
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/platform
```

### Missing Dependencies at Runtime
Run deployment tool to bundle Qt libraries:
- Windows: `windeployqt`
- macOS: `macdeployqt`
- Linux: `linuxdeployqt`

### Build Errors
- Verify Qt version compatibility (6.x required)
- Check compiler version (C++17 support required)
- Ensure all Qt modules installed

## Resources

- [Qt Documentation](https://doc.qt.io/)
- [CMake Documentation](https://cmake.org/documentation/)
- [Project Documentation](../docs/)

## Contributing

1. Create feature branch from `develop`
2. Follow Qt coding style
3. Write tests for new features
4. Test on all platforms if possible
5. Submit pull request

## License

[License information]
