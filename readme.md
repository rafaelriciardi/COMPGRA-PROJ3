Kevin Sakaguti Porto - 11039416

Rafael Riciardi Barreira - 11056916

# Seja bem vindo ao programa projeto3 -> "Space Trash"

O programa é desenvolvido em C++, se trata de uma um mapa 3D que simula o lixo espacial causado por satelites inutilizados. Ele oferece a capacidade do usuário de rotacionar um satelite principal, enquanto outros passam pela tela.

O projeto foi inspirado no "viewer6", onde ele cria um objeto, mapeia diferentes shaders de reflexão e textura no proprio objeto. Além do mapeamento do objeto, também é feito um cube mapping com uma imagem espacial.

Link para a aplicação: XXXXXXXXXXXXXXXXXXXXXXXXX

## O Projeto:

Essa representação visual proporciona uma visão interativa e educativa sobre o problema do lixo espacial, permitindo ao usuário explorar a dinâmica e a distribuição dos detritos orbitais no espaço. O programa visa conscientizar sobre a importância da gestão sustentável de satélites e resíduos espaciais.

<p align="center">
  <img src="https://github.com/rafaelriciardi/COMPGRA-PROJ3/blob/main/img/print_proj.png"/>
</p>

## Definição do ambiente
### Alterações em window.hpp

No window.hpp foi definido algumas variaveis e constantes. São eles:

```
static const int m_numObjs{100};
```
Que foi usado para definir a quantidade de satélites que serão gerados 

```
float m_angle{};
float angle;
```
Que são utilizados para rotacionar o satélite principal e rotacionar também os satelites que são gerados aleatoriamente

```
std::default_random_engine m_randomEngine;
```
Para gerar aleatoriamente a posição dos satelites que "voam" pelo mapa

### Alterações em window.cpp

Alteração do model (arquivo obj) em "onCreate()"
```
  // Load default model
  loadModel(assetsPath + "satellite.obj");
```

Criação de uma classe "randomNewObj()" onde é gerado valores aleatorios de posição X, Y e Z, para geração dos objetos.

```
void Window::randomNewObj(glm::vec3 &position, glm::vec3 &rotation) {
  // Get random position
  // x and y coordinates in the range [-20, 20]
  // z coordinates in the range [-100, 0]
  std::uniform_real_distribution<float> distPosXY(-2.0f, 2.0f);
  std::uniform_real_distribution<float> distPosZ(-50.0f, 0.0f);

  position = glm::vec3(distPosXY(m_randomEngine), distPosXY(m_randomEngine),
                       distPosZ(m_randomEngine));

  //  Get random rotation axis
  std::uniform_real_distribution<float> distRotAxis(-1.0f, 1.0f);

  rotation = glm::normalize(glm::vec3(distRotAxis(m_randomEngine),
                                      distRotAxis(m_randomEngine),
                                      distRotAxis(m_randomEngine)));
}
```

Alteração de "onPaint", onde foi adicionado a rotação dos objetos aleatorios, em função de 'deltaTime', e os objetos foram configurados para rodacionar em x e z em função de "angle". além da geração de "m_numObjs" objetos em um looping de repetição. 
```
for (const auto index : iter::range(m_numObjs)) {
    auto &position{m_objPositions.at(index)};

    if(angle<360.0f) {
      angle +=  deltaTime * 3.0f ;
    } else {
      angle = 0;
    }

    // Z coordinate increases by 10 units per second
    position.z += deltaTime * 1.0f;


    // Compute model matrix of the current obj
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(1, 0, 1));
}

```
Além disso, tiramos as condições do cubeMapping "renderSkyBox()" para que o cubemapping esteja ativado independente do Shader ou Texture.

```
  renderSkybox();
```
