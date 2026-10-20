# N33M1A5 VJGenerative V1.1 — RGBA / Canvas / VJ Pro

## Compatibilidade
Esta revisão é um **novo plugin FFGL Source**: `VJ Generative V1.1` (ID `VJG1`, arquivo `VJGenerativeV11.dll`). Pode coexistir com a V1.0 (`VJGN`) no mesmo Resolume.

Os **6 motores originais** estão preservados, sem mudanças em suas funções GLSL. Ao abrir a V1.1 com:
- **Motor:** 0 a 5
- **Color Mode:** Original
- **Source Opacity:** 100%
- **Canvas Aspect:** Auto / Output
- **Transform:** posição neutra, escala 100%, rotação 0°
o objetivo é manter a aparência da V1.0.

## RGBA ao estilo FFGL/Resolume
O SDK FFGL oferece tipos separados `FF_TYPE_RED`, `FF_TYPE_GREEN`, `FF_TYPE_BLUE` e `FF_TYPE_ALPHA`, usados para dois conjuntos de cor:

**RGBA A:** A - Red / Green / Blue / Alpha

**RGBA B:** B - Red / Green / Blue / Alpha

- **Color Mode:** Original / RGBA A / RGBA A+B / RGBA Animated
- **RGBA Combine:** Crossfade / Add / Screen / Multiply
- **RGBA Mix A/B:** de A para B
- **Source Opacity:** controle independente de transparência do plugin

O branco é obtido com R=G=B=1. Não há canal físico W neste FFGL.
A UI do Resolume pode mostrar os canais como controles separados, conforme o tratamento dos tipos FFGL; não é garantida a aparência exata do seletor HSB/RGB do gerador Solid Color interno.

**Source Opacity** e os canais A-Alpha/B-Alpha afetam o canal alpha da imagem de saída. As cores são enviadas como RGBA com pré-multiplicação de RGB pelo alpha, adequada para composição.

**Layer Blend Mode** (blend da camada em relação a outras camadas, como `Layer Determined`) é recurso do **Resolume**, não configurado pelo Source FFGL; escolha-o na camada/clipe do Resolume.

## Resoluções personalizadas / LED

O shader renderiza diretamente no framebuffer atual do Resolume, via `iResolution`. Não há bitmap ou vídeo a ser escalado. A saída real — por exemplo 3360×1008, 1920×1080, 3840×2160 — continua sob controle do Resolume e de sua composição/slice/output.

- **Canvas Aspect:** Auto / Output, 16:9, 9:16, 4:3, 32:9, Custom
- **Canvas Fit:** Adaptive, Fit, Fill, Stretch
- **Custom W / Custom H:** 256 a 8192 px, exibidos como pixels; controlam o aspecto **virtual** da composição, não mudam a resolução física do Resolume.
- **Position X/Y:** -100% a +100%, com centro neutro (0%)
- **Transform Scale:** 25% a 400%, neutro em 100%
- **Rotation:** -180° a +180°, neutro em 0°
- **Anchor X/Y:** deslocamento relativo, centro neutro.

**Fit** mantém a proporção do canvas virtual e permite barras transparentes; **Fill** preenche a saída por recorte; **Stretch** pode distorcer a geometria para preencher. **Auto / Output** preserva a proporção real da saída e deve ser a opção normal para LED ultrawide.

A qualidade espacial depende da resolução real e do FPS no Resolume. Valores altos podem reduzir FPS se a GPU estiver no limite; não há promessa de upscale IA nem 8K a 60 fps automaticamente.

## Motores
### 6 originais
0. Nevoa
1. Tunel
2. Lasers
3. Celulas
4. Aurora
5. Orbes

### 5 novos e independentes
6. Crystal Kaleido — padrões radiais, facetas e highlights
7. Particle Waves — ondas de pontos e trilhas sobrepostas
8. Light Curtains — cortinas de luz e nevoa
9. Cyber Grid — grade de profundidade e ambiente futurista
10. Solid RGBA — gerador de cor sólida com alpha e A/B mix

## VJ PRO
- **VJ Detail:** detalhamento estrutural dos motores novos
- **VJ Depth:** profundidade/câmera aparente dos motores novos
- **VJ Density:** densidade de elementos dos motores novos

Os motores originais não usam os controles VJ PRO, de propósito: o objetivo é manter sua aparência de referência intacta.

## Build e teste
Build com SDK oficial Resolume FFGL, Visual Studio MSBuild, Release x64.
1. Abrir **Actions** e baixar o artifact `VJGenerativeV11-RGBA-Windows-x64`.
2. Extrair `VJGenerativeV11.dll`.
3. Fechar o Resolume e instalar em `Documents/Resolume/Extra Effects`.
4. Abrir e procurar `VJ Gen V11`.
5. Testar todos 11 motores em 1920×1080 e LED custom, opacidade, alpha e RGB A/B.
6. Validar se o driver compila GLSL ao abrir (build C++ sozinho não faz essa validação).
