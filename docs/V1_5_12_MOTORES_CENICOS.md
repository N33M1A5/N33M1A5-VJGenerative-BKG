# N33M1A5 VJ Generative V1.5 — 12 motores selecionados para palco

## Objetivo
O usuário confirmou que os seis motores da V1.0.1 são excelentes e **Particle Wave** é o único da expansão anterior aprovado. Os outros 17 foram descartados da interface. Esta edição concentra-se em **cinco motores autorais NOVOS**, em vez de reapresentar efeitos antigos.

## Interface e motores
**Primeira faixa**, o mesmo parâmetro FFGL `Motor` e os mesmos seis botões originais:
Nevoa | Tunel | Lasers | Celulas | Aurora | Orbes.

**Segunda faixa**, parâmetro FFGL do mesmo tipo `Motor Adicional`, com opção de retorno *Classicos* seguida das seis opções em ordem alfabética:
- **Catedral Luz:** ambiente cenográfico de arcos em perspectiva, profundidade, reflexos e luz atmosférica
- **Chrome Tide:** metal líquido iridescente com reflexão especular, faixas luminosas e turbulência suave
- **Energy Rift:** fissura elétrica com caminhos ramificados, halos, choques e energia responsiva
- **Infinity Cubes:** mosaico de cubos e quadrados 3D perspectivos em rotação com parallax
- **Nova Pulse:** explosão de luz concêntrica, ondas de choque e lasers radiais reativos
- **Particle Wave:** o mesmo shader do motor aprovado na V1.3/V1.4.1

Todas as opções adicionais são **botões FFGL**, não a antiga lista de 24 motores. `AddElementSeparator(P_EXTRA_MOTOR,1)` indica uma quebra visual entre a opção de retorno e os seis novos botões; o layout de quebra depende do host Resolume.

Selecionar qualquer um dos seis primeiros desativa o motor adicional. Ao escolher *Classicos*, volta para o motor clássico anteriormente escolhido. A matemática dos seis clássicos permanece no arquivo `shaders.h` imutável da V1.0.1 (Git SHA `488f842c06fe9fa616d71b033e32c573fe90179a`).

## Parâmetros e paletas
Mantidos os comandos **Paleta (36 opções em ordem alfabética, 9 originais preservadas), Velocidade, Escala, Intensidade, Matiz, Simetria, Semente, BPM (60–180), Pulso, Contraste, Detalhe, Profundidade, Densidade, Inverter Movimento, Realce de Luz, Opacidade**.

Paleta personalizada RGB A/B, Alpha, mistura e modo Original funcionam em todos os motores. Shader em GLSL 410, render procedural; modo canvas Auto consulta dimensões físicas do framebuffer e suporte a formato LED personalizado/ultrawide.

## Pesquisa de direção de arte (não copiar algoritmos nem assets)
- [Resolume Footage](https://resolume.com/footage): LetThereBeLight, SyntezyChromium, ParticleParty, SyntezyFlux, LightCell, ChromaticFlow e atmosferas de palco
- [LIME ART GROUP](https://limeartgroup.com/shop/): visuais de impacto para LED ultrawide, vídeo mapping, eventos de grande porte, 4K e 8K
- [VJ Loops Farm](https://vjloopsfarm.com/): geometria, líquidos, neon, efeitos e túneis

Os cinco novos algoritmos usam geometria procedural, noise, iluminação e senoides autorais e **não contêm arquivos, vídeos ou códigos proprietários** desses sites. A estética é uma direção criativa, não reprodução de loops.

## Artefatos
- FFGL ID `VJ18`, `VJ Gen V1.5`
- DLL `VJGenerativeV150.dll`
- Instalador `install/INSTALAR_V1_5.bat`, `install/INSTALAR_V1_5.ps1`, para `Documentos/Resolume Arena/Extra Effects` sem modificar a V1.0.1.
- Projeto da V1.4.1 preservado separadamente; a V1.5 somente instala um novo arquivo de origem.

## Testes necessários antes de show
- Verificação exata do SHA da V1.0.1.
- GLSL syntax do original, cores personalizadas e motor adicional com cinco shaders.
- Renderização de 6 motores originais + seis novos via Mesa EGL OpenGL. Verificar cores, presença de imagem e animação de cada motor e gerar previews reais (artefato GitHub).
- Compilação Windows x64 com FFGL SDK oficial.
- **Ainda indispensável testar visual, layout de botões e FPS no Resolume Arena em sua máquina e saída LED real.**
