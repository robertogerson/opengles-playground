#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>
#include <SDL2/SDL_image.h>

struct sprite {
    GLfloat pos [2];
    GLfloat v_color [3];
    GLfloat tex_coords [2];
};

int window_width = 800;
int window_height = 600;

int main() {
  auto window = SDL_CreateWindow(
        "triangle",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        window_width, window_height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );

  auto glcontext = SDL_GL_CreateContext (window);
  struct sprite vertices[] = { {{-0.5f, -0.5f}, {1.0, 0.0, 0.0}, {1.0f, 1.0f}},
                               {{0.5f,  -0.5f}, {0.0, 1.0, 0.0}, {0.0f, 1.0f}},
                               {{0.5f,   0.5f}, {0.0, 0.0, 1.0}, {0.0f, 0.0f}},
                               {{-0.5f,  0.5f}, {1.0, 1.0, 0.0}, {1.0f, 0.0f}} };

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

  SDL_Surface *sdl_surf = IMG_Load ("gnu.png");
  if (!sdl_surf)
    {
      printf ("Error! Could not load image.");
      exit (1);
    }
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S , GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T , GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  /* load in texture */
  glTexImage2D(	GL_TEXTURE_2D, 0, sdl_surf->format->BytesPerPixel,
                sdl_surf->w, sdl_surf->h, 0, GL_RGBA,
                GL_UNSIGNED_BYTE, sdl_surf->pixels);
  SDL_FreeSurface (sdl_surf);

  auto vertexSource =
      "in vec2 pos;\n"
      "in vec3 color;\n"
      "in vec2 texcoord;\n"
      "out vec3 f_color;"
      "out vec2 f_texcoord;\n"
      "void main() {\n"
      "   f_color = color;\n"
      "   gl_Position = vec4 (pos.x, pos.y, 0.0, 1.0);"
      "   f_texcoord = texcoord;"
      "}\n";

  GLuint vertexShader = glCreateShader (GL_VERTEX_SHADER);
  glShaderSource (vertexShader, 1, &vertexSource, nullptr);
  glCompileShader (vertexShader);

  auto fragmentSource =
      "in vec3 f_color;\n"
      "in vec2 f_texcoord;\n"
      "out vec4 outColor;\n"
      "uniform sampler2D tex;\n"
      "void main() {\n"
      "   outColor = texture (tex, f_texcoord); //* vec4(f_color, 1);\n"
      "}\n";

  GLuint fragmentShader = glCreateShader (GL_FRAGMENT_SHADER);
  glShaderSource (fragmentShader, 1, &fragmentSource, nullptr);
  glCompileShader (fragmentShader);

  GLuint shaderProgram = glCreateProgram ();
  glAttachShader (shaderProgram, vertexShader);
  glAttachShader (shaderProgram, fragmentShader);
  glLinkProgram (shaderProgram);
  glUseProgram (shaderProgram);

  GLint posAttr = glGetAttribLocation (shaderProgram, "pos");
  GLint colorAttr = glGetAttribLocation (shaderProgram, "color");
  GLint texAttr = glGetAttribLocation (shaderProgram, "texcoord");

  for(;;) {
      SDL_Event e;
      while(SDL_PollEvent(&e))
        {
          if (e.type == SDL_WINDOWEVENT)
            {
              switch (e.window.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                  window_width = e.window.data1;
                  window_height = e.window.data2;
                  break;
                }
            }
          else if(e.type == SDL_QUIT) return 0;

        }

      glViewport (0, 0, window_width, window_height);
      glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      vertices[0].pos[0] -= 0.0005;

      glEnableVertexAttribArray (posAttr);
      glVertexAttribPointer(posAttr, 2, GL_FLOAT, GL_FALSE,
                            sizeof (struct sprite),
                            0);

      glEnableVertexAttribArray (colorAttr);
      glVertexAttribPointer (colorAttr, 3, GL_FLOAT, GL_FALSE,
                             sizeof (struct sprite),
                             (GLvoid*) (2 * sizeof (GLfloat)));

      glEnableVertexAttribArray (texAttr);
      glVertexAttribPointer (texAttr, 2, GL_FLOAT, GL_FALSE,
                             sizeof (struct sprite),
                             (GLvoid*) (5 * sizeof (GLfloat)) );

//      glDrawArrays (GL_TRIANGLES, 0, 3);
      glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      SDL_GL_SwapWindow(window);
    }
}
