$text = Get-Content -Path "tools/u2_map/u2_engine_map.json" -Raw
$obj = ConvertFrom-Json -InputObject $text
$props = $obj.dc_natives.psobject.Properties

$stubs = @()
$pure_math = @()
$others = @()

foreach ($p in $props) {
    $v = $p.Value
    $pro = $v.prologue_hex
    if ($pro.StartsWith("31c0c3")) {
        $stubs += [PSCustomObject]@{ Name=$v.cpp_name; Offset=$v.func_offset; Type="NoOp0" }
    } elseif ($pro -match "^b8([0-9a-f]{8})c3") {
        $stubs += [PSCustomObject]@{ Name=$v.cpp_name; Offset=$v.func_offset; Type="ReturnConst" }
    } elseif ($pro.StartsWith("c5fa1006") -or $pro.StartsWith("c5f857c0")) {
        $pure_math += [PSCustomObject]@{ Name=$v.cpp_name; Offset=$v.func_offset; Type="Math" }
    }
}

Write-Host "Found $($stubs.Count) stubs/constants:"
foreach ($s in $stubs) {
    Write-Host ("  {0,-35} {1} ({2})" -f $s.Name, $s.Offset, $s.Type)
}

Write-Host "`nFound $($pure_math.Count) math candidates:"
foreach ($m in $pure_math) {
    Write-Host ("  {0,-35} {1} ({2})" -f $m.Name, $m.Offset, $m.Type)
}

