$ErrorActionPreference = 'Stop'
$taskRoot = Split-Path -Parent $PSScriptRoot
$taskExe = [IO.Path]::GetFullPath((Join-Path $taskRoot 'out/bb-probe.exe'))
$taskProcess = Get-Process -Name bb-probe | Where-Object { $_.Path -eq $taskExe } | Select-Object -First 1
if (-not $taskProcess -or $taskProcess.MainWindowHandle -eq 0) { throw 'No workspace hybrid window' }
Add-Type @'
using System;
using System.Runtime.InteropServices;
public static class HybridWindowCapture {
    [StructLayout(LayoutKind.Sequential)] public struct Rect { public int Left, Top, Right, Bottom; }
    [DllImport("user32.dll")] public static extern IntPtr GetForegroundWindow();
    [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr handle, out Rect rect);
    [DllImport("user32.dll")] public static extern bool SetProcessDPIAware();
    [DllImport("user32.dll")] public static extern bool PrintWindow(IntPtr handle, IntPtr dc, uint flags);
}
'@
[void][HybridWindowCapture]::SetProcessDPIAware()
$taskHandle = $taskProcess.MainWindowHandle
$taskRect = New-Object HybridWindowCapture+Rect
if (-not [HybridWindowCapture]::GetWindowRect($taskHandle, [ref]$taskRect)) { throw 'Cannot read hybrid window rectangle' }
Add-Type -AssemblyName System.Drawing
$taskBitmap = New-Object Drawing.Bitmap ($taskRect.Right - $taskRect.Left), ($taskRect.Bottom - $taskRect.Top)
$taskGraphics = [Drawing.Graphics]::FromImage($taskBitmap)
try {
    if ([HybridWindowCapture]::GetForegroundWindow() -eq $taskHandle) {
        $taskGraphics.CopyFromScreen($taskRect.Left, $taskRect.Top, 0, 0, $taskBitmap.Size)
        Write-Output 'Source: foreground hybrid window'
    } else {
        $taskDc = $taskGraphics.GetHdc()
        try {
            if (-not [HybridWindowCapture]::PrintWindow($taskHandle, $taskDc, 2)) {
                throw 'Hybrid PrintWindow capture failed'
            }
            Write-Output 'Source: PrintWindow (GPU content may be unavailable)'
        } finally { $taskGraphics.ReleaseHdc($taskDc) }
    }
    $taskOutput = Join-Path $taskRoot 'user/diag_hybrid_window.png'
    $taskBitmap.Save($taskOutput, [Drawing.Imaging.ImageFormat]::Png)
    Write-Output $taskOutput
} finally {
    $taskGraphics.Dispose()
    $taskBitmap.Dispose()
}
