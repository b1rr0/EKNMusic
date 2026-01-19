# EKNMusic Installer - Inno Setup

Создание единого .exe установщика для EKNMusic с помощью Inno Setup.

## Что это?

**Inno Setup** - бесплатный инсталлятор для Windows приложений. Он создает **один .exe файл**, который:
- Содержит все файлы приложения внутри себя (запакованные)
- Распаковывает их при установке
- Создает ярлыки, записи в реестр и т.д.
- Добавляет программу в "Установка и удаление программ"

## Установка Inno Setup

1. Скачайте Inno Setup: https://jrsoftware.org/isdl.php
2. Скачайте русский языковой пакет (уже включен в setup.iss)
3. Установите Inno Setup

## Подготовка к сборке установщика

### 1. Соберите Release версию EKNMusic:

```bash
cd D:\programs\EKNMusic\client
cmake --preset mingw-release
cmake --build build-release --config Release
```

### 2. Запустите windeployqt для копирования Qt зависимостей:

```bash
D:\programs\6.10.1\mingw_64\bin\windeployqt.exe ^
  --release ^
  --no-translations ^
  D:\programs\EKNMusic\client\build-release\EKNMusic.exe
```

Это создаст все необходимые .dll файлы и папки (platforms, styles и т.д.)

### 3. Проверьте структуру:

```
client\build-release\
├── EKNMusic.exe
├── Qt6Core.dll
├── Qt6Gui.dll
├── Qt6Widgets.dll
├── Qt6Network.dll
├── Qt6Sql.dll
├── Qt6Multimedia.dll
├── platforms\
│   └── qwindows.dll
├── styles\
│   └── qwindowsvistastyle.dll
└── ... (другие dll)
```

## Создание установщика

### Способ 1: Через GUI

1. Откройте **Inno Setup Compiler**
2. File → Open → выберите `setup.iss`
3. Build → Compile (или F9)
4. Установщик будет создан в папке `installer\output\`

### Способ 2: Через командную строку

```bash
cd D:\programs\EKNMusic\installer
"C:\Program Files (x86)\Inno Setup 6\ISCC.exe" setup.iss
```

## Результат

После компиляции получите:
- **Файл**: `installer\output\EKNMusic_Setup_v1.0.0.exe` (~15-25 МБ)
- Это **единый самораспаковывающийся установщик**

## Что делает установщик?

При запуске `EKNMusic_Setup_v1.0.0.exe`:

1. **Приветствие** - показывает окно приветствия
2. **Выбор папки** - пользователь выбирает куда установить (по умолчанию: `C:\Program Files\EKNMusic`)
3. **Выбор опций**:
   - ☑ Создать ярлык на рабочем столе
   - ☐ Создать ярлык на панели быстрого запуска
   - ☐ Запускать при входе в Windows
4. **Установка** - распаковывает все файлы из .exe
5. **Завершение** - предлагает запустить приложение

## Возможности установщика

✅ **Все в одном файле** - один .exe содержит все
✅ **Сжатие LZMA2** - максимальное сжатие файлов
✅ **Создание ярлыков** - на рабочем столе, в меню Пуск
✅ **Автозагрузка** - запись в реестр
✅ **Деинсталлятор** - автоматически создается
✅ **Запись в реестр** - добавление в Programs & Features
✅ **Русский интерфейс** - полная русификация

## Настройка setup.iss

### Изменить версию:

```ini
AppVersion=1.0.0
```

### Изменить путь по умолчанию:

```ini
; Для Program Files:
DefaultDirName={autopf}\EKNMusic

; Для AppData:
DefaultDirName={localappdata}\EKNMusic
```

### Добавить иконку:

1. Создайте `client\resources\icon.ico`
2. В setup.iss уже настроено: `SetupIconFile=..\client\resources\icon.ico`

### Изменить файлы для установки:

```ini
[Files]
Source: "..\client\build-release\EKNMusic.exe"; DestDir: "{app}"
Source: "..\client\build-release\*.dll"; DestDir: "{app}"
; ... добавьте свои файлы
```

## Тестирование установщика

1. Запустите `EKNMusic_Setup_v1.0.0.exe`
2. Пройдите процесс установки
3. Проверьте:
   - [ ] Приложение установилось в правильную папку
   - [ ] Ярлык создался на рабочем столе
   - [ ] Приложение запускается
   - [ ] Появилось в "Установка и удаление программ"
   - [ ] Деинсталлятор работает

## Распространение

Теперь можно:
- Выложить `EKNMusic_Setup_v1.0.0.exe` на сайт для скачивания
- Разместить на GitHub Releases
- Отправить пользователям напрямую

Пользователю нужен **только один файл** - `EKNMusic_Setup_v1.0.0.exe`!

## Размер установщика

- Без сжатия: ~50-70 МБ
- С LZMA2 сжатием: ~15-25 МБ
- Зависит от количества Qt модулей

## Продвинутые функции

### Проверка версии при установке:

```pascal
[Code]
function InitializeSetup(): Boolean;
var
  OldVersion: String;
begin
  if RegQueryStringValue(HKCU, 'Software\EKNMusic', 'Version', OldVersion) then
  begin
    if MsgBox('Обнаружена версия ' + OldVersion + '. Удалить?',
              mbConfirmation, MB_YESNO) = IDYES then
    begin
      // Удалить старую версию
    end;
  end;
  Result := True;
end;
```

### Проверка .NET Framework, VC++ Redistributable и т.д.:

```pascal
[Code]
function IsDotNetInstalled(): Boolean;
begin
  Result := RegKeyExists(HKLM, 'SOFTWARE\Microsoft\.NETFramework\...');
end;
```

## Устранение неполадок

### Ошибка: "Unable to open file"
- Проверьте пути в секции [Files]
- Убедитесь что build-release содержит все файлы

### Установщик слишком большой
- Проверьте что не включили лишние .dll
- Используйте `--no-translations` в windeployqt
- Удалите ненужные Qt плагины

### Приложение не запускается после установки
- Проверьте что все .dll скопированы
- Запустите windeployqt повторно
- Проверьте platforms\qwindows.dll

## Полезные ссылки

- [Документация Inno Setup](https://jrsoftware.org/ishelp/)
- [Примеры скриптов](https://jrsoftware.org/ishelp/index.php?topic=samples)
- [Pascal Scripting](https://jrsoftware.org/ishelp/index.php?topic=isxfunc)

---

**Версия**: 1.0.0
**Автор**: EKNMusic Team
