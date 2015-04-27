#include "texture.h"

// Конструктор
CTexture::CTexture(){
    // В конструкторе инициализируем библиотеки IL и ILU.
    // Если этого не сделать, программа будет вываливатся с ошибкой.
    ilInit();
    iluInit();
}

// Деструктор
CTexture::~CTexture(){
}

// Функция, непосредственно загружающая текстуру в переданную структуру.
// В функцию передается тип файла, имя файла и указатель на текстуру.
void CTexture::LoadTexture(ILenum FileType, char *filename, TextureImage *texture){
    // Просим DevIL загрузить файл
    ilLoad(FileType, filename);

    // Проверяем, нет ли ошибок при открытии файла
    int err=ilGetError();
    if(err!=IL_NO_ERROR){
        const char* strError = iluErrorString(err);
        MessageBox(NULL, strError, "Ошибка при загрузке!", MB_OK);
        exit(1);
    }

    // Получаем данные текстуры
    texture->width = ilGetInteger(IL_IMAGE_WIDTH);          // Ширина
    texture->height = ilGetInteger(IL_IMAGE_HEIGHT);        // Высота
    texture->bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);  // Байт на пиксель

    // Загружаем данные в нашу текстуру
    texture->imageData = ilGetData();

    ilEnable(IL_CONV_PAL);

    // Тип данных изображения
    unsigned int type = ilGetInteger(IL_IMAGE_FORMAT);

    // Генерируем текстуру
    glGenTextures(1, &texture->texID);

    // Привязываем данные текстуры к ID
    glBindTexture(GL_TEXTURE_2D,texture->texID);

    // биндим мип-мапы
    gluBuild2DMipmaps(GL_TEXTURE_2D, texture->bpp, texture->width,
        texture->height, type, GL_UNSIGNED_BYTE, texture->imageData);

    // Устанавливаем качество текстур
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

void CTexture::FreeTexture(TextureImage *texture){
}
