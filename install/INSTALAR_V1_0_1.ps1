#requires -Version 5.1
$ErrorActionPreference = 'Stop'
Write-Host ''
Write-Host 'N33M1A5 VJ Generative 1.0.1 - Substituir V1.0' -ForegroundColor Cyan
Write-Host 'A identificacao do plugin e os 6 motores originais foram preservados.'
$running = Get-Process -ErrorAction SilentlyContinue | Where-Object {
    $_.ProcessName -match '^(Arena|Avenue|Resolume Arena|Resolume Avenue)$'
}
if ($running) {
    Write-Host 'Feche o Resolume Arena/Avenue antes de instalar.' -ForegroundColor Yellow
    throw 'Resolume em execucao. Instalacao cancelada.'
}
$dll = Join-Path $PSScriptRoot 'VJGenerativeV1.dll'
if (-not (Test-Path -LiteralPath $dll -PathType Leaf)) {
    throw "DLL ausente no pacote: $dll"
}
$docs = [Environment]::GetFolderPath([Environment+SpecialFolder]::MyDocuments)
if (-not $docs) { throw 'Nao foi possivel localizar a pasta Documentos do Windows.' }
$targetFolder = Join-Path $docs 'Resolume Arena\Extra Effects'
New-Item -ItemType Directory -Force -Path $targetFolder | Out-Null
$target = Join-Path $targetFolder 'VJGenerativeV1.dll'
$backupDir = Join-Path $docs 'N33M1A5\Backups'
New-Item -ItemType Directory -Force -Path $backupDir | Out-Null
$stamp = Get-Date -Format 'yyyyMMdd-HHmmss'
if (Test-Path -LiteralPath $target -PathType Leaf) {
    $backup = Join-Path $backupDir "VJGenerativeV1-previous-$stamp.dll"
    Copy-Item -LiteralPath $target -Destination $backup -Force
    Write-Host "V1.0 anterior preservada em: $backup" -ForegroundColor DarkGray
}
Copy-Item -LiteralPath $dll -Destination $target -Force
$sourceHash = (Get-FileHash -LiteralPath $dll -Algorithm SHA256).Hash
$installedHash = (Get-FileHash -LiteralPath $target -Algorithm SHA256).Hash
if ($sourceHash -ne $installedHash) { throw 'Falha de integridade: DLL de destino diferente da origem.' }
# Remove somente a variante paralela antiga, se existir na MESMA pasta.
$oldVariant = Join-Path $targetFolder 'VJGenerativeV101.dll'
if (Test-Path -LiteralPath $oldVariant -PathType Leaf) {
    $backup = Join-Path $backupDir "VJGenerativeV101-previous-$stamp.dll"
    Move-Item -LiteralPath $oldVariant -Destination $backup -Force
    Write-Host "Variante antiga com outro nome movida para: $backup" -ForegroundColor DarkGray
}
Write-Host ''
Write-Host 'Instalacao concluida e SHA256 confirmado.' -ForegroundColor Green
Write-Host "Instalado: $target"
Write-Host 'Nome no Resolume: VJ Generative'
Write-Host 'Versao: 1.0.1'
Write-Host 'Reabra Resolume Arena. Em caso de pasta FFGL personalizada, use a pasta configurada no Resolume.'
Write-Host ''
