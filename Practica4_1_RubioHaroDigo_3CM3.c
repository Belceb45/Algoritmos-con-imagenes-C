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
#include <string.h>
#include <dirent.h> // Para listar archivos en el directorio

/*Macros para los archivo (lectura del directorio)*/
#define MAX_IMAGES 100
#define MAX_FILENAME 256

/*Macros para maxima longitud de ASCII*/
#define MAX_FILA 1000
#define MAX_COL 1000

#define STB_IMAGE_IMPLEMENTATION
#include "stb_img/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_img/stb_image_write.h"

// Declaraciones de funciones
/*Declaraciones para RGB*/
void rotar_imagen_90(unsigned char *img, int width, int height, unsigned char *img_rotada, int *contador);
void rotar_imagen_recursiva(unsigned char *img, unsigned char *img_rotada, int x, int y, int width, int height, int channels, int full_width, int *contador);
void invertir_colores(unsigned char *img, int width, int height);
/*Declaraciones para ASCII*/
void leer_imagen(FILE *archivo, char imagen[MAX_FILA][MAX_COL], int *filas, int *columnas);
void rotar_imagen(char imagen[MAX_FILA][MAX_COL], int *filas, int *columnas);
void rec(char imagen[MAX_FILA][MAX_COL], int filas, int columnas);
void rotar_imagenASCII_recursiva(char imagen[MAX_FILA][MAX_COL], char transpuesta[MAX_COL][MAX_FILA], int x, int y, int width, int height);
void mostrar_imagen(FILE *archivo_salida, char imagen[MAX_FILA][MAX_COL], int filas, int columnas);
/*Declaraciones para listar imagenes*/
void archivosImg(char nombre[]);
int es_imagen(const char *nombre);
int main()
{
    FILE *tabla = fopen("tabla.csv", "at");
    char name[MAX_FILENAME];
    archivosImg(name);

    printf("Imagen a rotar: %s\n\n", name);

    // Mostrar opciones para elegir el tipo de rotación
    int opcion, caso;
    printf("Elige el tipo de caso:\n");
    printf("1. ASCII\n");
    printf("2. RGB\nR: ");
    scanf("%d", &caso);
    if (caso == 1)
    {
        // Abre el archivo de entrada para lectura
        FILE *archivo = fopen("img1.txt", "r");
        if (archivo == NULL)
        {
            printf("Error al abrir el archivo de entrada. Asegúrate de que el archivo 'img1.txt' exista.\n");
            return 1;
        }

        // Abre el archivo de salida para escribir la imagen rotada
        FILE *archivo_salida = fopen("img2.txt", "w");
        if (archivo_salida == NULL)
        {
            printf("Error al crear el archivo de salida. Asegúrate de tener permisos para escribir en el directorio.\n");
            fclose(archivo); // Cerrar archivo de entrada antes de salir
            return 1;
        }

        // Matriz para almacenar la imagen
        char imagen[MAX_FILA][MAX_COL];
        int filas = 0, columnas = 0;

        // Leer la imagen desde el archivo
        leer_imagen(archivo, imagen, &filas, &columnas);

        // Pedir al usuario el número de rotaciones
        int rotaciones = 0;
        printf("Numero de rotaciones (90 grados cada una): ");
        scanf("%d", &rotaciones);

        // Reducir las rotaciones a un valor entre 0 y 3
        rotaciones = rotaciones % 4;
        if (rotaciones < 0)
            rotaciones += 4; // Asegurar rotaciones positivas

        // Selección del método de rotación
        int metodo = 0;
        printf("1.Iterativo\n");
        printf("2.Recursivo\nR: ");
        scanf("%d", &metodo);

        // Rotar la imagen el número de veces requerido usando el método elegido
        for (int i = 0; i < rotaciones; i++)
        {
            if (metodo == 1)
            {
                // Rotación iterativa
                rotar_imagen(imagen, &filas, &columnas);
            }
            else if (metodo == 2)
            {
                rec(imagen, filas, columnas);
            }
        }
        mostrar_imagen(archivo_salida, imagen, filas, columnas);

        // Mostrar la imagen rotada en el archivo de salida

        // Cerrar los archivos
        fclose(archivo);
        fclose(archivo_salida);

        printf("Imagen rotada y guardada correctamente en 'img2.txt'.\n");
    }
    else if (caso == 2)
    {
        /*RGB*/
        int width, height, channels, contador = 0;
        // Cargar la imagen en formato RGB
        unsigned char *img = stbi_load(name, &width, &height, &channels, 3); // 3 indica que queremos RGB
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

        printf("\n\nElige el tipo de rotacion:\n");
        printf("1. Rotacion iterativa\n");
        printf("2. Rotacion recursiva\nR: ");
        scanf("%d", &opcion);

        // Rotar la imagen según la opción seleccionada
        if (opcion == 1)
        {

            rotar_imagen_90(img, width, height, img_modificada, &contador);
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
            rotar_imagen_recursiva(img, img_modificada_recu, 0, 0, width, height, channels, width, &contador);

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
        printf("\n\nContador: %d", contador);
        fprintf(tabla, "\n%d, %d", width, contador);
        stbi_image_free(img);
        free(img_modificada);
    }

    return 0;
}

// Funciones listado de imagenes ---------------------------------------------------------------------
void archivosImg(char nombre[MAX_FILENAME])
{
    FILE *fp;
    char path[MAX_FILENAME];
    char image_files[MAX_IMAGES][MAX_FILENAME];
    int image_count = 0;

// Comando específico según el sistema operativo
#if defined(_WIN32) || defined(_WIN64)
    fp = popen("dir /b /a-d | findstr /i \".jpg .jpeg .png .gif .bmp .tiff\"", "r");
#elif defined(__linux__) || defined(__APPLE__) && defined(__MACH__)
    fp = popen("ls | grep -E '\\.(jpg|jpeg|png|gif|bmp|tiff)$'", "r");
#else
    printf("Sistema operativo no soportado para listar archivos.\n");
    exit(1);
#endif

    // Verificación de error al ejecutar el comando
    if (fp == NULL)
    {
        perror("Error al ejecutar el comando");
        exit(1);
    }

    // Leer y almacenar los nombres de archivos de imagen en un arreglo
    while (fgets(path, sizeof(path), fp) != NULL && image_count < MAX_IMAGES)
    {
        path[strcspn(path, "\n")] = 0; // Elimina el salto de línea
        if (es_imagen(path))
        {
            strncpy(image_files[image_count], path, MAX_FILENAME);
            image_count++;
        }
    }
    pclose(fp);

    // Muestra la lista de archivos de imagen al usuario
    if (image_count == 0)
    {
        printf("No se encontraron archivos de imagen en el directorio.\n");
        exit(0);
    }

    printf("Archivos de imagen disponibles:\n");
    for (int i = 0; i < image_count; i++)
    {
        printf("%d. %s\n", i + 1, image_files[i]);
    }

    // Solicita al usuario que elija una imagen
    int choice;
    printf("\nElige la imagen que deseas usar: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > image_count)
    {
        printf("ERROR.\n");
        exit(1);
    }

    // Guarda el nombre del archivo seleccionado en el arreglo `selected_image`
    strncpy(nombre, image_files[choice - 1], MAX_FILENAME);
}

// Función para verificar si el archivo es una imagen
int es_imagen(const char *nombre_archivo)
{
    const char *extensiones[] = {".jpg", ".jpeg", ".png", ".gif", ".bmp", ".tiff"};
    for (int i = 0; i < sizeof(extensiones) / sizeof(extensiones[0]); i++)
    {
        if (strstr(nombre_archivo, extensiones[i]) != NULL)
        {
            return 1;
        }
    }
    return 0;
}

// Funciones RGB -----------------------------------------------------------------------------------
// Función para rotar la imagen 90 grados de manera iterativa
void rotar_imagen_90(unsigned char *img, int width, int height, unsigned char *img_rotada, int *contador)
{
    (*contador)++;
    (*contador)++;

    for (int i = 0; i < height; i++)
    {
        (*contador)++;
        (*contador)++;

        for (int j = 0; j < width; j++)
        {
            (*contador)++;
            (*contador)++;

            // Índice del píxel en la imagen original
            int index_original = (i * width + j) * 3;

            // Índice del píxel en la imagen rotada
            int index_rotada = (j * height + (height - 1 - i)) * 3;
            (*contador)++;
            (*contador)++;
            (*contador)++;

            // Copiar el valor RGB del píxel de la imagen original a la imagen rotada
            img_rotada[index_rotada] = img[index_original];         // R
            img_rotada[index_rotada + 1] = img[index_original + 1]; // G
            img_rotada[index_rotada + 2] = img[index_original + 2]; // B
        }
        (*contador)++;
        (*contador)++;
    }
    (*contador)++;
    (*contador)++;
}

// Función recursiva para rotar 90 grados una subimagen
void rotar_imagen_recursiva(unsigned char *img, unsigned char *img_rotada, int x, int y, int width, int height, int channels, int full_width, int *contador)
{
    // Caso base: Si la subimagen tiene solo un píxel
    
    if (width == 1 && height == 1)
    {

    
        int idx = (y * full_width + x) * channels;
        int rot_idx = (x * full_width + (full_width - 1 - y)) * channels;
        
        for (int i = 0; i < channels; i++)
        {
            (*contador)++;

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
        rotar_imagen_recursiva(img, img_rotada, x, y, mid_width, mid_height, channels, full_width, contador);

        // Subimagen superior derecha
        rotar_imagen_recursiva(img, img_rotada, x + mid_width, y, width - mid_width, mid_height, channels, full_width, contador);

        // Subimagen inferior izquierda
        rotar_imagen_recursiva(img, img_rotada, x, y + mid_height, mid_width, height - mid_height, channels, full_width, contador);

        // Subimagen inferior derecha
        rotar_imagen_recursiva(img, img_rotada, x + mid_width, y + mid_height, width - mid_width, height - mid_height, channels, full_width, contador);
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

// Funciones ASCII -----------------------------------------------------------------------------------------------------
// Función para leer la imagen desde el archivo
void leer_imagen(FILE *archivo, char imagen[MAX_FILA][MAX_COL], int *filas, int *columnas)
{
    *filas = 0;
    *columnas = 0;
    // Leer la imagen línea por línea
    while (fgets(imagen[*filas], MAX_COL, archivo))
    {
        int len = 0;
        // Encontrar el largo de la línea
        while (imagen[*filas][len] != '\0' && imagen[*filas][len] != '\n')
        {
            len++;
        }
        *columnas = (*columnas > len) ? *columnas : len; // Mantener el número máximo de columnas
        (*filas)++;
    }

    // Verificar que se haya leído la imagen correctamente
    printf("Imagen leída correctamente: %d filas, %d columnas.\n", *filas, *columnas);
}

// Función para rotar la imagen 90 grados (modificando la matriz original)
void rotar_imagen(char imagen[MAX_FILA][MAX_COL], int *filas, int *columnas)
{
    // Crear una matriz auxiliar para la transposición
    char transpuesta[MAX_COL][MAX_FILA];

    // Realizar la transposición
    for (int i = 0; i < *filas; i++)
    {
        for (int j = 0; j < *columnas; j++)
        {
            transpuesta[j][i] = imagen[i][j];
        }
    }

    // Ahora, la matriz transpuesta está lista, y la rotamos invirtiendo las filas
    for (int i = 0; i < *columnas; i++)
    {
        for (int j = 0; j < *filas; j++)
        {
            imagen[i][j] = transpuesta[i][*filas - 1 - j];
        }
    }

    // Intercambiar las dimensiones: filas se convierten en columnas y viceversa
    int temp = *filas;
    *filas = *columnas;
    *columnas = temp;

    // Mostrar mensaje de depuración
    printf("Imagen rotada 90 grados.\n");
}

// Función para mostrar la imagen (en el archivo de salida)
void mostrar_imagen(FILE *archivo_salida, char imagen[MAX_FILA][MAX_COL], int filas, int columnas)
{
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            fputc(imagen[i][j], archivo_salida); // Escribir en el archivo
        }
        fputc('\n', archivo_salida); // Escribir salto de línea
    }

    // Verificar que la imagen se haya guardado
    printf("Imagen rotada escrita correctamente en 'img2.txt'.\n");
}

void rotar_imagenASCII_recursiva(char imagen[MAX_FILA][MAX_COL], char transpuesta[MAX_COL][MAX_FILA], int x, int y, int width, int height)
{
    // Caso base: Si la subimagen tiene solo un píxel
    if (width == 1 && height == 1)
    {
        transpuesta[y][x] = imagen[x][y];
        return;
    }

    // Dividir la imagen en cuatro subimágenes
    int mid_width = width / 2;
    int mid_height = height / 2;

    // Llamadas recursivas para cada subimagen
    if (mid_width > 0 || mid_height > 0)
    {
        // Subimagen superior izquierda
        rotar_imagenASCII_recursiva(imagen, transpuesta, x, y, mid_width, mid_height);

        // Subimagen superior derecha
        rotar_imagenASCII_recursiva(imagen, transpuesta, x + mid_width, y, width - mid_width, mid_height);

        // Subimagen inferior izquierda
        rotar_imagenASCII_recursiva(imagen, transpuesta, x, y + mid_height, mid_width, height - mid_height);

        // Subimagen inferior derecha
        rotar_imagenASCII_recursiva(imagen, transpuesta, x + mid_width, y + mid_height, width - mid_width, height - mid_height);
    }

    // Combinar las subimágenes transpuestas en la imagen original
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            transpuesta[j][i] = imagen[i][j];
        }
    }
}

void rec(char imagen[MAX_FILA][MAX_COL], int filas, int columnas)
{
    // Crear una matriz auxiliar para la transposición
    char transpuesta[MAX_COL][MAX_FILA];

    // Llamada recursiva para rotar la imagen
    rotar_imagenASCII_recursiva(imagen, transpuesta, 0, 0, filas, columnas);

    // Copiar la imagen rotada a la matriz original
    for (int x = 0; x < columnas; x++)
    {
        for (int y = 0; y < filas; y++)
        {
            imagen[x][y] = transpuesta[x][y];
        }
    }

    // Intercambiar las dimensiones
    int temp = filas;
    filas = columnas;
    columnas = temp;
}

//----------------------------------------------------------------------------------------------------

/*

-Descripción General
    Este programa carga una imagen en formato RGB y permite rotarla. Primero, leemos la imagen desde un archivo (llamado imagen.png), y después el programa te da la opción de elegir entre dos tipos de rotación: iterativa o recursiva. Una vez que eliges la opción, aplica la rotación y guarda la imagen resultante en un archivo. Además, tenemos una función para invertir los colores de la imagen, por si quieres darle un toque distinto.

-Dependencias
    Para cargar y guardar las imágenes, el programa usa las bibliotecas stb_image y stb_image_write, que necesitas incluir en el proyecto para que funcione todo sin problemas.

-Funciones
    main()

    1.Cargar la imagen:
        Aquí usamos stbi_load para leer una imagen en formato RGB desde imagen.png. Si no se puede cargar, muestra un mensaje de error y termina.

    2.Asignar memoria:
        Reservo espacio para una nueva imagen donde guardaremos la imagen rotada y modificada.

    3.Seleccionar el tipo de rotación:
        Se despliega un menú para que elijas entre rotación iterativa (opción 1) o rotación recursiva (opción 2).

    4.Rotación iterativa:
        Si eliges la opción 1, llama a rotar_imagen_90, que rota la imagen 90 grados de forma iterativa. Luego, guarda el resultado en imagen_modificada.png. Si no puede guardarse, muestra un mensaje de error y termina.

    5.Rotación recursiva:
        Si eliges la opción 2, primero reservo espacio para la imagen que va a almacenar la rotación recursiva. Luego, se llama a rotar_imagen_recursiva, que rota la imagen 90 grados de manera recursiva, y el resultado se guarda en imagen_modificada_recu.png. De nuevo, si falla al guardar, muestra un mensaje de error y termina.

    6.Liberar memoria:
        Al final, libero la memoria asignada para las imágenes.


funcion --> rotar_imagen_90(unsigned char *img, int width, int height, unsigned char *img_rotada)

    Esta función rota la imagen dividiéndola en secciones más pequeñas y aplicando la rotación en cada sección recursivamente.
    Esto seria mi metodo de "divide y vencerás".

    Parámetros:
    -unsigned char *img: Es un puntero a los datos de la imagen original.
    -unsigned char *img_rotada: Puntero a la imagen rotada, donde se van a almacenar los píxeles en su nueva posición.
    -int x, y: Coordenadas (x, y) que indican la esquina superior izquierda de la subimagen en la que estamos trabajando.
    -int width, height: Ancho y alto de la subimagen actual.
    -int channels: Número de canales de color en la imagen (por ejemplo, 3 para RGB).
    -int full_width: Ancho total de la imagen original, que usamos para calcular las posiciones absolutas de cada píxel.

Proceso de Rotación:

Caso base (Un solo píxel):

    -Si la subimagen actual tiene solo un píxel (width == 1 y height == 1), estamos listos para rotarlo.
    -Calculamos la posición de este píxel en la imagen original (idx) y su posición en la imagen rotada (rot_idx).
    -Luego copiamos los valores de color (RGB) del píxel original a la nueva posición en img_rotada.

División de la Imagen:

    -Si la subimagen es más grande que un píxel, dividimos la subimagen en cuatro cuadrantes (superior izquierda, superior derecha, inferior izquierda, e inferior derecha).
    -Calculamos los anchos y alturas medios (mid_width y mid_height) para estas divisiones.

Llamadas Recursivas:

    -Llamamos a rotar_imagen_recursiva en cada uno de los cuatro cuadrantes.
    -Cada llamada se centra en un cuadrante específico y va dividiendo y rotando hasta que cada subimagen se reduce a un solo píxel, en cuyo punto se aplica la rotación.


Si existe alguna duda pueden mandar mensaje por mi github o por mi correo (rubiodiego001@gmail.com)


*/