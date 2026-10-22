# VJ Generative V1.7 — 82 motores

Esta versão reúne os sete motores existentes e os 75 motores correspondentes aos links únicos enviados. Spotlight 1 e Flow Dark Glass foram aprovados pelo operador. Stained Glass Sacred Bubble Gum foi refeito como janelas de igreja em arco ogival, com montantes, chumbo e vidro iluminado. Os demais motores novos estão implementados e aguardam avaliação visual no Resolume.

## Instalação

1. Fechar o Resolume.
2. Extrair `VJGenerativeV17.dll` e copiar para a pasta de plugins FFGL configurada no Resolume, por exemplo `Documentos/Resolume/Extra Effects`.
3. Reiniciar o Resolume e localizar **VJ Gen V1.7 82** em Sources.
4. Manter as versões anteriores disponíveis para as composições já salvas. A V1.7 tem identificação própria e não reinterpreta automaticamente os presets de outra DLL.

## Motores em botões

O grupo **MOTORES** contém 14 faixas de no máximo seis opções FFGL nativas. A seleção é única entre as faixas. Os seis clássicos, Particle Wave, Flow Dark Glass e Spotlight 1 aparecem primeiro; o vitral corrigido vem em seguida e os demais são organizados por nome. Alguns rótulos são abreviados para caber nos botões; o nome completo aparece no valor de exibição do controle.

A aparência final dos controles é desenhada pelo Resolume. O código usa o mesmo tipo de opção dos botões já aprovados, limitado a seis por faixa; não há um parâmetro com uma lista de 82 opções.

O [registro completo](REFERENCIAS_STORYLOOP_ATUALIZADAS.md) preserva todos os nomes e URLs. Nenhum link único foi descartado por ser uma coleção.

## Todos os controles em AJUSTES

Paletas, RGBA, movimento, composição e saída ficam no grupo **AJUSTES**.

- **Direção 0–360:** orienta a composição do motor. **Inverter Movimento** reverte a evolução da animação, preservando a fase acumulada. Velocidade zero pausa o movimento.
- **Velocidade, Escala, Intensidade, Matiz, Simetria e Semente:** alteram o efeito ativo. Semente também funciona no Particle Wave nesta versão.
- **BPM e Pulso:** o BPM controla a frequência do pulso; com Pulso zero, mudar BPM não altera o brilho. O pulso continua independente da velocidade da animação.
- **Contraste, Detalhe, Profundidade e Densidade:** ajustam contraste, definição das bordas, perspectiva e preenchimento luminoso. Nos motores que possuem camadas ou partículas, os controles também afetam seus parâmetros próprios.
- **Realce de Luz e Opacidade:** controlam luminosidade difusa e alpha de saída.
- **Canvas:** Auto usa a proporção real do viewport. Também há 16:9, 9:16, 32:9 e Custom. Largura e Altura Custom aparecem quando Custom é selecionado.
- **Período do Loop:** 4 a 120 segundos para o ciclo dos 75 motores novos. Nos sete motores preservados, ajusta a progressão temporal da animação original. Com Pulso ativo, o fechamento também depende de compatibilizar BPM e período.
- **Qualidade:** render interno a 50%, 75% ou 100% da saída, disponível nos 75 motores de múltiplas passagens.

Controles específicos aparecem onde se aplicam. Os ajustes de texto aparecem em Walls; Qualidade e Cores do Motor aparecem nos novos motores. Isso evita controles visíveis sem função para o efeito selecionado.

## Paletas e RGBA

As **36 paletas existentes**, em ordem alfabética, continuam disponíveis para todos os 82 motores. Selecionar outra paleta ativa suas cores nos novos motores. Em **Cores do Motor**, Referência recupera as cores próprias e Paleta usa a paleta selecionada.

Os modos RGBA A, A+B e A+B Animada têm prioridade sobre as cores de referência. Os oito canais RGBA, Mix A/B e as quatro operações de mistura foram testados. No modo RGBA A, o alpha vem de A; no modo A+B, vem da mistura de A e B. A Opacidade global multiplica esse alpha. Matiz também funciona no modo RGBA A.

## Walls com texto editável

**Walls — Names of God Line Add 2** recebe texto real via FFGL, com UTF-8 e acentos, além de tamanho, posição X/Y e layout Central ou Parede. O Windows rasteriza uma fonte instalada no sistema; não é necessário copiar uma fonte junto com a DLL. Texto vazio é permitido.

## Verificação da entrega

O workflow compila a DLL Windows x64 usando o SDK oficial do Resolume e executa:

- 82 seleções, persistência e faixas de no máximo seis botões;
- renderização da classe FFGL real em um host EGL, com teste dos ajustes, direção, inversão, pausa, RGBA e Canvas;
- 75 shaders individuais, movimento, fechamento do ciclo, 36 paletas por motor, RGBA, formatos largos de LED e retrato;
- restauração dos estados OpenGL e de upload da textura de texto;
- rasterização de texto no Windows, incluindo diferença entre caracteres com e sem acentos e suporte a texto vazio;
- integridade do shader clássico original e das paletas existentes.

Os arquivos `native_adjustments.json` e `validation.json` registram os resultados. Os testes gráficos automatizados usam Mesa llvmpipe por software. Eles não medem FPS na GPU do operador nem substituem a avaliação dentro do Resolume.

As prévias são renderizações dos próprios shaders. A equivalência artística aos materiais de referência precisa ser avaliada pelo operador; somente Spotlight 1 e Flow Dark Glass já receberam essa aprovação nesta etapa.
