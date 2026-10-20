#requires -Version 5.1
$ErrorActionPreference='Stop'
Write-Host ""
Write-Host "N33M1A5 VJ GENERATIVE UNIFIED V1.3 - RECUPERACAO DE CONTROLES" -ForegroundColor Cyan
Write-Host "Unico seletor Motor (24 opcoes). Preserva o VJ Generative V1.0.1."

$running=Get-Process -ErrorAction SilentlyContinue | Where-Object { $_.ProcessName -match '^(Arena|Avenue|Resolume Arena|Resolume Avenue)$' }
if($running){throw 'Feche completamente o Resolume Arena/Avenue antes de instalar.'}

$inputDll=Join-Path $PSScriptRoot 'VJGenerativeUnified.dll'
if(-not(Test-Path -LiteralPath $inputDll -PathType Leaf)){throw "Arquivo de origem ausente: $inputDll"}
$docs=[Environment]::GetFolderPath([Environment+SpecialFolder]::MyDocuments)
if([string]::IsNullOrWhiteSpace($docs)){throw 'Nao foi possivel descobrir a pasta Documentos atual.'}
$effects=Join-Path $docs 'Resolume Arena\Extra Effects'
New-Item -ItemType Directory -Path $effects -Force | Out-Null
$backupDir=Join-Path $docs ('N33M1A5\Backups\Unificado-'+(Get-Date -Format 'yyyyMMdd-HHmmss'))
New-Item -ItemType Directory -Path $backupDir -Force | Out-Null

# Do not touch the original stable V1.0.1 DLL.
$legacy=Join-Path $effects 'VJGenerativeV1.dll'
if(Test-Path -LiteralPath $legacy -PathType Leaf){
    Write-Host 'V1.0.1 original encontrada e preservada.' -ForegroundColor Green
}else{
    Write-Warning 'V1.0.1 nao foi encontrada neste diretorio. Confira seus diretorios FFGL no Resolume.'
}

# Quarantine old, broken experiments from the same plugin folder.
$previous=@(
  'VJGenerativeExpanded.dll',
  'VJGenerativeExpanded121.dll',
  'VJGenerativeExpanded122.dll',
  'VJGenerativeV11.dll',
  'VJGenerativeV11Safe.dll',
  'VJGenerativeV101.dll'
)
foreach($filename in $previous){
    $path=Join-Path $effects $filename
    if(Test-Path -LiteralPath $path -PathType Leaf){
        Move-Item -LiteralPath $path -Destination (Join-Path $backupDir $filename) -Force
        Write-Host "Versao experimental anterior isolada: $filename"
    }
}
$target=Join-Path $effects 'VJGenerativeUnified.dll'
if(Test-Path -LiteralPath $target -PathType Leaf){
    Move-Item -LiteralPath $target -Destination (Join-Path $backupDir 'VJGenerativeUnified-old.dll') -Force
}
Copy-Item -LiteralPath $inputDll -Destination $target -Force
$originHash=(Get-FileHash -LiteralPath $inputDll -Algorithm SHA256).Hash
$targetHash=(Get-FileHash -LiteralPath $target -Algorithm SHA256).Hash
if($originHash -ne $targetHash){throw 'Arquivo copiado nao passou na verificacao SHA256!'}
Write-Host ""
Write-Host "INSTALADO E VERIFICADO" -ForegroundColor Green
Write-Host "Pasta: $target"
Write-Host "Nome no Resolume: VJ Gen Unified"
Write-Host "24 motores no mesmo seletor + 9 paletas / paleta personalizada RGB A e B"
Write-Host "Shaders GPU: Controle 'Teste de Shader GPU' (padrao desligado)."
Write-Host "Backups das versoes anteriores: $backupDir"
Write-Host ""
Write-Host "Se Sources nao mostrar VJ Gen Unified, confira Preferences > Video > FFGL Directories."
