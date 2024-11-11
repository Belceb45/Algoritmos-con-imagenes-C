#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_img/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_img/stb_image_write.h"

// Función recursiva para rotar 90 grados una subimagen
void rotar_imagen_recursiva(unsigned char *img, unsigned char *img_rotada, int x, int y, int width, int height, int channels, int full_width) {
    // Caso base: Si la subimagen tiene solo un píxel
    if (width == 1 && height == 1) {
        int idx = (y * full_width + x) * channels;
        int rot_idx = (x * full_width + (full_width - 1 - y)) * channels;
        for (int i = 0; i < channels; i++) {
            img_rotada[rot_idx + i] = img[idx + i];  // Copiar el píxel en la nueva posición
        }
        return;
    }

    // Dividir la imagen en cuatro subimágenes
    int mid_width = width / 2;
    int mid_height = height / 2;

    // Llamadas recursivas para cada subimagen
    if (mid_width > 0 || mid_height > 0) {
        // Subimagen superior izquierda
        rotar_imagen_recursiva(img, img_rotada, x, y, mid_width, mid_height, channels, full_width);
        
        // Subimagen superior derecha
        rotar_imagen_recursiva(img, img_rotada, x + mid_width, y, width - mid_width, mid_height, channels, full_width);
        
        // Subimagen inferior izquierda
        rotar_imagen_recursiva(img, img_rotada, x, y + mid_height, mid_width, height - mid_height, channels, full_width);
        
        // Subimagen inferior derecha
        rotar_imagen_recursiva(img, img_rotada, x + mid_width, y + mid_height, width - mid_width, height - mid_height, channels, full_width);
    }
}


// Función principal
int main() {
    int width, height, channels;

    // Cargar la imagen
    unsigned char *img = stbi_load("imagen.png", &width, &height, &channels, 0);
    if (!img) {
        printf("Error al cargar la imagen.\n");
        return 1;
    }

    printf("Imagen cargada: %d x %d, canales: %d\n", width, height, channels);

    // Crear una imagen rotada vacía
    unsigned char *img_rotada = (unsigned char *)malloc(width * height * channels);
    if (img_rotada == NULL) {
        printf("Error al asignar memoria para la imagen rotada.\n");
        stbi_image_free(img);
        return 1;
    }

    // Inicializar img_rotada con ceros
    memset(img_rotada, 0, width * height * channels);

    // Rotar la imagen de forma recursiva
    rotar_imagen_recursiva(img, img_rotada, 0, 0, width, height, channels, width);

    // Guardar la imagen rotada
    if (stbi_write_png("imagen_rotada.png", height, width, channels, img_rotada, height * channels) == 0) {
        printf("Error al guardar la imagen.\n");
        stbi_image_free(img);
        free(img_rotada);
        return 1;
    }

    printf("Imagen rotada guardada correctamente.\n");

    // Liberar la memoria
    stbi_image_free(img);
    free(img_rotada);

    return 0;
}
