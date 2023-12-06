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
### Alterações em ground.cpp:

EM "void Ground::create(GLuint program)" foi alterado os parametros para diminuir o tamanho dos quadrados da malha.
```
  std::array<glm::vec3, 8> vertices{{{-0.1f, 0.0f, +0.1f},
                                     {-0.1f, 0.0f, -0.1f},
                                     {+0.1f, 0.0f, +0.1f},
                                     {+0.1f, 0.0f, -0.1f}}};
```
Dado esta alteração em "paint" tivemos que dividir as translações da malha por 5 também, pois da maneira que estava os quadrados das malhas estavam separados.
```
model = glm::translate(model, glm::vec3(x/5.0f, height, z/5.0f));
```
Para fazer as montanhas e lagos foi aplicado uma função senoide em relação a X e Z.
```
float height = amplitude * std::sin(frequency * x + phase) * std::sin(frequency * z + phase);
```
E como queriamos as alturas em função de 0.1 aplicamos a seguinte formula para a senoide se tornar discreta e não continua.
```
  height = std::round(height / 0.1f) * 0.1f;
```
por fim, adicionamos o parametro de altura ("height") na função de translação.
```
model = glm::translate(model, glm::vec3(x/5.0f, height, z/5.0f));
```
Para dar o aspecto de montanha aplicamos a cor verde no nas alturas medianas acima de 0, para os picos da montanha aplicamos a cor branca e para os lagos a cor azul, ainda no "paint"
```
if(height < 0){
  abcg::glUniform4f(m_colorLoc, 0.0f, 0.0f, 0.75f, 1.0f); 
}
else if (height > 0.7) {
  abcg::glUniform4f(m_colorLoc, 1.0f, 1.0f, 1.0f, 1.0f); 
}
else {
  abcg::glUniform4f(m_colorLoc, 0.0f, 1.0f, 0.0f, 1.0f);
}
```
### Alterações em "onPaint":

Em onPaint, foi alterado a criação das figuras, colocamos cubos para serem observados. E estes cubos em constante rotação no eixo X e Y.
```
if(angle<360.0f) {
    angle = angle + 15.0f ;
  } else {
    angle = 0;
  }

  glm::mat4 model{1.0f};
  model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
  model = glm::rotate(model, glm::radians(angle), glm::vec3(1, 1, 0));
  model = glm::scale(model, glm::vec3(0.1f));
  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 0.5f, -1.0f, 0.0f, 1.0f);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,nullptr);
```


## Definição da movimentação

### Modo de voo
Para implementar o modo de voo, foram atribuídas funções às teclas LSHIF (subir) e LCTRL (descer).
```
if (event.key.keysym.sym == SDLK_LSHIFT)
  m_flySpeed = 2.0f;
if (event.key.keysym.sym == SDLK_LCTRL)
  m_flySpeed = -2.0f;
```

Assim como as outras funções, a função de voo é chamada da atualizada pela seguinte chamada:
```
m_camera.fly(m_flySpeed * deltaTime);
```

E seu funcionamento utiliza como base um vetor fixo (0,1,0) que quando multiplicado pela velocidade e somado às posições anteriores de posição da camera e lookat, realiza o deslocamento do espectador na posição horizontal.
```
void Camera::fly(float speed) {
  // Instantiate height modifier vector
  glm::vec3 height_modifier{0.0f, 1.0f, 0.0f};

  // Move eye and lookat point based on height modifier and speed
  m_eye += height_modifier * speed;
  m_at += height_modifier * speed;

  computeViewMatrix();
}
```


Para melhorar a visualização no modo voo, também foram alteradas a distância de renderização e o fator de sombreamento de objetivos distantes.

### Rotação da câmera
Para deixar a movimentação mais fluida para o usuário e proporcionar uma melhor experiência de interação, alteramos os comandos de **pan** para as setas esquerda e direita do teclado e atribuimos uma nova função às setas cima e baixo.
```
if (event.key.keysym.sym == SDLK_UP)
  m_cameraSpeed = 2.0f;
if (event.key.keysym.sym == SDLK_DOWN)
  m_cameraSpeed = -2.0f;
```

Para que o jogador consigar olhar para cima e para baixo quando estiver voando, criamos essa nova função, que inclusive pode ser combinada na movimentação com as teclas wasd para voar de uma forma única pelo cenário.
```
void Camera::cameraFlyView(float speed) {
// Instantiate view_modifier vector
glm::vec3 view_modifier{0.0f, 1.0f, 0.0f};

// Move de lookat point based on the modifier and speed
m_at += view_modifier * speed;

computeViewMatrix();
}
```

