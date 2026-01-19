# Adding Anime Ace Font to EKNMusic

## Download Font

1. Download the Anime Ace font from: https://fonts-online.ru/fonts/anime-ace/download
2. Extract the downloaded ZIP file
3. You should find files like: `animeace2_reg.ttf` or similar

## Installation Steps

### Step 1: Add Font Files to Project

Copy the `.ttf` font files to:
```
D:\programs\EKNMusic\client\src\resources\fonts\
```

Create the `fonts` directory if it doesn't exist:
```bash
mkdir D:\programs\EKNMusic\client\src\resources\fonts
```

### Step 2: Update Resources File

If you have a `.qrc` (Qt Resource) file, add the font:

```xml
<RCC>
    <qresource prefix="/fonts">
        <file>resources/fonts/animeace2_reg.ttf</file>
    </qresource>
</RCC>
```

### Step 3: Load Font in Application

Add this code to `mainwindow.cpp` or `main.cpp`:

```cpp
#include <QFontDatabase>

// In main() or MainWindow constructor:
int fontId = QFontDatabase::addApplicationFont(":/fonts/resources/fonts/animeace2_reg.ttf");
if (fontId != -1) {
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont animeFont(fontFamily);
    QApplication::setFont(animeFont);
    qDebug() << "Anime Ace font loaded successfully";
} else {
    qWarning() << "Failed to load Anime Ace font";
}
```

### Alternative: Direct File Path

If not using Qt resources:

```cpp
int fontId = QFontDatabase::addApplicationFont("D:/programs/EKNMusic/client/src/resources/fonts/animeace2_reg.ttf");
if (fontId != -1) {
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont animeFont(fontFamily);
    QApplication::setFont(animeFont);
}
```

### Step 4: Apply Font to Specific Widgets

For specific widgets only:

```cpp
// In setupUI() methods
titleLabel->setFont(QFont("Anime Ace", 32, QFont::Bold));
```

Or in StyleSheet:

```cpp
titleLabel->setStyleSheet(
    "font-family: 'Anime Ace';"
    "font-size: 32px;"
    "font-weight: 600;"
    "color: #000000;"
);
```

## License Note

**IMPORTANT:** Anime Ace is freeware for:
- Independent comic book creation
- Non-profit use ONLY

**Restrictions:**
- Not for mainstream publishers without license
- Commercial non-comic applications require permission

Please review the full license terms included with the font download before using it in your project.

## Testing

After adding the font:

1. Build the project
2. Run the application
3. Verify that text appears in Anime Ace font
4. If font doesn't load, check console for error messages

## Troubleshooting

### Font Not Loading

Check:
- Font file path is correct
- Font file has `.ttf` extension
- Console shows "Font loaded successfully" message
- Try restarting Qt Creator after adding font

### Font Doesn't Apply

- Ensure font family name matches exactly
- Use `QFontDatabase::families()` to list all available fonts
- Check if StyleSheet is overriding the font

### Resource Not Found

- Rebuild project completely (Clean → Build)
- Check `.qrc` file is added to CMakeLists.txt
- Verify resource prefix matches path in code

## Current Status

⚠️ **Font not yet added** - Follow steps above to add Anime Ace font to the project.

The application currently uses system default fonts. Once you add the font files and implement the loading code, the entire application will use Anime Ace typography.
