#include <GL/glut.h>
#include <IL/il.h>
#include <IL/ilu.h>

GLfloat xAngle = 0, yAngle = 0, zMove = 0; // rotation angle of object and z dimension translating

GLuint  FilterTexture;      // filter of texture
GLuint  IndexTexture[3];    // index of texture

bool    keys[256];          // keys of keyboard
bool    active = true;      // application activity
bool    fullscreen = true;
bool    light;              // lights on or off
bool    Lb;                 // pressed or not "L" button
bool    Fb;                 // pressed or not "F" button
GLfloat LightAmbient[] = {0.5, 0.5, 0.5, 1.0}; // array of ambient light
GLfloat LightDiffuse[] = {1.0, 1.0, 1.0, 1.0}; // array of diffuse light
GLfloat LightPosition[] = {0.5, 0.5, 0.5, 1.0}; // coordinates of light source

void LoadTexture(const char* texName){
  // initializing il and ilu library
  ilInit();
  iluInit();
  // loading texture
  ilLoad(IL_BMP, texName);
  // processing of errors
  int err = ilGetError();
  if (err != IL_NO_ERROR){
    const char* strError = iluErrorString(err);
    printf("Error loading texture!\n");
    printf(strError);
    exit(EXIT_FAILURE);
  }
  // getting parameters of image
  int width  = ilGetInteger(IL_IMAGE_WIDTH);
  int height = ilGetInteger(IL_IMAGE_HEIGHT);
  int bpp    = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
  // getting data of image
  unsigned char* data = ilGetData();
  // determination type of image
  unsigned int type;
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
  // loading textures into memory
  glGenTextures(3, &IndexTexture[0]);
  // texture with filter on neighboring pixels
  glBindTexture(GL_TEXTURE_2D, IndexTexture[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  gluBuild2DMipmaps(GL_TEXTURE_2D, bpp, width, height, type, GL_UNSIGNED_BYTE, data);
  // texture with linear filter
  glBindTexture(GL_TEXTURE_2D, IndexTexture[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  gluBuild2DMipmaps(GL_TEXTURE_2D, bpp, width, height, type, GL_UNSIGNED_BYTE, data);
  // texture with mip-map
  glBindTexture(GL_TEXTURE_2D, IndexTexture[2]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  gluBuild2DMipmaps(GL_TEXTURE_2D, bpp, width, height, type, GL_UNSIGNED_BYTE, data);
}

// painting Scene
void Draw(void){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -6.0);
  glRotatef(xAngle, 1.0, 0.0, 0.0);
  glRotatef(yAngle, 0.0, 1.0, 0.0);
  glBindTexture(GL_TEXTURE_2D, IndexTexture[0]);
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
  glEnd();
  glutSwapBuffers();
  glutPostRedisplay();
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
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  // smoothing
  glShadeModel(GL_SMOOTH);
  // modificated perspective
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  // window settings
  glViewport(0, 0, Width, Height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (GLfloat)Width/(GLfloat)Height, 0.1, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char *argv[]){
  // initializing and create window GLUT
  glutInit(&argc, argv);
  glutInitWindowSize(640, 480);
  glutInitWindowPosition(0, 0);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("Open GL Hello World!");
  glutFullScreen();
  // loading textures
  LoadTexture("config.bmp");
  glEnable(GL_TEXTURE_2D);
  // installing fon light
  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
  // installing diffuse light
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
  // position of light
  glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
  // enable permission on light
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHTING);
  // defining events of window
  glutDisplayFunc(Draw);
  glutIdleFunc(Idle);
  //glutReshapeFunc(Reshape);
  // processing events of window
  glutMainLoop();
  // clear textures
  glDeleteTextures(3, &IndexTexture[0]);
  return 0;
}
