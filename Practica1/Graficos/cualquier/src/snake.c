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

typedef struct {
    int anchoPantalla;
    int altoPantalla;
    int contadorFrames;
    bool juegoTerminado;
    Comida fruta;
    Serpiente serpiente;
    Vector2 offset;
} Juego;

// Funciones principales
static void InicializarJuego(Juego* juego);         
static void ActualizarJuego(Juego* juego);          
static void DibujarJuego(const Juego* juego);             
static void ActualizarDibujoFrame(Juego* juego);   

// Funciones para el manejo de la serpiente
static Nodo* CrearNodo(Vector2 posicion, Vector2 velocidad);
static void AnadirNodo(Serpiente* serpiente);
static void LiberarSerpiente(Serpiente* serpiente);

// Inicio del juego
int main(void)
{
    Juego juego;
    juego.anchoPantalla = 800;
    juego.altoPantalla = 450;

    InitWindow(juego.anchoPantalla, juego.altoPantalla, "snake");
    InicializarJuego(&juego);
    SetTargetFPS(60);

    // Bucle principal del juego
    while (!WindowShouldClose())    // Detectar el botón de cierre de la ventana o la tecla ESC
    {
        ActualizarDibujoFrame(&juego);
    }

    // Libera memoria antes de cerrar el juego
    LiberarSerpiente(&(juego.serpiente));
    CloseWindow();

    return 0;
}

// Inicializa las variables del juego
void InicializarJuego(Juego* juego)
{
    juego->contadorFrames = 0;
    juego->juegoTerminado = false;

    juego->offset.x = juego->anchoPantalla % TAMANO_CUADRADO;
    juego->offset.y = juego->altoPantalla % TAMANO_CUADRADO;

    // Inicializa la serpiente
    juego->serpiente.cabeza = CrearNodo((Vector2){ juego->offset.x / 2, juego->offset.y / 2 }, (Vector2){ TAMANO_CUADRADO, 0 });
    juego->serpiente.cola = juego->serpiente.cabeza;
    juego->serpiente.colorCabeza = DARKBLUE;
    juego->serpiente.colorCuerpo = BLUE;

    // Inicializa la comida
    juego->fruta.tamano = (Vector2){ TAMANO_CUADRADO, TAMANO_CUADRADO };
    juego->fruta.color = SKYBLUE;
    juego->fruta.activa = false;
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

// Actualiza el juego
void ActualizarJuego(Juego* juego)
{
    if (!(juego->juegoTerminado))
    {
        // Control del jugador
        if (IsKeyPressed(KEY_RIGHT) && (juego->serpiente.cabeza->velocidad.x == 0))
        {
            juego->serpiente.cabeza->velocidad = (Vector2){ TAMANO_CUADRADO, 0 };
        }
        if (IsKeyPressed(KEY_LEFT) && (juego->serpiente.cabeza->velocidad.x == 0))
        {
            juego->serpiente.cabeza->velocidad = (Vector2){ -TAMANO_CUADRADO, 0 };
        }
        if (IsKeyPressed(KEY_UP) && (juego->serpiente.cabeza->velocidad.y == 0))
        {
            juego->serpiente.cabeza->velocidad = (Vector2){ 0, -TAMANO_CUADRADO };
        }
        if (IsKeyPressed(KEY_DOWN) && (juego->serpiente.cabeza->velocidad.y == 0))
        {
            juego->serpiente.cabeza->velocidad = (Vector2){ 0, TAMANO_CUADRADO };
        }

        // Movimiento de la serpiente
        if ((juego->contadorFrames % 5) == 0)
        {
            Nodo* actual = juego->serpiente.cabeza;
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

        // Colisión con los límites de la matriz
        if ((juego->serpiente.cabeza->posicion.x >= (juego->anchoPantalla - juego->offset.x)) ||
            (juego->serpiente.cabeza->posicion.y >= (juego->altoPantalla - juego->offset.y)) ||
            (juego->serpiente.cabeza->posicion.x < 0) || (juego->serpiente.cabeza->posicion.y < 0))
        {
            juego->juegoTerminado = true;
        }

        // Colisión con la cola de la serpiente
        Nodo* actual = juego->serpiente.cabeza->siguiente;
        while (actual != NULL)
        {
            if ((juego->serpiente.cabeza->posicion.x == actual->posicion.x) && (juego->serpiente.cabeza->posicion.y == actual->posicion.y))
            {
                juego->juegoTerminado = true;
            }
            actual = actual->siguiente;
        }
        
        // Colisión de la cabeza de la serpiente con la comida
        if ((juego->serpiente.cabeza->posicion.x < (juego->fruta.posicion.x + juego->fruta.tamano.x) && (juego->serpiente.cabeza->posicion.x + TAMANO_CUADRADO) > juego->fruta.posicion.x) &&
            (juego->serpiente.cabeza->posicion.y < (juego->fruta.posicion.y + juego->fruta.tamano.y) && (juego->serpiente.cabeza->posicion.y + TAMANO_CUADRADO) > juego->fruta.posicion.y))
        {
            AnadirNodo(&(juego->serpiente));
            juego->fruta.activa = false;
        }

        // Posición de la fruta
        if (!(juego->fruta.activa))
        {
            juego->fruta.activa = true;
            juego->fruta.posicion = (Vector2){ GetRandomValue(0, (juego->anchoPantalla / TAMANO_CUADRADO) - 1) * TAMANO_CUADRADO + juego->offset.x / 2, GetRandomValue(0, (juego->altoPantalla / TAMANO_CUADRADO) - 1) * TAMANO_CUADRADO + juego->offset.y / 2 };

            Nodo* actual = juego->serpiente.cabeza;
            while (actual != NULL)
            {
                while ((juego->fruta.posicion.x == actual->posicion.x) && (juego->fruta.posicion.y == actual->posicion.y))
                {
                    juego->fruta.posicion = (Vector2){ GetRandomValue(0, (juego->anchoPantalla / TAMANO_CUADRADO) - 1) * TAMANO_CUADRADO + juego->offset.x / 2, GetRandomValue(0, (juego->altoPantalla / TAMANO_CUADRADO) - 1) * TAMANO_CUADRADO + juego->offset.y / 2 };
                    actual = juego->serpiente.cabeza;
                }
                actual = actual->siguiente;
            }
        }

        juego->contadorFrames++;
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            LiberarSerpiente(&(juego->serpiente)); // Libera la memoria de la serpiente antes de reiniciar el juego
            InicializarJuego(juego);
            juego->juegoTerminado = false;
        }
    }
}


// Dibuja el juego
void DibujarJuego(const Juego* juego)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    if (!(juego->juegoTerminado))
    {
        // Dibuja la matriz
        for (int i = 0; i < juego->anchoPantalla / TAMANO_CUADRADO + 1; i++)
        {
            DrawLineV((Vector2){ TAMANO_CUADRADO * i + juego->offset.x / 2, juego->offset.y / 2 }, (Vector2){ TAMANO_CUADRADO * i + juego->offset.x / 2, juego->altoPantalla - juego->offset.y / 2 }, LIGHTGRAY);
        }

        for (int i = 0; i < juego->altoPantalla / TAMANO_CUADRADO + 1; i++)
        {
            DrawLineV((Vector2){ juego->offset.x / 2, TAMANO_CUADRADO * i + juego->offset.y / 2 }, (Vector2){ juego->anchoPantalla - juego->offset.x / 2, TAMANO_CUADRADO * i + juego->offset.y / 2 }, LIGHTGRAY);
        }

        // Dibuja la serpiente
        Nodo* actual = juego->serpiente.cabeza;
        while (actual != NULL)
        {
            DrawRectangleV(actual->posicion, (Vector2){ TAMANO_CUADRADO, TAMANO_CUADRADO }, (actual == juego->serpiente.cabeza) ? juego->serpiente.colorCabeza : juego->serpiente.colorCuerpo);
            actual = actual->siguiente;
        }

        // Dibuja la comida
        DrawRectangleV(juego->fruta.posicion, juego->fruta.tamano, juego->fruta.color);
    }
    else 
    {
        DrawText("PRESIONA [ENTER] PARA JUGAR DE NUEVO", GetScreenWidth() / 2 - MeasureText("PRESIONA [ENTER] PARA JUGAR DE NUEVO", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);
    }

    EndDrawing();
}


// Actualiza y dibujar (un frame)
void ActualizarDibujoFrame(Juego* juego)
{
    ActualizarJuego(juego);
    DibujarJuego(juego);
}
