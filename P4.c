#include <stdio.h>
#include <stdlib.h>

#define MAX_FILA 1000
#define MAX_COL 1000

// Prototipos de funciones
void leer_imagen(FILE *archivo, char imagen[MAX_FILA][MAX_COL], int *filas, int *columnas);
void rotar_imagen(char imagen[MAX_FILA][MAX_COL], int *filas, int *columnas);
void rec(char imagen[MAX_FILA][MAX_COL], int filas, int columnas);
void rotar_imagenASCII_recursiva(char imagen[MAX_FILA][MAX_COL], char transpuesta[MAX_COL][MAX_FILA], int x, int y, int width, int height);
void mostrar_imagen(FILE *archivo_salida, char imagen[MAX_FILA][MAX_COL], int filas, int columnas);

int main()
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
    return 0;
}

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