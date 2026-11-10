param(
    [int]$Timeout = 3
)

$env:PATH = "C:\msys64\clang64\bin;$env:PATH"
$gameDir = "C:\Users\Yohan\Desktop\Uncharted\CUSA02343"
$elfPath = "out\big2\eboot.elf"

Write-Host "[NativeTest] Launching out\uncharted2-native.exe with timeout ${Timeout}s..."
& out\uncharted2-native.exe --game $gameDir --elf $elfPath --timeout $Timeout
Write-Host "[NativeTest] Exit code: $LASTEXITCODE"
