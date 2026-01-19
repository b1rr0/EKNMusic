# Changelog

## [Latest] - 2026-01-19

### ✅ Fixed
- **Drag & Drop Order Saving**: Fixed issue where song order was not being saved correctly when dragging and dropping
  - Now properly reads visual order from QListWidget after drag & drop
  - Stores file paths in item data (Qt::UserRole) for accurate tracking
  - Order is immediately saved to `playlist.json` after reordering

### 🔄 Changed
- **Metadata Location**: Moved `playlist.json` from `songs/` to `metadata/` folder
  - Old location: `%AppData%/EKNMusic/songs/playlist.json`
  - New location: `%AppData%/EKNMusic/metadata/playlist.json`
  - Music files remain in `songs/` folder
  - Better separation of data and metadata

### 📂 New Folder Structure
```
%AppData%/EKNMusic/
├── songs/              ← Music files (MP3, FLAC, etc.)
│   ├── song1.mp3
│   ├── song2.flac
│   └── ...
└── metadata/           ← Application metadata (NEW)
    └── playlist.json   ← Song order & custom metadata
```

### 🎯 How Drag & Drop Now Works

1. **Before**: Drag & drop would update visual order but save wrong order to JSON
2. **After**:
   - Each QListWidgetItem stores its track's file path in Qt::UserRole
   - When drag & drop completes, reads items in visual order
   - Extracts file paths from items in correct sequence
   - Saves accurate order to JSON

### 🐛 Technical Details

#### Fixed in `DownloadedPage::onSongOrderChanged()`:
```cpp
// OLD (Wrong):
for (int i = 0; i < downloadedTracks.size(); ++i) {
    orderedFilePaths.append(downloadedTracks[i].filePath());
}

// NEW (Correct):
for (int i = 0; i < songListWidget->count(); ++i) {
    QListWidgetItem *item = songListWidget->item(i);
    QString filePath = item->data(Qt::UserRole).toString();
    orderedFilePaths.append(filePath);
}
```

#### Added in `DownloadedPage::loadDownloadedSongs()`:
```cpp
QListWidgetItem *item = new QListWidgetItem(songListWidget);
// Store file path for later retrieval
item->setData(Qt::UserRole, track.filePath());
```

### 📝 Debug Output
When dragging songs, you'll now see debug output:
```
Song order changed - saving new order
  Position 0: C:/Users/.../song1.mp3
  Position 1: C:/Users/.../song2.mp3
  Position 2: C:/Users/.../song3.mp3
New order saved to playlist.json with 3 tracks
Track order updated and saved
```

### ⚡ Performance Impact
- Minimal: Only stores one QString per item
- Instant lookup: O(1) retrieval from item data
- No additional memory overhead

### 🔄 Migration Notes
- No migration needed - new folder is created automatically
- Old `playlist.json` in `songs/` folder can be manually moved to `metadata/`
- Or let app create fresh playlist data

### 📚 Updated Documentation
- [x] PLAYLIST_PERSISTENCE.md - Updated all paths
- [x] README.md - Updated folder structure
- [x] FEATURES_SUMMARY.md - Updated paths
- [x] Main README.md - No changes needed (links still valid)

---

## Previous Updates

### [1.0.0] - Initial Release
- Playlist persistence system
- Metadata extraction from audio files
- Drag & drop reordering
- Refresh functionality
- Delete with file removal
