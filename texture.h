#ifndef __TEXTURE_H
#define __TEXTURE_H

#include <IL/il.h>

// Создадим структуру TextureImage для хранения данных текстуры.
typedef struct TextureImage{
  GLubyte *imageData;     // Данные текстуры
  GLuint  bpp;            // Байт на пиксел
  GLuint width,height;    // Высота и ширина
  GLuint texID;           // ID текстуры
}TextureImage;

// Класс текстур. Загрузка и высвобождение.
class CTexture{
public:
    CTexture();
    ~CTexture();
    // Собственно, класс наш делает немного - в нём только 2 функции -
    // загрузка и освобождение текстур =)
    void LoadTexture(ILenum FileType, char *filename, TextureImage *texture);
    void FreeTexture(TextureImage *texture);
};

#endif // __TEXTURE_H
