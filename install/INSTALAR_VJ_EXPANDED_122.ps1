#requires -Version 5.1
$ErrorActionPreference = 'Stop'
Write-Host ''
Write-Host 'N33M1A5 VJ Expanded V1.2.2 - 3 linhas de motores' -ForegroundColor Cyan
Write-Host 'Mantem VJ Generative V1.0.1 original instalado.'
$running=Get-Process -ErrorAction SilentlyContinue | Where-Object {$_.ProcessName -match '^(Arena|Avenue|Resolume Arena|Resolume Avenue)$'}
if($running){throw 'Feche o Resolume Arena/Avenue antes de instalar.'}
$source=Join-Path $PSScriptRoot 'VJGenerativeExpanded122.dll'
if(-not(Test-Path -LiteralPath $source -PathType Leaf)){throw "DLL nao encontrada: $source"}
$docs=[Environment]::GetFolderPath([Environment+SpecialFolder]::MyDocuments)
if(-not $docs){throw 'O Windows nao informou a pasta Documentos.'}
$destFolder=Join-Path $docs 'Resolume Arena\Extra Effects'
New-Item -Path $destFolder -ItemType Directory -Force | Out-Null
$destination=Join-Path $destFolder 'VJGenerativeExpanded122.dll'
if(Test-Path -LiteralPath $destination -PathType Leaf){
  $backupDir=Join-Path $docs 'N33M1A5\Backups'
  New-Item -Path $backupDir -ItemType Directory -Force | Out-Null
  $backup=Join-Path $backupDir ('VJGenerativeExpanded122-'+(Get-Date -Format 'yyyyMMdd_HHmmss')+'.dll')
  Copy-Item -LiteralPath $destination -Destination $backup -Force
  Write-Host "Copia da versao anterior salva em: $backup" -ForegroundColor DarkGray
}
Copy-Item -LiteralPath $source -Destination $destination -Force
$a=(Get-FileHash -LiteralPath $source -Algorithm SHA256).Hash
$b=(Get-FileHash -LiteralPath $destination -Algorithm SHA256).Hash
if($a -ne $b){throw 'Erro: SHA256 diferente depois da instalacao.'}
Write-Host ''
Write-Host 'INSTALACAO CONCLUIDA, DLL VERIFICADA.' -ForegroundColor Green
Write-Host "Destino: $destination"
Write-Host 'Source novo: VJ Expanded 122'
Write-Host 'Source antigo preservado: VJ Generative (V1.0.1)'
Write-Host 'Outras versoes experimentais nao foram removidas.'
Write-Host ''
Write-Host 'Caso o Source nao apareca, confira Preferences > Video > FFGL Directories.'
