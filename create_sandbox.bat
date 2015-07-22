echo off

SET BUILD_PLATFORM_REPO=https://github.com/jjasinski/BuildPlatform.git
SET SOLUTION_PATH=%CD%

if exist "%SOLUTION_PATH%\BuildPlatform" rmdir /q /s "%SOLUTION_PATH%\BuildPlatform"

git clone --depth=1 %BUILD_PLATFORM_REPO% BuildPlatform

cd BuildPlatform
call create_platform.bat
cd %SOLUTION_PATH%