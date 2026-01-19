# Debugging Drag & Drop Order Saving

## How to Debug

### 1. Run Application from Terminal
```bash
cd D:\programs\EKNMusic\client\build\Desktop_Qt_6_10_1_MinGW_64_bit-Debug
./EKNMusic.exe
```

### 2. Watch Console Output

When you drag and drop songs, you should see:

```
Song order changed - saving new order
  Position 0: C:/path/to/song1.mp3
  Position 1: C:/path/to/song2.mp3
  Position 2: C:/path/to/song3.mp3
PlaylistData::updateOrder called with 3 tracks
Current m_tracks size: 3
  Updated order for: C:/path/to/song1.mp3 -> index 0
  Updated order for: C:/path/to/song2.mp3 -> index 1
  Updated order for: C:/path/to/song3.mp3 -> index 2
After update, m_tracks contains 3 tracks
PlaylistData::saveToFile - Saving 3 tracks to: C:/path/metadata/playlist.json
  Track 0: Song Title 1 orderIndex: 0
  Track 1: Song Title 2 orderIndex: 1
  Track 2: Song Title 3 orderIndex: 2
Playlist data saved successfully! 1234 bytes written
Track order updated and saved
New order saved to playlist.json with 3 tracks
```

### 3. Check JSON File

Open the file:
```
%AppData%/EKNMusic/metadata/playlist.json
```

Should contain:
```json
{
  "version": 1,
  "tracks": [
    {
      "filePath": "C:/path/to/song1.mp3",
      "title": "Song Title 1",
      "artist": "Artist 1",
      "album": "Album 1",
      "duration": 180000,
      "orderIndex": 0
    },
    {
      "filePath": "C:/path/to/song2.mp3",
      "title": "Song Title 2",
      "artist": "Artist 2",
      "album": "Album 2",
      "duration": 240000,
      "orderIndex": 1
    }
  ]
}
```

## Common Issues

### Issue 1: "WARNING: Track not found in m_tracks"

**Symptom:**
```
PlaylistData::updateOrder called with 3 tracks
Current m_tracks size: 0
  WARNING: Track not found in m_tracks: C:/path/song.mp3
```

**Cause:** Tracks haven't been added to PlaylistData yet

**Solution:** Ensure tracks are loaded before drag & drop:
1. Check that `loadPlaylistData()` is called on startup
2. Check that `updateTrackMetadata()` is called for each track in `getDownloadedTracks()`

### Issue 2: orderIndex not updating

**Symptom:** All tracks have same orderIndex in JSON

**Cause:** `updateOrder()` not being called or not working

**Solution:** Check console output for "PlaylistData::updateOrder called"

### Issue 3: File not being written

**Symptom:** No "Playlist data saved successfully" message

**Cause:** Permission issue or file path problem

**Solution:**
1. Check write permissions for `%AppData%/EKNMusic/metadata/`
2. Verify folder exists (should be created automatically)
3. Check console for "Failed to open file for writing"

### Issue 4: JSON file exists but order doesn't persist

**Symptom:** File is written but order resets on restart

**Cause:** Order not being loaded on startup

**Solution:** Check that `loadPlaylistData()` is called in `MusicStorageService` constructor

## Testing Steps

1. **Add some music files** to `%AppData%/EKNMusic/songs/`
2. **Start application** - watch console output
3. **Go to Downloads page**
4. **Drag a song** from position 0 to position 2
5. **Watch console** - should see debug output
6. **Check JSON file** - should have updated orderIndex values
7. **Restart application**
8. **Check order** - should match the saved order

## Debug Checklist

- [ ] Console shows "Song order changed - saving new order"
- [ ] Console shows file paths in correct order
- [ ] Console shows "PlaylistData::updateOrder called"
- [ ] Console shows "Updated order for:" for each track
- [ ] Console shows "Playlist data saved successfully!"
- [ ] JSON file exists in metadata folder
- [ ] JSON file has correct orderIndex values
- [ ] Order persists after restart

## If Still Not Working

Check these critical points:

1. **Item Data Storage** (`downloadedpage.cpp:169`):
   ```cpp
   item->setData(Qt::UserRole, track.filePath());
   ```

2. **Order Collection** (`downloadedpage.cpp:289-291`):
   ```cpp
   QString filePath = item->data(Qt::UserRole).toString();
   orderedFilePaths.append(filePath);
   ```

3. **Order Update** (`playlistdata.cpp:68-71`):
   ```cpp
   if (m_tracks.contains(path)) {
       m_tracks[path].orderIndex = i;
   }
   ```

4. **Save to File** (`playlistdata.cpp:132`):
   ```cpp
   file.write(doc.toJson(QJsonDocument::Indented));
   ```

## Manual Fix

If order is completely broken, you can manually edit the JSON file:

1. Close application
2. Open `%AppData%/EKNMusic/metadata/playlist.json`
3. Manually set `orderIndex` values (0, 1, 2, 3...)
4. Save file
5. Restart application

Example:
```json
{
  "version": 1,
  "tracks": [
    {
      "filePath": "C:/path/song1.mp3",
      "orderIndex": 0
    },
    {
      "filePath": "C:/path/song2.mp3",
      "orderIndex": 1
    }
  ]
}
```
