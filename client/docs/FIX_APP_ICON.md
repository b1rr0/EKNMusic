# Виправлення помилки компіляції app.rc

## Проблема
```
error: [CMakeFiles\EKNMusic.dir\build.make:319: CMakeFiles/EKNMusic.dir/platform/windows/app.rc.obj] Error 1
```

## Причина
Файл іконки називався `logo.ico`, а в `app.rc` очікувався `icon.ico`.

## Рішення
✅ Файл перейменовано: `logo.ico` → `icon.ico`

## Поточна структура
```
platform/windows/
  ├── app.rc          ✅ (посилається на icon.ico)
  └── icon.ico        ✅ (перейменовано з logo.ico)
```

## Наступні кроки для компіляції

1. **В Qt Creator:**
   ```
   Build → Clean All
   Build → Rebuild All
   ```

2. **Або в терміналі:**
   ```bash
   cd D:\programs\EKNMusic\client
   cmake --build build --config Release --clean-first
   ```

3. **Якщо все ще помилка:**
   ```bash
   cd D:\programs\EKNMusic\client
   rm -rf build
   mkdir build
   cd build
   cmake ..
   cmake --build . --config Release
   ```

## Очікуваний результат
✅ Іконка з'явиться в:
- Taskbar Windows
- Заголовок вікна
- Файл EKNMusic.exe
- Alt+Tab switcher

## Перевірка іконки
Після успішної компіляції:
1. Запустіть EKNMusic.exe
2. Перевірте taskbar - має бути ваш логотип
3. Перевірте властивості .exe файлу - має бути іконка
