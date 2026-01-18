# First Iteration - Base Qt Spotify Clone

## Overview
This document outlines the first iteration of the EKNMusic application, a simple Spotify clone built with Qt.

## Features to Implement

### 1. Login Page
- User authentication interface
- Username/email and password input fields
- Login button
- Remember me option
- Sign up link (UI only for now)

### 2. Liked Songs Page
- Display list of user's liked/favorite songs
- Song item components showing:
  - Song title
  - Artist name
  - Album art thumbnail
  - Duration
  - Like/Unlike button
- Play button for each song

### 3. Player Page/Widget
- Bottom fixed player bar
- Currently playing song information
- Playback controls:
  - Play/Pause button
  - Previous track button
  - Next track button
  - Shuffle button
  - Repeat button
- Progress bar with seek functionality
- Volume control
- Current time / Total duration display

### 4. Search Page
- Search input field
- Search results displaying:
  - Songs matching search query
  - Artist name
  - Album information
  - Play and Add to Liked buttons

### 5. Downloaded Songs List
- Display locally downloaded songs
- Similar layout to Liked Songs
- Download status indicators
- Delete downloaded song option

## UI/UX Design

### Color Scheme (Spotify-like)
- Background: Dark theme (#121212)
- Sidebar: Darker shade (#000000)
- Primary accent: Green (#1DB954)
- Text: White (#FFFFFF)
- Secondary text: Gray (#B3B3B3)

### Layout Structure
```
+------------------+------------------------+
|                  |                        |
|   Sidebar        |    Content Area        |
|   - Logo         |    (Stacked Pages)     |
|   - Liked Songs  |                        |
|   - Search       |                        |
|   - Downloaded   |                        |
|                  |                        |
+------------------+------------------------+
|          Player Widget (Bottom)           |
+-------------------------------------------+
```

## Technical Components

### Main Window
- Navigation sidebar with menu items
- Stacked widget for page switching
- Fixed player widget at bottom

### Pages (QWidget-based)
1. **LoginWindow**: Separate window, shows before main app
2. **LikedSongsPage**: QListWidget with custom items
3. **SearchPage**: QLineEdit + QListWidget
4. **DownloadedSongsPage**: QListWidget similar to liked songs
5. **PlayerWidget**: Custom widget with media controls

### Data Models (For First Iteration)
- Mock/hardcoded data for display
- No actual backend integration yet

## Files to Create

### UI Components
- `mainwindow.h/cpp` - Main application window
- `loginwindow.h/cpp` - Login interface
- `likedsongs.h/cpp` - Liked songs page
- `searchpage.h/cpp` - Search interface
- `downloadedpage.h/cpp` - Downloaded songs list
- `playerwidget.h/cpp` - Player controls

### Supporting Files
- `main.cpp` - Application entry point
- `styles.qss` - Qt stylesheet for theming
- `CMakeLists.txt` - Build configuration

## Dependencies
- Qt6 Core
- Qt6 Widgets
- Qt6 Multimedia (for future audio playback)

## Notes
- This iteration focuses on UI/UX only
- No actual audio playback implementation
- No backend API integration
- Uses mock data for demonstration
- Foundation for future iterations
