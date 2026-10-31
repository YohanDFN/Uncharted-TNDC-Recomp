$text = Get-Content -Path "tools/u2_map/u2_engine_map.json" -Raw
$obj = ConvertFrom-Json -InputObject $text
$props = $obj.dc_natives.psobject.Properties

$candidates = @()
foreach ($p in $props) {
    $v = $p.Value
    if ($v.cpp_name -match 'Difficulty|Language|Level|Health|Speed|Count|Score|Stat|Combat|Aim|Cover|Fade|Timer|Time|Clock') {
        $candidates += [PSCustomObject]@{
            Name = $v.cpp_name
            Offset = $v.func_offset
            Prologue = $v.prologue_hex.Substring(0, [Math]::Min(16, $v.prologue_hex.Length))
        }
    }
}

Write-Host "Matched candidates: $($candidates.Count)"
foreach ($c in ($candidates | Select-Object -First 30)) {
    Write-Host ("{0,-35} {1} {2}" -f $c.Name, $c.Offset, $c.Prologue)
}

