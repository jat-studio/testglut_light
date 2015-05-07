#include <GL/glut.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <cstring>

int wnd;                    // id GLwindow

GLfloat xAngle = 0, yAngle = 0, zMove = 0; // rotation angle of object and z dimension translating

GLuint  FilterTexture = 0;  // filter of texture cube
GLuint  TextureCube[3];    // index of texture cube

// parameters of image
int width = 0;
int height = 0;
int bpp = 0;
unsigned int type;
unsigned char* data_img;

bool    light = false;      // lights on or off
bool    blend = false;      // blending on or off
GLfloat LightAmbient[] = {0.5, 0.5, 0.5, 1.0}; // array of ambient light
GLfloat LightDiffuse[] = {1.0, 1.0, 1.0, 1.0}; // array of diffuse light
GLfloat LightPosition[] = {0.5, 0.5, 0.5, 1.0}; // coordinates of light source

bool    enable_stars = false;      // rotating stars on or off
const int num = 50;                // count of stars

typedef struct{
  int r, g, b;   // colors
  GLfloat dist;  // distance from center
  GLfloat angle; // current angle of star
}
stars;

stars star[num]; // array of stars

// parameters of stars
GLfloat zoom = -15.0;
GLfloat tilt = 90.0;
GLfloat spin;
GLuint loop;
GLuint TextureStar[1];

void LoadTexture(const char* texName){
  // initializing il and ilu library
  ilInit();
  iluInit();
  // loading texture
  char ext[5] = "";
  ext[0] = texName[strlen(texName) - 4];
  ext[1] = texName[strlen(texName) - 3];
  ext[2] = texName[strlen(texName) - 2];
  ext[3] = texName[strlen(texName) - 1];
  ext[4] = NULL;
  if (strcmp(ext, ".bmp") == 0){
    ilLoad(IL_BMP, texName);
  }
  if (strcmp(ext, ".tga") == 0){
    ilLoad(IL_TGA, texName);
  }
  // processing of errors
  int err = ilGetError();
  if (err != IL_NO_ERROR){
    const char* strError = iluErrorString(err);
    printf("Error loading texture!\n");
    printf(strError);
    exit(EXIT_FAILURE);
  }
  // getting parameters of image
  width  = ilGetInteger(IL_IMAGE_WIDTH);
  height = ilGetInteger(IL_IMAGE_HEIGHT);
  bpp    = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
  // getting data of image
  data_img = ilGetData();
  // determination type of image
  switch (bpp){
    case 1:
      type = GL_RGB8;
    break;
    case 3:
      type = GL_RGB;
    break;
    case 4:
      type = GL_RGBA;
    break;
  }
}

// painting Scene
void Draw(void){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  /*glLoadIdentity();
  glTranslatef(0.0, 0.0, -6.0);
  glRotatef(xAngle, 1.0, 0.0, 0.0);
  glRotatef(yAngle, 0.0, 1.0, 0.0);
  glBindTexture(GL_TEXTURE_2D, TextureCube[FilterTexture]);
  // cube
  glBegin(GL_QUADS);
    // face
    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, 1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, 1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, 1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, 1.0);

    // back
    glNormal3f(0.0, 0.0, -1.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, -1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -1.0, -1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, 1.0, -1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, -1.0);

    // top
    glNormal3f(0.0, 1.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 1.0, 1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.0, 1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, -1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, -1.0);

    // bottom
    glNormal3f(0.0, -1.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, -1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, -1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -1.0, 1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0, 1.0);

    // left
    glNormal3f(-1.0, 0.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, -1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -1.0, 1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, 1.0, 1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, -1.0);

    // right
    glNormal3f(1.0, 0.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, -1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, -1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 1.0);
  glEnd();*/

  glBindTexture(GL_TEXTURE_2D, TextureStar[0]);
  for (loop = 0; loop < num; loop++){
    glLoadIdentity();
    glTranslatef(0.0, 0.0, zoom);
    glRotatef(tilt, 1.0, 0.0, 0.0);
    glRotatef(star[loop].angle, 0.0, 1.0, 0.0);
    glTranslatef(star[loop].dist, 0.0, 0.0);

    glRotatef(-star[loop].angle, 0.0, 1.0, 0.0);
    glRotatef(-tilt, 1.0, 0.0, 0.0);
    if (enable_stars){
      glColor4ub(star[(num - loop) - 1].r,
                 star[(num - loop) - 1].g,
                 star[(num - loop) - 1].b, 255);
      glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, 0.0);
        glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
        glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
        glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, 0.0);
      glEnd();
    }

    glRotatef(spin, 0.0, 0.0, 1.0);
    glColor4ub(star[loop].r, star[loop].g, star[loop].b, 255);
    glBegin(GL_QUADS);
      glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, 0.0);
      glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, 0.0);
    glEnd();
    spin += 0.01;
    star[loop].angle += float(loop) / num;
    star[loop].dist -= 0.01;
    if (star[loop].dist < 0.0){
      star[loop].dist += 5.0;
      star[loop].r = rand()%256;
      star[loop].g = rand()%256;
      star[loop].b = rand()%256;
    }
  }
  glutSwapBuffers();
}

// main cycle
void Idle(void){
  xAngle ++;
  yAngle ++;
  Draw();
}

// setting OpenGL
void Reshape(GLsizei Width, GLsizei Height){
  // fon color
  glClearColor(0, 0, 0, 0);
  // depth test
  glClearDepth(GL_TRUE);
  //glEnable(GL_DEPTH_TEST);
  //glDepthFunc(GL_LEQUAL);
  // smoothing paint of color
  glShadeModel(GL_SMOOTH);
  // blending
  glColor4f(1.0, 1.0, 1.0, 0.5); // full brightness 50% blending
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  // modificated perspective
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  // window settings
  glViewport(0, 0, Width, Height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (GLfloat)Width/(GLfloat)Height, 0.1, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

void Keyboard(unsigned char key, int x, int y){
  //printf("%u", key);
  //printf("\n");
  switch (key){
    // escape
    case 27:
      glDeleteTextures(3, &TextureCube[0]);
      glDeleteTextures(1, &TextureStar[0]);
      glutDestroyWindow(wnd);
    case 'l':
      if (light){
        glDisable(GL_LIGHTING);
        light = false;
      }
      else{
        glEnable(GL_LIGHTING);
        light = true;
      }
    break;
    case 't':
      if (enable_stars){
        enable_stars = false;
      }
      else{
        enable_stars = true;
      }
    break;
    case 'f':
      if (FilterTexture > 1){
        FilterTexture = 0;
      }
      else{
        FilterTexture++;
      }
    break;
    case 'b':
      if (blend){
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        blend = false;
      }
      else{
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        blend = true;
      }
    break;
    case GLUT_KEY_UP:
      tilt -= 0.5;
    break;
    case GLUT_KEY_DOWN:
      tilt += 0.5;
    break;
    case GLUT_KEY_PAGE_UP:
      zoom -= 0.2;
    break;
    case GLUT_KEY_PAGE_DOWN:
      zoom += 0.2;
    break;
  }
}

int main(int argc, char *argv[]){
  // initializing and create window GLUT
  glutInit(&argc, argv);
  glutInitWindowSize(1920, 1080);
  glutInitWindowPosition(0, 0);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  wnd = glutCreateWindow("Open GL Hello World!");

  // getting data of textures of cube
  LoadTexture("cube.bmp");
  // loading textures into memory
  glGenTextures(3, &TextureCube[0]);
  // texture with filter on neighboring pixels
  glBindTexture(GL_TEXTURE_2D, TextureCube[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  gluBuild2DMipmaps(GL_TEXTURE_2D, bpp, width, height, type, GL_UNSIGNED_BYTE, data_img);
  // texture with linear filter
  glBindTexture(GL_TEXTURE_2D, TextureCube[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  gluBuild2DMipmaps(GL_TEXTURE_2D, bpp, width, height, type, GL_UNSIGNED_BYTE, data_img);
  // texture with mip-map
  glBindTexture(GL_TEXTURE_2D, TextureCube[2]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  gluBuild2DMipmaps(GL_TEXTURE_2D, bpp, width, height, type, GL_UNSIGNED_BYTE, data_img);
  // getting data of textures of stars
  LoadTexture("star.tga");
  // loading textures into memory
  glGenTextures(1, &TextureStar[0]);
  // texture with linear filter
  glBindTexture(GL_TEXTURE_2D, TextureStar[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  gluBuild2DMipmaps(GL_TEXTURE_2D, bpp, width, height, type, GL_UNSIGNED_BYTE, data_img);
  // enabling textures
  glEnable(GL_TEXTURE_2D);

  // initializing stars
  for (loop = 0; loop < num; loop++){
    star[loop].angle = 0.0;
    star[loop].dist = (float(loop) / num) * 5.0;
    star[loop].r = rand()%256;
    star[loop].g = rand()%256;
    star[loop].b = rand()%256;
  }

  // installing fon light
  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
  // installing diffuse light
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
  // position of light
  glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
  // enable permission on light
  glEnable(GL_LIGHT1);

  // defining events of window
  glutDisplayFunc(Draw);
  glutReshapeFunc(Reshape);
  glutIdleFunc(Idle);
  glutKeyboardFunc(Keyboard);
  // processing events of window
  glutMainLoop();
  // clear textures
  glDeleteTextures(3, &TextureCube[0]);
  glDeleteTextures(1, &TextureStar[0]);
  return 0;
}
