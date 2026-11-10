$env:PATH = "C:\msys64\clang64\bin;C:\Windows\system32;C:\Windows"
$env:MSYSTEM = "CLANG64"
Remove-Item Env:\VULKAN_SDK -ErrorAction SilentlyContinue

Write-Host "Running CMake with MSYS2 CLANG64 toolchain..."
& "C:\msys64\clang64\bin\cmake.exe" -S gpu -B out/gpu -G Ninja `
    -DCMAKE_BUILD_TYPE=RelWithDebInfo `
    -DBB_PGO=off `
    -DBB_LTO=OFF `
    -DCMAKE_C_COMPILER=C:/msys64/clang64/bin/clang.exe `
    -DCMAKE_CXX_COMPILER=C:/msys64/clang64/bin/clang++.exe `
    -DVulkan_INCLUDE_DIR="C:/msys64/clang64/include" `
    -DVulkan_LIBRARY="C:/msys64/clang64/lib/libvulkan-1.dll.a"

if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake failed"
    exit 1
}

Write-Host "Building bb-probe and uncharted2-native..."
& "C:\msys64\clang64\bin\ninja.exe" -C out/gpu bb-probe uncharted2-native
if ($LASTEXITCODE -ne 0) {
    Write-Error "Ninja build failed"
    exit 1
}

Get-Process -Name "bb-probe" -ErrorAction SilentlyContinue | Stop-Process -Force -ErrorAction SilentlyContinue
Get-Process -Name "uncharted2-native" -ErrorAction SilentlyContinue | Stop-Process -Force -ErrorAction SilentlyContinue
Start-Sleep -Milliseconds 200
Copy-Item out/gpu/bb-probe.exe out/bb-probe.exe -Force -ErrorAction Stop
Copy-Item out/gpu/uncharted2-native.exe out/uncharted2-native.exe -Force -ErrorAction Stop
if (Test-Path dist/uncharted-windows/bin) {
    Copy-Item out/gpu/bb-probe.exe dist/uncharted-windows/bin/bb-probe.exe -Force
    Copy-Item out/gpu/uncharted2-native.exe dist/uncharted-windows/bin/uncharted2-native.exe -Force
}
Write-Host "Build and deployment successful!"

