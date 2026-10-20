# N33M1A5 VJ Expansion V1.2.1 — CORES abaixo de Paleta

## Pedido implementado
Na janela de parâmetros FFGL do novo **VJ Expanded 121**, os controles são registrados nesta ordem:
1. Motor (seis clássicos: Nevoa, Tunel, Lasers, Celulas, Aurora, Orbes)
2. Paleta (nove paletas da V1.0.1)
3. **CORES | RGBA** (grupo independente)
   - Cor / RGBA: Original Palette, RGBA A, RGBA A+B, RGBA Animated
   - Mistura RGBA: Crossfade, Add, Screen, Multiply
   - Mistura A/B
   - A Red, A Green, A Blue, A Alpha
   - B Red, B Green, B Blue, B Alpha
   - Opacidade
4. Velocidade, Escala, Intensidade, Matiz, Simetria, Semente, BPM (60–180), Pulso, Contraste (iguais à V1.0.1)
5. Motores Geometria, Luz, 3D (três faixas de seis motores adicionais cada), Detail, Depth, Density, Canvas.

A opção **Original Palette** é padrão. **Nenhum shader dos seis motores clássicos foi alterado**. Esses seis motores são renderizados pelo mesmo código-fonte de `shaders.h` da V1.0.1; controles RGBA adicionais aplicam-se exclusivamente ao render das extensões. A posição e o agrupamento são registrados via FFGL SDK; o aspecto exato do grupo é controlado pela versão do Resolume.

## Compatibilidade
A V1.2.1 usa identidade FFGL própria `VJ13`, exibe `VJ Expanded 121` e gera `VJGenerativeExpanded121.dll`. Motivo: colocar novos controles entre Paleta e Velocidade altera os **índices numéricos dos parâmetros** da V1.2 experimental. Não reutilizar configurações salvas da `VJ Gen Expanded` anterior sem conferir parâmetros.

**NÃO remove e NÃO substitui** o plugin `VJ Generative` V1.0.1 funcional, nem a V1.2 experimental. A instalação está em `install/INSTALAR_VJ_EXPANDED_121.bat` e `.ps1`, com backup do mesmo arquivo quando existir e destino `Documentos/Resolume Arena/Extra Effects`, respeitando redirecionamento de Documentos.

## Testes
- Pipeline CI verifica que o shader `shaders.h` da V1.0.1 está idêntico e que a ordem de registro Cor/Paleta/Velocidade está correta.
- Compilação GLSL 410 (original + expansão) e MSBuild x64.
- **Teste real ainda necessário** no Resolume Arena para confirmar render RGBA, alpha, aparência do grupo/controles, FPS, e canvases 1920x1080 / 3360x1008.
