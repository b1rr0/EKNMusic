# Radio UI - Чистый дизайн без лишних элементов

## Изменения

### 1. Удалены порядковые номера

**Было**: Слева от обложки отображались цифры 1, 2, 3, 4...

**Стало**: Порядковые номера полностью убраны

```cpp
// Убрано:
// QLabel *orderLabel = new QLabel(QString::number(m_songOrderCounter++), itemWidget);
// mainItemLayout->addWidget(orderLabel);

// Осталось:
mainItemLayout->addWidget(thumbnailLabel);
mainItemLayout->addWidget(songInfoWidget, 1);
```

### 2. Удалена длительность справа

**Было**: Справа отображалось время песни (3:21, 4:15 и т.д.)

**Стало**: Длительность полностью убрана

```cpp
// Убрано:
// QLabel *durationLabel = new QLabel(itemWidget);
// durationLabel->setText(QString("%1:%2").arg(minutes).arg(seconds...));
// mainItemLayout->addWidget(durationLabel);
```

### 3. Оставлена только 1 NEXT песня

**Было**: Показывались 2 следующие песни (NEXT + UPCOMING)

**Стало**: Только 1 NEXT песня

```cpp
// Было:
// int queueCount = qMin(2, m_queue.size());
// for (int i = 0; i < queueCount; i++) {
//     addSongToList(m_queue[i], i == 0 ? "NEXT" : "UPCOMING", false);
// }

// Стало:
if (!m_queue.isEmpty()) {
    addSongToList(m_queue[0], "NEXT", false);
}
```

**Итого треков в плейлисте**: 2 PREVIOUS + 1 NOW PLAYING + 1 NEXT = **4 трека**

### 4. Убраны скроллбары

```cpp
songListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
songListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
```

### 5. Текст обрезается с elipsis и доступен для копирования

**Реализация**:
- Текст обрезается с "..." если не помещается
- При наведении показывается tooltip с полным текстом
- Текст можно выделить и скопировать (TextSelectableByMouse)
- Курсор меняется на IBeam при наведении

```cpp
// Title
QFont titleFont;
titleFont.setPixelSize(18);
titleFont.setWeight(QFont::DemiBold);
titleText->setFont(titleFont);
titleText->setTextInteractionFlags(Qt::TextSelectableByMouse);
titleText->setCursor(Qt::IBeamCursor);

QFontMetrics metrics(titleFont);
QString elidedTitle = metrics.elidedText(song.title, Qt::ElideRight, 700);
titleText->setText(elidedTitle);
titleText->setToolTip(song.title);  // Full text on hover

// Artist
QFont artistFont;
artistFont.setPixelSize(15);
artistText->setFont(artistFont);
artistText->setTextInteractionFlags(Qt::TextSelectableByMouse);
artistText->setCursor(Qt::IBeamCursor);

QFontMetrics artistMetrics(artistFont);
QString elidedArtist = artistMetrics.elidedText(song.artist, Qt::ElideRight, 700);
artistText->setText(elidedArtist);
artistText->setToolTip(song.artist);  // Full text on hover
```

## Визуальный результат

### До изменений:
```
┌────┬───────┬────────────────────────┬──────┐
│  1 │ [70px]│ PREVIOUS               │ 3:21 │
│    │       │ Song Title             │      │
│    │       │ Artist Name            │      │
├────┼───────┼────────────────────────┼──────┤
│  2 │ [70px]│ PREVIOUS               │ 4:15 │
│    │       │ Another Song           │      │
│    │       │ Another Artist         │      │
├────┼───────┼────────────────────────┼──────┤
│  3 │ [70px]│ NOW PLAYING (синий)    │ 5:07 │
│    │       │ Current Song           │      │
│    │       │ Current Artist         │      │
├────┼───────┼────────────────────────┼──────┤
│  4 │ [70px]│ NEXT                   │ 2:54 │
│    │       │ Next Song              │      │
│    │       │ Next Artist            │      │
├────┼───────┼────────────────────────┼──────┤
│  5 │ [70px]│ UPCOMING               │ 3:30 │
│    │       │ Future Song            │      │
│    │       │ Future Artist          │      │
└────┴───────┴────────────────────────┴──────┘
        [──────────────────────────]  ← скроллбар
```

### После изменений:
```
┌───────┬─────────────────────────────────┐
│ [70px]│ PREVIOUS                        │
│       │ Song Title (можно копировать)   │
│       │ Artist Name (можно копировать)  │
├───────┼─────────────────────────────────┤
│ [70px]│ PREVIOUS                        │
│       │ Another Song...                 │  ← обрезано
│       │ Another Artist                  │
├───────┼─────────────────────────────────┤
│ [70px]│ NOW PLAYING (синий фон)         │
│       │ Current Song (можно копировать) │
│       │ Current Artist                  │
├───────┼─────────────────────────────────┤
│ [70px]│ NEXT                            │
│       │ Next Song                       │
│       │ Next Artist                     │
└───────┴─────────────────────────────────┘

✅ Нет цифр слева
✅ Нет времени справа
✅ Нет скроллбара
✅ Только 4 трека (было 5)
✅ Текст обрезается с ...
✅ Текст можно копировать
```

## Особенности работы с текстом

### Длинные названия
Если название песни не помещается:
```
Было:  "This is a very long song title that doesn't fit"
Стало: "This is a very long song title that do..."
```

### Tooltip при наведении
```
Наведение мышкой → Показывается полный текст во всплывающей подсказке
```

### Копирование текста
```
1. Наведите мышку на текст → курсор меняется на I-beam
2. Выделите текст мышкой
3. Ctrl+C для копирования
4. Можно скопировать название песни или исполнителя
```

## Количество треков

| Категория | Было | Стало |
|-----------|------|-------|
| PREVIOUS | 2 | 2 |
| NOW PLAYING | 1 | 1 |
| NEXT | 1 | 1 |
| UPCOMING | 1 | 0 (убрано) |
| **ИТОГО** | **5** | **4** |

### Высота контейнера
- Было: 5 × 100px = 500px
- Стало: 4 × 100px = 400px
- Остается 100px пустого пространства

## Файлы изменены

### client/src/ui/baseradiopage.cpp

**updateSongList()** - только 1 NEXT песня:
```cpp
// Add upcoming songs (from queue) - only 1 NEXT song
if (!m_queue.isEmpty()) {
    addSongToList(m_queue[0], "NEXT", false);
}
```

**addSongToList()** - убраны номера, время, добавлен elided text:
- Убран orderLabel
- Убран durationLabel
- Margins: 20px (вместо 16px слева)
- Title: elided text с QFontMetrics, TextSelectableByMouse
- Artist: elided text с QFontMetrics, TextSelectableByMouse
- Tooltip: полный текст при наведении

**setupRightPanel()** - убраны скроллбары:
```cpp
songListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
songListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
```

## Тестирование

1. Откройте радио-страницу
2. Проверьте визуально:
   - ✅ Нет цифр слева от обложек
   - ✅ Нет времени справа
   - ✅ Нет горизонтального скроллбара внизу
   - ✅ Показывается 4 трека (2+1+1)

3. Проверьте длинные названия:
   - ✅ Текст обрезается с "..."
   - ✅ При наведении показывается tooltip с полным текстом

4. Проверьте копирование:
   - ✅ Курсор меняется на I-beam при наведении на текст
   - ✅ Можно выделить текст мышкой
   - ✅ Можно скопировать текст (Ctrl+C)

## Преимущества

1. **Чистота**: убраны лишние элементы (номера, время)
2. **Фокус**: внимание на названии песни и исполнителе
3. **Компактность**: 4 трека вместо 5
4. **Удобство**: текст можно копировать для поиска
5. **Аккуратность**: нет скроллбаров
6. **UX**: tooltip показывает полный текст при обрезании
