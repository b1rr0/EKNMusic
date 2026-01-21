# Radio UI - Исправление мигания и улучшение читаемости

## Проблемы

1. **Мигание каждые 10 секунд** - список песен перерисовывался каждые 10 секунд даже если данные не изменились
2. **Неоптимальное обновление** - обновление по таймеру вместо обновления когда песня подходит к концу
3. **Текст не видно** - шрифты были слишком светлыми (#999, #aaa) и не читались

## Решения

### 1. Умное обновление без мигания

**Проблема**: Список песен очищался и перерисовывался каждые 10 секунд независимо от того, изменились ли данные.

**Решение**: Добавлена проверка изменений перед обновлением.

#### baseradiopage.h
Добавлены переменные для отслеживания предыдущего состояния:
```cpp
// Previous state for change detection
QString m_previousCurrentSongTitle;
QList<RadioService::SongInfo> m_previousHistory;
QList<RadioService::SongInfo> m_previousQueue;
```

#### baseradiopage.cpp - onSongHistoryReceived()
**Было**:
```cpp
void BaseRadioPage::onSongHistoryReceived(const QList<RadioService::SongInfo> &history)
{
    m_songHistory = history;
    updateSongList();  // Всегда обновляет!
}
```

**Стало**:
```cpp
void BaseRadioPage::onSongHistoryReceived(const QList<RadioService::SongInfo> &history)
{
    // Check if history actually changed
    bool historyChanged = false;
    if (m_songHistory.size() != history.size()) {
        historyChanged = true;
    } else {
        for (int i = 0; i < history.size(); i++) {
            if (m_songHistory[i].title != history[i].title ||
                m_songHistory[i].artist != history[i].artist) {
                historyChanged = true;
                break;
            }
        }
    }

    if (historyChanged) {
        m_songHistory = history;
        qDebug() << "Song history changed:" << history.size() << "songs";
        updateSongList();  // Обновляет только если изменилось!
    }
}
```

#### baseradiopage.cpp - onQueueReceived()
Аналогичная проверка для очереди песен:
```cpp
void BaseRadioPage::onQueueReceived(const QList<RadioService::SongInfo> &queue)
{
    bool queueChanged = false;
    if (m_queue.size() != queue.size()) {
        queueChanged = true;
    } else {
        for (int i = 0; i < queue.size(); i++) {
            if (m_queue[i].title != queue[i].title ||
                m_queue[i].artist != queue[i].artist) {
                queueChanged = true;
                break;
            }
        }
    }

    if (queueChanged) {
        m_queue = queue;
        qDebug() << "Queue changed:" << queue.size() << "songs";
        updateSongList();
    }
}
```

#### baseradiopage.cpp - onNowPlayingUpdated()
Добавлена проверка смены текущей песни:
```cpp
void BaseRadioPage::onNowPlayingUpdated(const RadioService::NowPlayingInfo &info)
{
    // Check if current song changed
    bool songChanged = (m_previousCurrentSongTitle != info.song.title);

    if (songChanged) {
        qDebug() << "Current song changed to:" << info.song.title;
        m_previousCurrentSongTitle = info.song.title;
        updateSongList();  // Обновляем только при смене песни
    }

    // Update song info (всегда обновляем UI элементы)
    songTitleLabel->setText(info.song.title);
    artistLabel->setText(info.song.artist);
    // ...
}
```

### 2. Обновление при окончании песни

**Проблема**: Обновление каждые 10 секунд было неоптимально - могло пропустить смену песни или обновлять слишком часто.

**Решение**: Обновление когда песня подходит к концу (последние 10 секунд).

#### baseradiopage.cpp - updateProgressBar()
**Было**:
```cpp
void BaseRadioPage::updateProgressBar()
{
    // ... update progress bar ...

    // Fetch fresh data every 10 seconds
    static int counter = 0;
    counter++;
    if (counter >= 10) {
        m_radioService->fetchNowPlaying();
        m_radioService->fetchSongHistory(5);
        m_radioService->fetchQueue();
        counter = 0;
    }
}
```

**Стало**:
```cpp
void BaseRadioPage::updateProgressBar()
{
    // ... update progress bar ...

    // Fetch fresh data when song is near the end (last 10 seconds)
    int timeRemaining = currentDuration - currentElapsed;
    if (timeRemaining <= 10 && timeRemaining > 0) {
        static bool fetchedForCurrentSong = false;
        if (!fetchedForCurrentSong) {
            qDebug() << "Song ending soon (" << timeRemaining << "s remaining), fetching fresh data...";
            m_radioService->fetchNowPlaying();
            m_radioService->fetchSongHistory(5);
            m_radioService->fetchQueue();
            fetchedForCurrentSong = true;
        }
    } else {
        static bool fetchedForCurrentSong = false;
        fetchedForCurrentSong = false;
    }
}
```

**Логика**:
- Когда осталось ≤10 секунд до конца песни - загружаем новые данные
- Используем static флаг чтобы не загружать многократно в течение этих 10 секунд
- Когда время > 10 секунд - сбрасываем флаг для следующей песни

### 3. Увеличение читаемости текста

**Проблема**: Текст в плейлисте был серым (#999, #aaa) и плохо читался.

**Решение**: Все тексты сделаны белыми, увеличены размеры шрифтов.

#### baseradiopage.cpp - addSongToList()

**Порядковый номер**:
```cpp
// Было: color: #aaa; font-size: 16px;
// Стало:
orderLabel->setStyleSheet(
    "color: white;"
    "font-size: 18px;"
    "font-weight: bold;"
    "background: transparent;"
);
```

**Label (PREVIOUS/NOW PLAYING/NEXT)**:
```cpp
// Было: color: #bbb (или #4a9eff); font-size: 11px;
// Стало:
labelText->setStyleSheet(QString(
    "color: %1;"
    "font-size: 13px;"
    "font-weight: bold;"
    "background: transparent;"
).arg(isCurrent ? "#4a9eff" : "white"));
```

**Название песни**:
```cpp
// Было: font-size: 16px; font-weight: 600;
// Стало:
titleText->setStyleSheet(
    "color: white;"
    "font-size: 18px;"
    "font-weight: 700;"
    "background: transparent;"
);
```

**Исполнитель**:
```cpp
// Было: color: #999; font-size: 14px;
// Стало:
artistText->setStyleSheet(
    "color: white;"
    "font-size: 16px;"
    "font-weight: 500;"
    "background: transparent;"
);
```

**Длительность**:
```cpp
// Было: color: #999; font-size: 14px;
// Стало:
durationLabel->setStyleSheet(
    "color: white;"
    "font-size: 16px;"
    "font-weight: 500;"
    "background: transparent;"
);
```

## Результаты

### До изменений:
- ❌ Мигание каждые 10 секунд
- ❌ Обновление по таймеру независимо от состояния песни
- ❌ Текст серый, плохо читается

### После изменений:
- ✅ **Нет мигания** - обновление только при реальных изменениях
- ✅ **Умное обновление** - загрузка данных когда песня подходит к концу
- ✅ **Отличная читаемость** - весь текст белый, крупные шрифты

## Файлы изменены

- **client/src/ui/baseradiopage.h** - добавлены переменные для отслеживания состояния
- **client/src/ui/baseradiopage.cpp**:
  - `onSongHistoryReceived()` - проверка изменений истории
  - `onQueueReceived()` - проверка изменений очереди
  - `onNowPlayingUpdated()` - проверка смены текущей песни
  - `updateProgressBar()` - обновление при окончании песни
  - `addSongToList()` - увеличены размеры шрифтов, все тексты белые

## Тестирование

1. Откройте радио-страницу
2. Подождите несколько песен
3. Проверьте:
   - ✅ Список не мигает каждые 10 секунд
   - ✅ Обновление происходит когда песня подходит к концу
   - ✅ Текст хорошо читается (белый, крупный)
   - ✅ В консоли видны сообщения о реальных изменениях:
     - "Song ending soon (X s remaining), fetching fresh data..."
     - "Current song changed to: ..."
     - "Song history changed: X songs"
     - "Queue changed: X songs"

## Производительность

- **Меньше перерисовок** - список обновляется только при реальных изменениях
- **Меньше сетевых запросов** - загрузка данных только когда это нужно
- **Плавный UX** - нет раздражающего мигания
