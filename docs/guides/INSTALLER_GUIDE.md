@echo off
echo ========================================
echo  EKNMusic - Сборка установщика
echo ========================================
echo.

echo [1/4] Очистка старых файлов...
cd client
if exist build-release rmdir /s /q build-release
cd ..

echo [2/4] Сборка Release версии EKNMusic...
cd client
cmake --preset mingw-release
if errorlevel 1 (
    echo ОШИБКА: Не удалось сконфигурировать CMake
    pause
    exit /b 1
)

cmake --build build-release --config Release
if errorlevel 1 (
    echo ОШИБКА: Не удалось собрать приложение
    pause
    exit /b 1
)

echo [3/4] Копирование Qt зависимостей...
D:\programs\6.10.1\mingw_64\bin\windeployqt.exe --release --no-translations build-release\EKNMusic.exe
if errorlevel 1 (
    echo ОШИБКА: Не удалось запустить windeployqt
    pause
    exit /b 1
)

cd ..

echo [4/4] Создание установщика...
"C:\Program Files (x86)\Inno Setup 6\ISCC.exe" installer\setup.iss
if errorlevel 1 (
    echo ОШИБКА: Не удалось создать установщик
    pause
    exit /b 1
)

echo.
echo ========================================
echo  Успешно!
echo ========================================
echo.
echo Установщик создан:
echo installer\output\EKNMusic_Setup_v1.0.0.exe
echo.
pause
