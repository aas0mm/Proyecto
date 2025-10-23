//Idea de codigo para el proyecto

#include <stdio.h>
#include <string.h>

#define DIM 7 // Dimensiones del tablero

typedef struct {
    int fila_origen, columna_origen;
    int fila_destino, columna_destino;
} Movimiento;

//Declaracion de funciones (no estan completas)
int es_movimiento_valido();
void aplicar_movimiento();
void deshacer_movimiento();
int contar_fichas();
int resolver();

int main() {
    char tablero[DIM][DIM]; // Para definir 'X' como casilla valida y 'O' como casilla invalida
    int tablero_fichas[DIM][DIM]; // 1 = espacio con ficha, 0 = espacio vacio
    Movimiento movimientos[32]; //Maximo de movimientos posibles
    int cantidad_movimientos = 0;

    //Leer la forma del tablero
    int i, j;
    for (i = 0; i < DIM; i++) {
        scanf("%s", tablero[i]);
    }

    //Inicializar fichas en el tablero
    for (i = 0; i < DIM; i++) {
        for (j = 0; j < DIM; j++) {
            if (tablero[i][j] == 'X') {
                tablero_fichas[i][j] = 1;
            }
            else {
                tablero_fichas[i][j] = 0;
            }
        }
    }

    tablero_fichas[3][3] = 0;

    if (resolver()) {
        printf("En %d movimientos se encontro la solucion\n", cantidad_movimientos);
        for (i = 0; i < cantidad_movimientos; i++) {
            printf("%d: posicion <%d, %d> a posicion <%d, %d>\n", i + 1, movimientos[i].fila_origen + 1, movimientos[i].columna_origen + 1, movimientos[i].fila_destino, movimientos[i].columna_destino);
        }
    }
    else {
        printf("No se encontro solucion\n");
    }

    return 0;
}