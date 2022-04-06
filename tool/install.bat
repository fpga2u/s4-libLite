

@REM SET BUILD_MODE=Debug
@REM SET BUILD_MODE=Release
@REM SET COMPILER="Visual Studio 15 2017 Win64"
SET COMPILER="Visual Studio 17 2022"

@REM 安装目录，相对于build目录：
SET INSTALL_PATH="../install"

SET CURRENT_PATH=%cd%

cd /d %~dp0
cd ..
if exist build_install (
    echo "use existing build_install"
) else (
    md build_install
)
cd build_install
@REM echo y|DEL /S /Q %INSTALL_PATH%_debug
@REM echo y|DEL /S /Q %INSTALL_PATH%_release

echo y|DEL CMakeCache.txt
cmake .. -G %COMPILER% -DCMAKE_INSTALL_PREFIX=%INSTALL_PATH%_debug
if "%1"=="verbose" ( pause )
cmake --build . --config Debug --target install

echo y|DEL CMakeCache.txt
cmake .. -G %COMPILER% -DCMAKE_INSTALL_PREFIX=%INSTALL_PATH%_release
if "%1"=="verbose" ( pause )
cmake --build . --config Release --target install
cd %CURRENT_PATH%