# Recuperação da V1.0 original — problema de instalação (20/10/2026)

**Objetivo:** voltar à DLL V1.0 original, compilada e previamente utilizada, sem alterar shaders, engines ou parâmetros.

## Problema encontrado
Os antigos instaladores copiavam para `Documentos\Resolume\Extra Effects`. Em instalação usual de **Resolume Arena**, a pasta verificada é `Documentos\Resolume Arena\Extra Effects`. No Avenue, é `Documentos\Resolume Avenue\Extra Effects`. Verifique em *Preferences > Video > FFGL Directories* se o host foi configurado para uma pasta personalizada. Veja a [documentação oficial do Resolume](https://resolume.com/support/en/directory-list) e [More Effects](https://resolume.com/support/en/effects).

## DLL original preservada
- Nome: `VJGenerativeV1.dll`
- SHA-256: `4545c59e220511bf4cce3f8ba02337149b22e57a0b213804dd6e7f81082e4758`
- Arquitetura: PE32+ Windows x64, exports `plugMain` e `SetLogCallback`.
- Código fonte original preservado no histórico do repositório, commit `458999dd09ca10acb76092af73a9515627197b22`.

## Instalador de recuperação entregue na conversa ChatGPT
Pacote: `N33M1A5_RECUPERAR_V1_0_ORIGINAL_ARENA_CORRIGIDO.zip`.

1. Fechar Resolume.
2. Extrair o ZIP.
3. Executar `01_RECUPERAR_V1_ORIGINAL_ARENA.bat`.
4. A instalação usa a pasta **Documentos real do Windows**, inclusive se ela estiver redirecionada para OneDrive.
5. Versões conflitantes na pasta alvo são movidas a uma pasta de quarentena fora da busca do Resolume; os projetos ficam intocados.
6. O instalador confere SHA-256 na origem e no destino.
7. Reiniciar o Resolume e verificar **Sources > VJ Generative**.

Se o Source não aparece ou fica preto, executar `03_DIAGNOSTICO_SOMENTE_LEITURA.bat` e analisar o relatório; o script busca versões na pasta alvo, consulta as DLLs do runtime MSVC e tenta localizar logs. Não prometer “funcionando” antes do teste dentro do host real.

**Dependências:** caso falte runtime VC++, instalar x64 pelo [link Microsoft](https://aka.ms/vc14/vc_redist.x64.exe).

**Importante:** Não executar antigos desinstaladores após recuperar a DLL, pois apagam também a versão que acabou de ser instalada.
