//mevcut dosyaların derlenmesi




@echo off
echo Derleniyor...
gcc  main.c video.c cpu.c rom.c ppu.c opcode_transfer.c opcode_logical.c opcode_arithmetic.c opcode_branch.c  opcode_misc.c opcode_stack.c opcode_store.c -o zupzup.exe

if %errorlevel% neq 0 (
    echo Derleme hatasi oluşrtu.
    pause
    exit /b
)

echo Calistiriliyor...
build.exe
pause


