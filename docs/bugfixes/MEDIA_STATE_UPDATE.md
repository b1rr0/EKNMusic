# Media State Management Update

## Зміни в системі відтворення медіа

### 🎯 Головна мета
Забезпечити, щоб **радіо і музика не грали одночасно**. Коли вмикається один джерело - автоматично вимикається інший.

---

## ✅ Що було зроблено

### 1. **MediaStateManager** - Глобальний менеджер стану медіа

Створено новий singleton сервіс `MediaStateManager` який контролює:
- Який джерело медіа активне (радіо або музика)
- Автоматично зупиняє одне джерело коли запускається інше
- Відправляє сигнали про зміну активного джерела

**Файли:**
- `client/src/services/mediastatemanager.h`
- `client/src/services/mediastatemanager.cpp`

**Enum MediaSource:**
```cpp
enum class MediaSource {
    None,           // Нічого не грає
    MusicPlayer,    // Грає музика
    RadioStream     // Грає радіо
};
```

**Ключові методи:**
- `requestPlayback(MediaSource)` - Запит на відтворення (зупинить інше джерело)
- `notifyStopped(MediaSource)` - Повідомлення про зупинку
- `isActive(MediaSource)` - Перевірка чи активне джерело

**Сигнали:**
- `stopMusicPlayer()` - Зупинити музику (коли запускається радіо)
- `stopRadio()` - Зупинити радіо (коли запускається музика)
- `activeSourceChanged(MediaSource)` - Змінилось активне джерело

---

### 2. **RadioService** - Інтеграція з MediaStateManager

**Зміни:**
```cpp
void RadioService::playRadio() {
    // Request playback - automatically stops music if playing
    MediaStateManager::instance()->requestPlayback(
        MediaStateManager::MediaSource::RadioStream
    );
    // ... start streaming
}

void RadioService::stopRadio() {
    // ... stop streaming
    // Notify that radio stopped
    MediaStateManager::instance()->notifyStopped(
        MediaStateManager::MediaSource::RadioStream
    );
}
```

**Підключення сигналу:**
```cpp
// Listen for global stop command
connect(MediaStateManager::instance(), &MediaStateManager::stopRadio,
        this, &RadioService::stopRadio);
```

---

### 3. **PlayerService** - Інтеграція з MediaStateManager

**Зміни:**
```cpp
void PlayerService::play() {
    if (m_currentTrack.isValid()) {
        // Request playback - automatically stops radio if playing
        MediaStateManager::instance()->requestPlayback(
            MediaStateManager::MediaSource::MusicPlayer
        );
        m_mediaPlayer->play();
    }
}

void PlayerService::stop() {
    m_mediaPlayer->stop();
    // Notify that music stopped
    MediaStateManager::instance()->notifyStopped(
        MediaStateManager::MediaSource::MusicPlayer
    );
}
```

**Підключення сигналу:**
```cpp
// Listen for global stop command
connect(MediaStateManager::instance(), &MediaStateManager::stopMusicPlayer,
        this, &PlayerService::stop);
```

---

### 4. **RadioPage UI** - Покращення інтерфейсу

#### 🔒 Прогрес-бар тепер тільки для відображення (не клікабельний)

**Зміни:**
```cpp
progressBar->setEnabled(false);        // Вимкнути взаємодію
progressBar->setFocusPolicy(Qt::NoFocus); // Не фокусується
progressBar->setStyleSheet(
    // ... existing styles ...
    "QSlider:disabled {"
    "   opacity: 1.0;"  // Залишити повну видимість
    "}"
);
```

#### 🎨 Play/Pause кнопка тепер використовує іконки замість тексту

**Було:** `▶` / `⏸` (текстові символи)
**Стало:** `playButton.png` / `pauseButton.jpg` (іконки з ресурсів)

**Зміни:**
```cpp
// Initialization
playPauseBtn->setIcon(QIcon(":/images/src/resources/images/playButton.png"));
playPauseBtn->setIconSize(QSize(50, 50));

// State change
void RadioPage::onPlaybackStateChanged(bool isPlaying) {
    if (isPlaying) {
        playPauseBtn->setIcon(QIcon(":/images/src/resources/images/pauseButton.jpg"));
    } else {
        playPauseBtn->setIcon(QIcon(":/images/src/resources/images/playButton.png"));
    }
}
```

**Оновлено resources.qrc:**
- Додано `pauseButton.jpg` до ресурсів

---

## 🔄 Як це працює

### Сценарій 1: Користувач слухає музику, потім вмикає радіо
```
1. Музика грає (MediaSource::MusicPlayer активна)
2. Користувач натискає Play на радіо
3. RadioService викликає MediaStateManager::requestPlayback(RadioStream)
4. MediaStateManager відправляє сигнал stopMusicPlayer()
5. PlayerService отримує сигнал і зупиняє музику
6. Радіо починає грати
```

### Сценарій 2: Користувач слухає радіо, потім вмикає музику
```
1. Радіо грає (MediaSource::RadioStream активна)
2. Користувач натискає Play на пісні
3. PlayerService викликає MediaStateManager::requestPlayback(MusicPlayer)
4. MediaStateManager відправляє сигнал stopRadio()
5. RadioService отримує сигнал і зупиняє радіо
6. Музика починає грати
```

### Сценарій 3: Користувач вручну зупиняє відтворення
```
1. Музика/радіо грає
2. Користувач натискає Stop/Pause
3. Service викликає MediaStateManager::notifyStopped()
4. MediaStateManager встановлює activeSource = None
```

---

## 📁 Змінені файли

### Нові файли:
- ✅ `client/src/services/mediastatemanager.h`
- ✅ `client/src/services/mediastatemanager.cpp`

### Оновлені файли:
- ✅ `client/src/services/radioservice.h`
- ✅ `client/src/services/radioservice.cpp`
- ✅ `client/src/services/playerservice.cpp`
- ✅ `client/src/ui/radiopage.cpp`
- ✅ `client/resources.qrc` (додано pauseButton.jpg)
- ✅ `client/CMakeLists.txt` (додано mediastatemanager)

---

## 🧪 Тестування

### Перевірте ці сценарії:

1. **Музика → Радіо**
   - Запустіть музику
   - Запустіть радіо
   - ✅ Музика має автоматично зупинитись

2. **Радіо → Музика**
   - Запустіть радіо
   - Запустіть музику
   - ✅ Радіо має автоматично зупинитись

3. **UI Радіо**
   - Натисніть на прогрес-бар радіо
   - ✅ Нічого не має статись (не інтерактивний)
   - Натисніть Play
   - ✅ Іконка має змінитись на Pause
   - Натисніть Pause
   - ✅ Іконка має змінитись на Play

4. **Консольні логи**
   - Дивіться на Debug консоль
   - ✅ Мають бути повідомлення про зміну MediaSource

---

## 🐛 Debug інформація

В консолі ви побачите:
```
MediaStateManager initialized
Media playback requested for source: 2
Stopping music player
Active media source changed to: 2
Starting radio stream: https://radio.eknm.in/listen/eknm_intercom/radio.mp3
```

---

## 🎨 UI Покращення

### Прогрес-бар радіо:
- ❌ **Було:** Можна клікати і змінювати позицію
- ✅ **Стало:** Тільки відображення, не можна взаємодіяти

### Play/Pause кнопка:
- ❌ **Було:** Текстові символи ▶/⏸
- ✅ **Стало:** Красиві іконки з файлів playButton.png / pauseButton.jpg

---

## 🔮 Майбутні покращення

- [ ] Додати fade-out/fade-in при переключенні джерел
- [ ] Додати візуальну індикацію активного джерела
- [ ] Додати налаштування пріоритету джерел
- [ ] Додати історію відтворення

---

## ✨ Результат

Тепер у вашому додатку **неможливо одночасно грати музику і радіо**! 🎵📻

Система автоматично керує станом відтворення і забезпечує плавний перехід між джерелами.
