#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_img/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_img/stb_image_write.h"

void rotar_pixel_recursivo(unsigned char *img, int size, unsigned char *img_rotada, int x, int y, int channels)
{
    // Caso base: Si hemos alcanzado todos los píxeles
    if (y >= size) {
        return;  // Se ha recorrido toda la imagen, termina la recursión
    }
    if (x >= size) {
        // Al llegar al final de una fila, movemos al siguiente
        rotar_pixel_recursivo(img, size, img_rotada, 0, y + 1, channels);
        return;
    }
    
    // Índice del píxel en la imagen original (img)
    int index_original = (y * size + x) * channels;

    // Índice del píxel en la imagen rotada (img_rotada)
    // Para una rotación de 90 grados en sentido horario:
    int index_rotada = ((size - 1 - x) * size + y) * channels;

    // Copiar todos los canales del píxel de la imagen original a la imagen rotada
    for (int i = 0; i < channels; i++) {
        img_rotada[index_rotada + i] = img[index_original + i];
    }

    // Llamada recursiva para el siguiente píxel en la fila
    rotar_pixel_recursivo(img, size, img_rotada, x + 1, y, channels);
}

int main() {
    int width, height, channels;

    // Cargar la imagen manteniendo el canal alfa si existe
    unsigned char *img = stbi_load("imagen.png", &width, &height, &channels, 0);
    if (img == NULL) {
        printf("Error al cargar la imagen.\n");
        return 1;
    }

    if (width != height) {
        printf("La imagen no es cuadrada. Este programa está diseñado para imágenes cuadradas.\n");
        stbi_image_free(img);
        return 1;
    }

    printf("Imagen cargada: %d x %d, canales: %d\n", width, height, channels);

    // Crear una nueva imagen para la rotación
    unsigned char *img_rotada = (unsigned char *)malloc(width * height * channels);
    if (img_rotada == NULL) {
        printf("Error al asignar memoria para la imagen rotada.\n");
        stbi_image_free(img);
        return 1;
    }

    // Inicializar img_rotada con ceros
    memset(img_rotada, 0, width * height * channels);

    // Rotar la imagen de forma recursiva
    rotar_pixel_recursivo(img, width, img_rotada, 0, 0, channels);

    // Guardar la nueva imagen rotada como PNG para mantener la transparencia
    if (stbi_write_png("imagen_rotada_recursiva.png", width, height, channels, img_rotada, width * channels) == 0) {
        printf("Error al guardar la imagen.\n");
        stbi_image_free(img);
        free(img_rotada);
        return 1;
    }

    printf("Imagen rotada guardada correctamente como 'imagen_rotada_recursiva.png'.\n");

    // Liberar la memoria
    stbi_image_free(img);
    free(img_rotada);

    return 0;
}