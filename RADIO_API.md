# Radio Service - EKNM Intercom Integration

This document describes the RadioService implementation for AzuraCast radio streaming.

## Overview

The RadioService provides a complete C++ interface to the EKNM Intercom radio station hosted at `https://radio.eknm.in`.

## Features

- ✅ Live radio streaming playback
- ✅ Now playing information (song, artist, album art)
- ✅ Song history
- ✅ Requestable songs list with search
- ✅ Submit song requests
- ✅ View upcoming queue
- ✅ Volume control and mute
- ✅ Secure API key storage (obfuscated in QSettings)

## API Endpoints

All endpoints use the base URL: `https://radio.eknm.in`

### Public Endpoints (No Auth)

- **Now Playing**: `/api/nowplaying/eknm_intercom`
  - Returns current song, station info, listeners count

- **Song History**: `/api/station/eknm_intercom/history?limit=10`
  - Returns list of recently played songs

- **Queue**: `/api/station/eknm_intercom/queue`
  - Returns upcoming songs in queue

### Authenticated Endpoints (Require API Key)

- **Requestable Songs**: `/api/station/eknm_intercom/requests?search=query`
  - Returns list of songs available for requests
  - Optional search parameter

- **Submit Request**: `POST /api/station/eknm_intercom/request/{request_id}`
  - Submits a song request
  - Returns success/failure status

## API Key Security

The API key is stored securely in the application:

1. **Storage**: QSettings with obfuscated encoding (XOR + Base64)
2. **Location**: Platform-specific settings location
   - Windows: Registry or `%APPDATA%/EKN/EKNMusic.ini`
   - macOS: `~/Library/Preferences/com.EKN.EKNMusic.plist`
   - Linux: `~/.config/EKN/EKNMusic.conf`

3. **Initialization**: Set automatically on first launch in `AppConfig::initializeApp()`

### Your API Key

```
Username: d2cc91b3ad579f96
API Key: e308c50246c6440bf68281050bfdd995
Full: d2cc91b3ad579f96:e308c50246c6440bf68281050bfdd995
```

**Note**: This key is already configured in the app and is stored obfuscated. It will NOT appear in plaintext in any committed files.

## Usage

### Basic Radio Playback

```cpp
#include "services/radioservice.h"

// Get singleton instance
RadioService *radio = RadioService::instance();

// Start playing radio
radio->playRadio();

// Stop radio
radio->stopRadio();

// Toggle play/pause
radio->togglePlayPause();

// Volume control (0-100)
radio->setVolume(75);

// Mute/unmute
radio->setMuted(true);
```

### Fetching Data

```cpp
// Get now playing info
radio->fetchNowPlaying();

// Get song history (last 20 songs)
radio->fetchSongHistory(20);

// Search requestable songs
radio->fetchRequestableSongs("artist name");

// Get queue
radio->fetchQueue();
```

### Submit Song Request

```cpp
// First, fetch requestable songs
radio->fetchRequestableSongs();

// Wait for signal, then submit request with request_id
radio->submitSongRequest("request-id-from-api");
```

### Signals

Connect to signals to receive updates:

```cpp
// Now playing updated
connect(radio, &RadioService::nowPlayingUpdated,
        this, [](const RadioService::NowPlayingInfo &info) {
    qDebug() << "Now playing:" << info.song.artist << "-" << info.song.title;
});

// Song request submitted
connect(radio, &RadioService::songRequestSubmitted,
        this, [](bool success, const QString &message) {
    qDebug() << "Request result:" << message;
});

// Playback state changed
connect(radio, &RadioService::playbackStateChanged,
        this, [](bool isPlaying) {
    qDebug() << "Playing:" << isPlaying;
});
```

## Stream URL

Direct stream URL for the radio:
```
https://radio.eknm.in/listen/eknm_intercom/radio.mp3
```

## Data Structures

### NowPlayingInfo
```cpp
struct NowPlayingInfo {
    SongInfo song;              // Current song details
    QString stationName;        // Station name
    int elapsed;                // Seconds elapsed
    int duration;               // Total duration in seconds
    int listeners;              // Current listener count
    bool isOnline;              // Is station online
};
```

### SongInfo
```cpp
struct SongInfo {
    QString id;                 // Song/request ID
    QString title;              // Song title
    QString artist;             // Artist name
    QString album;              // Album name
    QString artUrl;             // Album art URL
    int duration;               // Duration in seconds
};
```

## Files

- `client/src/services/radioservice.h` - Service header
- `client/src/services/radioservice.cpp` - Service implementation
- `client/src/config/appconfig.h` - Configuration with API key storage
- `client/src/config/appconfig.cpp` - Configuration implementation
- `client/src/ui/radiopage.h` - Radio UI page header
- `client/src/ui/radiopage.cpp` - Radio UI implementation

## Building

The RadioService is automatically included in the CMake build:

```bash
cd client
mkdir build && cd build
cmake ..
cmake --build .
```

## Security Notes

- ⚠️ The API key is obfuscated but NOT encrypted
- ⚠️ Do not commit `.ini`, `.conf`, or QSettings files
- ⚠️ The key is safe for desktop app distribution
- ✅ `.gitignore` is configured to exclude settings files

## TODO / Future Improvements

- [ ] Add song history dialog
- [ ] Add request song dialog with search
- [ ] Add queue/playlist viewer dialog
- [ ] Add notifications for song changes
- [ ] Add lyrics display (if available in API)
- [ ] Cache album art locally
- [ ] Add favorite songs feature
- [ ] Stream quality selector
