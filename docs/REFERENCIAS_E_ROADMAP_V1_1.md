# N33M1A5 VJGenerative — Pesquisa técnica e direção artística V1.1
**Data:** 20/10/2026 · **Status:** Pesquisa/proposta — nenhuma alteração nesta etapa à DLL V1.0.

## O objetivo
Criar loops gerativos com identidade visual própria e qualidade para shows, igrejas, casamentos e eventos corporativos, mantendo integração estável com Resolume FFGL Source. Evitar aumentar a quantidade de engines sem melhorar a qualidade.

## Auditoria da V1.0 (arquivos reais)
`VJGenerative.cpp` + `shaders.h`:
- Seis engines: **Nevoa, Tunel, Lasers, Celulas, Aurora, Orbes**.
- Nove paletas; onze parâmetros FFGL (Motor, Paleta, Velocidade, Escala, Intensidade, Matiz, Simetria, Semente, BPM, Pulso, Contraste).
- O código usa OpenGL GLSL 4.10; um fragment shader para todas as engines.
- `iTime` é tempo acumulado via `std::chrono::steady_clock` (dependente do parâmetro Speed). `uBeat` é calculado a partir de BPM local (60–180).
- Não há garantia de seamless loop: o tempo entra linearmente em funções como noise/fBm.
- Linhas e lasers não usam antialias derivativo `fwidth` no shader atual.
- Presets, variantes estruturais, correção de layout para ultra-wide e medição automática de GPU ainda não foram implementados.

## Fontes consultadas

### Integração e estabilidade
1. [Resolume FFGL oficial](https://github.com/resolume/ffgl) — exemplos Source, API, classes para parâmetros, events, comunicação com host.
2. [FFGLPluginSDK.h](https://github.com/resolume/ffgl/blob/master/source/lib/ffgl/FFGLPluginSDK.h) — `SetBeatInfo(bpm,barPhase)`, `SetTime`, ciclo de vida dos plugins.
3. [Manual Resolume — Parameter Animation](https://resolume.com/support/en/parameter-animation) — BPM Sync, FFT externo/interno, controle por parâmetros, presets.
4. [Manual Resolume — Wire FFT](https://resolume.com/support/de/wire-fft) — panorama de análise de frequências e controles reativos.
5. [Manual Resolume — Wire UI](https://resolume.com/support/en/wire-user-interface) — grupos de parâmetros, painéis limpos e presets.

### Técnicas de shader e GPU
6. [The Book of Shaders](https://thebookofshaders.com/?lan=pt) — fBm, ruído, Voronoi, paletas, padrões e shaping functions.
7. [GLSL fwidth](https://docs.gl/sl4/fwidth) — antialiasing de linhas/bordas e elementos repetitivos.
8. [Khronos — OpenGL Performance](https://wikis.khronos.org/opengl/Performance) — separar gargalos CPU/fragment/pixel e medir em resoluções diferentes.
9. [TouchDesigner — Instancing](https://derivative.ca/UserGuide/Instance) — estruturas 3D replicadas e controladas por dados, referência conceitual para geometria de fundo.
10. [TouchDesigner — Creating a Feedback](https://learn.derivative.ca/courses/100-fundamentals/lessons/102-tops-working-with-images/topic/creating-a-feedback/) — persistência de imagem, trilhas e feedback de quadros anteriores (exigiria FBO/passes).
11. [TouchDesigner — Audio-reactive & Instancing](https://derivative.ca/community-post/tutorial/audio-reactivity-and-instancing-tutorial/69454) — mandalas e geometrias conduzidas por áudio.

### Direção visual e loops profissionais (apenas referências estéticas)
12. [Pixabay — VJ Loops](https://pixabay.com/pt/videos/search/vj%20loops/) — variedade geral.
13. [Motion Array — VJ Loops](https://motionarray.com/browse/search/vj-loop/) — túnel, kaleidoscópio, 3D, dark/neon e visualizações rítmicas.
14. [Motion Array — Kaleidoscope](https://motionarray.com/browse/search/kaleidoscope/) — mandalas geométricas e espelhamento.
15. [Motion Array — Looping Tunnel Background](https://blog.motionarray.com/learn/after-effects/after-effects-looping-backgrounds/) — câmera/profundidade e repetição estrutural.
16. [Envato — VJ Loop Golden Kaleidoscope](https://elements.envato.com/vj-loop-golden-kaleidescope-FLM4DZR) — aparência metálica, detalhes e contraste.
17. [Story Loop — VJ Loops](https://storyloop.com/type/vj-loop/) — feixes abstratos, palco, atmosfera e movimento.
18. [VJLoops AI — Abstract](https://vjloops.ai/video/abstract/) — taxonomia por estética/evento (wedding, ambient, geometric, tunnel etc.).

As referências comerciais servem para análise de composição/material/ritmo, nunca para copiar, incorporar vídeos ou reproduzir trabalhos de terceiros.

## Melhorias priorizadas para V1.1

### P0 — qualidade visual e segurança
1. **Preservar 6 motores**, mas diferenciar assinatura visual com 4 modos cada: CLEAN / DEPTH / ENERGY / CINEMATIC.
2. **Antialias adaptativo** com `fwidth` nos lasers, grades, mandalas, halos e linhas; remover serrilhado cintilante em LED.
3. **Loop seamless REAL**, por parâmetro `Loop Seconds` (8 / 12 / 16 / 24). Introduzir `phase=2*pi*fract(time/duration)` e trajetórias fechadas em noise; não basta reiniciar o relógio.
4. **BPM do host FFGL** via `bpm` e `barPhase` de `SetBeatInfo` quando disponível; `BPM manual` como fallback. Sem prometer FFT interno automático: FFT pode modular parâmetros nativamente no Resolume.
5. **Adaptação de composição**: 16:9, ultrawide 3360x1008 e FIT/FILL; center-clean, edge-energy.
6. **GLSL runtime checker**: extrair shaders de `shaders.h` e validar sintaxe no CI com `glslangValidator`, além do MSBuild C++; testes reais no Resolume continuam necessários.

### P1 — mudanças específicas por motor
| Motor | Renderização proposta | Controles realmente transformadores |
|---|---|---|
| Névoa | Fluid / liquid glass / silk; curvas volumétricas falsas e highlights anisotrópicos | Warp, Viscosity, Gloss, Flow, Light Direction |
| Túnel | Rodapé/corredor geométrico 3D aparente com paredes/reflexos, anéis com espessura e câmera | Geometry (ring/hex/grid), Depth, Camera Move, Reflectivity |
| Lasers | Feixes volumétricos com haze e foco, scan ágil sem “linhas infinitamente finas” | Spread, Beam Width, Scan, Haze, Strobe (com limite seguro) |
| Células | Voronoi com bordas suaves, facetado, morph metálico/cristalino | Cell Count, Bevel, Morph, Metallic |
| Aurora | Cortinas sobrepostas, fade por camada, parallax e atmosferas cromáticas coerentes | Layers, Drift, Depth, Softness |
| Orbes | Bokeh por profundidade/lente, tamanhos e highlights distintos, foreground/midground/background | Focus, Bokeh Size, DOF, Density, Drift |

### P2 — experiência de operação
- Um menu `LOOK` por motor com 4 variações curadas e parâmetros essenciais.
- Presets por uso: **WEDDING**, **WORSHIP**, **CORPORATE**, **PARTY**, **VJ LOOPS** (presets de parâmetros, não novas engines).
- Macro `SURPRISE` limitada a paletas/variações coerentes por segmento; `MUTATE` com amplitude pequena.
- Em primeiro momento operar pelo **próprio Resolume** e sua animação nativa; qualquer painel externo deverá ser opcional e somente depois da estabilidade do FFGL.

### P3 — performance e testes
- Perfis `LIVE` e `HIGH`: variar número de octaves, luzes, layers, passos de render; confirmar ganho real em hardware, não apenas um nome na UI.
- Testar pelo menos 1920x1080 / 3840x2160 / 3360x1008, 30 e 60 fps, animação de parâmetros e 2–4 instâncias.
- Cobrar desempenho em GPU real, sem prometer 100% de utilização ou 60 fps em qualquer projeto.
- Coletar captura de quadro, log de shader, FPS e impressão visual antes de declarar “pronto para show”.
- Fixar uma revisão testada do SDK FFGL (commit/tag), em vez de depender indefinidamente do branch `master`.

## Critérios de aceite sugeridos
1. DLL carrega como FFGL Source sem crash, gera todos os seis motores e abre novamente após reiniciar Resolume.
2. 24 looks (6 x 4) visivelmente diferentes em estrutura/material/movimento, não apenas troca de cor.
3. `BPM Host` acompanha mudanças de BPM e fase do Resolume; fallback manual verificável.
4. Seamless: comparar quadros de início/fim ao longo de um ciclo com tempo exato (considerar ciclo amostrado e tolerância).
5. Visual sem flicker/serrilhado perceptível em LED ultrawide.
6. Build CI reproduzível com validação do C++ e do GLSL; DLL testada em Windows x64/Resolume.

## Ordem de implementação
**Etapa 1:** corrigir Sync/Loop e antialias, mantendo a V1.0 intacta.
**Etapa 2:** refazer materiais/profundidade em Túnel, Lasers, Névoa.
**Etapa 3:** refazer Aurora, Células, Orbes.
**Etapa 4:** presets e controle simplificado, benchmark real.
