# Playlist Persistence System

## Overview

EKNMusic automatically saves your playlist order and custom metadata changes to ensure your personalized settings persist across app restarts.

## Features

### 1. **Automatic Order Saving**
When you drag and drop songs to reorder them in the Downloads page, the new order is automatically saved to a JSON file.

### 2. **Metadata Preservation**
Any custom metadata changes (title, artist, album) are saved and restored when the app starts.

### 3. **File Location**
All playlist data is stored in:
```
%AppData%/EKNMusic/metadata/playlist.json
```

## File Format

The `playlist.json` file uses a simple JSON structure:

```json
{
  "version": 1,
  "tracks": [
    {
      "filePath": "C:/Users/YourName/AppData/Roaming/EKNMusic/songs/song1.mp3",
      "title": "Song Title",
      "artist": "Artist Name",
      "album": "Album Name",
      "duration": 245000,
      "orderIndex": 0
    },
    {
      "filePath": "C:/Users/YourName/AppData/Roaming/EKNMusic/songs/song2.mp3",
      "title": "Another Song",
      "artist": "Another Artist",
      "album": "Another Album",
      "duration": 180000,
      "orderIndex": 1
    }
  ]
}
```

### Field Descriptions

- **version**: Format version number (currently 1)
- **tracks**: Array of track information objects
  - **filePath**: Absolute path to the audio file
  - **title**: Song title
  - **artist**: Artist name
  - **album**: Album name
  - **duration**: Track duration in milliseconds
  - **orderIndex**: Position in the playlist (0-based)

## How It Works

### On App Startup
1. App loads `playlist.json` from the data directory
2. When loading songs, the saved order is applied
3. Saved metadata overrides extracted metadata from files
4. New files not in the JSON are added at the end

### When You Reorder Songs
1. You drag and drop a song to a new position
2. The app detects the order change
3. All track positions are updated in memory
4. The `playlist.json` file is immediately updated

### When You Delete a Song
1. The physical file is removed from disk
2. The track is removed from `playlist.json`
3. Other tracks' order indices remain intact

### When You Refresh the List
1. The folder is rescanned for all music files
2. Tracks are loaded in the saved order from `playlist.json`
3. New files are added at the end with new order indices
4. Missing files are automatically removed from the JSON

## Implementation Details

### Classes Involved

#### `PlaylistData`
- Located in: `src/models/playlistdata.h/cpp`
- Manages the in-memory playlist data
- Handles JSON serialization/deserialization
- Methods:
  - `setTrackData()` - Add or update track
  - `getAllTracksOrdered()` - Get tracks sorted by order
  - `updateOrder()` - Update positions of multiple tracks
  - `saveToFile()` / `loadFromFile()` - Persistence

#### `MusicStorageService`
- Located in: `src/services/musicstorageservice.h/cpp`
- Singleton service managing music storage
- New methods:
  - `savePlaylistData()` - Save to JSON file
  - `loadPlaylistData()` - Load from JSON file
  - `updateTrackOrder()` - Update and save order
  - `updateTrackMetadata()` - Update and save metadata
  - `playlistDataFilePath()` - Get JSON file path

#### `DownloadedSongsPage`
- Located in: `src/ui/downloadedpage.h/cpp`
- Calls `updateTrackOrder()` when drag & drop completes
- Automatically triggers save on order change

## Data Flow

```
User Drags Song
    ↓
QListWidget detects rowsMoved signal
    ↓
DownloadedSongsPage::onSongOrderChanged()
    ↓
Collect new order of file paths
    ↓
MusicStorageService::updateTrackOrder()
    ↓
PlaylistData::updateOrder()
    ↓
PlaylistData::saveToFile()
    ↓
Write playlist.json to disk
```

## Benefits

1. **Persistent Playlists**: Your custom order survives app restarts
2. **Metadata Control**: Edit and preserve custom metadata
3. **Fast Loading**: Saved data loads instantly without re-scanning
4. **Automatic Sync**: Changes are saved immediately
5. **Fault Tolerant**: Missing files are gracefully handled

## Error Handling

- If `playlist.json` is missing, app creates a new one
- If JSON is corrupted, app starts fresh with scanned files
- If a saved file is missing, it's skipped during load
- All operations are logged for debugging

## Future Enhancements

Potential improvements for future versions:

- Playlist versioning and backups
- Multiple playlist support
- Import/export playlists
- Cloud sync support
- Undo/redo for order changes
- Playlist history

## Technical Notes

### Performance
- JSON file is only written when changes occur
- File I/O is performed synchronously (small file size)
- Order lookup uses O(n log n) sorting

### Thread Safety
- All operations run on the main thread
- No concurrent access issues

### File Size
- Typical playlist.json: ~1-10 KB
- Scales linearly with track count
- No performance impact for normal library sizes (< 10,000 tracks)

## Troubleshooting

### Playlist order not saving
1. Check if app has write permissions to AppData folder
2. Look for errors in debug console
3. Delete `playlist.json` and let app recreate it

### Tracks appear in wrong order
1. Click the refresh button (⟳) to rescan folder
2. Check `playlist.json` for duplicate entries
3. Delete JSON file to reset order

### JSON file corrupted
1. App will log error and start fresh
2. Delete `playlist.json` manually if needed
3. Order will reset to default (filename alphabetical)
