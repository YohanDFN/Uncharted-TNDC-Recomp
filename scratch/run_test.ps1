param(
    [int]$Duration = 8
)

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$rootDir = Split-Path -Parent $scriptDir

$python = "C:\msys64\clang64\bin\python.exe"
if (-not (Test-Path $python)) {
    $python = "python"
}

& $python "$rootDir\scratch\auto_test.py" $Duration
