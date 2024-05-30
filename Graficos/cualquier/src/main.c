#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


#define MAX 200
#define MAX_PREGUNTAS 50
#define MAX_CHAR 150

typedef struct _preguntas
{
    int indice;
    char pregunta[MAX_CHAR];
    char respuesta1[MAX_CHAR];
    char respuesta2[MAX_CHAR];
    char respuesta3[MAX_CHAR];
    int correcta;
} Tpregunta;

int textoMenu(Font customFont, int menuOption, int screenWidth);
int textoMenuTema(Font customFont, int opTema, int screenWidth);
int textoNumPreguntas(Font customFont, int opNum, int screenWidth);
typedef enum
{
    menuSect,
    gameSect,
    optionsSect,
    recordsSect
} GameSection;

typedef enum
{
    inactivo,
    menuTema,
    menuModo,
    menuOpciones,
} MenuConfig;

int CargarPreguntas(Tpregunta preguntas[], const char nombreArchivo[])
{
    FILE *fa;
    int i = 0;
    fa = fopen(nombreArchivo, "r");
    if(fa == NULL)
    {
        printf("error al cargar preguntas\n");
        return 0;
    }

    while(i < MAX && fgets(preguntas[i].pregunta, sizeof(preguntas[i].pregunta), fa) != NULL)
    {
        preguntas[i].indice = i;
        // elimina el caracter de nueva linea al final de la pregunta
        preguntas[i].pregunta[strcspn(preguntas[i].pregunta, "\n")] = "\0";

        fgets(preguntas[i].respuesta1, sizeof(preguntas[i].respuesta1), fa);
        fgets(preguntas[i].respuesta2, sizeof(preguntas[i].respuesta2), fa);
        fgets(preguntas[i].respuesta3, sizeof(preguntas[i].respuesta3), fa);

        //elimina el caracter de nueva linea al final de las respuestas
        preguntas[i].respuesta1[strcspn(preguntas[i].respuesta1, "\n")] = "\0";
        preguntas[i].respuesta2[strcspn(preguntas[i].respuesta2, "\n")] = "\0";
        preguntas[i].respuesta3[strcspn(preguntas[i].respuesta3, "\n")] = "\0";

        fscanf(fa, "%d", &preguntas[i].correcta);
        fgetc(fa);  // consumir el caracter de linea despuest del numero
        i++;
    }

    fclose(fa);
    return i;
}

void imprimirPregunta(Tpregunta pregunta[], int i, int contPreguntas)
{
    DrawText(TextFormat("pregunta %d: %s", contPreguntas +1, pregunta[i].pregunta), 10, 10, 20, BLACK);
    DrawText(TextFormat("1) %s", pregunta[i].respuesta1), 10, 40, 20, BLACK);
    DrawText(TextFormat("2) %s", pregunta[i].respuesta2), 10, 70, 20, BLACK);
    DrawText(TextFormat("3) %s", pregunta[i].respuesta3), 10, 100, 20, BLACK);
}

int msge()
{
    int op = -1;
    while(op<1 || op >2)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("bienvenido al juego de preguntas", 10, 10, 20, BLACK);
        DrawText("1. Jugar", 10, 40, 20, BLACK);
        DrawText("2. salir", 10, 70, 20, BLACK);
        DrawText("ingrese una opcion:", 10, 100, 20, BLACK);

        if(IsKeyPressed(KEY_ONE))
        {
            op = 1;
        }
        else if(IsKeyPressed(KEY_TWO))
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

    while(op<1 || op>4)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("selecione el tema", 10, 10, 20, BLACK);
        DrawText("1. espanol", 10, 40, 20, BLACK);
        DrawText("2. matematicas", 10, 70, 20, BLACK);
        DrawText("ingrese una opcion", 10, 160, 20, BLACK);

        if(IsKeyPressed(KEY_ONE))
        {
            op = 1;
        }
        else if(IsKeyPressed(KEY_TWO))
        {
            op = 2;
        }
        else if(IsKeyPressed(KEY_THREE))
        {
            op = 3;
        }
        else if(IsKeyPressed(KEY_FOUR))
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

    while(op<1 || op>3)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("seleccione el modo de juego", 10, 10, 20, BLACK);
        DrawText("1. 5 Preguntas", 10, 40, 20, BLACK);
        DrawText("2. 10 preguntas", 10, 70, 20, BLACK);
        DrawText("3. 20 preguntas", 10, 100, 20, BLACK);
        DrawText("ingrese una opcion", 10, 130, 20, BLACK);

        if(IsKeyPressed(KEY_ONE))
        {
            op = 1;
        }
        else if(IsKeyPressed(KEY_TWO))
        {
            op = 2;
        }
        else if(IsKeyPressed(KEY_THREE))
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
    int i;
    for(i= 0; i< contador; i++)
    {
        num = indices[i];
        if(preguntas[num].indice == nuevoIndice)
        {
            return 1;
        }
    }
    return 0;
}

int generarPreguntaAleatoria(Tpregunta preguntas[], int contPreguntas, int indices[], int preguntasCargadas)
{
    int i, encontrado;
    do
    {
        i = rand()%preguntasCargadas;
        encontrado = preguntaYaSeleccionada(preguntas, contPreguntas, i, indices);

    } while (encontrado == 1);

    indices[contPreguntas] = i;

    BeginDrawing();
    ClearBackground(RAYWHITE);

    imprimirPregunta(preguntas, i, contPreguntas);

    DrawText("seleccionar Respuesta:", 10, 130, 20, BLACK);
    DrawText("seleccionar Respuesta:", 10, 130, 20, BLACK);
    DrawText("1         2         3", 10, 160, 20, BLACK);

    EndDrawing();

    int respuesta = -1;

    while(respuesta == -1)
    {
        if(IsKeyDown(KEY_ONE))
        {
            respuesta = 1;
        }
        else if(IsKeyDown(KEY_TWO))
        {
            respuesta = 2;
        }
        else if(IsKeyDown(KEY_THREE))
        {
            respuesta = 3;
        }

        sleep(200);
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    if(respuesta == preguntas[i].correcta)
    {
        DrawText("respuesta correcta", 10, 190, 20, GREEN);
        return 1;
    }
    else
    {
        DrawText("respuesta incorrecta", 10, 190, 20, RED);
        return 0;
    }
    EndDrawing();
    
}

void generarRecordsPuntaje(int puntuacion, char nombre[], char nombreArchivo[])
{
    FILE *fa;
    fa = fopen(nombreArchivo, "a");
    if(fa == NULL)
    {
        printf("error al abrir el archivo\n");
        return;
    }

    if(fa)
    {
        fprintf(fa, "%s %d\n", nombre, puntuacion);
        fclose(fa);
        printf("records actualizados exitosamente\n");
        system("PAUSE");
    }
}

void dibujarPregunta(Tpregunta pregunta)
{
    DrawText(pregunta.pregunta, 10, 10, 20, BLACK);
    DrawText(pregunta.respuesta1, 10, 40, 20, BLACK);
    DrawText(pregunta.respuesta2, 10, 70, 20, BLACK);
    DrawText(pregunta.respuesta3, 10, 100, 20, BLACK);
}

void juego()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "racequickz");

    int puntuacion;
    int op, opTema, opModo;
    int indices[MAX_PREGUNTAS];
    Tpregunta preguntas[MAX_PREGUNTAS];
    int contPreguntas = 0;
    int numPreguntas = 0;
    int preguntasCargadas = 0;

    do
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        op = msge();

        switch(op)
        {
        case 1:
            opTema = msgeTema();
            switch(opTema)
            {
            case 1:
                preguntasCargadas = CargarPreguntas(preguntas, "espanol.txt");
                break;

            case 2:
                preguntasCargadas = CargarPreguntas(preguntas, "matematicas.txt");
                break;

            default: 
                break;
            }

            opModo = msgeModo();
            switch(op)
            {
            case 1:
                numPreguntas = 5;
                break;

            case 2:
                numPreguntas = 10;
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

            } while (contPreguntas < numPreguntas);
            break;

        case 2:
            // salir
            CloseWindow();
            break;
        default:
            break;
        }

        EndDrawing();

    } while (!WindowShouldClose());
}


int main(void)
{
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "RaceQuickZ");

    Image icon = LoadImage("icono.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
    SetTargetFPS(60);

    GameSection currentSection = menuSect;
    MenuConfig menuConfigs = inactivo;

    int menuOption = 0;
    int opTema = 0;
    int opModo = 0;
    char nombreArchivo[MAX_CHAR];
    int numPreguntas = 0;
    int preguntasCargadas = 0;
    int indices[MAX_PREGUNTAS];
    Tpregunta preguntas[MAX_PREGUNTAS];

    Image backgroundImage = LoadImage("bgmain.png");
    Texture2D backgroundTexture = LoadTextureFromImage(backgroundImage);

    Font customFont = LoadFont("RaceSport.ttf");

    UnloadImage(backgroundImage);

    while (!WindowShouldClose())
    {
        MenuConfig tempMenuConfig = menuConfigs;

        if (IsKeyPressed(KEY_UP) && menuOption > 0)
        {
            menuOption--;
        }
        else if (IsKeyPressed(KEY_DOWN) && menuOption < 3)
        {
            menuOption++;
        }
        else if (IsKeyPressed(KEY_ENTER))
        {
            // Acciones del menú
            switch (menuOption)
            {
            case 0:
                // Lógica para jugar
                currentSection = gameSect;
                menuConfigs = menuTema;
                break;
            case 1:
                // Lógica para mostrar opciones
                currentSection = optionsSect;
                break;
            case 2:
                // Lógica para mostrar records
                currentSection = recordsSect;
                break;
            case 3:
                // Lógica para salir
                CloseWindow();
                break;
            }
        }

        if (tempMenuConfig == menuTema && IsKeyPressed(KEY_UP) && opTema > 0)
        {
            opTema--;
        }
        else if (tempMenuConfig == menuTema && IsKeyPressed(KEY_DOWN) && opTema < 4)
        {
            opTema++;
        }
        else if (tempMenuConfig == menuTema && IsKeyPressed(KEY_ENTER))
        {
            switch (opTema)
            {
            case 0:
                preguntasCargadas = CargarPreguntas(preguntas, "espanol.txt");
                menuConfigs = menuModo;
                break;
            case 1:
                preguntasCargadas = CargarPreguntas(preguntas, "matematicas.txt");
                menuConfigs = menuModo;
                break;
            case 2:
                preguntasCargadas = CargarPreguntas(preguntas, "ciencias.txt");
                menuConfigs = menuModo;
                break;
            case 3:
                preguntasCargadas = CargarPreguntas(preguntas, "historiaYGeografia.txt");
                menuConfigs = menuModo;
                break;
            case 4:
                currentSection = menuSect;
                menuConfigs = inactivo;
                break;
            default:
                break;
            }
        }

        if (tempMenuConfig == menuModo && IsKeyPressed(KEY_UP) && opModo > 0)
        {
            opModo--;
        }
        else if (tempMenuConfig == menuModo && IsKeyPressed(KEY_DOWN) && opModo < 3)
        {
            opModo++;
        }
        else if (tempMenuConfig == menuModo && IsKeyPressed(KEY_ENTER))
        {
            switch (opModo)
            {
            case 0:
                numPreguntas = 5;
                strcpy(nombreArchivo, "records5.txt");
                break;
            case 1:
                numPreguntas = 10;
                strcpy(nombreArchivo, "records10.txt");
                break;
            case 2:
                numPreguntas = 15;
                strcpy(nombreArchivo, "records15.txt");
                break;
            }
        }

        // Dibujar
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Dibujar el fondo
        DrawTexture(backgroundTexture, 0, 0, WHITE);

        // Dibujar el menú
        switch(currentSection)
        {
        case menuSect:
            // Dibujar elementos del menú
            textoMenu(customFont, menuOption, screenWidth);
            break;
        case gameSect:
            switch (menuConfigs)
            {
            case menuTema:
                // Dibujar elementos del menú de temas
                textoMenuTema(customFont, opTema, screenWidth);
                break;
            case menuModo:
                // Dibujar elementos del menú de modos
                textoNumPreguntas(customFont, opModo, screenWidth);
                break;
            default:
                break;
            }
            break;
        }

        EndDrawing();
    }

    UnloadTexture(backgroundTexture);
    UnloadFont(customFont);

    CloseWindow();

    return 0;
}

int textoMenu(Font customFont, int menuOption, int screenWidth)
{
    DrawTextEx(customFont, menuOption == 0 ? "[1] JUGAR" : "1. JUGAR", (Vector2){screenWidth / 2 - MeasureTextEx(customFont, "[1] Jugar", 40, 0).x / 2, 450}, 45, 0, WHITE);
    DrawTextEx(customFont, menuOption == 1 ? "[2] OPCIONES" : "2. OPCIONES", (Vector2){screenWidth / 2 - MeasureTextEx(customFont, "[2] Opciones", 40, 0).x / 2, 480}, 45, 0, WHITE);
    DrawTextEx(customFont, menuOption == 2 ? "[3] RECORDS" : "3. RECORDS", (Vector2){screenWidth / 2 - MeasureTextEx(customFont, "[3] Records", 40, 0).x / 2, 510}, 45, 0, WHITE);
    DrawTextEx(customFont, menuOption == 3 ? "[4] SALIR" : "4. SALIR", (Vector2){screenWidth / 2 - MeasureTextEx(customFont, "[4] SALIR", 40, 0).x / 2, 540}, 45, 0, WHITE);
    return menuOption;
}

int textoMenuTema(Font customFont, int opTema, int screenWidth)
{
    DrawTextEx(customFont, opTema == 0 ? "[1] ESPANOL" : "1. ESPANOL", (Vector2){screenWidth / 2 - MeasureTextEx(customFont, "[1] ESPANOL", 40, 0).x / 2, 450}, 45, 0, WHITE);
    DrawTextEx(customFont, opTema == 1 ? "[2] MATEMATICAS" : "2. MATEMATICAS", (Vector2){screenWidth / 2 - MeasureTextEx(customFont, "[2] MATEMATICAS", 40, 0).x / 2, 480}, 45, 0, WHITE);
    DrawTextEx(customFont, opTema == 2 ? "[3] CIENCIAS" : "3. CIENCIAS", (Vector2){screenWidth / 2 - MeasureTextEx(customFont, "[3] CIENCIAS", 40, 0).x / 2, 510}, 45, 0, WHITE);
    DrawTextEx(customFont, opTema == 3 ? "[4] HISTORIA Y GEOGRAFIA" : "4. HISTORIA Y GEOGRAFIA", (Vector2){screenWidth / 2 - MeasureTextEx(customFont, "[4] HISTORIA Y GEOGRAFIA", 40, 0).x / 2, 540}, 45, 0, WHITE);
    DrawTextEx(customFont, opTema == 4 ? "[5] REGRESAR" : "5. REGRESAR", (Vector2){screenWidth / 2 - MeasureTextEx(customFont, "[5] REGRESAR", 40, 0).x / 2, 570}, 45, 0, WHITE);
    return opTema;
}

int textoNumPreguntas(Font customFont, int opNum, int screenWidth)
{

    DrawTextEx(customFont, opNum == 0 ? "[1] 5 PREGUNTAS" : "1. 5 PREGUNTAS", (Vector2){screenWidth / 2 - MeasureTextEx(customFont, "[1] 5 PREGUNTAS", 40, 0).x / 2, 450}, 45, 0, WHITE);
    DrawTextEx(customFont, opNum == 1 ? "[2] 10 PREGUNTAS" : "2. 10 PREGUNTAS", (Vector2){screenWidth / 2 - MeasureTextEx(customFont, "[2] 10 PREGUNTAS", 40, 0).x / 2, 480}, 45, 0, WHITE);
    DrawTextEx(customFont, opNum == 2 ? "[3] 15 PREGUNTAS" : "3. 15 PREGUNTAS", (Vector2){screenWidth / 2 - MeasureTextEx(customFont, "[3] 15 PREGUNTAS", 40, 0).x / 2, 510}, 45, 0, WHITE);
    DrawTextEx(customFont, opNum == 3 ? "[4] REGRESAR" : "4. REGRESAR", (Vector2){screenWidth / 2 - MeasureTextEx(customFont, "[4] REGRESAR", 40, 0).x / 2, 540}, 45, 0, WHITE);
    return opNum;
}

// int main()
// {
//     juego();
//     return 0;
// }