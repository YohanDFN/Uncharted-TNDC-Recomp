$text = Get-Content -Path "tools/u2_map/u2_engine_map.json" -Raw
$obj = ConvertFrom-Json -InputObject $text
$matched = $obj.assertions | Where-Object { $_.line -eq 1361 }
Write-Host "Matched line 1361: $($matched.Count)"
foreach ($m in $matched) {
    Write-Host ($m | ConvertTo-Json -Compress)
}
