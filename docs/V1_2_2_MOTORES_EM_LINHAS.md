# N33M1A5 VJ Expanded V1.2.2 — linhas de motores nativas

## Interface solicitada

A ordem dos parâmetros registrados pelo FFGL é:

1. **Motor** (6 opções originais, mesma UI nativa da V1.0.1): Nevoa, Tunel, Lasers, Celulas, Aurora, Orbes
2. **Motor - Geometria** (opção V1 de retorno + Crystal, Hex, Mandala, Orbitas, Prismas, Fractal)
3. **Motor - Luz** (opção V1 de retorno + Cortinas, Beams, Particulas, Bokeh, Plasma, Ribbons)
4. **Motor - 3D** (opção V1 de retorno + Grid, Kaleido, Estrelas, Circuitos, Chuva, SolidRGBA)
5. **Paleta** (9 opções originais)
6. **CORES | RGBA** (Original Palette / A / A+B / Animated, Mistura, dois conjuntos RGBA, Opacidade)
7. Velocidade, Escala, Intensidade, Matiz, Simetria, Semente, BPM (60–180), Pulso, Contraste — exatamente os mesmos nomes/valores padrão da V1.0.1
8. Detail, Depth, Density, **Inverter Movimento** e **Realce de Luz**, em seção avançada
9. Canvas Auto/16:9/9:16/32:9/Custom e dimensões virtuais

**As linhas de motores 2–4 usam o mesmo tipo `SetOptionParamInfo` da linha original**, com opções FFGL apresentadas nativamente pelo Resolume; a disposição exata de botões e eventuais quebras depende da largura do painel e versão do host. Foram reduzidos nomes longos para ajudar a caberem horizontalmente.

## Comportamento do motor

- Os seis motores originais continuam compilados a partir do arquivo `shaders.h` da V1.0.1 com SHA de blob Git `488f842c06fe9fa616d71b033e32c573fe90179a`. Nenhum shader V1.0.1 foi editado.
- Clicar em um motor das faixas novas ativa o shader de expansão. As outras faixas retornam à opção V1. Clicar em qualquer botão do Motor original ou na opção V1 da faixa ativa retorna ao render clássico.
- **Os comandos Velocidade, Escala, Intensidade, Matiz, Simetria, Semente, BPM, Pulso e Contraste se aplicam às 18 gerações abstratas.** Paleta também.
- **SolidRGBA** é um gerador de cor sólida: deliberadamente não responde a controles de geometria, zoom, contraste ou BPM, mas responde ao RGBA/Opacidade. Isso é esperado para uma cor sólida.
- **Inverter Movimento** (padrão desligado) inverte o avanço do relógio do shader novo mantendo continuidade ao trocar a direção; não altera velocidade dos seis originais.
- **Realce de Luz** (padrão zero) aumenta destaques luminosos dos 17 motores novos não sólidos, sem aplicar blur real e sem alterar render V1.0.1.
- **CORES | RGBA** atua no shader novo; padrão `Original Palette` mantém a estética de paleta original.

## Compatibilidade e segurança

Este é um FFGL Source **experimental separado**:
- DLL: `VJGenerativeExpanded122.dll`
- Nome no Resolume: `VJ Expanded 122`
- FFGL ID: `VJ14`

Não substitui `VJGenerativeV1.dll` da V1.0.1. Tem novo ID porque a ordem de parâmetros mudou; presets dos FFGL V1.2/V1.2.1 não devem ser usados automaticamente no V1.2.2.

O modo `Canvas Auto/Output` respeita proporção do framebuffer. O shader procedural calcula pixels na resolução fornecida pelo Resolume, inclusive ultrawide como 3360×1008. Custom W/H ajustam **a proporção de render virtual**, não a resolução física do painel. Valores extremos ou várias instâncias poderão ter impacto de GPU.

## Como testar
1. Fechar Resolume.
2. Extrair o ZIP e executar `INSTALAR_VJ_EXPANDED_122.bat`.
3. Abrir **Sources → VJ Expanded 122**.
4. Confirmar as três novas faixas abaixo dos seis motores e o grupo RGBA após Paleta.
5. Experimentar cada motor, trocar Paleta, Velocidade, Escala, Simetria, BPM; testar Reverse e Glow e Alpha.
6. Testar canvas 1920×1080 e 3360×1008; conferir FPS antes de usar ao vivo.

CI de GitHub: verificação de identidade da V1.0.1, ordem dos parâmetros, validação GLSL e MSBuild x64. A execução visual real dentro do Resolume ainda precisa de confirmação do operador.
