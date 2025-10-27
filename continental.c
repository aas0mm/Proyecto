//  Autores: Consuelo Arriagada y Antonia Orellana
//  Programacion Seccion 2
//  23 al 27 de octubre de 2025

#include <stdio.h>
#include <string.h>

#define DIMENSION 8 // Dimensiones del tablero para poder guardarlo desde la posicion (1,1)

typedef struct {
    int fila_origen, columna_origen;        
    int fila_destino, columna_destino;
} Movimiento; 

//Declaracion de funciones
void leer_tablero(void);
void inicializar_fichas(void);
int es_movimiento_valido(int, int, int, int); //verifica si un movimiento es valido (esta dentro de los limites del tablero)
void aplicar_movimiento(int, int, int, int); //genera movimientos en el tablero y actualiza el estado de las fichas
void deshacer_movimiento(int, int, int, int); //revierte un movimiento cuando no existen mas movimientos validos
int contar_fichas(void); // para contar cuantas fichas hay en el tablero
int resolver(int); //busca la solucion del solitario

//Variables globales
char tablero[DIMENSION][DIMENSION]; // Para definir 'X' como casilla valida y 'O' como casilla invalida
int tablero_fichas[DIMENSION][DIMENSION]; // Define el estado de las casillas del tablero,  1 = espacio con ficha, 0 = espacio vacio
Movimiento movimientos[32]; //Almacena los movimientos realizados
int cantidad_movimientos = 0; //contador

int main() {
    leer_tablero();
    inicializar_fichas();
    printf("Resolviendo...\n");

    int i, fichas_iniciales = contar_fichas();

    if (resolver(fichas_iniciales)) {
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

void leer_tablero(void) { //leemos el formato del tablero desde (1,1) a (7,7) y transformamos la fila y columna 0 en casillas invalidas
    int i, j;
    char c;
    // Marcar bordes como inválidos
    for (i = 0; i < DIMENSION; i++) {
        tablero[0][i] = 'O';
        tablero[i][0] = 'O';
    }

    for (i = 1; i <= 7; i++) {
        j = 1;
        while (j <= 7) {
            scanf("%c", &c);
            if (c == '\n') {
                continue; //ignoramos saltos de linea
            }
            if (c == ' ') {
                c = 'X';   // Transformar espacios en 'X'
            }
            tablero[i][j] = c;
            j++;
        }
    }
}

void inicializar_fichas(void) { // las casillas con fichas contienen un 1 y las casillas vacias contienen ceros
    int i, j;

    for (i = 1; i < DIMENSION; i++) {
        for (j = 1; j < DIMENSION; j++) {
            if (tablero[i][j] == 'X') {
                tablero_fichas[i][j] = 1;
            }
            else {
                tablero_fichas[i][j] = 0;
            }
        }
    }

    tablero_fichas[4][4] = 0; //casilla central vacia
}

int es_movimiento_valido(int f_origen, int c_origen, int f_destino, int c_destino) {
    int f_medio = (f_origen + f_destino) / 2; //definimos la posicion que esta en medio de la posicion original y la posicion de destino para borrarla cuando se haga el movimiento
    int c_medio = (c_origen + c_destino) / 2;

    if (f_destino < 1 || f_destino >= DIMENSION || c_destino < 1 || c_destino >= DIMENSION) {
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

void deshacer_movimiento(int f_origen, int c_origen, int f_destino, int c_destino) { //marcamos la posicion anterior de las fichas para anular cambios
    int f_medio = (f_origen + f_destino) / 2;
    int c_medio = (c_origen + c_destino) / 2;

    tablero_fichas[f_origen][c_origen] = 1;
    tablero_fichas[f_medio][c_medio] = 1;
    tablero_fichas[f_destino][c_destino] = 0;

    cantidad_movimientos--;
}

int contar_fichas(void) {
    int contador = 0, i, j;
    for (i = 1; i < DIMENSION; i++) {
        for (j = 1; j < DIMENSION; j++) {
            if (tablero_fichas[i][j] == 1) {
                contador++;
            }
        }
    }
    return contador;
}

int resolver(int fichas_restantes) {
    if (fichas_restantes == 1 && tablero_fichas[4][4] == 1) {
        return 1;
    }

    int fila, col;
    for (fila = 1; fila < DIMENSION; fila++) {
        for (col = 1; col < DIMENSION; col++) {
            int desplazamientos_fila[4] = {0, 0, -2, 2};     // izquierda, derecha, arriba, abajo
            int desplazamientos_columna[4] = {-2, 2, 0, 0};

            for (int d = 0; d < 4; d++) {
                int nueva_fila = fila + desplazamientos_fila[d];
                int nueva_columna = col + desplazamientos_columna[d];

                if (es_movimiento_valido(fila, col, nueva_fila, nueva_columna)) {
                    aplicar_movimiento(fila, col, nueva_fila, nueva_columna);
                    if (resolver(fichas_restantes - 1)) {
                        return 1;
                    }
                    deshacer_movimiento(fila, col, nueva_fila, nueva_columna);
                }
            }
        }
    }
    return 0;
}