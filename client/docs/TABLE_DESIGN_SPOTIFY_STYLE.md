# Spotify-Style Table Design for Downloads Page

## Overview
Redesign the Downloads page song list to display as a table-like layout similar to Spotify, with columns for different metadata.

## Design Requirements

### Must Have Features:
1. ✅ **Play Button Column** - Shows play/playing state
2. ✅ **Album Art Column** - Song cover image
3. ✅ **Title & Artist Column** - Song name with artist below
4. ✅ **Date Added Column** - When file was added
5. ✅ **Duration Column** - Song length (mm:ss)
6. ✅ **File Size Column** - Size in MB
7. ✅ **No Spacing** - Remove gaps between rows (currently 12px)

## Visual Layout

```
┌────────────────────────────────────────────────────────────────────────────────────┐
│ [#] [▶] [IMG] Title                    Date Added    Duration    Size    [Delete] │
│                Artist                                                               │
├────────────────────────────────────────────────────────────────────────────────────┤
│  1  [▶] [🎵] Song Name Here             Jan 19, 2026   3:45      8.5 MB    [✕]    │
│          Artist Name                                                                │
├────────────────────────────────────────────────────────────────────────────────────┤
│  2  [♪] [🎵] Currently Playing Song     Jan 18, 2026   4:12      9.2 MB    [✕]    │
│          Another Artist                                                             │
└────────────────────────────────────────────────────────────────────────────────────┤
```

## Column Specifications

### 1. Index Column (40px)
- **Content**: Row number (1, 2, 3...)
- **Alignment**: Center
- **Font**: Regular, gray color
- **Purpose**: Visual reference for position

### 2. Play Button Column (60px)
- **Default State**: `▶` Play button
- **Playing State**: `♪♫♪` Animated music icon
- **Hover**: Highlight in blue
- **Click**: Start/pause playback
- **Size**: 32x32px button

### 3. Album Art Column (60px)
- **Content**: Song cover image (QPixmap)
- **Size**: 48x48px image
- **Fallback**: Musical note icon if no art
- **Border**: 1px subtle border
- **Border Radius**: 4px

### 4. Title & Artist Column (Flexible, ~40%)
- **Line 1**: Song title (bold, 14px)
- **Line 2**: Artist name (regular, 12px, gray)
- **Alignment**: Left
- **Overflow**: Ellipsis (...)
- **Hover**: Slightly brighter

### 5. Date Added Column (120px)
- **Format**: "Jan 19, 2026" or "Today" / "Yesterday"
- **Font**: Regular, 12px
- **Alignment**: Left
- **Color**: Gray (#888)

### 6. Duration Column (80px)
- **Format**: "mm:ss" (e.g., "3:45")
- **Font**: Regular, 12px
- **Alignment**: Center
- **Color**: Gray (#888)

### 7. File Size Column (100px)
- **Format**: "X.X MB" or "XX.X MB"
- **Font**: Regular, 12px
- **Alignment**: Right
- **Color**: Gray (#888)

### 8. Delete Button Column (50px)
- **Content**: ✕ symbol
- **Hover**: Red background
- **Click**: Delete file + remove from list
- **Size**: 24x24px

## Row Styling

### Normal State
```css
background: #1a1a1a;
border: none;
border-bottom: 1px solid #2a2a2a;
height: 64px;
padding: 8px 16px;
```

### Hover State
```css
background: #2a2a2a;
cursor: pointer;
```

### Playing State
```css
background: #1a3a1a; /* Slight green tint */
border-left: 3px solid #1db954; /* Spotify green accent */
```

### Selected State
```css
background: #2a2a2a;
border-left: 3px solid #1db954;
```

## Spacing Changes

### Remove Current Spacing
```cpp
// REMOVE THIS:
songListWidget->setSpacing(12);

// ADD THIS:
songListWidget->setSpacing(0); // No gaps between items
```

## Play Button Animation

### State Machine
```
Normal (▶) → Clicked → Playing (♪)
Playing (♪) → Clicked → Paused (▶)
Playing (♪) → Other Song Clicked → Normal (▶)
```

### Animation for Playing State
```cpp
// Animate between these characters every 500ms:
Frame 1: ♪
Frame 2: ♫
Frame 3: ♪
Frame 4: ♫
```

## Implementation Details

### QListWidget Custom Item
Each item will be a custom widget with QHBoxLayout:

```cpp
QWidget *itemWidget = new QWidget();
QHBoxLayout *layout = new QHBoxLayout(itemWidget);
layout->setContentsMargins(16, 8, 16, 8);
layout->setSpacing(12);

// Add columns:
layout->addWidget(indexLabel);        // #
layout->addWidget(playButton);        // ▶
layout->addWidget(albumArtLabel);     // [IMG]
layout->addWidget(titleArtistWidget); // Title + Artist
layout->addWidget(dateLabel);         // Date Added
layout->addWidget(durationLabel);     // mm:ss
layout->addWidget(sizeLabel);         // X.X MB
layout->addWidget(deleteButton);      // ✕
```

### Title & Artist Widget (Vertical Stack)
```cpp
QWidget *titleArtistWidget = new QWidget();
QVBoxLayout *vLayout = new QVBoxLayout(titleArtistWidget);
vLayout->setSpacing(2);
vLayout->setContentsMargins(0, 0, 0, 0);

QLabel *titleLabel = new QLabel(track.title());
titleLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #fff;");

QLabel *artistLabel = new QLabel(track.artist());
artistLabel->setStyleSheet("font-size: 12px; color: #888;");

vLayout->addWidget(titleLabel);
vLayout->addWidget(artistLabel);
```

### Date Added Logic
```cpp
QString formatDateAdded(const QDateTime &dateTime) {
    QDate today = QDate::currentDate();
    QDate addedDate = dateTime.date();

    if (addedDate == today) {
        return "Today";
    } else if (addedDate == today.addDays(-1)) {
        return "Yesterday";
    } else {
        return addedDate.toString("MMM d, yyyy");
    }
}
```

### File Size Calculation
```cpp
QString formatFileSize(qint64 bytes) {
    double mb = bytes / (1024.0 * 1024.0);
    if (mb < 10.0) {
        return QString::number(mb, 'f', 1) + " MB";
    } else {
        return QString::number(mb, 'f', 0) + " MB";
    }
}
```

## Data Requirements

### Track Model Updates
Need to add these fields to `Track` class:
```cpp
QDateTime dateAdded() const;
void setDateAdded(const QDateTime &dt);

qint64 fileSize() const;
void setFileSize(qint64 size);
```

### Metadata Extraction
Update `MetadataExtractor` to extract:
- File creation/modification date
- File size from QFileInfo

### PlaylistData JSON
Update JSON schema to include:
```json
{
  "filePath": "...",
  "title": "...",
  "artist": "...",
  "album": "...",
  "duration": 245000,
  "orderIndex": 0,
  "dateAdded": "2026-01-19T15:30:00",
  "fileSize": 8912345
}
```

## Color Palette

```css
/* Spotify-inspired dark theme */
Background:       #121212
Row Background:   #1a1a1a
Row Hover:        #2a2a2a
Row Border:       #2a2a2a
Text Primary:     #ffffff
Text Secondary:   #888888
Accent Green:     #1db954
Delete Red:       #ff4444
Playing Tint:     #1a3a1a
```

## Drag & Drop Behavior

### Visual Feedback
- **Dragging**: Semi-transparent row follows cursor
- **Drop Target**: Blue line between rows
- **Drop Zone**: Highlight target position

### Drag Handle
Option 1: Keep ⋮⋮ icon in front of index column
Option 2: Make entire row draggable (recommended)

```cpp
songListWidget->setDragEnabled(true);
songListWidget->setDragDropMode(QAbstractItemView::InternalMove);
songListWidget->setDefaultDropAction(Qt::MoveAction);
```

## Performance Optimizations

### Lazy Loading
- Only create visible item widgets
- Use QListWidget's viewport update signals
- Cache rendered widgets

### Image Caching
- Cache album art QPixmap objects
- Reuse same image for multiple tracks with same album

### Animation Timer
- Single QTimer for all playing animations
- Only animate visible items
- Stop timer when no songs playing

## Accessibility

### Keyboard Navigation
- Up/Down arrows: Navigate rows
- Space: Play/pause selected track
- Delete: Remove selected track
- Drag with mouse only (no keyboard drag)

### Screen Reader
- Play button: "Play [Song Title]" or "Now playing [Song Title]"
- Delete button: "Delete [Song Title]"
- Each column announces its value

## Comparison: Before vs After

### Before (Current Design)
```
┌─────────────────────────────────┐
│ ⋮⋮ ♪ Song Title                 │
│      Artist Name                 │
│      Album · 3:45           [✕]  │
└─────────────────────────────────┘
  ← 12px spacing →
┌─────────────────────────────────┐
│ ⋮⋮ ♪ Another Song                │
│      Another Artist              │
│      Album 2 · 4:12         [✕]  │
└─────────────────────────────────┘
```

### After (New Table Design)
```
┌────────────────────────────────────────────────────────────┐
│ 1  [▶] [🎵] Song Title           Jan 19   3:45   8.5 MB [✕]│
│            Artist Name                                      │
├────────────────────────────────────────────────────────────┤
│ 2  [▶] [🎵] Another Song         Jan 18   4:12   9.2 MB [✕]│
│            Another Artist                                   │
└────────────────────────────────────────────────────────────┘
```

## Implementation Files to Modify

1. **src/ui/downloadedpage.h**
   - Add method: `QWidget* createSongItemWidget(const Track &track, int index)`
   - Add method: `void updatePlayingState(const QString &filePath)`
   - Add member: `QTimer *m_playingAnimationTimer`
   - Add member: `QString m_currentPlayingFile`

2. **src/ui/downloadedpage.cpp**
   - Modify: `loadDownloadedSongs()` to use custom widgets
   - Add: Animation timer for playing state
   - Update: Styling to remove spacing and use table layout

3. **src/models/track.h**
   - Add: `QDateTime m_dateAdded`
   - Add: `qint64 m_fileSize`
   - Add getters/setters

4. **src/models/playlistdata.h/cpp**
   - Update JSON serialization for new fields

5. **src/services/metadataextractor.cpp**
   - Extract file size and date from QFileInfo

## Testing Checklist

- [ ] Rows display without spacing
- [ ] Play button changes state on click
- [ ] Playing animation works smoothly
- [ ] Album art displays correctly
- [ ] Title/Artist stacked vertically
- [ ] Date shows "Today"/"Yesterday" correctly
- [ ] Duration formats as mm:ss
- [ ] File size displays in MB
- [ ] Delete button works
- [ ] Drag & drop still functions
- [ ] Hover effects work on all interactive elements
- [ ] Order persists after drag & drop

## Future Enhancements

- [ ] Sortable columns (click header to sort)
- [ ] Multi-select with Ctrl/Shift
- [ ] Context menu (right-click)
- [ ] Column resize with drag
- [ ] Show/hide column options
- [ ] Playlist time (total duration at bottom)
- [ ] Selection count indicator

---

**Design Version**: 1.0
**Created**: 2026-01-19
**Status**: Pending Implementation
