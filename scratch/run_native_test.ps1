param(
    [int]$Timeout = 10,
    [int]$Display = 1
)

$env:PATH = "C:\msys64\clang64\bin;$env:PATH"
$gameDir = "C:\Users\Yohan\Desktop\Uncharted\CUSA02343"
$elfPath = "out\big2\eboot.elf"

Write-Host "[NativeTest] Lancement de out\uncharted2-native.exe sur l'ecran $Display (Fermeture auto: ${Timeout}s)..."
$p = Start-Process -FilePath "out\uncharted2-native.exe" -ArgumentList "--game `"$gameDir`" --elf `"$elfPath`" --display $Display --timeout $Timeout" -NoNewWindow -Wait -PassThru
Write-Host "[NativeTest] Code de sortie: $($p.ExitCode)"
