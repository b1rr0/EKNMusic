@echo off
echo ========================================
echo   EKNMusic Build Script
echo ========================================
echo.

REM Check if Qt path exists (adjust if your Qt is installed elsewhere)
set QT_PATH=C:\Qt\6.6.0\msvc2019_64
if not exist "%QT_PATH%" (
    echo ERROR: Qt not found at %QT_PATH%
    echo Please adjust QT_PATH in this script to match your Qt installation
    echo.
    pause
    exit /b 1
)

echo Qt found at: %QT_PATH%
echo.

REM Create build directory
if not exist build (
    echo Creating build directory...
    mkdir build
)

cd build

echo Configuring project with CMake...
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="%QT_PATH%" ..
if errorlevel 1 (
    echo.
    echo ERROR: CMake configuration failed!
    echo Make sure Visual Studio 2022 Build Tools are installed
    cd ..
    pause
    exit /b 1
)

echo.
echo Building project...
cmake --build . --config Release
if errorlevel 1 (
    echo.
    echo ERROR: Build failed!
    echo Check the error messages above
    cd ..
    pause
    exit /b 1
)

echo.
echo ========================================
echo   Build Successful!
echo ========================================
echo.

REM Deploy Qt DLLs
if exist Release\EKNMusic.exe (
    echo Deploying Qt dependencies...
    "%QT_PATH%\bin\windeployqt.exe" Release\EKNMusic.exe --release
    echo.
    echo Application ready at: %CD%\Release\EKNMusic.exe
    echo.

    choice /C YN /M "Do you want to run the application now"
    if errorlevel 2 goto end
    if errorlevel 1 goto run
)

:run
echo.
echo Starting EKNMusic...
cd Release
start EKNMusic.exe
cd ..
goto end

:end
cd ..
echo.
echo Done!
pause
