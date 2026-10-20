#requires -Version 5.1
$ErrorActionPreference='Stop'
Write-Host ''
Write-Host 'N33M1A5 VJ Generative V1.4.1 - Botoes + 36 Paletas' -ForegroundColor Cyan
Write-Host 'Seis motores classicos + apenas Particle Wave. Nao apaga nenhuma versao anterior.'
$running=Get-Process -ErrorAction SilentlyContinue | Where-Object {$_.ProcessName -match '^(Arena|Avenue|Resolume Arena|Resolume Avenue)$'}
if($running){throw 'Feche o Resolume Arena/Avenue antes de instalar.'}
$source=Join-Path $PSScriptRoot 'VJGenerativeV141.dll'
if(-not (Test-Path -LiteralPath $source -PathType Leaf)){throw "DLL nao encontrada: $source"}
$docs=[Environment]::GetFolderPath([Environment+SpecialFolder]::MyDocuments)
if(-not $docs){throw 'Nao foi possivel descobrir a pasta Documentos.'}
$dir=Join-Path $docs 'Resolume Arena\Extra Effects'
New-Item -ItemType Directory -Path $dir -Force | Out-Null
$destination=Join-Path $dir 'VJGenerativeV141.dll'
if(Test-Path -LiteralPath $destination -PathType Leaf){
    $backupDir=Join-Path $docs 'N33M1A5\Backups'
    New-Item -ItemType Directory -Path $backupDir -Force | Out-Null
    $backup=Join-Path $backupDir ('VJGenerativeV141-'+(Get-Date -Format 'yyyyMMdd_HHmmss')+'.dll')
    Copy-Item -LiteralPath $destination -Destination $backup -Force
    Write-Host "Backup da V1.4.1 anterior em: $backup"
}
Copy-Item -LiteralPath $source -Destination $destination -Force
$hash1=(Get-FileHash -LiteralPath $source -Algorithm SHA256).Hash
$hash2=(Get-FileHash -LiteralPath $destination -Algorithm SHA256).Hash
if($hash1 -ne $hash2){throw 'Erro na verificacao SHA256 apos a copia.'}
Write-Host ''
Write-Host 'INSTALACAO VALIDADA COM SUCESSO' -ForegroundColor Green
Write-Host "Destino: $destination"
Write-Host 'Nome no Resolume: VJ Gen V1.4.1'
Write-Host 'Motor: seis botoes originais'
Write-Host 'Motor Adicional: Classicos / Particle Wave'
Write-Host '36 paletas em ordem alfabetica, incluindo as nove originais.'
Write-Host 'Nao remove V1.0.1, V1.3 nem outros plugins.'
Write-Host 'Se nao aparecer, confira Preferences > Video > FFGL Directories.'
