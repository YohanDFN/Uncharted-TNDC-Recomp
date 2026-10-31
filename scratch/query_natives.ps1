$text = Get-Content -Path "tools/u2_map/u2_engine_map.json" -Raw
$obj = ConvertFrom-Json -InputObject $text
Write-Host "Total props:" ($obj.dc_natives.psobject.Properties | Measure-Object).Count
$first = $obj.dc_natives.psobject.Properties | Select-Object -First 5
foreach ($p in $first) {
    Write-Host $p.Name ":" ($p.Value | ConvertTo-Json -Compress)
}
