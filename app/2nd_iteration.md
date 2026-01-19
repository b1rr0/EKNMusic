# Second Iteration - Music Player Implementation

## Overview
This document outlines the second iteration of the EKNMusic application, focusing on implementing core music playback functionality with Qt.

## Goals
- Implement functional music player with playback controls
- Create music file storage and management system
- Extract player logic into dedicated service layer
- Synchronize player state across UI components

## Features to Implement

### 1. Music Player Core Functionality

#### 1.1 Player Page Implementation
Create a dedicated player page with full playback controls.

**Design Reference:**
![Player Example.png](../client/src/resources/images/Player%20Exampe.png)

**Required Components:**
- Play/Pause button
- Previous/Next track buttons
- Progress bar with seek functionality
- Volume control
- Time display (current/total)
- Album artwork display
- Track information (title, artist, album)

#### 1.2 Playback Logic
Implement core music player functionality:
- **Audio playback**: Use Qt Multimedia (QMediaPlayer) for audio playback
- **Playlist management**: Queue system for multiple tracks
- **Playback controls**: Play, pause, stop, skip forward/backward
- **Progress tracking**: Real-time progress updates and seeking
- **Volume control**: Adjustable volume with mute functionality

#### 1.3 Music File Storage
Create a directory structure for storing music files:
- **Local storage directory**: `~/EKNMusic/songs/` or application data directory
- **File organization**: Organize by artist/album or maintain flat structure
- **Supported formats**: MP3, FLAC, WAV, OGG
- **Metadata extraction**: Read ID3 tags for track information

#### 1.4 Downloaded Songs Integration
Display downloaded songs in the Downloads tab:
- Scan local storage directory on app startup
- Show list of available tracks with metadata
- Allow playback from Downloads page
- Sync with player controls

### 2. Player State Synchronization

#### 2.1 Main Window Player Status
Display current playback status in the main window:
- Mini player bar at bottom showing:
  - Currently playing track info
  - Play/Pause button
  - Progress indicator
  - Like/Unlike button
- Sync state with full Player page
- Persist player state across page navigation

### 3. Architecture Improvements

#### 3.1 Player Service Layer
Extract player logic into a dedicated service class:

**PlayerService responsibilities:**
- Manage QMediaPlayer instance
- Handle playlist queue
- Emit signals for state changes
- Provide playback control methods
- Store current playback state

**Benefits:**
- Separation of concerns
- Reusable player logic across UI components
- Easier testing and maintenance
- Centralized state management

#### 3.2 Persistent Music Directory
Ensure music directory persists after application build:
- Use QStandardPaths for cross-platform directory location
- Create directory structure on first launch if not exists
- Store directory path in application settings
- Maintain directory integrity across app updates

## Technical Implementation Notes

### Qt Multimedia Setup
```cpp
// Add to CMakeLists.txt
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
target_link_libraries(EKNMusic PRIVATE Qt6::Multimedia)
```

### Recommended File Structure
```
client/src/
├── services/
│   └── playerservice.h/cpp
├── ui/
│   └── playerpage.h/cpp
└── models/
    └── track.h/cpp
```

### Music Storage Location
- **Windows**: `%APPDATA%/EKNMusic/songs/`
- **macOS**: `~/Library/Application Support/EKNMusic/songs/`
- **Linux**: `~/.local/share/EKNMusic/songs/`

## Success Criteria
- [x] User can play, pause, and skip tracks
- [x] Player displays current track information
- [x] Downloaded songs are accessible and playable
- [x] Player state syncs between full player and mini player
- [x] Music directory persists after app restart
- [x] Player logic is separated into service layer
