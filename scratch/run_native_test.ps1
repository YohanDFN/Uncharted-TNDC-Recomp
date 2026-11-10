param(
    [int]$Timeout = 0
)

$env:PATH = "C:\msys64\clang64\bin;$env:PATH"
$gameDir = "C:\Users\Yohan\Desktop\Uncharted\CUSA02343"
$elfPath = "out\big2\eboot.elf"

if ($Timeout -gt 0) {
    Write-Host "[NativeTest] Launching out\uncharted2-native.exe with timeout ${Timeout}s..."
    & out\uncharted2-native.exe --game $gameDir --elf $elfPath --timeout $Timeout
} else {
    Write-Host "[NativeTest] Launching out\uncharted2-native.exe (Mode interactif - Appuyez sur ECHAP pour quitter)..."
    & out\uncharted2-native.exe --game $gameDir --elf $elfPath
}
Write-Host "[NativeTest] Exit code: $LASTEXITCODE"
