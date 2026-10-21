# N33M1A5 // VJ GENERATIVE BKG

Plugin **FFGL Source nativo para Resolume Arena/Avenue**, criado a partir dos arquivos fornecidos pelo autor.

## V1.6 — expansão em desenvolvimento

Esta branch contém os sete motores existentes e três novos motores integrados ao código da DLL de desenvolvimento V1.6 LAB.

- [Protótipos, vídeos e validação OpenGL](prototypes/gate_a/README.md)
- [Escopo confirmado: 82 motores (7 existentes + 75 novos)](docs/REFERENCIAS_STORYLOOP_ATUALIZADAS.md)
- [Especificação e etapas de integração](docs/STORYLOOP_22_MOTORES_PADRAO_CINEMATOGRAFICO.md)

[Instalação e limites da V1.6 LAB](docs/V1_6_LAB.md). A DLL de teste é disponibilizada pelo workflow após compilação e testes bem-sucedidos. As instruções abaixo descrevem a base histórica.

## V1 — Base independente

- **6 motores:** Névoa, Túnel, Lasers, Células, Aurora e Orbes.
- **9 paletas:** Arco-íris, Fogo, Oceano, Neon, Ultravioleta, Floresta, Gelo, Pôr do Sol e Monocromático.
- Controles: motor, paleta, velocidade, escala, intensidade, matiz, simetria, semente, BPM, pulso e contraste.
- Renderização no contexto OpenGL do Resolume; não utiliza arquivos de vídeo.
- FFGL Source com zero entradas.
- Código-fonte C++ e GLSL nos arquivos enviados pelo autor.

## Build Windows x64 (GitHub Actions)

A cada push, a ação **Build VJGenerativeV1 DLL** baixa o **SDK FFGL oficial do Resolume** e compila o plugin com MSBuild para `Release | x64`. A DLL aparece no artefato **VJGenerativeV1-Windows-x64**.

1. Abra a aba **Actions** do repositório.
2. Selecione **Build VJGenerativeV1 DLL**.
3. Clique **Run workflow** (ou aguarde o build do push).
4. Baixe o artefato; extraia `VJGenerativeV1.dll`.
5. Com o Resolume fechado, copie a DLL para `Documentos/Resolume/Extra Effects` (ou para a pasta de plugins FFGL configurada).
6. Reinicie o Resolume e procure **VJ Generative** em Sources.

## Compatibilidade e limites

- Compilação: Visual Studio / MSBuild, Windows x64, SDK oficial `resolume/ffgl`.
- GLSL `#version 410 core`: é compilado pelo driver OpenGL quando a instância abre no Resolume; o sucesso do CI não certifica aparência nem FPS no computador de destino.
- Esta base **não contém painel externo, botão SEND nem os 70 motores** das versões anteriores; é um recomeço fiel aos três arquivos fornecidos.

## Código original

Os arquivos principais são `VJGenerative.cpp` e `shaders.h`. O arquivo de build foi corrigido e movido para `.github/workflows/build.yml`.
