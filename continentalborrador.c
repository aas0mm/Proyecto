#include <stdio.h>
#include <string.h>

#define DIM 8 // Dimensiones del tablero para ingresarlo desde la posicion (1,1)

typedef struct {
    int fila_origen, columna_origen;
    int fila_destino, columna_destino;
} Movimiento;

void mostrar_tablero(); // solo para verificar que esta bien
int es_movimiento_valido(int, int, int, int); //verifica si un movimiento es valido (esta dentro de los limites del tablero)
void aplicar_movimiento(int, int, int, int); //genera movimientos en el tablero
void deshacer_movimiento(int, int, int, int); //revierte un movimiento cuando no existen mas movimientos validos
int contar_fichas(); // para contar cuantas fichas hay en el tablero
int resolver();

//Variables globales
char tablero[DIM][DIM]; // Para definir 'X' como casilla valida y 'O' como casilla invalida
int tablero_fichas[DIM][DIM]; // 1 = espacio con ficha, 0 = espacio vacio
Movimiento movimientos[32]; //Maximo de movimientos posibles
int cantidad_movimientos = 0;

int main() {
    //Leer la forma del tablero
    int i, j;

    for (i = 0; i < DIM; i++) {
        for (j = 0; j < DIM; j++) {
            tablero[0][j] = 'O';
        }
        tablero[i][0] = 'O';
    }

    for (i = 1; i <= 7; i++) {
        for (j = 1; j <= 7; j++) {
            scanf("%c", &tablero[i][j]);
            if (tablero[i][j] == ' ') {
                tablero[i][j] = 'X';
            }
        }
        scanf("%*c"); // Consumir el '\n' al final de cada línea
    }

    mostrar_tablero();

    //Inicializar fichas en el tablero
    for (i = 1; i < DIM; i++) {
        for (j = 1; j < DIM; j++) {
            if (tablero[i][j] == 'X') {
                tablero_fichas[i][j] = 1;
            }
            else {
                tablero_fichas[i][j] = 0;
            }
        }
    }

    tablero_fichas[4][4] = 0;

    if (resolver()) {
        printf("En %d movimientos se encontro la solucion\n", cantidad_movimientos);
        for (i = 0; i < cantidad_movimientos; i++) {
            printf("%d: posicion <%d, %d> a posicion <%d, %d>\n", i + 1, movimientos[i].fila_origen, movimientos[i].columna_origen, movimientos[i].fila_destino, movimientos[i].columna_destino);
        }
    }
    else {
        printf("No se encontro solucion\n");
    }

    return 0;
}

void mostrar_tablero() {
    for (int i = 1; i < DIM; i++) {
        for (int j = 1; j < DIM; j++) {
            printf("%c", tablero[i][j]);
        }
        printf("\n");
    }
}

int es_movimiento_valido(int f1, int c1, int f2, int c2) {
    int f_medio = (f1 + f2) / 2;
    int c_medio = (c1 + c2) / 2;

    if (f2 < 0 || f2 >= DIM || c2 < 0 || c2 >= DIM) {
        return 0;
    }
    if (tablero[f1][c1] != 'X' || tablero[f_medio][c_medio] != 'X' || tablero[f2][c2] != 'X') {
        return 0;
    }
    if (tablero_fichas[f1][c1] != 1 || tablero_fichas[f_medio][c_medio] != 1 || tablero_fichas[f2][c2] != 0) {
        return 0;
    }

    return 1;
}

void aplicar_movimiento(int f1, int c1, int f2, int c2) {
    int f_medio = (f1 + f2) / 2;
    int c_medio = (c1 + c2) / 2;

    tablero_fichas[f1][c1] = 0;
    tablero_fichas[f_medio][c_medio] = 0;
    tablero_fichas[f2][c2] = 1;

    Movimiento nuevo_movimiento;
    nuevo_movimiento.fila_origen = f1;
    nuevo_movimiento.columna_origen = c1;
    nuevo_movimiento.fila_destino = f2;
    nuevo_movimiento.columna_destino = c2;

    movimientos[cantidad_movimientos] = nuevo_movimiento;
    cantidad_movimientos++;
}

void deshacer_movimiento(int f1, int c1, int f2, int c2) {
    int f_medio = (f1 + f2) / 2;
    int c_medio = (c1 + c2) / 2;

    tablero_fichas[f1][c1] = 1;
    tablero_fichas[f_medio][c_medio] = 1;
    tablero_fichas[f2][c2] = 0;

    cantidad_movimientos--;
}

int contar_fichas() {
    int contador = 0, i, j;
    for (i = 0; i < DIM; i++) {
        for (j = 0; j < DIM; j++) {
            if (tablero_fichas[i][j] == 1) {
                contador++;
            }
        }
    }
    return contador;
}

int resolver() {
    if (contar_fichas() == 1 && tablero_fichas[4][4] == 1) {
        return 1;
    }
    
    int f, c;
    for (f = 1; f < DIM; f++) {
        for (c = 1; c < DIM; c++) {
            int desplazamientos_fila[4] = {-2, 2, 0, 0};
            int desplazamientos_columna[4] = {0, 0, -2, 2};

            int d;
            for (d = 0; d < 4; d++) {
                int nueva_fila = f + desplazamientos_fila[d];
                int nueva_columna = c + desplazamientos_columna[d];

                if (es_movimiento_valido(f, c, nueva_fila, nueva_columna)) {
                    aplicar_movimiento(f, c, nueva_fila, nueva_columna);
                    if (resolver()) {
                        return 1;
                    }
                    deshacer_movimiento(f, c, nueva_fila, nueva_columna);
                }
            }
        }
    }
    return 0;
}