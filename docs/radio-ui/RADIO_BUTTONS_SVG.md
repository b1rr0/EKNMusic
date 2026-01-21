# Radio Buttons - SVG Icons without Background

## Изменения

Убран круглый фон с кнопок play/pause и используются только SVG иконки.

### 1. Используются SVG вместо PNG/JPG

**Было**:
```cpp
playPauseBtn->setIcon(QIcon(":/images/src/resources/images/playButton.png"));
// При переключении:
playPauseBtn->setIcon(QIcon(":/images/src/resources/images/pauseButton.jpg"));
```

**Стало**:
```cpp
playPauseBtn->setIcon(QIcon(":/images/src/resources/images/playButton.svg"));
// При переключении:
playPauseBtn->setIcon(QIcon(":/images/src/resources/images/pauseButton.svg"));
```

### 2. Убран круглый фон с hover эффектом

**Было**:
```cpp
playPauseBtn->setStyleSheet(
    "QPushButton {"
    "   background-color: transparent;"
    "   border: none;"
    "   border-radius: 25px;"
    "}"
    "QPushButton:hover {"
    "   background-color: rgba(255, 255, 255, 0.1);"  // Круглый фон при наведении
    "}"
);
```

**Стало**:
```cpp
playPauseBtn->setStyleSheet(
    "QPushButton {"
    "   background-color: transparent;"
    "   border: none;"
    "}"
    // Hover эффект убран!
);
```

### 3. Размер иконки увеличен до 50x50

**Было**: IconSize 40x40 в кнопке 50x50 (padding 5px)

**Стало**: IconSize 50x50 в кнопке 50x50 (без padding)

```cpp
playPauseBtn->setFixedSize(50, 50);
playPauseBtn->setIconSize(QSize(50, 50));  // Было: 40, 40
```

## SVG файлы

### playButton.svg
- ✅ Создан пользователем
- Размер: 512x512pt
- Fill: #000000 (черный)
- Форма: треугольник ▶

### pauseButton.svg
- ✅ Создан пользователем
- Размер: 1200x1200pt
- Fill: #000000 (черный)
- Форма: два вертикальных прямоугольника ||

## Resources.qrc

Добавлены SVG файлы:
```xml
<qresource prefix="/images">
    <file>src/resources/images/playButton.png</file>
    <file>src/resources/images/playButton.svg</file>
    <file>src/resources/images/pauseButton.jpg</file>
    <file>src/resources/images/pauseButton.svg</file>
    <!-- ... -->
</qresource>
```

## Визуальное сравнение

### До изменений:
```
┌──────────────┐
│              │
│   ┌──────┐   │  ← Круглый светлый фон при hover
│   │  ▶   │   │  ← PNG иконка 40x40
│   └──────┘   │
│              │
└──────────────┘
    50x50px
```

### После изменений:
```
┌──────────────┐
│              │
│      ▶       │  ← Нет фона, только иконка
│              │  ← SVG иконка 50x50
│              │
│              │
└──────────────┘
    50x50px
```

## Преимущества SVG

1. **Векторная графика**: масштабируется без потери качества
2. **Четкость**: идеально резкие линии на любом разрешении
3. **Размер файла**: меньше чем PNG
4. **Гибкость**: легко менять цвет через CSS/код
5. **Чистота**: только иконка, без фона

## Файлы изменены

### client/src/ui/baseradiopage.cpp

**setupLeftPanel()** - использование SVG:
```cpp
playPauseBtn->setFixedSize(50, 50);
playPauseBtn->setIcon(QIcon(":/images/src/resources/images/playButton.svg"));
playPauseBtn->setIconSize(QSize(50, 50));
playPauseBtn->setStyleSheet(
    "QPushButton {"
    "   background-color: transparent;"
    "   border: none;"
    "}"
);
```

**onPlaybackStateChanged()** - переключение SVG:
```cpp
if (isPlaying) {
    playPauseBtn->setIcon(QIcon(":/images/src/resources/images/pauseButton.svg"));
} else {
    playPauseBtn->setIcon(QIcon(":/images/src/resources/images/playButton.svg"));
}
```

### client/resources.qrc

Добавлены:
```xml
<file>src/resources/images/playButton.svg</file>
<file>src/resources/images/pauseButton.svg</file>
```

## Тестирование

1. Пересоберите проект (чтобы обновились ресурсы):
   ```bash
   cd D:\programs\EKNMusic\build
   cmake --build . --config Release
   ```

2. Запустите приложение

3. Проверьте:
   - ✅ Кнопка Play отображается четко (SVG)
   - ✅ Нет круглого фона при наведении
   - ✅ При клике Play меняется на Pause
   - ✅ Кнопка Pause отображается четко (SVG)
   - ✅ Иконки заполняют всю кнопку (50x50)
   - ✅ Нет белого круга при hover

## Результат

✅ Чистые кнопки без фона
✅ Четкие SVG иконки
✅ Размер 50x50px
✅ Нет hover эффекта (круглого фона)
✅ Только активная иконка видна
