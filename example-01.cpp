#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

struct attributes {
    GLfloat pos [2];
    GLfloat v_color [3];
};

int main() {
  auto window = SDL_CreateWindow(
        "triangle",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );

  auto glcontext = SDL_GL_CreateContext(window);

  struct attributes vertices[] = { {{-0.5f, -0.5f}, {1.0, 0.0, 0.0}},
                                   {{0.5f,  -0.5f}, {0.0, 1.0, 0.0}},
                                   {{0.5f,   0.5f}, {0.0, 0.0, 1.0}},
                                   {{-0.5f,   0.5f}, {1.0, 1.0, 0.0}} };

  GLuint elements[] = {
    0, 1, 2,
    2, 3, 0
  };

  GLuint vbo = 0;
  glGenBuffers (1, &vbo);
  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  glBufferData (GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  GLuint ebo;
  glGenBuffers (1, &ebo);
  glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (elements), elements, GL_STATIC_DRAW);

  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);

  auto vertexSource =
      "in vec2 pos;\n"
      "in vec3 color;\n"
      "out vec3 f_color;\n"
      "void main() { f_color = color; gl_Position = vec4 (pos.x, pos.y, 0.0, 1.0); }\n";

  GLuint vertexShader = glCreateShader (GL_VERTEX_SHADER);
  glShaderSource (vertexShader, 1, &vertexSource, nullptr);
  glCompileShader (vertexShader);

  auto fragmentSource =
      "in vec3 f_color; out vec4 outColor; void main() { outColor = vec4(f_color, 1); }\n";

  GLuint fragmentShader = glCreateShader (GL_FRAGMENT_SHADER);
  glShaderSource (fragmentShader, 1, &fragmentSource, nullptr);
  glCompileShader (fragmentShader);

  GLuint shaderProgram = glCreateProgram ();
  glAttachShader (shaderProgram, vertexShader);
  glAttachShader (shaderProgram, fragmentShader);
  glLinkProgram (shaderProgram);
  glUseProgram (shaderProgram);

  auto posAttr = glGetAttribLocation (shaderProgram, "pos");
  auto colorAttr = glGetAttribLocation (shaderProgram, "color");

  for(;;) {
      SDL_Event e;
      while(SDL_PollEvent(&e))
        {
          if(e.type == SDL_QUIT) return 0;
        }

      glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glEnableVertexAttribArray (posAttr);
      glVertexAttribPointer(posAttr, 2, GL_FLOAT, GL_FALSE,
                            sizeof (struct attributes),
                            0);

      glEnableVertexAttribArray (colorAttr);
      glVertexAttribPointer(colorAttr, 3, GL_FLOAT, GL_FALSE,
                            sizeof (struct attributes),
                            (GLvoid*) (2 * sizeof (GLfloat)) );

//      glDrawArrays(GL_TRIANGLES, 0, 3);
      glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      SDL_GL_SwapWindow(window);
    }
}
