#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_img/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_img/stb_image_write.h"


// Declaraciones
void rotar_imagen_90(unsigned char *img, int width, int height, unsigned char *img_rotada);
void rotar_pixel_recursivo(unsigned char *img, int size, unsigned char *img_rotada, int x, int y);
void invertir_colores(unsigned char *img, int width, int height);

int main()
{
    int width, height, channels;

    // Cargar la imagen en formato RGB
    unsigned char *img = stbi_load("imagen.png", &width, &height, &channels, 3); // 3 indica que queremos RGB

    if (img == NULL)
    {
        printf("Error al cargar la imagen.\n");
        return 1;
    }

    printf("Imagen cargada: %d x %d, canales: %d\n", width, height, channels);

    // Crear una nueva imagen para la rotación + cambio de color
    unsigned char *img_modificada = (unsigned char *)malloc(width * height * 3);
    if (img_modificada == NULL)
    {
        printf("Error al asignar memoria para la imagen rotada y modificada.\n");
        stbi_image_free(img);
        return 1;
    }

    rotar_imagen_90(img, width, height, img_modificada);

    // Guardar la nueva imagen modificada y rotada
    if (stbi_write_jpg("imagen_modificada.jpg", height, width, 3, img_modificada, 90) == 0)
    {
        printf("Error al guardar la imagen.\n");
        stbi_image_free(img);
        free(img_modificada);
        return 1;
    }

    // Forma recursiva
    unsigned char *img_modificada_recu = (unsigned char *)malloc(width * height * 3);
    if (img_modificada_recu == NULL)
    {
        printf("Error al asignar memoria para la imagen rotada y modificada.\n");
        stbi_image_free(img);
        free(img_modificada);
        return 1;
    }

    printf("Imagen rotada y modificada guardada correctamente 1.\n");

    rotar_pixel_recursivo(img, width, img_modificada_recu, 0, 0);
    if (stbi_write_jpg("imagen_modificada_recu.jpg", height, width, 3, img_modificada_recu, 90) == 0)
    {
        printf("Error al guardar la imagen.\n");
        stbi_image_free(img);
        free(img_modificada);
        free(img_modificada_recu);
        return 1;
    }

    printf("Imagen rotada y modificada guardada correctamente 2.\n");

    // Liberar la memoria
    stbi_image_free(img);
    free(img_modificada);
    free(img_modificada_recu); // Liberar img_modificada_recu también

    return 0;
}


//Funcion para rotar la imagen de forma iterativa
void rotar_imagen_90(unsigned char *img, int width, int height, unsigned char *img_rotada)
{
    // Cada píxel en la imagen original es RGB, y estamos rotando 90 grados.
    // La nueva imagen tendrá el ancho y la altura intercambiados.
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Índice del píxel en la imagen original (img)
            int index_original = (y * width + x) * 3;

            // Índice del píxel en la imagen rotada (img_rotada)
            int index_rotada = (x * height + (height - 1 - y)) * 3;

            // Copiar el valor RGB del píxel de la imagen original a la imagen rotada
            img_rotada[index_rotada] = img[index_original];         // R
            img_rotada[index_rotada + 1] = img[index_original + 1]; // G
            img_rotada[index_rotada + 2] = img[index_original + 2]; // B
        }
    }
}





//Funcion para rotar la imagen de forma recursiva
void rotar_pixel_recursivo(unsigned char *img, int size, unsigned char *img_rotada, int x, int y)
{
    // Caso base: Si hemos alcanzado todos los píxeles
    if (y >= size) {
        return;  // Se ha recorrido toda la imagen, termina la recursión
    }
    if (x >= size) {
        // Al llegar al final de una fila, movemos al siguiente
        rotar_pixel_recursivo(img, size, img_rotada, 0, y + 1);
        return;
    }
    
    // Índice del píxel en la imagen original (img)
    int index_original = (y * size + x) * 3;

    // Índice del píxel en la imagen rotada (img_rotada)
    // Para una rotación de 90 grados en sentido horario:
    int index_rotada = ((size - 1 - x) * size + y) * 3;

    // Copiar el valor RGB del píxel de la imagen original a la imagen rotada
    img_rotada[index_rotada] = img[index_original];         // R
    img_rotada[index_rotada + 1] = img[index_original + 1]; // G
    img_rotada[index_rotada + 2] = img[index_original + 2]; // B

    // Llamada recursiva para el siguiente píxel en la fila
    rotar_pixel_recursivo(img, size, img_rotada, x + 1, y);
}

void invertir_colores(unsigned char *img, int width, int height)
{
    // Modificar cada píxel de la imagen
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Calcular el índice del píxel en el arreglo 1D (la imagen es una matriz de píxeles)
            int index = (y * width + x) * 3; // Cada píxel tiene 3 componentes (R, G, B)

            // Obtener los valores de los componentes RGB
            unsigned char r = img[index];     // Componente rojo
            unsigned char g = img[index + 1]; // Componente verde
            unsigned char b = img[index + 2]; // Componente azul

            // Invertir los colores
            img[index] = 255 - r;     // Invertir rojo
            img[index + 1] = 255 - g; // Invertir verde
            img[index + 2] = 255 - b; // Invertir azul
        }
    }
}