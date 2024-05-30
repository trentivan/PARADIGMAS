#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>

#define ANCHO_PANTALLA 1280
#define ALTO_PANTALLA 720
#define TAMANO_CUADRADO 20
#define LONGITUD_MAX_SERPIENTE 100
#define MAX_COMIDA 4

bool gameOver = false;

typedef struct Nodo {
	Vector2 posicion;
	struct Nodo* siguiente;
}Nodo;

typedef struct {
	Nodo* cabeza;
	Vector2 velocidad;
	int longitud;
}Serpiente;

typedef struct Comida {
    Vector2 posicion;
    bool activa;
} Comida;

void IniciarJuego(Serpiente* serpiente, Comida* comida);
void ActualizarJuego(Serpiente* serpiente, Comida* comida);
void DibujarJuego(Serpiente* serpiente, Comida* comida);
void DibujarFondo();
void ReiniciarJuego(Serpiente* serpiente, Comida* comida);

int main(void)
{
    InitWindow(ANCHO_PANTALLA, ALTO_PANTALLA, "Serpiente paradigmas");

    Serpiente serpiente;
    Comida comida[MAX_COMIDA];

    IniciarJuego(&serpiente, comida);

    SetTargetFPS(10); // Velocidad del juego

    while (!WindowShouldClose())
    {
        if (gameOver && IsKeyPressed(KEY_SPACE)) {
            ReiniciarJuego(&serpiente, comida);
        }

        if (!gameOver) {
            ActualizarJuego(&serpiente, comida);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DibujarJuego(&serpiente, comida);
        EndDrawing();
    }

    while (serpiente.cabeza != NULL) {
        Nodo* eliminar = serpiente.cabeza;
        serpiente.cabeza = serpiente.cabeza->siguiente;
        free(eliminar);
    }

    CloseWindow();

    return 0;
}

void IniciarJuego(Serpiente* serpiente, Comida* comida)
{
    if (gameOver) return;

    serpiente->cabeza = (Nodo*)malloc(sizeof(Nodo));
    serpiente->cabeza->posicion = Vector2{ ANCHO_PANTALLA / 2.0f, ALTO_PANTALLA / 2.0f };
    serpiente->cabeza->siguiente = NULL;
    serpiente->velocidad = Vector2{ TAMANO_CUADRADO, 0 };
    serpiente->longitud = 1;

    for (int i = 0; i < MAX_COMIDA; i++) {
        comida[i].posicion = Vector2{static_cast<float>(GetRandomValue(0, (ANCHO_PANTALLA / TAMANO_CUADRADO) - 1) * TAMANO_CUADRADO),
                           static_cast<float>(GetRandomValue(0, (ALTO_PANTALLA / TAMANO_CUADRADO) - 1) * TAMANO_CUADRADO) };
        comida[i].activa = true;
    }
}

void ActualizarJuego(Serpiente* serpiente, Comida* comida)
{
    if (IsKeyPressed(KEY_RIGHT) && serpiente->velocidad.x == 0) { serpiente->velocidad = Vector2{ TAMANO_CUADRADO, 0 }; }
    if (IsKeyPressed(KEY_LEFT) && serpiente->velocidad.x == 0) { serpiente->velocidad = Vector2{ -TAMANO_CUADRADO, 0 }; }
    if (IsKeyPressed(KEY_UP) && serpiente->velocidad.y == 0) { serpiente->velocidad = Vector2{ 0, -TAMANO_CUADRADO }; }
    if (IsKeyPressed(KEY_DOWN) && serpiente->velocidad.y == 0) { serpiente->velocidad = Vector2{ 0, TAMANO_CUADRADO }; }

	Nodo* nuevoNodo = (Nodo*)malloc(sizeof(Nodo));
	nuevoNodo->posicion = Vector2{ serpiente->cabeza->posicion.x + serpiente->velocidad.x, 
                                  serpiente->cabeza->posicion.y + serpiente->velocidad.y };
	nuevoNodo->siguiente = serpiente->cabeza;
	serpiente->cabeza = nuevoNodo;
	serpiente->longitud++;

    if (serpiente->cabeza->posicion.x >= ANCHO_PANTALLA) serpiente->cabeza->posicion.x = 0;
    else if (serpiente->cabeza->posicion.x < 0) serpiente->cabeza->posicion.x = ANCHO_PANTALLA - TAMANO_CUADRADO;
    if (serpiente->cabeza->posicion.y >= ALTO_PANTALLA) serpiente->cabeza->posicion.y = 0;
    else if (serpiente->cabeza->posicion.y < 0) serpiente->cabeza->posicion.y = ALTO_PANTALLA - TAMANO_CUADRADO;

    for (int i = 0; i < MAX_COMIDA; i++) {
        if (comida[i].activa && serpiente->cabeza->posicion.x == comida[i].posicion.x && serpiente->cabeza->posicion.y == comida[i].posicion.y) {
            comida[i].posicion = Vector2{ static_cast<float>(GetRandomValue(0, (ANCHO_PANTALLA / TAMANO_CUADRADO) - 1) * TAMANO_CUADRADO),
                                           static_cast<float>(GetRandomValue(0, (ALTO_PANTALLA / TAMANO_CUADRADO) - 1) * TAMANO_CUADRADO) };
            comida[i].activa = true;
            serpiente->longitud++;
        }
    }

    Nodo* temp = serpiente->cabeza;
    for (int i = 0; i < serpiente->longitud; i++) {
        if (temp->siguiente == NULL) {
            break;
        }
        temp = temp->siguiente;
    }


    temp = serpiente->cabeza->siguiente;
    while (temp != NULL) {
        if (serpiente->cabeza->posicion.x == temp->posicion.x && serpiente->cabeza->posicion.y == temp->posicion.y) {
            gameOver = true;
            break;
        }
        temp = temp->siguiente;
    }

    if (!gameOver) {
        Nodo* tail = serpiente->cabeza;
        for (int i = 0; i < serpiente->longitud - 1; i++) {
            tail = tail->siguiente;
        }

        Nodo* last = tail->siguiente;
        tail->siguiente = NULL;
        free(last);
        serpiente->longitud--;
    }
}

void DibujarJuego(Serpiente* serpiente, Comida* comida)
{

    if(gameOver){
        DrawText("GAME OVER", ANCHO_PANTALLA / 2 - MeasureText("GAME OVER", 40) / 2, ALTO_PANTALLA / 2 - 40, 40, RED);
        DrawText("Presiona [ESPACIO] para reiniciar", ANCHO_PANTALLA / 2 - MeasureText("Presiona [ESPACIO] para reiniciar", 20) / 2, ALTO_PANTALLA / 2, 20, DARKGRAY);
    }
    else {

	    DibujarFondo();
        Nodo* temp = serpiente->cabeza;
        while (temp != NULL) {
            DrawRectangleV(temp->posicion, Vector2{ TAMANO_CUADRADO, TAMANO_CUADRADO }, BLUE);
            temp = temp->siguiente;
        }

        for (int i = 0; i < MAX_COMIDA; i++) {
            if (comida[i].activa) {
                DrawRectangleV(comida[i].posicion, Vector2{ TAMANO_CUADRADO, TAMANO_CUADRADO }, RED);
            }
        }

    }
}

void DibujarFondo() {
	for (int i = 0; i < ANCHO_PANTALLA / TAMANO_CUADRADO; i++) {
		for (int j = 0; j < ALTO_PANTALLA / TAMANO_CUADRADO; j++) {
			if ((i + j) % 2 == 0) {
				DrawRectangle(i * TAMANO_CUADRADO, j * TAMANO_CUADRADO, TAMANO_CUADRADO, TAMANO_CUADRADO, GREEN);
			}
			else {
				DrawRectangle(i * TAMANO_CUADRADO, j * TAMANO_CUADRADO, TAMANO_CUADRADO, TAMANO_CUADRADO, DARKGREEN);
			}
		}
	}
}

void ReiniciarJuego(Serpiente* serpiente, Comida* comida) {
    while (serpiente->cabeza != NULL) {
        Nodo* eliminar = serpiente->cabeza;
        serpiente->cabeza = serpiente->cabeza->siguiente;
        free(eliminar);
    }

	IniciarJuego(serpiente, comida);
	gameOver = false;

}