#include <stdio.h>

#define TAMANO 7
#define MAX_MOVIMIENTOS 32

// Estructura para representar un movimiento
typedef struct {
    int fila_origen, columna_origen;
    int fila_destino, columna_destino;
} Movimiento;

// Tablero de diseño (X = válida, O = inválida)
char diseño_tablero[TAMANO][TAMANO];

// Tablero de fichas (1 = ficha, 0 = vacío)
int estado_tablero[TAMANO][TAMANO];

// Arreglo para guardar los movimientos realizados
Movimiento lista_movimientos[MAX_MOVIMIENTOS];
int total_movimientos = 0;

// Verifica si un movimiento es válido
int es_valido(int f1, int c1, int f2, int c2) {
    int f_medio = (f1 + f2) / 2;
    int c_medio = (c1 + c2) / 2;

    // Verifica límites y que todas las casillas involucradas sean válidas
    if (f2 < 0 || f2 >= TAMANO || c2 < 0 || c2 >= TAMANO) return 0;
    if (diseño_tablero[f1][c1] != 'X' || diseño_tablero[f_medio][c_medio] != 'X' || diseño_tablero[f2][c2] != 'X') return 0;

    // Verifica que haya ficha en origen y en medio, y que destino esté vacío
    if (estado_tablero[f1][c1] != 1 || estado_tablero[f_medio][c_medio] != 1 || estado_tablero[f2][c2] != 0) return 0;

    return 1;
}

// Aplica un movimiento
void aplicar(int f1, int c1, int f2, int c2) {
    int f_medio = (f1 + f2) / 2;
    int c_medio = (c1 + c2) / 2;

    estado_tablero[f1][c1] = 0;
    estado_tablero[f_medio][c_medio] = 0;
    estado_tablero[f2][c2] = 1;

    lista_movimientos[total_movimientos++] = (Movimiento){f1, c1, f2, c2};
}

// Revierte un movimiento (backtracking)
void deshacer(int f1, int c1, int f2, int c2) {
    int f_medio = (f1 + f2) / 2;
    int c_medio = (c1 + c2) / 2;

    estado_tablero[f1][c1] = 1;
    estado_tablero[f_medio][c_medio] = 1;
    estado_tablero[f2][c2] = 0;

    total_movimientos--;
}

// Cuenta cuántas fichas hay en el tablero
int contar_fichas() {
    int contador = 0;
    for (int i = 0; i < TAMANO; i++)
        for (int j = 0; j < TAMANO; j++)
            if (estado_tablero[i][j] == 1)
                contador++;
    return contador;
}

// Algoritmo de backtracking para encontrar solución
int resolver() {
    if (contar_fichas() == 1 && estado_tablero[3][3] == 1)
        return 1; // Solución encontrada

    for (int f = 0; f < TAMANO; f++) {
        for (int c = 0; c < TAMANO; c++) {
            // Posibles direcciones: arriba, abajo, izquierda, derecha
            int df[4] = {-2, 2, 0, 0};
            int dc[4] = {0, 0, -2, 2};

            for (int d = 0; d < 4; d++) {
                int f2 = f + df[d];
                int c2 = c + dc[d];

                if (es_valido(f, c, f2, c2)) {
                    aplicar(f, c, f2, c2);
                    if (resolver()) return 1; // Detener al encontrar solución
                    deshacer(f, c, f2, c2);
                }
            }
        }
    }
    return 0; // No se encontró solución en esta rama
}

int main() {
    // Leer diseño del tablero
    for (int i = 0; i < TAMANO; i++)
        scanf("%s", diseño_tablero[i]);

    // Inicializar estado del tablero con fichas en casillas válidas
    for (int i = 0; i < TAMANO; i++)
        for (int j = 0; j < TAMANO; j++)
            estado_tablero[i][j] = (diseño_tablero[i][j] == 'X') ? 1 : 0;

    // Casilla central vacía al inicio
    estado_tablero[3][3] = 0;

    // Resolver el solitario
    if (resolver()) {
        printf("En %d movimientos se encontró la solución\n", total_movimientos);
        for (int i = 0; i < total_movimientos; i++) {
            printf("%d: posición <%d,%d> a posición <%d,%d>\n",
                   i + 1,
                   lista_movimientos[i].fila_origen + 1, lista_movimientos[i].columna_origen + 1,
                   lista_movimientos[i].fila_destino + 1, lista_movimientos[i].columna_destino + 1);
        }
    } else {
        printf("No se encontró solución\n");
    }

    return 0;
}
