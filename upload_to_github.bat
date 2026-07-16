@echo off
chcp 65001 >nul
echo ====================================
echo ESP32-Watch GitHub Upload Script
echo ====================================
echo.

REM Configure Git user info
echo [Config] Setting Git user info...
git config user.name "Suci"
git config user.email "3314881686@qq.com"
echo Git user configured
echo.

REM Check if Git repo is initialized
if not exist ".git" (
    echo [Step 1/7] Initializing Git repository...
    git init
    echo Git repository initialized
    echo.
) else (
    echo [Step 1/7] Git repository exists, skipping
    echo.
)

REM Check remote repository
git remote get-url origin >nul 2>&1
if errorlevel 1 (
    echo [Step 2/7] Adding remote repository...
    git remote add origin https://github.com/suci135/ESP32-Watch.git
    echo Remote repository added
    echo.
) else (
    echo [Step 2/7] Remote repository exists
    for /f "delims=" %%i in ('git remote get-url origin') do set CURRENT_REMOTE=%%i
    echo Current remote: %CURRENT_REMOTE%
    echo.
    
    if not "%CURRENT_REMOTE%"=="https://github.com/suci135/ESP32-Watch.git" (
        echo Updating remote URL...
        git remote set-url origin https://github.com/suci135/ESP32-Watch.git
        echo Remote URL updated
        echo.
    )
)

REM Handle submodules
echo [Step 3/7] Checking submodules...
if exist ".gitmodules" (
    echo Initializing submodules...
    git submodule update --init --recursive
    echo.
) else (
    echo Checking for untracked submodules...
    if exist "lib\Arduino-CST816T-Library\.git" (
        echo Found submodule: lib/Arduino-CST816T-Library
        echo Adding as submodule...
        git submodule add https://github.com/your-repo/Arduino-CST816T-Library.git lib/Arduino-CST816T-Library 2>nul
        if errorlevel 1 (
            echo Submodule already tracked or needs manual setup
        )
        echo.
    )
)

REM Add all files
echo [Step 4/7] Adding files to staging area...
git add .
git add -u
echo Files added
echo.

REM Show status
echo [Step 5/7] Current status:
git status --short
echo.

REM Commit changes
echo [Step 6/7] Committing changes...
set /p COMMIT_MSG="Enter commit message (press Enter for default): "
if "%COMMIT_MSG%"=="" (
    set COMMIT_MSG=Update project files
)
git commit -m "%COMMIT_MSG%"
if errorlevel 1 (
    echo No changes to commit or commit failed
    echo.
)
echo.

REM Ensure we're on main branch
echo [Step 7/7] Ensuring main branch...
for /f "delims=" %%i in ('git branch --show-current') do set CURRENT_BRANCH=%%i

if "%CURRENT_BRANCH%"=="" (
    echo Creating and switching to main branch...
    git checkout -b main
    echo.
) else if not "%CURRENT_BRANCH%"=="main" (
    echo Current branch: %CURRENT_BRANCH%
    echo Switching to main branch...
    git checkout main >nul 2>&1
    if errorlevel 1 (
        echo Main branch doesn't exist, creating it...
        git checkout -b main
    )
    echo.
) else (
    echo Already on main branch
    echo.
)

REM Push to GitHub
echo [Step 8/8] Pushing to GitHub (main branch)...
echo Pushing to remote repository...

REM Check if remote main branch exists by actually checking remote refs
git fetch origin >nul 2>&1
git ls-remote --heads origin main | findstr "main" >nul 2>&1
if not errorlevel 1 (
    echo Remote main branch exists, pulling first...
    git pull origin main --rebase
    if errorlevel 1 (
        echo Pull failed, attempting to push with force...
        echo.
    )
) else (
    echo Remote main branch doesn't exist yet, will create it
    echo.
)

REM Push to main branch (use --force-with-lease for safety if there are conflicts)
git push --set-upstream origin main
if errorlevel 1 (
    echo First push failed, trying with --force-with-lease...
    git push --set-upstream origin main --force-with-lease
)

if errorlevel 1 (
    echo.
    echo ====================================
    echo Push Failed!
    echo ====================================
    echo.
    echo Possible reasons:
    echo 1. Repository not created on GitHub
    echo 2. Authentication required
    echo 3. Network connection issue
    echo 4. Conflicts need to be resolved
    echo.
    echo Try manually:
    echo   git push --set-upstream origin main
    echo.
    pause
    exit /b 1
) else (
    echo.
    echo ====================================
    echo Upload Successful!
    echo ====================================
    echo.
    echo Project pushed to:
    echo https://github.com/suci135/ESP32-Watch
    echo.
    echo Branch: main
    echo All changes are now on the main branch
    echo.
)

pause
