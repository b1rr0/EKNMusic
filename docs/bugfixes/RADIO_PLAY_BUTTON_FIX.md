# Radio Play/Pause Button - Fix

## Проблемы

1. **Кнопка Play сбоку и не влезает** - иконка 40x40 в кнопке 40x40, поэтому обрезается
2. **Кнопка Pause с черным фоном** - pauseButton.jpg имеет черный круглый фон

## Решения

### 1. Исправлена кнопка Play

**Было**:
```cpp
playPauseBtn->setFixedSize(40, 40);
playPauseBtn->setIconSize(QSize(40, 40));
```

**Проблема**: Иконка 40x40 пикселей в кнопке 40x40 пикселей = нет места для padding, иконка обрезается по краям.

**Стало**:
```cpp
playPauseBtn->setFixedSize(50, 50);
playPauseBtn->setIconSize(QSize(40, 40));
```

**Решение**: Кнопка 50x50, иконка 40x40 = по 5px padding с каждой стороны, иконка центрирована.

### 2. Создана кнопка Pause с прозрачным фоном

**Было**: `pauseButton.jpg` - черный круглый фон с белой паузой ||

**Стало**: `pauseButton.png` - прозрачный фон с белой паузой ||

**Файлы**:
- ✅ Создан `pauseButton.svg` - векторная иконка паузы
- ⚠️ Нужно создать `pauseButton.png` из SVG

**Код изменен**:
```cpp
// Было:
playPauseBtn->setIcon(QIcon(":/images/src/resources/images/pauseButton.jpg"));

// Стало:
playPauseBtn->setIcon(QIcon(":/images/src/resources/images/pauseButton.png"));
```

## Создание pauseButton.png

### Автоматическая конвертация SVG → PNG

Если у вас установлен Inkscape:
```bash
cd "D:\programs\EKNMusic\client\src\resources\images"
inkscape pauseButton.svg -w 512 -h 512 --export-filename=pauseButton.png
```

Если у вас установлен ImageMagick:
```bash
cd "D:\programs\EKNMusic\client\src\resources\images"
magick pauseButton.svg -resize 512x512 pauseButton.png
```

### Онлайн конвертация (самый простой)

1. Откройте https://cloudconvert.com/svg-to-png
2. Загрузите `pauseButton.svg`
3. Установите размер: 512x512
4. Конвертируйте и скачайте `pauseButton.png`
5. Скопируйте в `D:\programs\EKNMusic\client\src\resources\images\`

### Вручную в Photopea

1. Откройте https://www.photopea.com/
2. File → Open → выберите `pauseButton.svg`
3. Image → Image Size → 512x512px
4. File → Export As → PNG
5. Сохраните как `pauseButton.png`

## Параметры иконок

### Play Button (playButton.png)
- ✅ Размер: любой (автоматически масштабируется)
- ✅ Фон: прозрачный
- ✅ Форма: треугольник ▶
- ✅ Цвет: черный/белый

### Pause Button (pauseButton.png)
- ⚠️ Размер: 512x512px (рекомендуется)
- ✅ Фон: прозрачный (создан SVG)
- ✅ Форма: два прямоугольника ||
  - Левый бар: 140x380px, закругленные углы 20px
  - Правый бар: 140x380px, закругленные углы 20px
  - Расстояние между барами: 92px
- ✅ Цвет: белый (#FFFFFF)

### Кнопка в UI
- Размер кнопки: 50x50px
- Размер иконки: 40x40px
- Padding: 5px с каждой стороны
- Border radius: 25px (круглая кнопка)
- Hover: rgba(255, 255, 255, 0.1)

## Файлы изменены

### client/src/ui/baseradiopage.cpp

**setupLeftPanel()** - увеличена кнопка:
```cpp
playPauseBtn->setFixedSize(50, 50);  // Было: 40x40
playPauseBtn->setIconSize(QSize(40, 40));
playPauseBtn->setStyleSheet(
    "QPushButton {"
    "   background-color: transparent;"
    "   border: none;"
    "   border-radius: 25px;"  // Было: 20px
    "}"
    "QPushButton:hover {"
    "   background-color: rgba(255, 255, 255, 0.1);"
    "}"
);
```

**onPlaybackStateChanged()** - изменен путь:
```cpp
if (isPlaying) {
    playPauseBtn->setIcon(QIcon(":/images/src/resources/images/pauseButton.png"));
} else {
    playPauseBtn->setIcon(QIcon(":/images/src/resources/images/playButton.png"));
}
```

### client/src/resources/images/

**Созданы файлы**:
- ✅ `pauseButton.svg` - векторная иконка паузы с прозрачным фоном
- ⚠️ `pauseButton.png` - нужно создать из SVG (см. инструкции выше)

## Тестирование

1. **Создайте pauseButton.png**:
   - Конвертируйте SVG → PNG (см. инструкции выше)
   - Скопируйте в `client/src/resources/images/`

2. **Пересоберите проект**:
   ```bash
   cd D:\programs\EKNMusic\build
   cmake --build . --config Release
   ```

3. **Запустите приложение**

4. **Проверьте**:
   - ✅ Кнопка Play отображается по центру (не сбоку)
   - ✅ Кнопка Play не обрезается
   - ✅ При нажатии Play меняется на Pause
   - ✅ Кнопка Pause имеет прозрачный фон (не черный круг)
   - ✅ Кнопка Pause отображается корректно

## Визуальное сравнение

### До изменений:
```
┌────────┐
│  ▶    │  ← Play обрезан сбоку
└────────┘
   40x40

┌────────┐
│  ●──●  │  ← Pause с черным кругом
└────────┘
```

### После изменений:
```
┌──────────┐
│   ▶️     │  ← Play по центру
└──────────┘
   50x50

┌──────────┐
│   ││     │  ← Pause прозрачный фон
└──────────┘
   50x50
```

## Альтернативное решение (временное)

Если не получается создать PNG, можно временно использовать Unicode символ:

```cpp
// В onPlaybackStateChanged():
if (isPlaying) {
    playPauseBtn->setText("⏸");  // Unicode pause
    playPauseBtn->setStyleSheet(
        "QPushButton { font-size: 30px; color: white; background: transparent; border: none; }"
    );
} else {
    playPauseBtn->setIcon(QIcon(":/images/src/resources/images/playButton.png"));
}
```

Но лучше создать правильный PNG файл из SVG!
