$text = Get-Content -Path "tools/u2_map/u2_engine_map.json" -Raw
$obj = ConvertFrom-Json -InputObject $text
$props = $obj.dc_natives.psobject.Properties

foreach ($p in $props) {
    $v = $p.Value
    if ($v.cpp_name -match 'Difficulty') {
        Write-Host ("{0,-35} {1} {2}" -f $v.cpp_name, $v.func_offset, $v.lisp_symbol)
    }
}

