# Song History Display - Fix

## Проблема
Прошлые песни (PREVIOUS) не отображались в списке воспроизведения.

## Причины

### 1. История не обновлялась периодически
История загружалась только один раз при открытии страницы, но не обновлялась каждые 10 секунд вместе с queue и now playing.

### 2. Неправильный порядок отображения
API возвращает историю в хронологическом порядке (от старых к новым), но мы показывали первые 3 элемента вместо последних 3.

### 3. Возможное дублирование
Текущая песня могла дублироваться в истории и в "NOW PLAYING".

## Решение

### 1. Добавлена периодическая загрузка истории

**Файл**: `client/src/ui/baseradiopage.cpp` - метод `updateProgressBar()`

```cpp
// Fetch fresh data every 10 seconds
if (counter >= 10) {
    m_radioService->fetchNowPlaying();
    m_radioService->fetchSongHistory(5);  // ✅ Добавлено
    m_radioService->fetchQueue();
    counter = 0;
}
```

### 2. Исправлен порядок отображения

**Было**:
```cpp
int historyCount = qMin(3, m_songHistory.size());
for (int i = 0; i < historyCount; i++) {
    addSongToList(m_songHistory[i], "PREVIOUS", false);
}
```

**Стало**:
```cpp
// History comes in chronological order, so we want the last items (most recent)
int historyToShow = 0;
int startIndex = qMax(0, m_songHistory.size() - 3);

for (int i = startIndex; i < m_songHistory.size(); i++) {
    // Skip if this is the current song
    if (m_songHistory[i].title == currentTitle &&
        m_songHistory[i].artist == currentInfo.song.artist) {
        continue;
    }
    addSongToList(m_songHistory[i], "PREVIOUS", false);
    historyToShow++;
    if (historyToShow >= 3) break;
}
```

### 3. Добавлена проверка пустых песен

```cpp
void BaseRadioPage::addSongToList(const RadioService::SongInfo &song, const QString &label, bool isCurrent)
{
    // Skip empty songs
    if (song.title.isEmpty() && song.artist.isEmpty()) {
        return;
    }
    // ... rest of method
}
```

### 4. Добавлена отладочная информация

```cpp
void BaseRadioPage::onSongHistoryReceived(const QList<RadioService::SongInfo> &history)
{
    m_songHistory = history;

    qDebug() << "Song history received:" << history.size() << "songs";
    for (int i = 0; i < history.size(); i++) {
        qDebug() << "  [" << i << "]" << history[i].artist << "-" << history[i].title;
    }

    updateSongList();
}
```

## Как это работает теперь

### Порядок песен в списке (сверху вниз):
1. **PREVIOUS** - до 3 последних прошлых песен (не включая текущую)
2. **NOW PLAYING** - текущая песня (выделена синим)
3. **NEXT** - следующая песня из очереди
4. **UPCOMING** - до 4 песен из очереди после следующей

### Обновление данных:
- **Каждую секунду**: обновляется прогресс-бар текущей песни
- **Каждые 10 секунд**: загружаются:
  - Now Playing (текущая песня)
  - Song History (последние 5 песен)
  - Queue (очередь)

### Логика отображения истории:
1. API возвращает до 5 последних песен
2. Берем последние 3 из них (`startIndex = max(0, size - 3)`)
3. Пропускаем текущую песню если она есть в истории
4. Показываем максимум 3 песни с меткой "PREVIOUS"

## Тестирование

1. Откройте радио-страницу
2. Подождите несколько песен
3. Проверьте, что в списке появляются:
   - 3 предыдущие песни (PREVIOUS)
   - Текущая песня (NOW PLAYING) - выделена синим
   - Следующая песня (NEXT)
   - Будущие песни (UPCOMING)
4. Убедитесь, что список обновляется автоматически каждые 10 секунд
5. Проверьте логи в консоли - должны видеть "Song history received: X songs"

## Файлы изменены
- `client/src/ui/baseradiopage.cpp`
  - `updateProgressBar()` - добавлена загрузка истории
  - `updateSongList()` - исправлен порядок и дедупликация
  - `onSongHistoryReceived()` - добавлена отладка
  - `addSongToList()` - добавлена проверка пустых песен
