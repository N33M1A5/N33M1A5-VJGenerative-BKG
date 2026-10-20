#requires -Version 5.1
$ErrorActionPreference = 'Stop'
Write-Host ''
Write-Host 'N33M1A5 VJ GENERATIVE EXPANDED V1.2' -ForegroundColor Cyan
Write-Host 'Este instalador NAO substitui VJGenerativeV1.dll (versao estavel).'
$running = Get-Process -ErrorAction SilentlyContinue | Where-Object { $_.ProcessName -match '^(Arena|Avenue|Resolume Arena|Resolume Avenue)$' }
if($running){ throw 'Feche o Resolume antes de instalar.' }
$source = Join-Path $PSScriptRoot 'VJGenerativeExpanded.dll'
if(-not (Test-Path -LiteralPath $source -PathType Leaf)){ throw "DLL ausente: $source" }
$docs=[Environment]::GetFolderPath([Environment+SpecialFolder]::MyDocuments)
if(-not $docs){ throw 'Pasta Documentos nao encontrada.' }
$dir=Join-Path $docs 'Resolume Arena\Extra Effects'
New-Item -ItemType Directory -Path $dir -Force | Out-Null
$dest=Join-Path $dir 'VJGenerativeExpanded.dll'
if(Test-Path -LiteralPath $dest -PathType Leaf){
    $backupDir=Join-Path $docs 'N33M1A5\Backups'
    New-Item -ItemType Directory -Path $backupDir -Force | Out-Null
    $backup=Join-Path $backupDir ('VJGenerativeExpanded-'+(Get-Date -Format 'yyyyMMdd_HHmmss')+'.dll')
    Copy-Item -LiteralPath $dest -Destination $backup -Force
    Write-Host "Versao Expanded anterior salva em $backup"
}
Copy-Item -LiteralPath $source -Destination $dest -Force
$srcHash=(Get-FileHash -LiteralPath $source -Algorithm SHA256).Hash
$destHash=(Get-FileHash -LiteralPath $dest -Algorithm SHA256).Hash
if($srcHash -ne $destHash){throw 'Falha na verificacao da copia.'}
Write-Host ''
Write-Host 'Instalacao verificada com sucesso.' -ForegroundColor Green
Write-Host "Destino: $dest"
Write-Host 'Fonte no Resolume: VJ Gen Expanded'
Write-Host 'Fonte antiga preservada: VJ Generative'
Write-Host ''
Write-Host 'Se nao aparecer, confira Preferences > Video > FFGL Directories e Visual C++ x64.'
