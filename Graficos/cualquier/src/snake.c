#include "raylib.h"
#include <stdlib.h>

#define TAMANO_CUADRADO 40

// Estructuras
typedef struct Nodo {
    Vector2 posicion;
    Vector2 velocidad;
    struct Nodo* siguiente;
} Nodo;

typedef struct {
    Nodo* cabeza;
    Nodo* cola;
    Color colorCabeza;
    Color colorCuerpo;
} Serpiente;

typedef struct {
    Vector2 posicion;
    Vector2 tamano;
    bool activa;
    Color color;
} Comida;

// variables globales
static const int anchoPantalla = 800;
static const int altoPantalla = 450;

static int contadorFrames = 0;
static bool juegoTerminado = false;

static Comida fruta = { 0 };
static Serpiente serpiente;
static Vector2 offset = { 0 };

// funciones principales
static void InicializarJuego(void);         
static void ActualizarJuego(void);          
static void DibujarJuego(void);             
static void ActualizarDibujoFrame(void);   

// funciones para el manejo de la serpiente
static Nodo* CrearNodo(Vector2 posicion, Vector2 velocidad);
static void AnadirNodo(Serpiente* serpiente);
static void LiberarSerpiente(Serpiente* serpiente);

// inicio del juego
int main(void)
{
    InitWindow(anchoPantalla, altoPantalla, "snake");
    InicializarJuego();
    SetTargetFPS(60);

    // bucle principal del juego
    while (!WindowShouldClose())    // detectar el botón de cierre de la ventana o la tecla ESC
    {
        ActualizarDibujoFrame();
    }

    // libera memoria antes de cerrar el juego
    LiberarSerpiente(&serpiente);
    CloseWindow();

    return 0;
}

// inicializa las variables del juego
void InicializarJuego(void)
{
    contadorFrames = 0;
    juegoTerminado = false;

    offset.x = anchoPantalla % TAMANO_CUADRADO;
    offset.y = altoPantalla % TAMANO_CUADRADO;

    // inicializar serpiente
    serpiente.cabeza = CrearNodo((Vector2){ offset.x / 2, offset.y / 2 }, (Vector2){ TAMANO_CUADRADO, 0 });
    serpiente.cola = serpiente.cabeza;
    serpiente.colorCabeza = DARKBLUE;
    serpiente.colorCuerpo = BLUE;

    // inicializar comida
    fruta.tamano = (Vector2){ TAMANO_CUADRADO, TAMANO_CUADRADO };
    fruta.color = SKYBLUE;
    fruta.activa = false;
}

Nodo* CrearNodo(Vector2 posicion, Vector2 velocidad)
{
    Nodo* nodo = (Nodo*)malloc(sizeof(Nodo));
    nodo->posicion = posicion;
    nodo->velocidad = velocidad;
    nodo->siguiente = NULL;
    return nodo;
}

void AnadirNodo(Serpiente* serpiente)
{
    Vector2 nuevaPosicion = serpiente->cola->posicion;
    if (serpiente->cola->velocidad.x == TAMANO_CUADRADO)
        nuevaPosicion.x -= TAMANO_CUADRADO;
    else if (serpiente->cola->velocidad.x == -TAMANO_CUADRADO)
        nuevaPosicion.x += TAMANO_CUADRADO;
    else if (serpiente->cola->velocidad.y == TAMANO_CUADRADO)
        nuevaPosicion.y -= TAMANO_CUADRADO;
    else if (serpiente->cola->velocidad.y == -TAMANO_CUADRADO)
        nuevaPosicion.y += TAMANO_CUADRADO;

    Nodo* nuevoNodo = CrearNodo(nuevaPosicion, (Vector2){ 0, 0 });
    serpiente->cola->siguiente = nuevoNodo;
    serpiente->cola = nuevoNodo;
}

void LiberarSerpiente(Serpiente* serpiente)
{
    Nodo* actual = serpiente->cabeza;
    while (actual != NULL)
    {
        Nodo* siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
    serpiente->cabeza = NULL;
    serpiente->cola = NULL;
}

// actualiza el juego (un frame)
void ActualizarJuego(void)
{
    if (!juegoTerminado)
    {
        // control del jugador
        if (IsKeyPressed(KEY_RIGHT) && (serpiente.cabeza->velocidad.x == 0))
        {
            serpiente.cabeza->velocidad = (Vector2){ TAMANO_CUADRADO, 0 };
        }
        if (IsKeyPressed(KEY_LEFT) && (serpiente.cabeza->velocidad.x == 0))
        {
            serpiente.cabeza->velocidad = (Vector2){ -TAMANO_CUADRADO, 0 };
        }
        if (IsKeyPressed(KEY_UP) && (serpiente.cabeza->velocidad.y == 0))
        {
            serpiente.cabeza->velocidad = (Vector2){ 0, -TAMANO_CUADRADO };
        }
        if (IsKeyPressed(KEY_DOWN) && (serpiente.cabeza->velocidad.y == 0))
        {
            serpiente.cabeza->velocidad = (Vector2){ 0, TAMANO_CUADRADO };
        }

        // movimiento de la serpiente
        if ((contadorFrames % 5) == 0)
        {
            Nodo* actual = serpiente.cabeza;
            Vector2 posicionAnterior = actual->posicion;
            Vector2 posicionTemp;

            actual->posicion.x += actual->velocidad.x;
            actual->posicion.y += actual->velocidad.y;

            while (actual->siguiente != NULL)
            {
                actual = actual->siguiente;
                posicionTemp = actual->posicion;
                actual->posicion = posicionAnterior;
                posicionAnterior = posicionTemp;
            }
        }

        // colisión con los límites de la matriz
        if ((serpiente.cabeza->posicion.x >= (anchoPantalla - offset.x)) ||
            (serpiente.cabeza->posicion.y >= (altoPantalla - offset.y)) ||
            (serpiente.cabeza->posicion.x < 0) || (serpiente.cabeza->posicion.y < 0))
        {
            juegoTerminado = true;
        }

        // colisión con la cola de la serpiente
            Nodo* actual = serpiente.cabeza->siguiente;
            while (actual != NULL)
            {
                if ((serpiente.cabeza->posicion.x == actual->posicion.x) && (serpiente.cabeza->posicion.y == actual->posicion.y))
                {
                    juegoTerminado = true;
                }
                actual = actual->siguiente;
            }
        
        // Colisión de la cabeza de la serpiente con la comida
        if ((serpiente.cabeza->posicion.x < (fruta.posicion.x + fruta.tamano.x) && (serpiente.cabeza->posicion.x + TAMANO_CUADRADO) > fruta.posicion.x) &&
            (serpiente.cabeza->posicion.y < (fruta.posicion.y + fruta.tamano.y) && (serpiente.cabeza->posicion.y + TAMANO_CUADRADO) > fruta.posicion.y))
        {
            AnadirNodo(&serpiente);
            fruta.activa = false;
        }

        // Posición de la fruta
        if (!fruta.activa)
        {
            fruta.activa = true;
            fruta.posicion = (Vector2){ GetRandomValue(0, (anchoPantalla / TAMANO_CUADRADO) - 1) * TAMANO_CUADRADO + offset.x / 2, GetRandomValue(0, (altoPantalla / TAMANO_CUADRADO) - 1) * TAMANO_CUADRADO + offset.y / 2 };

            Nodo* actual = serpiente.cabeza;
            while (actual != NULL)
            {
                while ((fruta.posicion.x == actual->posicion.x) && (fruta.posicion.y == actual->posicion.y))
                {
                    fruta.posicion = (Vector2){ GetRandomValue(0, (anchoPantalla / TAMANO_CUADRADO) - 1) * TAMANO_CUADRADO + offset.x / 2, GetRandomValue(0, (altoPantalla / TAMANO_CUADRADO) - 1) * TAMANO_CUADRADO + offset.y / 2 };
                    actual = serpiente.cabeza;
                }
                actual = actual->siguiente;
            }
        }

        contadorFrames++;
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            LiberarSerpiente(&serpiente); // Libera la memoria antes de reiniciar el juego
            InicializarJuego();
            juegoTerminado = false;
        }
    }
}

// Dibujar juego (un frame)
void DibujarJuego(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    if (!juegoTerminado)
    {
        // Dibuja la matriz
        for (int i = 0; i < anchoPantalla / TAMANO_CUADRADO + 1; i++)
        {
            DrawLineV((Vector2){ TAMANO_CUADRADO * i + offset.x / 2, offset.y / 2 }, (Vector2){ TAMANO_CUADRADO * i + offset.x / 2, altoPantalla - offset.y / 2 }, LIGHTGRAY);
        }

        for (int i = 0; i < altoPantalla / TAMANO_CUADRADO + 1; i++)
        {
            DrawLineV((Vector2){ offset.x / 2, TAMANO_CUADRADO * i + offset.y / 2 }, (Vector2){ anchoPantalla - offset.x / 2, TAMANO_CUADRADO * i + offset.y / 2 }, LIGHTGRAY);
        }

        // Dibuja la serpiente
        Nodo* actual = serpiente.cabeza;
        while (actual != NULL)
        {
            DrawRectangleV(actual->posicion, (Vector2){ TAMANO_CUADRADO, TAMANO_CUADRADO }, (actual == serpiente.cabeza) ? serpiente.colorCabeza : serpiente.colorCuerpo);
            actual = actual->siguiente;
        }

        // Dibuja la comida
        DrawRectangleV(fruta.posicion, fruta.tamano, fruta.color);
    }
    else DrawText("PRESIONA [ENTER] PARA JUGAR DE NUEVO", GetScreenWidth() / 2 - MeasureText("PRESIONA [ENTER] PARA JUGAR DE NUEVO", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);

    EndDrawing();
}

// Actualiza y dibujar (un frame)
void ActualizarDibujoFrame(void)
{
    ActualizarJuego();
    DibujarJuego();
}