# Watches the C files that the LVGL Pro Editor exports (Ctrl+E / Ctrl+B) and,
# once they stop changing, rebuilds ../inclin_fw and flashes it to the ESP32.
# Started by the VS Code task "ESP32: Auto-flash on export". Stop it with the trash icon.
param(
    [string]$Port = "COM4",
    [int]$QuietSeconds = 2
)

$ui = Split-Path -Parent $PSScriptRoot
$fw = Join-Path (Split-Path -Parent $ui) "inclin_fw"
$skip = '[/\x5C](build|preview-build|preview-bin|sim|[.]vscode)[/\x5C]'

# idf.py refuses to run if a Git Bash / MSYS environment leaked in.
Remove-Item Env:MSYSTEM -ErrorAction SilentlyContinue
. 'C:\Espressif\tools\Microsoft.v6.1.PowerShell_profile.ps1' *> $null

function Get-Stamp {
    Get-ChildItem -Path $ui -Recurse -File -Include *.c, *.h, *.cmake -ErrorAction SilentlyContinue |
        Where-Object { $_.FullName -notmatch $skip } |
        Measure-Object -Property LastWriteTimeUtc -Maximum |
        ForEach-Object { $_.Maximum }
}

$last = Get-Stamp
Write-Host "Watching $ui for exports. Flashing to $Port."

while ($true) {
    Start-Sleep -Seconds 1
    $now = Get-Stamp
    if ($now -eq $last) { continue }

    # Wait until the export has finished writing files.
    do {
        $prev = $now
        Start-Sleep -Seconds $QuietSeconds
        $now = Get-Stamp
    } while ($now -ne $prev)
    $last = $now

    Write-Host "`n=== Export detected at $(Get-Date -Format HH:mm:ss): build & flash ==="
    Push-Location $fw
    idf.py -p $Port build flash
    if ($LASTEXITCODE -eq 0) { Write-Host "=== Flashed, ESP restarted ===" -ForegroundColor Green }
    else { Write-Host "=== Build/flash FAILED (exit $LASTEXITCODE) ===" -ForegroundColor Red }
    Pop-Location
    $last = Get-Stamp
    Write-Host "Watching for the next export..."
}
