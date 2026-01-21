# EKNMusic - Version 1.0.0 (First Iteration)

**Release Date:** January 19, 2026

## Overview

This is the first stable release of EKNMusic, a minimalist music player application built with Qt 6. This release focuses on delivering a clean, functional UI with a black/white/gray color scheme inspired by minimalist design principles.

## What's New

### Features

#### User Interface
- ✅ **Minimalist Design**: Clean black, white, and gray color scheme throughout the application
- ✅ **Login Window**: User authentication interface with email/password inputs
- ✅ **Main Window**: Navigation sidebar with three main sections:
  - Liked Songs
  - Search
  - Downloads
- ✅ **Custom Logo**: Integration of application logo from resources
- ✅ **Responsive Layout**: Adapts to different window sizes

#### Pages

**Liked Songs Page**
- Display list of liked songs with mock data
- Card-based layout with white cards on light gray background
- Song information: title, artist, duration
- Play button for each song
- Like/Unlike functionality (UI only)

**Search Page**
- Search input with rounded corners
- Real-time search filtering
- Display search results in card format
- Album art placeholder
- Add to liked songs button
- Play button for each result

**Downloaded Songs Page**
- List of downloaded tracks
- Display file size and duration
- Download status indicator
- Delete button for each song
- Play functionality (UI only)

#### Player Widget
- Mini player bar at bottom of main window
- Playback controls: play/pause, previous, next, shuffle, repeat
- Progress bar with time display (current/total)
- Volume control with mute functionality
- Album artwork display area
- Currently playing track information

### Design Specifications

**Color Palette:**
- Background: `#FFFFFF` (white), `#f5f5f5` (light gray)
- Sidebar: `#ffffff` with `#e0e0e0` border
- Text: `#000000` (black), `#666666` (gray), `#888888` (light gray)
- Buttons: Black background (`#000000`) with white text, or white with gray border
- Hover states: `#f5f5f5` background, `#333333` for dark elements
- Borders: 1px solid `#e0e0e0` / `#d0d0d0`

**Typography:**
- Title: 32px, weight 600, black
- Body: 13-15px, weight 400-600
- Secondary: 11-13px, gray

**Components:**
- Rounded corners: 4-8px border radius
- Shadows: Subtle `0 2px 8px rgba(0,0,0,0.1)` on hover
- Consistent spacing: 15-40px margins, 10-25px padding

## Technical Details

### Architecture
- **Language**: C++17
- **Framework**: Qt 6.x
- **Build System**: CMake
- **Supported Platforms**: Windows 10+, macOS 11+, Linux

### Project Structure
```
client/src/
├── ui/
│   ├── loginwindow.h/cpp      # Login interface
│   ├── mainwindow.h/cpp       # Main application window
│   ├── likedsongs.h/cpp       # Liked songs page
│   ├── searchpage.h/cpp       # Search page
│   ├── downloadedpage.h/cpp   # Downloaded songs page
│   └── playerwidget.h/cpp     # Player controls widget
└── resources/
    └── images/                # Application images and logo
```

### Mock Data
This release uses mock data for demonstration purposes:
- 10 pre-populated songs in Liked Songs
- 15 songs available for search
- 5 songs in Downloads section

## Known Limitations

### Iteration 1 Scope
This release is focused on UI/UX implementation. The following features are **not yet implemented**:

- ❌ Actual music playback (QMediaPlayer integration pending)
- ❌ Real authentication system (accepts any credentials)
- ❌ Backend API integration
- ❌ Persistent data storage (local database)
- ❌ File system integration for music files
- ❌ Metadata extraction from audio files
- ❌ Playlist management
- ❌ Sign-up functionality
- ❌ Settings and preferences

### Planned for Next Release (Iteration 2)
See [app/2nd_iteration.md](app/2nd_iteration.md) for details:
- Music player service implementation
- Qt Multimedia (QMediaPlayer) integration
- Local music file storage and scanning
- Playback functionality
- Player state synchronization
- Persistent music directory

## Installation

### Prerequisites
- **Windows**: Visual Studio 2022, Qt 6.6+
- **macOS**: Xcode Command Line Tools, Qt 6.6+
- **Linux**: GCC/Clang, Qt 6.6+

### Building from Source

```bash
# Clone repository
git clone https://github.com/yourusername/EKNMusic.git
cd EKNMusic/client

# Create build directory
mkdir build && cd build

# Configure (replace path to Qt)
cmake -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x ..

# Build
cmake --build . --config Release

# Run
./Release/EKNMusic.exe     # Windows
./EKNMusic                  # Linux/macOS
```

See [README.md](README.md) for detailed build instructions.

## Screenshots

### Login Window
Clean login interface with logo and minimal form design.

### Main Window
Three-panel layout: sidebar navigation, content area, and bottom player bar.

### Liked Songs
Card-based song list with play buttons and metadata.

### Search
Real-time search with filtered results.

## Credits

**Development:**
- Written entirely with AI assistance (Claude, GPT)
- UI Design inspired by minimalist and Ronin design principles

**Technologies:**
- Qt Framework
- CMake
- C++17/20

## Changelog

### Version 1.0.0 (2026-01-19)
- Initial release
- Minimalist UI implementation
- Login window with authentication UI
- Main window with sidebar navigation
- Liked Songs page with mock data
- Search page with filtering
- Downloaded Songs page
- Player widget with controls
- Consistent black/white/gray design system
- Logo integration from resources
- Mock data for demonstration

## Upgrade Notes

This is the first release. No upgrades required.

## Feedback & Contributing

We welcome feedback and contributions!

- Report bugs: [GitHub Issues](https://github.com/yourusername/EKNMusic/issues)
- Feature requests: [GitHub Discussions](https://github.com/yourusername/EKNMusic/discussions)
- Pull requests: See [CONTRIBUTING.md](CONTRIBUTING.md)

## Next Steps

After installing:
1. Launch the application
2. Enter any email/password (authentication is mock)
3. Explore the UI: Liked Songs, Search, Downloads
4. Check the mini player at the bottom
5. Provide feedback for next iteration

## Support

For help and support:
- Check the [README.md](README.md)
- Review [docs/](docs/) directory
- Open an issue on GitHub

---

**Thank you for trying EKNMusic v1.0.0!**

We're excited to bring you music playback functionality in the next iteration.
