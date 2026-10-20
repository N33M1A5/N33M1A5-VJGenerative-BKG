# N33M1A5 VJ Gen Unified V1.3 — 24 motores, paletas realmente funcionais

## Por que as versões anteriores não funcionavam
1. Os motores extras dependiam de três parâmetros/bancos separados e de uma variável interna `activeBank`. As atualizações do controle clássico `Motor` podiam restaurar o banco zero, fazendo os novos botões parecerem não funcionar.
2. Os controles RGBA eram ligados apenas ao shader de expansão e não modificavam a função `pal()` dos seis motores originais. A opção Original Palette mantinha as cores da V1.0.1, mas RGBA não alterava as paletas fixas.
3. A declaração `SetParamInfof` consultava GetFloatParameter antes de os valores padrão de alguns parâmetros serem inicializados, criando comportamento instável quando o host criava ou restaurava clipes.

## Correções
- **Motor: um único parâmetro FFGL com 24 opções.** IDs 0–5 = Nevoa, Tunel, Lasers, Celulas, Aurora, Orbes, exatamente na ordem V1.0.1. IDs 6–23 = novos motores em sequência, com divisores FFGL antes de 6, 12, 18. Não há mais `activeBank`.
- Motor único controla shaders diferentes internamente, mas há **uma interface unificada** de seleção e todos compartilham Velocidade, Escala, Intensidade, Matiz, Simetria, Semente, BPM, Pulso e Contraste. Em `Solid RGBA`, parâmetros de geometria não produzem efeito (por definição).
- **Paleta fixa:** as nove opções originais continuam, sem modificar `shaders.h` da V1.0.1.
- **Paleta personalizada:** `Modo de Cor = RGBA A`, `Paleta A+B` ou `A+B Animada` utiliza canais RGBA A e B *dentro da função pal()* de **todos os 24 motores**, incluindo os seis originais. Permite Crossfade, Add, Screen e Multiply, com Mix Cor A/B. Selecionar Original retorna às nove paletas fixas.
- **Shader original intacto:** quando Motor 0..5 e Modo Original, Opacidade 100% e Realce 0, usa literalmente o shader da V1.0.1, sem processamento extra. Customizar cor/opacidade/glow compila variante da mesma fonte original alterando apenas a paleta/saída, nunca a matemática original dos seis motores.
- **Parâmetros têm valores padrão inicializados antes de serem anunciados ao FFGL**, evitando registros com valores indeterminados.
- **Direção de movimento:** inverte sinal da progressão do tempo para todos os motores; padrão desativado preserva o relógio original.
- **Canvas Auto:** utiliza a resolução real do framebuffer do Resolume e calcula proceduralmente, compatível com saída personalizada como 3360 × 1008. Presets 16:9 / 9:16 / 32:9 / Custom controlam aspecto virtual quando ativados.
- **Teste de Shader GPU:** ativa uma imagem quadriculada magenta/ciano na expansão para facilitar diagnóstico; o próprio parâmetro também reporta GPU OK / GPU ERRO como texto de status.

## Testes de CI
- SHA Git de `shaders.h` V1.0.1 preservado: `488f842c06fe9fa616d71b033e32c573fe90179a`.
- Validação GLSL 410 dos programas original, original com paleta personalizada, e expansão.
- Renderização real em Mesa EGL/OpenGL 4.5 por software: **6 motores originais + 18 novos** produzindo pixels não pretos; mudança de cor visível com paleta personalizada nos dois grupos; mudança visual do Particle Wave entre dois instantes.
- Compilação Windows x64 com SDK oficial FreeFrameGL Resolume e exports `plugMain` / `SetLogCallback`.
- **Não é um teste físico no Resolume/RTX**, portanto confirmar no Arena antes de uso ao vivo.

## Segurança e instalação
O V1.3 é distribuído inicialmente sob **nome de teste único `VJ Gen Unified`, FFGL ID `VJ15`, arquivo `VJGenerativeUnified.dll`**. Trata-se de **um só Source de 24 motores**, não três plugins ou bancos. A versão estável `VJ Generative` (V1.0.1) continua intacta para fallback. O instalador move as versões experimentais antigas para backup, não exclui seus projetos.

A identificação é diferente da V1.0.1 porque a posição dos parâmetros RGBA foi inserida abaixo de Paleta. Reutilizar o FFGL ID antigo faria presets armazenados mapearem controles antigos em índices novos, com risco de alterar composições salvas. A troca definitiva do plugin padrão deve ser considerada somente após testes em seus projetos.

## Sequência de validação sugerida no Resolume Arena
1. Feche o Arena, instale via `INSTALAR_UNIFIED_V1_3.bat`, reabra.
2. Crie um Source novo **VJ Gen Unified**. Verifique os motores 0–5. Eles devem aparecer como na V1.0.1.
3. Selecione Crystal (motor 6), depois Digital Rain (último motor). Qualquer outro motor novo deve responder diretamente sem habilitar outro botão.
4. Volte para Nevoa; selecione `Modo de Cor -> Paleta A+B`, configure A=R1 G0 B0 e B=R0 G0 B1. A imagem deve mudar de paleta. Retorne a Original, e o visual V1.0.1 retorna.
5. Ajuste Paleta (9 opções), Velocidade, Escala, BPM, Simetria, Direção e Glow.
6. Ative `Teste de Shader GPU` em motor novo: deve aparecer padrao magenta/ciano. Desative antes do show.
7. Verifique 1920×1080 e 3360×1008, FPS e diferentes camadas.

## Referências de direção artística (sem código/vídeos copiados)
- https://resolume.com/footage
- https://vjloopsfarm.com/
- https://limeartgroup.com/shop/
- https://storyloop.com/motion-collections/
- https://beeple-crap.com/vjloops
- https://vjlab.com.br/generative.html#demo
