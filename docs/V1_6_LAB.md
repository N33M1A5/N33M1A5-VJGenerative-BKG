# VJ Gen V1.6 LAB — primeiro lote

**Escopo final: 82 motores, sendo 7 existentes e 75 novos, um para cada URL única enviada.** Este lote de desenvolvimento disponibiliza 10: os sete existentes e Flow Dark Glass, Spotlight 1 e Stained Glass Sacred Bubble Gum. Os demais 72 novos motores permanecem pendentes. Não há botões que fingem implementar motores ausentes.

## Instalação da DLL de teste

Após o workflow concluir com sucesso, baixar o artefato `N33-VJ-V1.6-LAB-Windows-x64`, extrair `VJGenerativeV16Lab.dll` e copiar para a pasta FFGL configurada no Resolume, com o aplicativo fechado. Ela aparece como **VJ Gen V1.6 LAB**, ID FFGL `VJ61`, distinto da V1.4.1. A DLL estável deve continuar disponível.

Na faixa **Motores V1.6**, selecionar Flow Dark Glass, Spotlight 1 ou Sacred Bubble Gum. **Base** retorna aos controles dos sete motores existentes. O nome completo Stained Glass Sacred Bubble Gum foi abreviado no botão por espaço.

**Cores V1.6** permite manter as cores da referência ou usar uma das 36 paletas. Os modos RGBA A e A+B prevalecem quando escolhidos. Período controla o ciclo geométrico de 4 a 120 segundos. Com Pulso ativo, fechar um loop também depende de compatibilizar o BPM com esse período. Qualidade define render interno a 50%, 75% ou 100% das dimensões de saída, seguido de composição para o viewport real.

## O que foi implementado

- Três algoritmos GLSL independentes e multipass com intermediários RGBA16F.
- Vitral com lancetas, juntas, rosáceas pequenas e iluminação transmitida em movimento.
- Spotlight com vidro canelado e luzes móveis; a referência é da coleção Glass.
- Flow Dark Glass com interseção em perspectiva de superfície ondulada, oclusão das dobras, reflexos e dispersão cromática aproximada.
- Tempo e batidas acumulados no C++, mudança de velocidade sem reinício do relógio.
- Paletas, RGBA, contraste, alpha, loop, proporção do canvas e controles compartilhados.
- Recursos OpenGL por instância, realocação de resolução e restauração dos estados do host.

## Verificação e limitações

O renderizador C++ foi executado em um host de teste EGL/Mesa llvmpipe; foram verificados resize, restauração de estados, 36 paletas e alpha dos modos RGBA. Os shaders foram renderizados em 1080p, LED 3360×1008, 4K e retrato, com testes de movimento, controles e fechamento do ciclo. Os vídeos anexados são renderizações reais.

A compilação Windows é feita no GitHub Actions e só é considerada concluída quando o workflow terminar com sucesso. **Ainda não foi testado no Resolume nem na GPU do operador.** Não há certificação de 60 fps ou de equivalência visual final às referências. Os três visuais continuam sujeitos a refinamento.

Foram examinados seis quadros distribuídos pelas prévias públicas de 15 segundos de cada referência. Isso melhora a análise da evolução, mas não equivale à inspeção de todos os frames dos arquivos originais. As prévias do catálogo não são incluídas na DLL nem no repositório.

As próximas implementações devem seguir o [registro completo de 82 motores](REFERENCIAS_STORYLOOP_ATUALIZADAS.md), preservando cada URL como um motor próprio. Walls — Names of God ainda requer edição real de texto, incluindo acentos. Os sete motores estáveis, seus arquivos-fonte e a compilação V1.4.1 não foram alterados.
