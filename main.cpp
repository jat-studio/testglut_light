#include <GL/glut.h>
#include <IL/il.h>
#include <IL/ilu.h>

GLfloat rAngle = 0;         // rotation angle of object

GLuint  IndexTexture[1];    // index of texture

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
  // loading texture into memory
  glGenTextures(1, &IndexTexture[0]);
  glBindTexture(GL_TEXTURE_2D, IndexTexture[0]);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  gluBuild2DMipmaps(GL_TEXTURE_2D, bpp, width, height, type, GL_UNSIGNED_BYTE, data);
  // clear array
  if (data){
    delete [] data;
    data = NULL;
  }
}

// painting Scene
void Draw(void){
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
   glTranslatef(0.0, 0.0, -5.0);
   glRotatef(rAngle, 1.0, 0.0, 0.0);
   glBindTexture(GL_TEXTURE_2D, IndexTexture[0]);
   glBegin(GL_TRIANGLES);
     glTexCoord2f(0.5, 1.0); glVertex3f(0.0, 1.0, 0.0);
     glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.577);
     glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, -1.0, -0.288);

     glTexCoord2f(0.5, 1.0); glVertex3f(0.0, 1.0, 0.0);
     glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.577);
     glTexCoord2f(0.0, 0.0); glVertex3f(0.5, -1.0, -0.288);

     glTexCoord2f(0.5, 1.0); glVertex3f(0.0, 1.0, 0.0);
     glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, -1.0, -0.288);
     glTexCoord2f(0.0, 0.0); glVertex3f(0.5, -1.0, -0.288);

     glTexCoord2f(0.5, 1.0); glVertex3f(0.0, -1.0, 0.577);
     glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, -1.0, -0.288);
     glTexCoord2f(0.0, 0.0); glVertex3f(0.5, -1.0, -0.288);
   glEnd();
   glutSwapBuffers();
}

// main cycle
void Idle(void){
  rAngle++;
  Draw();
}

// setting OpenGL
void Reshape(GLsizei Width, GLsizei Height){
  glClearColor(0, 0, 0, 0);
  glClearDepth(GL_TRUE);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glViewport(0, 0, Width, Height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (GLfloat)Width/(GLfloat)Height, 0.1, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char *argv[]){
  // initializing and create window GLUT
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInit(&argc, argv);
  glutInitWindowSize(640, 480);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Open GL Hello World!");
  // loading textures
  LoadTexture("config.bmp");
  glEnable(GL_TEXTURE_2D);
  // defining events of window
  glutDisplayFunc(Draw);
  glutIdleFunc(Idle);
  glutReshapeFunc(Reshape);
  // processing events of window
  glutMainLoop();
  // clear textures
  glDeleteTextures(1, &IndexTexture[0]);
  return 0;
}
