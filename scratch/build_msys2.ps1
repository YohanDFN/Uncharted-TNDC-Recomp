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

Write-Host "Building bb-probe..."
& "C:\msys64\clang64\bin\ninja.exe" -C out/gpu bb-probe
if ($LASTEXITCODE -ne 0) {
    Write-Error "Ninja build failed"
    exit 1
}

Copy-Item out/gpu/bb-probe.exe out/bb-probe.exe -Force
if (Test-Path dist/uncharted-windows/bin) {
    Copy-Item out/gpu/bb-probe.exe dist/uncharted-windows/bin/bb-probe.exe -Force
}
Write-Host "Build and deployment successful!"

