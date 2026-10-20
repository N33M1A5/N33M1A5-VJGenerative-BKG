#requires -Version 5.1
$ErrorActionPreference = 'Stop'
Write-Host ''
Write-Host 'N33M1A5 VJ GENERATIVE V1.5 — 12 MOTORES DE PALCO' -ForegroundColor Cyan
Write-Host 'Esta versao de teste nao altera a V1.0.1 nem a V1.4.1.'
$processes=Get-Process -ErrorAction SilentlyContinue | Where-Object {
  $_.ProcessName -match '^(Arena|Avenue|Resolume Arena|Resolume Avenue)$'
}
if($processes){throw 'Feche o Resolume Arena/Avenue antes de instalar.'}
$source=Join-Path $PSScriptRoot 'VJGenerativeV150.dll'
if(-not (Test-Path -LiteralPath $source -PathType Leaf)){throw "DLL nao encontrada: $source"}
$docs=[Environment]::GetFolderPath([Environment+SpecialFolder]::MyDocuments)
if([string]::IsNullOrWhiteSpace($docs)){throw 'Nao encontrei a pasta Documentos do Windows.'}
$dir=Join-Path $docs 'Resolume Arena\Extra Effects'
New-Item -Path $dir -ItemType Directory -Force | Out-Null
$target=Join-Path $dir 'VJGenerativeV150.dll'
if(Test-Path -LiteralPath $target -PathType Leaf){
  $bk=Join-Path $docs 'N33M1A5\Backups'
  New-Item -Path $bk -ItemType Directory -Force | Out-Null
  $backup=Join-Path $bk ('VJGenerativeV150-'+(Get-Date -Format 'yyyyMMdd_HHmmss')+'.dll')
  Copy-Item -LiteralPath $target -Destination $backup -Force
  Write-Host "Backup anterior: $backup" -ForegroundColor DarkGray
}
Copy-Item -LiteralPath $source -Destination $target -Force
$sourceHash=(Get-FileHash -LiteralPath $source -Algorithm SHA256).Hash
$targetHash=(Get-FileHash -LiteralPath $target -Algorithm SHA256).Hash
if($sourceHash -ne $targetHash){throw 'ERRO: SHA256 da DLL instalada nao confere.'}
Write-Host ''
Write-Host 'Instalacao concluida e DLL verificada.' -ForegroundColor Green
Write-Host "Pasta: $target"
Write-Host 'Source: VJ Gen V1.5'
Write-Host '6 originais intocados + Particle Wave + 5 novos motores premium.'
Write-Host '36 paletas e editor RGBA. A V1.0.1 continua intacta.'
Write-Host 'Se nao aparecer: Preferences > Video > FFGL Directories.'
