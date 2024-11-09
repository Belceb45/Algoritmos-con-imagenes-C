#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_img/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_img/stb_image_write.h"

// Declaraciones
void rotar_imagen_90(unsigned char *img, int width, int height, unsigned char *img_rotada);
void invertir_colores(unsigned char *img, int width, int height);

int main()
{
    int width, height, channels;

    // Cargar la imagen en formato RGB
    unsigned char *img = stbi_load("img2.png", &width, &height, &channels, 3); // 3 indica que queremos RGB

    if (img == NULL)
    {
        printf("Error al cargar la imagen.\n");
        return 1;
    }

    printf("Imagen cargada: %d x %d, canales: %d\n", width, height, channels);

    // Crear una nueva imagen para la rotación + cambio de color
    unsigned char *img_modificada = (unsigned char *)malloc(width * height * 3); // La imagen rotada tendrá un tamaño de width * height * 3
    if (img_modificada == NULL)
    {
        printf("Error al asignar memoria para la imagen rotada y modificada.\n");
        stbi_image_free(img);
        return 1;
    }

    // Primero, rotar la imagen 90 grados en sentido horario
    rotar_imagen_90(img, width, height, img_modificada);

    // Ahora, aplicar el cambio de colores (invertir)
    // invertir_colores(img_modificada, height, width); // Aquí usamos 'height' y 'width' invertidos

    // Guardar la nueva imagen modificada y rotada
    if (stbi_write_jpg("imagen_rotada_y_modificada.jpg", height, width, 3, img_modificada, 90) == 0)
    {
        printf("Error al guardar la imagen.\n");
        stbi_image_free(img);
        free(img_modificada);
        return 1;
    }

    printf("Imagen rotada y modificada guardada correctamente.\n");

    // Liberar la memoria
    stbi_image_free(img);
    free(img_modificada);
    return 0;
}

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