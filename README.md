# faith — apprendre OpenGL avec le jeu de la vie

J'ai fait ce projet pour apprendre OpenGL. Le jeu de la vie sert de prétexte : il donne une grille qui change à chaque frame, et il faut l'afficher avec le GPU.

La base du code reprend le tutoriel [Hello Triangle de LearnOpenGL](https://learnopengl.com/Getting-started/Hello-Triangle) (fenêtre GLFW, GLAD, shaders, VAO / VBO / EBO). Je l'ai ensuite découpé en classes et étendu avec une texture, des uniforms et un fragment shader qui fait le rendu.

> **Règles du jeu de la vie** — une cellule vivante survit avec 2 ou 3 voisines, sinon elle meurt. Une cellule morte avec 3 voisines naît.

## Le pipeline OpenGL étape par étape

Tout le rendu se trouve dans `renderer/`. `Renderer::init()` lance les étapes dans l'ordre :

### 1. Fenêtre et contexte — GLFW (`_initWindow`)

```cpp
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
```

On demande un contexte **OpenGL 3.3 core profile**, sans l'ancien pipeline fixe (`glBegin`/`glEnd`). C'est la version utilisée par LearnOpenGL. `glfwMakeContextCurrent` attache ce contexte au thread courant.

### 2. Chargement des fonctions — GLAD (`_initGlad`)

Les fonctions OpenGL modernes sont fournies par le driver et doivent être récupérées à l'exécution. `gladLoadGLLoader(glfwGetProcAddress)` s'en charge. On règle ensuite le `glViewport` et un callback qui le met à jour quand la fenêtre est redimensionnée.

### 3. Shaders (`Shader`, `_initShader`, `_shaderProgramInit`)

Chaque `Shader` lit son fichier `.glsl` et le compile (`glCreateShader` → `glShaderSource` → `glCompileShader`). En cas d'erreur, il affiche le log de compilation (`glGetShaderInfoLog`).
Le vertex shader et le fragment shader sont ensuite attachés puis linkés dans un **shader program** (`glCreateProgram` → `glAttachShader` → `glLinkProgram`).

### 4. Géométrie : un quad plein écran (`_initQuad`)

On ne dessine que 4 sommets, déjà en coordonnées normalisées (NDC, de -1 à 1). Ils forment deux triangles qui couvrent tout l'écran :

```cpp
float vertices[] = {  1,  1, 0,   1, -1, 0,   -1, -1, 0,   -1,  1, 0 };
unsigned int indexes[] = { 0, 1, 2,   0, 2, 3 };
```

| Objet   | Rôle                                                                           |
|---------|--------------------------------------------------------------------------------|
| **VBO** | buffer GPU qui contient les sommets                                            |
| **EBO** | buffer d'indices : réutilise les sommets au lieu de les dupliquer (4 au lieu de 6) |
| **VAO** | mémorise la configuration (VBO lié, EBO lié, `glVertexAttribPointer`)           |

`glVertexAttribPointer(0, 3, GL_FLOAT, ...)` explique au GPU comment lire le buffer : 3 floats par sommet, envoyés à `layout (location = 0)` dans le vertex shader.

Le vertex shader se contente de transmettre la position :

```glsl
gl_Position = vec4(aPos, 1.0);
```

### 5. La grille comme texture

C'est la partie qui va au-delà du tuto. Au lieu de créer de la géométrie pour chaque cellule, la grille entière est stockée dans une **texture 2D**, avec **1 texel = 1 cellule** :

```cpp
glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
```

- `GL_RED` : un seul canal de 8 bits suffit (`255` = vivante, `0` = morte).
- `GL_NEAREST` : pas d'interpolation, donc chaque cellule s'affiche comme un carré net quand la texture est étirée sur la fenêtre.
- `nullptr` : on alloue la texture sans données. Elle est remplie à chaque frame.

À chaque génération, `Renderer::update()` convertit la grille en `std::vector<uint8_t>` et l'envoie au GPU avec `glTexSubImage2D`. Cette fonction met à jour le contenu d'une texture existante sans la réallouer.

### 6. Uniforms (`_setUniform`)

Les uniforms sont des variables globales du shader, fixées depuis le CPU avant chaque draw :

| Uniform        | Type        | Valeur envoyée                               |
|----------------|-------------|----------------------------------------------|
| `u_grid`       | `sampler2D` | unité de texture 0 (`glActiveTexture`)       |
| `u_time`       | `float`     | `glfwGetTime()`, pour animer                 |
| `u_resolution` | `vec2`      | taille du framebuffer, pour calculer les UV  |

### 7. Fragment shader : le vrai rendu

Le fragment shader s'exécute **pour chaque pixel** du quad. Il calcule les coordonnées UV du pixel, lit la cellule correspondante dans la texture et choisit une couleur :

```glsl
vec2 uv = gl_FragCoord.xy / u_resolution;
float cell = texture(u_grid, uv).r;

if (cell > 0.5)
    FragColor = vec4(1.0, 0.5 + 0.5 * sin(u_time * 4.0), 0.0, 1.0); // vivante : orange qui pulse
else
    FragColor = vec4(0.0, 0.05, 0.15, 1.0);                         // morte : bleu nuit
```

### 8. Boucle de rendu (`main.cpp`, `Renderer::draw`)

```cpp
while (!renderer.shouldClose())   // Échap ferme la fenêtre
{
    game.step();                   // CPU : génération suivante
    renderer.update(game.getGrid()); // CPU → GPU : texture
    renderer.draw();               // clear, useProgram, uniforms, bind VAO, glDrawElements, swap
}
```

`glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0)` dessine les 6 indices de l'EBO. `glfwSwapBuffers` échange ensuite le front buffer et le back buffer (double buffering).

## Ce que j'ai appris

- Le cycle de vie d'un contexte OpenGL avec GLFW et GLAD
- La compilation et le link de shaders GLSL, avec la gestion des erreurs
- VBO / EBO / VAO et la description des attributs de sommets
- Utiliser une **texture comme tableau de données** plutôt que comme image
- Mettre à jour une texture à chaque frame avec `glTexSubImage2D`
- Les uniforms pour passer le temps, la résolution et un sampler
- Faire le rendu dans le fragment shader à partir de `gl_FragCoord`

## Structure

```
.
├── main.cpp                  # boucle principale
├── Makefile
├── game/                     # logique du jeu de la vie (CPU)
│   ├── Cell.hpp
│   ├── Environment.{hpp,cpp}
│   └── GameOfLife.{hpp,cpp}
└── renderer/                 # tout le code OpenGL
    ├── Renderer.{hpp,cpp}
    ├── Shader.hpp
    ├── Shaders.cpp
    └── shaders/
        ├── vertex.glsl
        └── fragment.glsl
```

## Compilation

Dépendances : `g++` (C++17), OpenGL 3.3+, [GLFW](https://www.glfw.org/) compilé en statique, [GLAD](https://glad.dav1d.de/) (3.3 core), libs X11 (`X11`, `Xrandr`, `Xi`), `pthread`, `dl`.

Le `Makefile` attend cette arborescence :

```
opengl/
├── includes/
│   ├── glad/        # include/ + src/glad.c
│   └── glfw/        # sources GLFW
├── build/src/       # libglfw3.a
└── gameOfLife/      # ce projet
```

```sh
cmake -S ../includes/glfw -B ../build && cmake --build ../build   # une seule fois, pour GLFW
make
./faith
```

Lancez le programme depuis la racine du projet : les shaders sont chargés avec les chemins relatifs `renderer/shaders/*.glsl`.

La taille de la grille se règle dans `main.cpp` (10×10 par défaut). `GameOfLife` et `Renderer` doivent recevoir les mêmes dimensions.

## Pistes pour aller plus loin

- Calculer les générations **sur le GPU** (ping-pong entre deux textures avec des framebuffers), au lieu de le faire sur le CPU
- Ajouter un compute shader (OpenGL 4.3+)
- Dessiner des cellules à la souris
- Afficher une grille entre les cellules dans le fragment shader
