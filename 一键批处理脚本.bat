@echo off
:: 第一行就切换UTF-8编码，解决中文乱码
chcp 65001 >nul
setlocal enabledelayedexpansion
echo ==============================================
echo        Single Folder Upload GitHub Tool
echo Put this bat into your project root folder
echo ==============================================
echo.

:: 你的GitHub用户名，无需修改
set "GITHUB_USER=Wayne1114884232"

:: 获取当前文件夹名称作为仓库名
for %%i in ("%cd%") do set "REPO_NAME=%%~nxi"
echo Current folder name(Repo Name): !REPO_NAME!
echo GitHub Repo URL: https://github.com/!GITHUB_USER!/!REPO_NAME!.git
echo.

:: 检测Git是否安装
git --version >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: Git not found, install Git and add to PATH first!
    pause
    exit /b
)

:: 清理子目录内嵌套.git文件夹
echo Cleaning nested .git folders...
for /d %%d in (*) do (
    if exist "%%d\.git" (
        rmdir /s /q "%%d\.git" 2>nul
        echo Cleaned: %%d\.git
    )
)
echo.

:: 初始化仓库
echo Step 1: git init
git init >nul 2>&1

:: 添加全部文件
echo Step 2: git add all files
git add .

:: 提交代码，捕获提交结果
echo Step 3: git commit
git commit -m "Initial upload project !REPO_NAME!" --allow-empty >commit_log.txt 2>&1
if %errorlevel% neq 0 (
    echo WARNING: Commit failed, no files to commit or error occurred
    del commit_log.txt
    pause
    exit /b
)
del commit_log.txt

:: 重命名默认分支为main
git branch -M main

:: 绑定远程仓库
echo Step 4: bind remote origin
git remote remove origin 2>nul
git remote add origin https://github.com/!GITHUB_USER!/!REPO_NAME!.git

:: 推送代码
echo Step 5: push to GitHub main branch
git push -u origin main

echo.
if %errorlevel% equ 0 (
    echo ==============================================
    echo SUCCESS: Upload finished, Repo: !REPO_NAME!
    echo ==============================================
) else (
    echo FAILED! Check these points:
    echo 1. Create empty repo with same name on GitHub, DO NOT check README
    echo 2. No single file larger than 100MB (zip/dll/exe)
    echo 3. Network connection to GitHub
)
echo.
pause