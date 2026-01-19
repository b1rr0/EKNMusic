# EKNMusic Documentation

Welcome to the EKNMusic documentation!

## 📚 Documentation Files

### [Playlist Persistence System](PLAYLIST_PERSISTENCE.md)
Comprehensive guide on how EKNMusic saves and loads:
- Song order (drag & drop positions)
- Custom metadata changes
- Playlist state across app restarts

**File location**: `%AppData%/EKNMusic/metadata/playlist.json`

## 🎵 Key Features

### Downloads Page Features

1. **Drag & Drop Reordering**
   - Drag songs up or down to change order
   - Order automatically saves to `playlist.json`
   - Persists across app restarts

2. **Song Deletion**
   - Click ✕ button to delete song
   - Removes file from disk
   - Updates `playlist.json` automatically

3. **Refresh Button (⟳)**
   - Rescans music folder
   - Restores deleted files if they reappear
   - Finds new files added manually
   - Maintains saved order

4. **Metadata Extraction**
   - Automatically reads ID3 tags from audio files
   - Extracts: title, artist, album, duration, album art
   - Supports: MP3, FLAC, WAV, OGG, M4A

## 🗂️ Data Storage

### Music Files
```
%AppData%/EKNMusic/
├── songs/              ← Your music files
│   ├── song1.mp3
│   ├── song2.mp3
│   └── song3.flac
└── metadata/           ← Playlist data
    └── playlist.json   ← Order & metadata
```

### Playlist JSON Structure
```json
{
  "version": 1,
  "tracks": [
    {
      "filePath": "...",
      "title": "Song Title",
      "artist": "Artist Name",
      "album": "Album Name",
      "duration": 245000,
      "orderIndex": 0
    }
  ]
}
```

## 🔧 Technical Details

### Architecture
- **Qt 6.10.1** with C++17
- **Qt Multimedia** for metadata extraction
- **JSON persistence** for playlist state
- **Singleton pattern** for service management

### Key Classes

| Class | Purpose |
|-------|---------|
| `PlaylistData` | Manages playlist order & metadata |
| `MusicStorageService` | Singleton managing file storage & JSON |
| `MetadataExtractor` | Extracts metadata from audio files |
| `Track` | Represents a single music track |
| `DownloadedSongsPage` | UI for Downloads with drag & drop |

### Data Flow

```
User Action → UI Component → Service Layer → Data Model → JSON File
```

## 📝 Quick Start

1. **Add Songs**: Drop music files into `%AppData%/EKNMusic/songs/`
2. **Open App**: Songs load automatically in saved order
3. **Reorder**: Drag and drop songs (saves automatically)
4. **Refresh**: Click ⟳ to rescan folder
5. **Delete**: Click ✕ to remove song from disk

## 🐛 Troubleshooting

### Order not saving?
- Check write permissions to AppData folder
- Look for errors in console output
- Try deleting `playlist.json` to reset

### Songs in wrong order?
- Click refresh button (⟳)
- Check for duplicate entries in JSON
- Delete JSON to reset to default order

### App crashes on startup?
- Corrupt JSON file - delete `playlist.json`
- Check for invalid file paths in JSON
- Verify music files still exist

## 🚀 Future Features

Planned enhancements:
- [ ] Multiple playlist support
- [ ] Playlist import/export
- [ ] Cloud sync
- [ ] Playlist history & undo
- [ ] Album view grouping
- [ ] Smart playlists

## 📖 More Documentation

For detailed information, see:
- [Playlist Persistence System](PLAYLIST_PERSISTENCE.md)

---

**EKNMusic** - Simple, fast, and elegant music player
