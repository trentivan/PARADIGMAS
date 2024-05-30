#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// #include "testjuego.h"

#define MAX 200
#define MAX_PREGUNTAS 50
#define MAX_CHAR 150

typedef struct _Preguntas
{
    int indice;
    char pregunta[MAX_CHAR];
    char respuesta1[MAX_CHAR];
    char respuesta2[MAX_CHAR];
    char respuesta3[MAX_CHAR];
    int correcta;
} Tpregunta;

// Resto de tus funciones y definiciones aquí...
int cargarPreguntas(Tpregunta preguntas[], const char nombreArchivo[])
{
    FILE *fa;
    int i = 0;
    fa = fopen(nombreArchivo, "r");
    if (fa == NULL)
    {
        printf("ERROR AL CARGAR PREGUNTAS\n");
        return 0;
    }

    while (i < MAX && fgets(preguntas[i].pregunta, sizeof(preguntas[i].pregunta), fa) != NULL)
    {
        preguntas[i].indice = i;
        // Elimina el carácter de nueva línea al final de la pregunta
        preguntas[i].pregunta[strcspn(preguntas[i].pregunta, "\n")] = '\0';

        fgets(preguntas[i].respuesta1, sizeof(preguntas[i].respuesta1), fa);
        fgets(preguntas[i].respuesta2, sizeof(preguntas[i].respuesta2), fa);
        fgets(preguntas[i].respuesta3, sizeof(preguntas[i].respuesta3), fa);

        // Elimina el carácter de nueva línea al final de las respuestas
        preguntas[i].respuesta1[strcspn(preguntas[i].respuesta1, "\n")] = '\0';
        preguntas[i].respuesta2[strcspn(preguntas[i].respuesta2, "\n")] = '\0';
        preguntas[i].respuesta3[strcspn(preguntas[i].respuesta3, "\n")] = '\0';

        fscanf(fa, "%d", &preguntas[i].correcta);
        fgetc(fa); // Consumir el carácter de nueva línea después del número
        i++;
    }

    fclose(fa);
    return i;
}

void imprimirPregunta(Tpregunta pregunta[], int i, int contPreguntas)
{
    DrawText(TextFormat("Pregunta %d: %s", contPreguntas + 1, pregunta[i].pregunta), 10, 10, 20, BLACK);
    DrawText(TextFormat("1) %s", pregunta[i].respuesta1), 10, 40, 20, BLACK);
    DrawText(TextFormat("2) %s", pregunta[i].respuesta2), 10, 70, 20, BLACK);
    DrawText(TextFormat("3) %s", pregunta[i].respuesta3), 10, 100, 20, BLACK);
}

int msge()
{
    int op = -1;

    while (op < 1 || op > 2)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Bienvenido al juego de preguntas", 10, 10, 20, BLACK);
        DrawText("1. Jugar", 10, 40, 20, BLACK);
        DrawText("2. Salir", 10, 70, 20, BLACK);
        DrawText("Ingrese una opción:", 10, 100, 20, BLACK);

        // Agrega código para manejar la entrada del usuario en la interfaz gráfica
        // Puedes utilizar funciones de Raylib para detectar clics en botones, por ejemplo.

        // Aquí un ejemplo básico, deberás ajustarlo según tus necesidades
        if (IsKeyPressed(KEY_ONE))
        {
            op = 1;
        }
        else if (IsKeyPressed(KEY_TWO))
        {
            op = 2;
        }

        EndDrawing();
    }

    return op;
}

int msgeTema()
{
    int op = -1;

    while (op < 1 || op > 4)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Seleccione el tema", 10, 10, 20, BLACK);
        DrawText("1. Español", 10, 40, 20, BLACK);
        DrawText("2. Matemáticas", 10, 70, 20, BLACK);
        DrawText("3. Ciencias", 10, 100, 20, BLACK);
        DrawText("4. Historia Y Geografía", 10, 130, 20, BLACK);
        DrawText("Ingrese una opción:", 10, 160, 20, BLACK);

        // Agrega código para manejar la entrada del usuario en la interfaz gráfica
        // Puedes utilizar funciones de Raylib para detectar clics en botones, por ejemplo.

        // Aquí un ejemplo básico, deberás ajustarlo según tus necesidades
        if (IsKeyPressed(KEY_ONE))
        {
            op = 1;
        }
        else if (IsKeyPressed(KEY_TWO))
        {
            op = 2;
        }
        else if (IsKeyPressed(KEY_THREE))
        {
            op = 3;
        }
        else if (IsKeyPressed(KEY_FOUR))
        {
            op = 4;
        }

        EndDrawing();
    }

    return op;
}

int msgeModo()
{
    int op = -1;

    while (op < 1 || op > 3)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Seleccione el modo de juego", 10, 10, 20, BLACK);
        DrawText("1. 5 Preguntas", 10, 40, 20, BLACK);
        DrawText("2. 10 Preguntas", 10, 70, 20, BLACK);
        DrawText("3. 20 Preguntas", 10, 100, 20, BLACK);
        DrawText("Ingrese una opción:", 10, 130, 20, BLACK);

        // Agrega código para manejar la entrada del usuario en la interfaz gráfica
        // Puedes utilizar funciones de Raylib para detectar clics en botones, por ejemplo.

        // Aquí un ejemplo básico, deberás ajustarlo según tus necesidades
        if (IsKeyPressed(KEY_ONE))
        {
            op = 1;
        }
        else if (IsKeyPressed(KEY_TWO))
        {
            op = 2;
        }
        else if (IsKeyPressed(KEY_THREE))
        {
            op = 3;
        }

        EndDrawing();
    }

    return op;
}

int preguntaYaSeleccionada(Tpregunta preguntas[], int contador, int nuevoIndice, int indices[])
{
    int num;
    for (int i = 0; i < contador; i++)
    {
        num = indices[i];
        if (preguntas[num].indice == nuevoIndice)
        {
            return 1; // ya encontrado
        }
    }
    return 0; // no encontrado
}

int generarPreguntaAleatoria(Tpregunta preguntas[], int contPreguntas, int indices[], int preguntasCargadas)
{
    int i, encontrado;
    do
    {
        i = rand() % preguntasCargadas;
        encontrado = preguntaYaSeleccionada(preguntas, contPreguntas, i, indices);

    } while (encontrado == 1);

    indices[contPreguntas] = i;

    // Mostrar pregunta
    BeginDrawing();
    ClearBackground(RAYWHITE);

    imprimirPregunta(preguntas, i, contPreguntas);

    DrawText("ivan", 10, 190, 20, BLACK);
    DrawText("Seleccionar Respuesta:", 10, 130, 20, BLACK);
    DrawText("1       2       3", 10, 160, 20, BLACK);

    EndDrawing();
    // printf("hola\n");
    int respuesta = -1;

    while (respuesta == -1)
    {
        // Actualizar el estado de respuesta según las teclas presionadas
        if (IsKeyDown(KEY_ONE))
        {
            respuesta = 1;
            // DrawText("Respuesta 1", 10, 190, 20, BLACK);
        }
        else if (IsKeyDown(KEY_TWO))
        {
            respuesta = 2;
        }
        else if (IsKeyDown(KEY_THREE))
        {
            respuesta = 3;
        }
        // printf("hola\n");

        // Esperar unos milisegundos para permitir que Raylib maneje otros eventos
        sleep(2000);
    }

    // Mostrar resultado fuera del bucle de espera
    BeginDrawing();
    ClearBackground(RAYWHITE);
    // printf("hola\n");

    if (respuesta == preguntas[i].correcta)
    {
        DrawText("Respuesta correcta", 10, 190, 20, BLACK);
        return 1; 
    }
    else
    {
        DrawText("Respuesta incorrecta", 10, 190, 20, BLACK);
        return 0;
    }
    // printf("hola\n");

    EndDrawing();

}

void generarRecordsPuntaje(int puntuacion, char nombre[], char nombreArchivo[])
{
    FILE *fa;
    fa = fopen(nombreArchivo, "a");
    if (fa == NULL)
    {
        printf("ERROR AL ABRIR EL ARCHIVO\n");
        return;
    }

    if (fa)
    {
        fprintf(fa, "%s %d\n", nombre, puntuacion);
        fclose(fa);
        printf("RECORDS ACTUALIZADOS EXITOSAMENTE!\n");
        system("PAUSE");
    }
}

//**********
void dibujarPregunta(Tpregunta pregunta)
{
    DrawText(pregunta.pregunta, 10, 10, 20, BLACK);
    DrawText(pregunta.respuesta1, 10, 40, 20, BLACK);
    DrawText(pregunta.respuesta2, 10, 70, 20, BLACK);
    DrawText(pregunta.respuesta3, 10, 100, 20, BLACK);
}

void juego()
{
    // Inicializar Raylib
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "RaceQuickz");

    int puntuacion;
    int op, opTema, opModo;
    int indices[MAX_PREGUNTAS];
    Tpregunta preguntas[MAX_PREGUNTAS];
    int contPreguntas = 0;
    int numPreguntas = 0;
    int preguntasCargadas = 0;

    // Resto de tu código aquí...

    do
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        op = msge();

        switch (op)
        {
        case 1:
            opTema = msgeTema();
            switch (opTema)
            {
            case 1:
                preguntasCargadas = cargarPreguntas(preguntas, "espanol.txt");
                break;
            case 2:
                preguntasCargadas = cargarPreguntas(preguntas, "matematicas.txt");
                break;
            case 3:
                preguntasCargadas = cargarPreguntas(preguntas, "ciencias.txt");
                break;
            case 4:
                preguntasCargadas = cargarPreguntas(preguntas, "historiaYGeografia.txt");
                break;
            default:
                break;
            }

            opModo = msgeModo();
            switch (opModo)
            {
            case 1:
                numPreguntas = 5;
                break;
            case 2:
                numPreguntas = 10;
                break;
            case 3:
                numPreguntas = 20;
                break;
            default:
                break;
            }

            // inicio del juego
            puntuacion = 0;
            contPreguntas = 0;

            do
            {
                puntuacion += generarPreguntaAleatoria(preguntas, contPreguntas, indices, preguntasCargadas);
                contPreguntas++;
                printf("hola\n");

            } while (contPreguntas < numPreguntas);

            // Aquí puedes agregar el código para mostrar la puntuación final en la interfaz gráfica

            break;

        case 2:
            // Salir
            CloseWindow();
            break;
        default:
            break;
        }

        EndDrawing();

    } while (!WindowShouldClose());
}

int main()
{
    juego(); // Llama a tu función principal para iniciar el juego

    return 0;
}