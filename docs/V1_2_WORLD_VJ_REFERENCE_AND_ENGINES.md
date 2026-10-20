# N33M1A5 VJ Expansion V1.2 — pesquisa e motores
**Estado:** desenvolvimento/teste no Resolume; V1.0.1 continua sendo a edição estável principal.

## Regra de preservação
- O arquivo `shaders.h` está **100% idêntico** ao shader da V1.0.1, inclusive os seis motores e a correção de costura angular do Túnel.
- O parâmetro original `Motor` mantém os seis nomes: **Nevoa, Tunel, Lasers, Celulas, Aurora, Orbes**.
- O renderer dos seis motores clássicos continua sendo `shader.Compile(kVertexShader,kFragmentShader)`, separado do renderer adicional `expansionShader`.
- A expansão usa um FFGL Source independente `VJ Gen Expanded` (unique ID `VJ12`), evitando substituir o `VJ Generative` que já funciona.

## Novas faixas de botões no Resolume

### GEOMETRIA — 6 motores inéditos
| Botão | Identidade visual |
|---|---|
| Crystal | Cristais radiais e refração estilizada |
| Hex Mosaic | Painéis hexagonais luminosos |
| Mandala | Geometria concêntrica e pétalas |
| Orbital Rings | Anéis independentes orbitando |
| Polygon Prism | Prismas facetados em mosaico |
| Fractal Bloom | Florescimento fractal luminoso |

### LUZ / PALCO — 6 motores inéditos
| Botão | Identidade visual |
|---|---|
| Light Curtains | Cortinas de luz atmosférica |
| Stage Beams | Feixes de palco com névoa |
| Particle Waves | Linhas de partículas animadas |
| Bokeh Lights | Discos de luz desfocados |
| Liquid Plasma | Fluxo líquido com domain warping |
| Neon Ribbons | Fitas neon com trilhas |

### PROFUNDIDADE — 6 motores inéditos
| Botão | Identidade visual |
|---|---|
| Cyber Grid | Grade cibernética em perspectiva |
| Kaleido Pulse | Geometria caleidoscópica pulsante |
| Starfield Warp | Campo espacial em perspectiva |
| Circuit Board | Trilhas e nós de circuitos |
| Digital Rain | Chuva digital em colunas |
| Solid RGBA | Cor sólida, mix e canal alpha |

Cada linha contém uma opção **Classico V1**, para retornar ao render original. Selecionar outro motor em qualquer faixa ativa apenas essa faixa. **Total: 6 clássicos + 18 adicionais = 24 motores**. Não declarar 70 motores nem alegar reproduzir algoritmos de terceiros.

## Novos controles (apenas render VJ)
- **Detail, Depth, Density:** ajustes de detalhe, profundidade e densidade.
- **RGBA Mode:** original, RGBA A, RGBA A+B, RGBA Animated.
- **RGBA Blend:** Crossfade, Add, Screen, Multiply.
- **Cores A/B:** canais R, G, B, Alpha em cada cor.
- **Opacity:** transparência dos motores novos.
- **Canvas:** Auto / Output, 16:9, 9:16, 32:9, Custom.
- **Custom Width / Height:** valores internos de 128 a 8192, exibidos em px.

**Canvas Auto** usa `iResolution` do contexto de render atual do Resolume. O gerador é procedimental, não reescala vídeos. Na opção `Custom`, width/height ajustam a **proporção virtual do shader**; a resolução física permanece configurada em Composition / Advanced Output do Resolume. Formatos como 3360×1008 devem ser testados no host real.

A saída FFGL tem canais RGBA; alpha permite sobreposição ao compositor, mas o modo de blend da camada ainda é controlado pelo Resolume.

## Fontes profissionais para direção artística
1. [Resolume Footage](https://resolume.com/footage) — 6K, geometria, textura, fluxos e motivos para painéis
2. [VJ Loops Farm](https://vjloopsfarm.com/) — caleidoscópios, glitch, túneis, wireframe e palcos
3. [LIME ART GROUP](https://limeartgroup.com/shop/) — palcos ultrawide, DXV/HAP e packs 4K/8K
4. [Story Loop](https://storyloop.com/motion-collections/) — atmosferas suaves, worship, texturas e loops para eventos
5. [Beeple VJ Loops](https://beeple-crap.com/vjloops) — visuais geométricos autorais e arquivos de referência
6. [Motion Array VJ Loops](https://motionarray.com/stock-video/vj-loops-1/) — pesquisa por resolução, tema e framerate
7. [Storyblocks VJ Loops](https://www.storyblocks.com/motion-graphics/time/search/vj-loops) — abstrato, ciência, partículas e tecnologia
8. [Mixkit Abstract Backgrounds](https://mixkit.co/free-stock-video/abstract-background/) — referências gratuitas de cor e movimento
9. [VJLAB Generative](https://vjlab.com.br/generative.html#demo) — referência **de organização** (70 motores em 6 grupos, combinações e controle ao vivo), não código copiado

Todas as implementações do shader nesta expansão foram escritas como novos algoritmos GLSL; **nenhum vídeo ou arquivo proprietário foi incluído**.

## Testes antes de usar em evento
- GitHub CI: SHA exato do shader V1.0.1, compilação GLSL 410 dos dois programas e MSBuild x64.
- No Resolume: verificar 6 originais, cada uma das três faixas, 18 motores, RGBA, composições 1920×1080 e 3360×1008, múltiplos Source clips, trocas de motor durante playback e FPS.
- Manter `VJGenerativeV1.dll` (V1.0.1) disponível. Esta DLL experimental se chama `VJGenerativeExpanded.dll`.
