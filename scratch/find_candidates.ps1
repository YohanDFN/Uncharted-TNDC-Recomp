$text = Get-Content -Path "tools/u2_map/u2_engine_map.json" -Raw
$obj = ConvertFrom-Json -InputObject $text
$props = $obj.dc_natives.psobject.Properties

$matches = $props | Where-Object { $_.Name -match 'Lerp|Scale|Clamp|Angle|Sin|Cos|Abs|Min|Max|Vector|Quat|Matrix|Distance|Normalize|Dot|Cross' }
foreach ($m in $matches) {
    $val = $m.Value
    Write-Host ("{0,-35} {1,-30} offset={2} prologue={3}" -f $val.cpp_name, $val.lisp_symbol, $val.func_offset, $val.prologue_hex.Substring(0, [Math]::Min(16, $val.prologue_hex.Length)))
}
