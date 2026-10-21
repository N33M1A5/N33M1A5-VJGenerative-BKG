# N33M1A5 VJ Generative — padrão Story Loop / Motion Backgrounds

**Status:** especificação de produto e direção artística aprovada como referência; **nenhum novo motor compilado nesta branch ainda**. A prioridade passa a ser QUALIDADE cinematográfica, não quantidade de shaders.

## Materiais pesquisados

- https://storyloop.com/type/motion-backgrounds — catálogo de motion backgrounds; contém categorias, coleções, estilo para worship, apresentações, projeção e conteúdo LED
- https://storyloop.com/type/vj-loop/ — loops para VJ, shows e Resolume
- https://storyloop.com/downloads/awakening-worship-backgrounds/ — a página oficial descreve *horizon sunrise*, *burst*, *glow*, *light rays* e *streaks*; oferece 4K e ultrawide, MP4/ProRes e títulos editáveis
- https://storyloop.com/downloads/holy-light-18-2/ — a página oficial descreve formas luminosas abstratas, bokeh, raios e streaks; 4K/HD/ultrawide; clipes de 15s
- https://storyloop.com/aesthetic/stained-glass/ — série oficial de vitrais, incluindo Stained Glass Sacred Bubble Gum

Os links individuais abaixo foram fornecidos pelo operador. Quando o item não pôde ser aberto integralmente pelo indexador, a direção visual é **proposta a confirmar por prévia/screenshot** e não uma afirmação sobre quadros do vídeo.

## Inventário integral: 6 originais + Particle Wave + 15 referências = 22 motores

### Núcleo protegido (7)

1. Nevoa — shader original V1.0.1
2. Tunel — shader original V1.0.1 com costura angular corrigida
3. Lasers — shader original V1.0.1
4. Celulas — shader original V1.0.1
5. Aurora — shader original V1.0.1
6. Orbes — shader original V1.0.1
7. Particle Wave — único motor adicional aprovado da V1.4.1

### Quinze motores Story Loop (nomes-base dados pelo operador)

| Nome pedido | Página de referência | Direção técnica proposta para uma criação PROCEDURAL ORIGINAL |
|---|---|---|
| Awakening B_1 | https://storyloop.com/downloads/awakening-b_1-2/ | Sunrise/horizonte luminoso, volumetric rays e ascensão suave, a validar com preview |
| Chroma Blur 13 | https://storyloop.com/downloads/chroma_blur_13/ | Cores difusas com depth of field, cromaticidade e foco lento, sem ruído visual |
| Ethereal H Finished | https://storyloop.com/downloads/ethereal-h_finished/ | Atmosfera etérea, neblina volumétrica em camadas e luz ambiental gradual |
| Fair Lights 2 | https://storyloop.com/downloads/fair-lights-2-2/ | Camadas de partículas de luz e rastros de profundidade, sem confundir com Particle Wave |
| Flow 18 | https://storyloop.com/downloads/flow-18/ | Fluxos contínuos macios e camadas de fitas com iluminação rica |
| Flow Dark Glass | https://storyloop.com/downloads/flow-dark-glass/ | Formas de vidro escuro, refrações suaves, highlights e fresnel |
| Flow RMX Glass 3 | https://storyloop.com/downloads/flow-rmx-glass-3/ | Vidro mais dinâmico, distorção facetada e reflexos cromáticos |
| Lunar Drift 2A | https://storyloop.com/downloads/lunar-drift-2a-2/ | Camadas de luz fria, profundidade, lentidão e partículas atmosféricas |
| New Vision 1 | https://storyloop.com/downloads/new-vision-1-2/ | Construir a identidade específica somente após cotejar prévia visual |
| New Vision 6 | https://storyloop.com/downloads/new-vision-6-2/ | Contraste técnico/estético próprio em relação ao New Vision 1 |
| Pentecost Blaze 1 | https://storyloop.com/downloads/pentecost-blaze-1/ | Chamas orgânicas, brasas, glow e distorção térmica, sem vídeos embutidos |
| Spotlight 1 | https://storyloop.com/downloads/spotlight_1/ | Cone volumétrico, penumbra, falloff de luz realista e feixe móvel |
| Stained Glass Sacred Bubble Gum | https://storyloop.com/downloads/stained_glass_sacred_bubble_gum_prores-2/ | Vitrais luminosos policromáticos, juntas escuras, luz transmitida |
| TRIR Visual4K 15 Full | https://storyloop.com/downloads/trir-visual4k-15-full/ | Em validação de prévia; identidade visual independente, sem inferir composição |
| Walls — Names of God Line Add 2 | https://storyloop.com/downloads/_-walls-names-of-god_line-add_2/ | Background de parede/textura e **texto editável real** via parâmetro FFGL de texto, sem texto rasterizado fixo |

O link de **Flow 18** chegou duas vezes e foi contado uma vez. Os nomes foram mantidos a partir dos rótulos/URLs enviados. Correções de título e o enquadramento/cor exatos dependem de ver as prévias de cada item.

## Interface: O OPERADOR EXIGE BOTÕES, NÃO LISTA

- Linha 1: **Motor** — exatamente os 6 botões da V1.0.1, mesma ordem e mesma aparência nativa FFGL, sem alteração de índices/parâmetros legados na DLL estável.
- Linhas 2–4: grupos de **até 6 botões FF_TYPE_OPTION**, nomes compactos onde necessário com etiqueta completa em tooltip/descrição; novos motores em ordem alfabética, com Particle Wave incluído entre os 16 novos. A interface não deve colapsar em um dropdown: isso precisa ser confirmado com captura de tela da versão do Resolume do operador antes de liberar.
- O host FFGL é quem desenha os controles. Cada nova faixa é um parâmetro de opção próprio; a implementação precisa ter **um único motor ativo autoritativo** e garantir que notificações redundantes do host não revertam a seleção.
- **Paleta** logo após os botões: 36 cores em ordem alfabética, 9 algoritmos originais mapeados pelo identificador interno original e 27 gradientes novos. **PALETA PERSONALIZADA RGBA** imediatamente depois.
- Os controles de Speed, Scale, Intensity, Hue, Symmetry, Seed, BPM, Pulse, Contrast devem funcionar de forma coerente em todos os motores apropriados, além de Detail, Depth, Density, Glow, Direction, Opacity, Loop Period.
- O motor **Walls — Names of God** exige parâmetro de tipo **FF_TYPE_TEXT**, `SetTextParameter`/`GetTextParameter`, suporte a acentos portugueses, posicionamento, alinhamento, tamanho, entrelinha, alpha e textura da composição. Não alegar texto editável até renderizar caracteres reais com GPU font atlas, sem distribuir arquivos de fonte de terceiros.

## Arquitetura para alcançar a estética pedida

O padrão visual desejado exige mais que uma função `vec3 pal()` e linhas em seno/cosseno:

1. **Render por camadas:** fundo, objeto/forma principal, glow difuso, véu atmosférico, partículas, vinheta e composição final.
2. **Iluminação:** volumetric raymarch adaptativo quando adequado; Fresnel, reflexões aproximadas, refração, scattering, bloom selectivo e tone mapping, todos com ajustes de custo.
3. **Movimento:** evoluções físicas ou parametrizadas, `loopPhase = TAU * fract(time / period)`, continuidade temporal inclusive na troca de início/fim. Não depender de reinício brusco da animação.
4. **Render de texto:** biblioteca de rasterização legalmente distribuível ou atlas gerado em tempo de execução, upload GPU seguro, GPU compositing, edição FFGL real.
5. **Resolução:** render procedural respeitando o viewport real em Canvas Auto e aspect ratio de LED (p.ex. 3360×1008, 1920×1080, 3840×2160); arte se adaptando ao corte, não apenas esticando.
6. **Variações profissionais:** controles de composição e câmera, quantidade/densidade, velocidade, direção, foco, cor, profundidade, blur/bloom, sem descaracterizar cada motor.
7. **Qualidade de luz e cor:** pelo menos 16 bits de precisão intermediária quando render-to-texture for necessário, dither sutil e ajuste de contraste para LED; evitar clipping generalizado.
8. **GPU budget:** modo normal para shows, modo high quality para single source; benchmark 1/2/4 fontes e teste na RTX do operador. Meta 60 FPS em 1080p para uma instância, mas não afirmar atingimento antes de medir.

## Entrega progressiva: gate de qualidade antes de escalar

**Gate A — prova visual**: selecionar três visuais distintos — `Stained Glass Sacred Bubble Gum`, `Spotlight 1`, `Flow Dark Glass` — e produzir protótipos GLSL com 2–3 quadros de referência e vídeo preview. Comparar luz, composição, textura e movimento; operador valida, não promover motors medíocres.

**Gate B — template de motor**: depois da aprovação, incorporar arquitetura de múltiplas passagens, teste de costuras e controles dentro de FFGL, mantendo V1.0.1 intocável; gerar DLL de teste isolada.

**Gate C — motor textual**: implementar `Walls — Names of God` com texto editável, tipografia consistente, sem dependências ausentes e teste de acentos.

**Gate D — biblioteca**: desenvolver os demais 12 visuais individualmente com teste visual; somente após aprovação integrar os 15 na DLL final.

## Testes mandatórios

- SHA de blob `shaders.h` da V1.0.1 continua `488f842c06fe9fa616d71b033e32c573fe90179a`.
- Testes GLSL 410 e **renderização real headless OpenGL**, capturando pelo menos 3 frames por motor e delta entre frames para detectar motor preto/estático.
- Cada motor novo precisa ter assinatura de imagem e animação própria, não simples recoloração de outro shader.
- Verificação visual de loops sem salto ao fim, alpha efetivo, cores/presets e desempenho 1080p/4K/ultrawide.
- Teste final *obrigatório* dentro do Resolume Arena no ambiente do operador, sem substituir a V1.0.1 funcional até a aprovação.
- Presets preexistentes não podem ser silenciosamente reinterpretados com outro índice; usar ID FFGL novo para cada alteração incompatível e fornecer caminho claro de migração.

## Direitos e fidelidade

Os links do Story Loop são **referências estéticas e comportamentais**, não autorização para copiar, redistribuir ou incorporar seus vídeos, fotos, texturas ou fontes ao plugin. A licença padrão da Story Loop proíbe modificações e redistribuição; algumas coleções têm licença estendida, cuja aplicação depende do tipo de uso. Caso o operador possua materiais licenciados e deseje o frame **literalmente idêntico**, a solução adequada é usar os vídeos no Resolume de acordo com a licença, não alegar que shaders genéricos reproduzem os mesmos pixels. Este projeto cria motores originais inspirados na direção artística.
