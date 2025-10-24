//  Autores: Consuelo Arriagada y Antonia Orellana
//  Programacion Seccion 2
//  23 al 27 de octubre de 2025

#include <stdio.h>
#include <string.h>

#define DIMENSION 7 // Dimensiones del tablero (7x7)

typedef struct {
    int fila_origen, columna_origen;        
    int fila_destino, columna_destino;
} Movimiento;

//Declaracion de funciones
int es_movimiento_valido(int, int, int, int); //verifica si un movimiento es valido (esta dentro de los limites del tablero)
void aplicar_movimiento(int, int, int, int); //genera movimientos en el tablero y actualiza el estado de las fichas
void deshacer_movimiento(int, int, int, int); //revierte un movimiento cuando no existen mas movimientos validos
int contar_fichas(); // para contar cuantas fichas hay en el tablero
int resolver(); //busca la solucion del solitario

//Variables globales
char tablero[DIMENSION][DIMENSION]; // Para definir 'X' como casilla valida y 'O' como casilla invalida
int tablero_fichas[DIMENSION][DIMENSION]; // Define el estado de las casillas del tablero,  1 = espacio con ficha, 0 = espacio vacio
Movimiento movimientos[32]; //Almacena los movimientos realizados
int cantidad_movimientos = 0; //contador

int main() {
    //Leemos la forma del tablero
    int i, j;
    char linea[DIMENSION + 2]; // +2 para '\n' y '\0'

    for (i = 0; i < DIMENSION; i++) {
        fgets(linea, sizeof(linea), stdin);
        for (j = 0; j < DIMENSION; j++) {
            tablero[i][j] = linea[j];
            if (tablero[i][j] == ' ') {
                tablero[i][j] = 'X'; // Tomamos el espacio de la casilla central como valida al leer la entrada de datos transformandola en 'X'
            }
        }
    }

    //Inicializamos fichas en el tablero
    for (i = 0; i < DIMENSION; i++) {
        for (j = 0; j < DIMENSION; j++) {
            if (tablero[i][j] == 'X') {
                tablero_fichas[i][j] = 1;
            }
            else {
                tablero_fichas[i][j] = 0;
            }
        }
    }
    tablero_fichas[3][3] = 0; //casilla central (sin ficha)

    if (resolver()) {
        printf("En %d movimientos se encontro la solucion\n", cantidad_movimientos);
        for (i = 0; i < cantidad_movimientos; i++) {
            printf("%d: posicion <%d, %d> a posicion <%d, %d>\n", i + 1, movimientos[i].fila_origen + 1, movimientos[i].columna_origen + 1, movimientos[i].fila_destino + 1, movimientos[i].columna_destino + 1);
        }
    }
    else {
        printf("No se encontro solucion\n");
    }

    return 0;
}

int es_movimiento_valido(int f_origen, int c_origen, int f_destino, int c_destino) {
    int f_medio = (f_origen + f_destino) / 2;
    int c_medio = (c_origen + c_destino) / 2;

    if (f_destino < 0 || f_destino >= DIMENSION || c_destino < 0 || c_destino >= DIMENSION) {
        return 0;
    }
    if (tablero[f_origen][c_origen] != 'X' || tablero[f_medio][c_medio] != 'X' || tablero[f_destino][c_destino] != 'X') {
        return 0;
    }
    if (tablero_fichas[f_origen][c_origen] != 1 || tablero_fichas[f_medio][c_medio] != 1 || tablero_fichas[f_destino][c_destino] != 0) {
        return 0;
    }

    return 1;
}

void aplicar_movimiento(int f_origen, int c_origen, int f_destino, int c_destino) {
    int f_medio = (f_origen + f_destino) / 2;
    int c_medio = (c_origen + c_destino) / 2;

    tablero_fichas[f_origen][c_origen] = 0;
    tablero_fichas[f_medio][c_medio] = 0;
    tablero_fichas[f_destino][c_destino] = 1;

    Movimiento nuevo_movimiento;
    nuevo_movimiento.fila_origen = f_origen;
    nuevo_movimiento.columna_origen = c_origen;
    nuevo_movimiento.fila_destino = f_destino;
    nuevo_movimiento.columna_destino = c_destino;

    movimientos[cantidad_movimientos] = nuevo_movimiento;
    cantidad_movimientos++;
}

void deshacer_movimiento(int f_origen, int c_origen, int f_destino, int c_destino) {
    int f_medio = (f_origen + f_destino) / 2;
    int c_medio = (c_origen + c_destino) / 2;

    tablero_fichas[f_origen][c_origen] = 1;
    tablero_fichas[f_medio][c_medio] = 1;
    tablero_fichas[f_destino][c_destino] = 0;

    cantidad_movimientos--;
}

int contar_fichas() {
    int contador = 0, i, j;
    for (i = 0; i < DIMENSION; i++) {
        for (j = 0; j < DIMENSION; j++) {
            if (tablero_fichas[i][j] == 1) {
                contador++;
            }
        }
    }
    return contador;
}

int resolver() {
    if (contar_fichas() == 1 && tablero_fichas[3][3] == 1) {
        return 1;
    }
    
    int fila, col;
    for (fila = 0; fila < DIMENSION; fila++) {
        for (col = 0; col < DIMENSION; col++) {
            int desplazamientos_fila[4] = {-2, 2, 0, 0};
            int desplazamientos_columna[4] = {0, 0, -2, 2};

            int d;
            for (d = 0; d < 4; d++) {
                int nueva_fila = fila + desplazamientos_fila[d];
                int nueva_columna = col + desplazamientos_columna[d];

                if (es_movimiento_valido(fila, col, nueva_fila, nueva_columna)) {
                    aplicar_movimiento(fila, col, nueva_fila, nueva_columna);
                    if (resolver()) {
                        return 1;
                    }
                    deshacer_movimiento(fila, col, nueva_fila, nueva_columna);
                }
            }
        }
    }
    return 0;
}