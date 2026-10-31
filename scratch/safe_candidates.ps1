$text = Get-Content -Path "tools/u2_map/u2_engine_map.json" -Raw
$obj = ConvertFrom-Json -InputObject $text
$props = $obj.dc_natives.psobject.Properties

$offsets = @{}
foreach ($p in $props) {
    $v = $p.Value
    $off = [int64]$v.func_offset
    $offsets[$off] = $v.cpp_name
}

$sortedOffsets = $offsets.Keys | Sort-Object

Write-Host "Checking function sizes..."
$safe_candidates = @()

for ($i = 0; $i -lt $sortedOffsets.Count - 1; $i++) {
    $cur = $sortedOffsets[$i]
    $next = $sortedOffsets[$i+1]
    $diff = $next - $cur
    $name = $offsets[$cur]
    if ($diff -ge 16) {
        $safe_candidates += [PSCustomObject]@{
            Name = $name
            Offset = ("0x{0:X8}" -f $cur)
            Size = $diff
        }
    }
}

Write-Host "Safe size candidates (>= 16 bytes):" $safe_candidates.Count
$math = $safe_candidates | Where-Object { $_.Name -match 'Lerp|Clamp|Quat|Vector|Angle|Min|Max|Time|Fade|Camera|Player|Anim|Sound' }
Write-Host "Filtered candidates:" $math.Count
foreach ($m in ($math | Select-Object -First 40)) {
    Write-Host ("{0,-35} {1} (size {2} bytes)" -f $m.Name, $m.Offset, $m.Size)
}

