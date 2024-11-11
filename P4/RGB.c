/*INSTITUTO POLITECNICO NACIONAL
ESCUELA SUPERIOR DE COMPUTO

Analisis y Diseno de Algoritmos
Grupo: 3CM3

Alumnos:
    Rubio Haro Diego

Profesor:
    LUNA BENOSO BEJAMIN

Fecha: 10-Noviembre-2024

Practica 4: Divide y Venceras//
    -Rotar una Imagen (RGB/ASCII)*/


#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_img/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_img/stb_image_write.h"

// Declaraciones de funciones
void rotar_imagen_90(unsigned char *img, int width, int height, unsigned char *img_rotada);
void rotar_imagen_recursiva(unsigned char *img, unsigned char *img_rotada, int x, int y, int width, int height, int channels, int full_width);
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
        stbi_image_free(img); // Liberar imagen original si hubo error
        return 1;
    }

    // Mostrar opciones para elegir el tipo de rotación
    int opcion,caso;
    printf("Elige el tipo de caso:\n");
    printf("1. ASCII\n");
    printf("2. RGB\n");
    printf("Elige el tipo de rotacion:\n");
    printf("1. Rotacion iterativa\n");
    printf("2. Rotacion recursiva\nR: ");
    scanf("%d", &opcion);

    // Rotar la imagen según la opción seleccionada
    if (opcion == 1)
    {
        rotar_imagen_90(img, width, height, img_modificada);
        // Guardar la nueva imagen rotada e invertida
        if (stbi_write_png("imagen_modificada.png", width, height, 3, img_modificada, width * 3) == 0)
        {
            printf("Error al guardar la imagen.\n");
            stbi_image_free(img);
            free(img_modificada);
            return 1;
        }
        printf("Imagen rotada y modificada guardada correctamente.\n");
    }
    else if (opcion == 2)
    {
        unsigned char *img_modificada_recu = (unsigned char *)malloc(width * height * 3);
        if (img_modificada_recu == NULL)
        {
            printf("Error al asignar memoria para la imagen rotada y modificada recursivamente.\n");
            stbi_image_free(img);
            free(img_modificada);
            return 1;
        }

        // Rotar la imagen de forma recursiva
        rotar_imagen_recursiva(img, img_modificada_recu, 0, 0, width, height, channels, width);

        // Guardar la imagen rotada recursivamente
        if (stbi_write_png("imagen_modificada_recu.png", height, width, channels, img_modificada_recu, height * channels) == 0)
        {
            printf("Error al guardar la imagen recursiva.\n");
            // No liberar img_modificada_recu para que no se borre antes de tiempo
            return 1;
        }

        printf("Imagen rotada recursivamente guardada correctamente.\n");

        // No liberar img_modificada_recu porque no se quiere borrar aún
    }
    else
    {
        printf("Opción no válida.\n");
        stbi_image_free(img);
        free(img_modificada);
        return 1;
    }

    // Ahora no liberamos la memoria de las imágenes, las dejamos tal como están
    stbi_image_free(img);
    free(img_modificada);

    return 0;
}

// Función para rotar la imagen 90 grados de manera iterativa
void rotar_imagen_90(unsigned char *img, int width, int height, unsigned char *img_rotada)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Índice del píxel en la imagen original
            int index_original = (y * width + x) * 3;

            // Índice del píxel en la imagen rotada
            int index_rotada = (x * height + (height - 1 - y)) * 3;

            // Copiar el valor RGB del píxel de la imagen original a la imagen rotada
            img_rotada[index_rotada] = img[index_original];         // R
            img_rotada[index_rotada + 1] = img[index_original + 1]; // G
            img_rotada[index_rotada + 2] = img[index_original + 2]; // B
        }
    }
}

// Función recursiva para rotar 90 grados una subimagen
void rotar_imagen_recursiva(unsigned char *img, unsigned char *img_rotada, int x, int y, int width, int height, int channels, int full_width)
{
    // Caso base: Si la subimagen tiene solo un píxel
    if (width == 1 && height == 1)
    {
        int idx = (y * full_width + x) * channels;
        int rot_idx = (x * full_width + (full_width - 1 - y)) * channels;
        for (int i = 0; i < channels; i++)
        {
            img_rotada[rot_idx + i] = img[idx + i]; // Copiar el píxel en la nueva posición
        }
        return;
    }

    // Dividir la imagen en cuatro subimágenes
    int mid_width = width / 2;
    int mid_height = height / 2;

    // Llamadas recursivas para cada subimagen
    if (mid_width > 0 || mid_height > 0)
    {
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

// Función para invertir los colores de la imagen
void invertir_colores(unsigned char *img, int width, int height)
{
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


/*
    

Descripción General;

Este programa carga una imagen en formato RGB, permite al usuario elegir entre dos tipos de rotación (iterativa o recursiva), aplica la rotación seleccionada y guarda la imagen resultante en un archivo. Además, incluye una función para invertir los colores de la imagen.

### Dependencias

El programa utiliza las bibliotecas `stb_image` y `stb_image_write` para cargar y guardar imágenes. Estas bibliotecas deben estar incluidas en el proyecto.

### Funciones

#### `main()`

1. **Carga de la imagen**:
    - Utiliza `stbi_load` para cargar una imagen en formato RGB desde un archivo llamado `imagen.png`.
    - Verifica si la imagen se cargó correctamente. Si no, imprime un mensaje de error y termina el programa.

2. **Asignación de memoria**:
    - Asigna memoria para una nueva imagen que se utilizará para almacenar la imagen rotada y modificada.

3. **Selección de la opción de rotación**:
    - Muestra un menú para que el usuario elija entre rotación iterativa (opción 1) o rotación recursiva (opción 2).

4. **Rotación iterativa**:
    - Si el usuario elige la opción 1, llama a la función `rotar_imagen_90` para rotar la imagen 90 grados de manera iterativa.
    - Guarda la imagen rotada en un archivo llamado `imagen_modificada.png` utilizando `stbi_write_png`.
    - Verifica si la imagen se guardó correctamente. Si no, imprime un mensaje de error y termina el programa.

5. **Rotación recursiva**:
    - Si el usuario elige la opción 2, asigna memoria para una nueva imagen que se utilizará para almacenar la imagen rotada recursivamente.
    - Llama a la función `rotar_imagen_recursiva` para rotar la imagen 90 grados de manera recursiva.
    - Guarda la imagen rotada en un archivo llamado `imagen_modificada_recu.png` utilizando `stbi_write_png`.
    - Verifica si la imagen se guardó correctamente. Si no, imprime un mensaje de error y termina el programa.

6. **Liberación de memoria**:
    - Libera la memoria asignada para las imágenes cargadas y modificadas.

#### `rotar_imagen_90(unsigned char *img, int width, int height, unsigned char *img_rotada)`

Esta función rota una imagen 90 grados de manera iterativa.

- **Parámetros**:
    - `img`: Puntero a la imagen original.
    - `width`: Ancho de la imagen original.
    - `height`: Altura de la imagen original.
    - `img_rotada`: Puntero a la imagen rotada.

- **Funcionamiento**:
    - Recorre cada píxel de la imagen original.
    - Calcula el índice del píxel en la imagen original y en la imagen rotada.
    - Copia los valores RGB del píxel de la imagen original a la imagen rotada en la nueva posición.

#### `rotar_imagen_recursiva(unsigned char *img, unsigned char *img_rotada, int x, int y, int width, int height, int channels, int full_width)`

Esta función rota una imagen 90 grados de manera recursiva.

- **Parámetros**:
    - `img`: Puntero a la imagen original.
    - `img_rotada`: Puntero a la imagen rotada.
    - `x`: Coordenada x de la subimagen.
    - `y`: Coordenada y de la subimagen.
    - `width`: Ancho de la subimagen.
    - `height`: Altura de la subimagen.
    - `channels`: Número de canales de la imagen.
    - `full_width`: Ancho total de la imagen original.

- **Funcionamiento**:
    - Caso base: Si la subimagen tiene solo un píxel, copia el píxel a la nueva posición en la imagen rotada.
    - Divide la imagen en cuatro subimágenes.
    - Llama recursivamente a sí misma para rotar cada una de las subimágenes.

#### `invertir_colores(unsigned char *img, int width, int height)`

Esta función invierte los colores de una imagen.

- **Parámetros**:
    - `img`: Puntero a la imagen.
    - `width`: Ancho de la imagen.
    - `height`: Altura de la imagen.

- **Funcionamiento**:
    - Recorre cada píxel de la imagen.
    - Calcula el índice del píxel en el arreglo 1D.
    - Invierte los valores RGB del píxel.

### Ejecución del Programa

1. **Compilación**:
    - Asegúrate de tener las bibliotecas `stb_image` y `stb_image_write` en tu proyecto.
    - Compila el programa con un compilador de C, por ejemplo:
      ```sh
      gcc -o rotar_imagen RGB.c -lm
      ```

2. **Ejecución**:
    - Ejecuta el programa:
      ```sh
      ./rotar_imagen
      ```

3. **Interacción**:
    - El programa cargará la imagen `imagen.png`.
    - Te pedirá que elijas entre rotación iterativa (1) o rotación recursiva (2).
    - Guardará la imagen rotada en `imagen_modificada.png` o `imagen_modificada_recu.png` según la opción seleccionada.

### Notas

- Asegúrate de que el archivo `imagen.png` esté en el mismo directorio que el ejecutable del programa.
- La función `invertir_colores` no se utiliza en el flujo actual del programa, pero está disponible para su uso si es necesario.

Esta documentación debería ayudarte a entender cómo funciona el programa y cómo interactuar con él.


 */