# First Iteration - Spotify Clone Base Application

## Overview

First iteration focuses on creating a basic Spotify-like Qt desktop application with core UI screens and navigation structure. No backend integration in this iteration - focus is on UI/UX foundation.

## Goals

- Create a visually appealing Spotify-inspired interface
- Implement navigation between main screens
- Build reusable UI components
- Establish application architecture patterns

## Features to Implement

### 1. Login Page

**Purpose**: User authentication entry point

**UI Components**:
- Application logo/branding
- Email input field
- Password input field
- "Login" button
- "Remember me" checkbox
- "Forgot password?" link
- "Sign up" link
- Background with Spotify-like gradient

**Functionality** (First Iteration):
- UI layout only
- Navigate to main window on button click
- No actual authentication

**Design Notes**:
- Center-aligned login form
- Dark theme consistent with Spotify
- Smooth animations on focus/hover
- Responsive layout

---

### 2. Main Window with Navigation Sidebar

**Purpose**: Main application container with navigation

**Sidebar Components**:
- App logo at top
- Navigation buttons:
  - Home/Search icon + "Search"
  - Heart icon + "Liked Songs"
  - Download icon + "Downloaded"
- Fixed width (200-240px)
- Dark background (#000000 or #121212)

**Content Area**:
- Dynamic content based on selected page
- Full-width layout
- Dark background (#181818)

**Player Bar** (Bottom):
- Fixed at bottom of window
- Spans full width
- Height: 90-100px
- Dark background (#181818)

**Layout Structure**:
```
┌─────────────────────────────────────────┐
│  ┌──────┐  ┌──────────────────────┐    │
│  │      │  │                      │    │
│  │ Side │  │   Content Area       │    │
│  │ bar  │  │   (Stacked Pages)    │    │
│  │      │  │                      │    │
│  │      │  │                      │    │
│  └──────┘  └──────────────────────┘    │
│  ┌──────────────────────────────────┐  │
│  │       Player Bar (Bottom)        │  │
│  └──────────────────────────────────┘  │
└─────────────────────────────────────────┘
```

---

### 3. Search Page

**Purpose**: Search and browse music catalog

**UI Components**:

**Search Bar**:
- Prominent search input at top
- Icon (magnifying glass) inside input
- Placeholder: "What do you want to listen to?"
- Rounded corners, subtle border

**Search Results Section**:
- Song results in list/grid format
- Each result shows:
  - Album artwork thumbnail (64x64px)
  - Song title (bold)
  - Artist name (gray text)
  - Duration
  - Play button (on hover)
  - More options menu (three dots)

**Categories/Browse** (When no search):
- Genre cards in grid layout
- Colorful category tiles
- "Browse All" section

**Layout**:
```
┌─────────────────────────────────────────┐
│  [🔍 Search input field...............] │
│                                         │
│  Results / Browse Categories:          │
│  ┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐  │
│  │ Rock │ │ Pop  │ │Jazz  │ │etc.  │  │
│  └──────┘ └──────┘ └──────┘ └──────┘  │
│                                         │
│  Songs:                                 │
│  [🎵] Song Title - Artist      3:45 [▶] │
│  [🎵] Song Title - Artist      4:12 [▶] │
│  [🎵] Song Title - Artist      2:58 [▶] │
└─────────────────────────────────────────┘
```

---

### 4. Liked Songs Page

**Purpose**: Display user's liked/favorited songs

**UI Components**:

**Header Section**:
- Large gradient background
- Heart icon (filled, gradient)
- "Liked Songs" title (large, bold)
- Song count (e.g., "142 songs")
- Play button (large, circular, green)
- Shuffle button

**Song List**:
- Table/list view with columns:
  - # (index)
  - Title (with album art thumbnail)
  - Artist
  - Album
  - Date added
  - Duration
  - Like button (heart, filled)
- Row hover effects
- Play on row click
- Right-click context menu

**Actions**:
- Play all button
- Shuffle button
- Sort options (title, artist, date added, duration)
- Search within liked songs

**Layout**:
```
┌─────────────────────────────────────────┐
│  ┌───────────────────────────────────┐  │
│  │  ❤️  LIKED SONGS                  │  │
│  │  142 songs                        │  │
│  │  [▶ Play]  [🔀 Shuffle]          │  │
│  └───────────────────────────────────┘  │
│                                         │
│  #  Title          Artist    Album  ⏱️  │
│  ─────────────────────────────────────  │
│  1  [🎵] Song 1    Artist A  Alb A 3:45 │
│  2  [🎵] Song 2    Artist B  Alb B 4:12 │
│  3  [🎵] Song 3    Artist C  Alb C 2:58 │
└─────────────────────────────────────────┘
```

---

### 5. Downloaded Songs Page

**Purpose**: Show locally downloaded music files

**UI Components**:

**Header**:
- Download icon
- "Downloaded Songs" title
- Storage used indicator
- Filter/sort options

**Song List** (Similar to Liked Songs):
- List view with columns:
  - # (index)
  - Title (with album art)
  - Artist
  - Album
  - File size
  - Download date
  - Duration
- Download status indicator (100% for completed)
- Delete option (remove from device)

**Filter Options**:
- All downloaded
- Recently downloaded
- By artist
- By album

**Storage Info Bar**:
- "X MB used of Y MB available"
- Storage management link

**Layout**:
```
┌─────────────────────────────────────────┐
│  📥 DOWNLOADED SONGS                    │
│  Storage: 1.2 GB used                   │
│  [Sort: Recent ▼] [Filter ▼]           │
│                                         │
│  #  Title          Artist    Size   ⏱️  │
│  ─────────────────────────────────────  │
│  1  [🎵] Song 1    Artist A  5.2MB 3:45 │
│  2  [🎵] Song 2    Artist B  6.1MB 4:12 │
│  3  [🎵] Song 3    Artist C  4.8MB 2:58 │
└─────────────────────────────────────────┘
```

---

### 6. Player Widget (Bottom Bar)

**Purpose**: Playback controls and currently playing track info

**UI Components**:

**Left Section - Now Playing**:
- Album artwork (64x64px)
- Song title
- Artist name
- Heart button (like/unlike)

**Center Section - Playback Controls**:
- Shuffle button
- Previous track button
- Play/Pause button (large, circular)
- Next track button
- Repeat button
- Progress bar with time (current / total)

**Right Section - Volume & Extras**:
- Queue button
- Volume slider
- Full-screen button
- More options menu

**Layout**:
```
┌─────────────────────────────────────────────────────────┐
│ [🎵] Song      | ⏮️ ⏯️ ⏭️  [▓▓▓▓▓░░░] 2:30/4:15 | 🔊▓▓░░ │
│     Artist     | 🔀         🔁                  |       │
└─────────────────────────────────────────────────────────┘
```

---

## Technical Architecture

### File Structure

```
client/src/
├── main.cpp
├── ui/
│   ├── mainwindow.h/cpp         # Main application window
│   ├── loginwindow.h/cpp        # Login screen
│   ├── widgets/
│   │   ├── playerwidget.h/cpp   # Bottom player bar
│   │   ├── songlistitem.h/cpp   # Reusable song list item
│   │   ├── navbutton.h/cpp      # Sidebar navigation button
│   │   └── searchbar.h/cpp      # Search input component
│   └── pages/
│       ├── likedsongspage.h/cpp # Liked songs screen
│       ├── searchpage.h/cpp     # Search screen
│       └── downloadedpage.h/cpp # Downloaded songs screen
├── models/
│   └── song.h/cpp               # Song data model
└── resources/
    └── styles.qss               # Application stylesheet
```

### Qt Widgets Used

- **QMainWindow**: Main application window
- **QStackedWidget**: Page navigation
- **QListWidget/QTableWidget**: Song lists
- **QPushButton**: Buttons and navigation
- **QLineEdit**: Search input, login fields
- **QSlider**: Volume, progress bar
- **QLabel**: Text labels, album art
- **QVBoxLayout/QHBoxLayout**: Layouts

### Design Patterns

1. **MVC-like Structure**:
   - Models: Song data structures
   - Views: UI components (pages, widgets)
   - Controllers: Event handlers, navigation logic

2. **Signal-Slot Connections**:
   - Navigation buttons → Page switching
   - Player controls → Playback signals
   - Song items → Play/like actions

3. **Reusable Components**:
   - SongListItem widget for all song displays
   - NavButton for sidebar navigation
   - Consistent styling via QSS

---

## Styling and Theme

### Color Palette (Spotify-inspired)

- **Background Dark**: `#000000`, `#121212`
- **Surface**: `#181818`, `#282828`
- **Primary Green**: `#1DB954`
- **Text Primary**: `#FFFFFF`
- **Text Secondary**: `#B3B3B3`
- **Hover**: `#282828`
- **Active**: `#1DB954`

### Typography

- **Font Family**: Segoe UI, Helvetica, Arial
- **Headings**: 24-32px, Bold
- **Body**: 14-16px, Regular
- **Small**: 12px, Regular

### UI Design Principles

1. **Dark Theme**: Reduce eye strain, modern aesthetic
2. **High Contrast**: White text on dark backgrounds
3. **Green Accents**: Use Spotify green for primary actions
4. **Smooth Animations**: Hover effects, transitions
5. **Rounded Corners**: Modern, friendly appearance
6. **Consistent Spacing**: 8px grid system

### Stylesheet Example Structure

```css
/* Main Window */
QMainWindow {
    background-color: #121212;
}

/* Sidebar */
#sidebar {
    background-color: #000000;
    max-width: 240px;
}

/* Navigation Buttons */
QPushButton#navButton {
    background: transparent;
    color: #B3B3B3;
    border: none;
    padding: 8px 16px;
    text-align: left;
}

QPushButton#navButton:hover {
    color: #FFFFFF;
    background-color: #282828;
}

QPushButton#navButton:pressed {
    color: #1DB954;
}

/* Player Widget */
#playerWidget {
    background-color: #181818;
    border-top: 1px solid #282828;
}

/* Buttons */
QPushButton#playButton {
    background-color: #1DB954;
    border-radius: 20px;
    color: #FFFFFF;
}
```

---

## Data Models

### Song Model (Mock Data for Iteration 1)

```cpp
struct Song {
    QString id;
    QString title;
    QString artist;
    QString album;
    QString albumArtUrl;
    int duration;        // in seconds
    bool isLiked;
    bool isDownloaded;
    QString filePath;    // for downloaded songs
    qint64 fileSize;     // in bytes
    QDateTime addedDate;
};
```

### Mock Data

For first iteration, create hardcoded song data:
- 20-30 sample songs
- Mix of liked and non-liked
- Some marked as downloaded
- Varied artists, albums, durations

---

## Navigation Flow

```
[Login Window]
      ↓
  (Login Click)
      ↓
[Main Window] → Default: Search Page
      ↓
  ┌───┴────┬──────────┐
  ↓        ↓          ↓
Search  Liked    Downloaded
Page    Songs      Songs
  ↓        ↓          ↓
  └────────┴──────────┘
         ↓
   [Player Widget]
   (Always visible)
```

---

## User Interactions (Iteration 1)

### Login Page
- ✅ Click "Login" → Show Main Window
- ❌ No validation (future iteration)
- ❌ No API call (future iteration)

### Navigation
- ✅ Click sidebar button → Switch page
- ✅ Highlight active page button
- ✅ Smooth page transitions

### Song Lists
- ✅ Display mock song data
- ✅ Hover effects on rows
- ✅ Click song → Show in player widget
- ❌ No actual playback (future iteration)

### Player Widget
- ✅ Display current song info
- ✅ Show progress bar (mock progress)
- ✅ Play/Pause button (visual only)
- ❌ No actual audio playback (future iteration)

### Search
- ✅ Search input field
- ✅ Display all songs when empty
- ✅ Filter displayed songs by search text
- ❌ No backend search (future iteration)

---

## Build and Run Instructions

### Prerequisites
- Qt 6.x installed
- CMake 3.16+
- C++ compiler

### Build Commands

```bash
cd client
mkdir build
cd build

# Configure
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x

# Build
cmake --build . --config Release

# Run
./EKNMusic  # or EKNMusic.exe on Windows
```

### Testing Checklist

- [ ] Application launches to login window
- [ ] Login button navigates to main window
- [ ] All sidebar navigation buttons work
- [ ] Each page displays correctly
- [ ] Search filters song list
- [ ] Song items display properly
- [ ] Player widget shows at bottom
- [ ] Clicking song updates player widget
- [ ] Application closes cleanly
- [ ] Dark theme applied consistently

---

## Future Iterations (Not in Scope)

- Backend API integration
- Real authentication
- Actual audio playback
- File upload/download
- User preferences/settings
- Playlist creation
- Album views
- Artist pages
- Queue management
- Keyboard shortcuts

---

## Development Notes

### Component Priorities

1. **High Priority** (Core UI):
   - MainWindow structure
   - Login window
   - Sidebar navigation
   - Page switching
   - Basic styling

2. **Medium Priority**:
   - Song list displays
   - Player widget
   - Search functionality

3. **Low Priority** (Polish):
   - Hover animations
   - Smooth transitions
   - Advanced styling

### Testing Strategy

- Manual UI testing
- Test on Windows, macOS, Linux
- Verify responsiveness at different window sizes
- Check dark theme consistency
- Validate navigation flow

### Known Limitations (Iteration 1)

- No audio playback
- No backend connectivity
- Mock/hardcoded data only
- No persistence (songs reset on restart)
- No user accounts
- No file operations
- Limited error handling
