# N33M1A5 VJ Generative V1.6 — cinco motores novos após avaliação visual

## Objetivo
Manter sete motores aprovados (**Nevoa, Tunel, Lasers, Celulas, Aurora, Orbes e Particle Wave**) e adicionar somente cinco novos, agora com conceitos e qualidades visuais pensadas para grandes painéis de LED e uso real por VJs.

**12 motores no total.** Os clássicos têm shader byte a byte idêntico ao da V1.0.1. Particle Wave mantém seu algoritmo já aprovado.

## Nova linha de botões
- Classicos (volta ao motor original selecionado)
- Arquitetura
- Chrome Flow
- Electric Storm
- Golden Fan
- Particle Wave
- Stage Rays

Os seis botões dos originais permanecem na primeira faixa. As seis opções acima, sem contar Classicos, aparecem no mesmo tipo de controles FFGL por opções, com nomes curtos. A quebra visual e largura final são responsabilidade do Resolume.

## Usos reais
| Motor | Direção estética | Uso sugerido |
|---|---|---|
| Arquitetura | Pórticos e estruturas de palco, wireframe em profundidade e centro livre | Corporativo, apresentações, DJs e fundos de palco |
| Chrome Flow | Fitas fluidas lisas com reflexos cromados e gradientes largos | Casamentos, recepção de clientes, cenografia LED contínua |
| Electric Storm | Múltiplos raios elétricos de alto contraste e halo atmosférico | Aberturas de shows, viradas, música eletrônica |
| Golden Fan | Leques simétricos Art Deco com arcos, ornamentos e movimento lento | Formaturas, gala, igrejas, eventos sociais |
| Stage Rays | Feixes direcionais volumétricos e cortinas luminosas preservando área central | Palco com músicos, bandas, adoração e shows |
| Particle Wave | Ondas e linhas de partículas luminosas, original aprovado | Momentos imersivos, música ambiente |

Os motores usam as **36 paletas ordenadas alfabeticamente**, incluindo o mapeamento exato das nove antigas; RGB A/B personalizado, Velocity, Scale, Intensity, Hue, Symmetry, Seed, BPM, Pulse, Contrast, Detail, Depth e Density. Para eventos ao vivo é preferível ajustar a intensidade e a escala conforme a luminância e a resolução real do painel.

## Referências profissionais revisadas
Foram estudadas as características de grandes catálogos de VJ loops, **como inspiração de arte, não para copiar vídeos, algoritmos nem recursos**:
- [LIME ART GROUP – Black Stage](https://limeartgroup.com/shop/vj-loops-pack-vol-94-black-stage/) — arquiteturas com linhas claras no preto e feixes simétricos
- [LIME ART GROUP – Elika](https://limeartgroup.com/shop/vj-loops-pack-vol-52-elika/) — leques radiais dourados, ornamentos Art Deco e fundo escuro
- [LIME ART GROUP – Gnosis Lightning](https://limeartgroup.com/shop/vj-loops-pack-vol-85-gnosis-lightning/) — raios elétricos, energia e cenas para palco
- [LIME ART GROUP – Lights Bundle](https://limeartgroup.com/shop/lights-vj-loops-bundle/) — beams, kaleidoscópios, luz volumétrica e visuais LED
- [LIME ART GROUP – Strukt](https://limeartgroup.com/shop/vj-loops-pack-vol-22-strukt/) — grandes estruturas geométricas em vidro
- [LIME ART GROUP – Light Leaks](https://limeartgroup.com/shop/light-leaks-vjloops-video/) — luzes difusas e efeitos limpos para sobreposição

## Qualidade e validação
- Todos os seis shaders da V1.0.1 preservados com seu hash Git original
- Verificação GLSL 410 para o shader original, variante RGB/paleta e expansão com cinco novos motores
- Renderização de quadros PPM reais em Mesa EGL OpenGL, validação de alteração temporal
- Compilação Windows x64 baseada no SDK FFGL do Resolume
- Previews de todos os seis motores extras, gerados automaticamente pelo build
- Não foi testado no hardware NVIDIA/Resolume do operador; testar com um Source novo em camada separada antes de shows

## Entrega
- Nome FFGL: `VJ Gen V1.6`, ID `VJ19`
- Arquivo: `VJGenerativeV160.dll`
- Instalador não destrutivo: `install/INSTALAR_V1_6.bat`
- Diretório correto padrão: `Documents/Resolume Arena/Extra Effects`
- A V1.0.1, V1.4.1 e V1.5 não são apagadas.
