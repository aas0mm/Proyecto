#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TAMANIO_TABLERO 7
#define MAX_MOVIMIENTOS 100

// Direcciones: arriba, derecha, abajo, izquierda
const int dx[] = {-1, 0, 1, 0};
const int dy[] = {0, 1, 0, -1};

typedef struct {
    int fila_origen, columna_origen;
    int fila_destino, columna_destino;
} Movimiento;

char tablero[TAMANIO_TABLERO][TAMANIO_TABLERO];
bool casilla_valida[TAMANIO_TABLERO][TAMANIO_TABLERO];
Movimiento solucion[MAX_MOVIMIENTOS];
int contador_solucion = 0;
bool solucion_encontrada = false;

// Verifica si una posición está dentro del tablero y es válida
bool es_posicion_valida(int fila, int columna) {
    return fila >= 0 && fila < TAMANIO_TABLERO && 
           columna >= 0 && columna < TAMANIO_TABLERO && 
           casilla_valida[fila][columna];
}

// Verifica si un movimiento es válido
bool es_movimiento_valido(int fila_origen, int columna_origen, int fila_destino, int columna_destino) {
    // Verificar posiciones válidas
    if (!es_posicion_valida(fila_origen, columna_origen) || 
        !es_posicion_valida(fila_destino, columna_destino))
        return false;
    
    // La posición de origen debe tener ficha
    if (tablero[fila_origen][columna_origen] != 'X')
        return false;
    
    // La posición destino debe estar vacía
    if (tablero[fila_destino][columna_destino] != '.')
        return false;
    
    // Debe ser un salto de exactamente 2 casillas en línea recta
    int diferencia_fila = fila_destino - fila_origen;
    int diferencia_columna = columna_destino - columna_origen;
    
    if ((abs(diferencia_fila) == 2 && diferencia_columna == 0) || 
        (abs(diferencia_columna) == 2 && diferencia_fila == 0)) {
        // Verificar que hay una ficha en medio
        int fila_media = fila_origen + diferencia_fila / 2;
        int columna_media = columna_origen + diferencia_columna / 2;
        
        if (es_posicion_valida(fila_media, columna_media) && 
            tablero[fila_media][columna_media] == 'X') {
            return true;
        }
    }
    
    return false;
}

// Realiza un movimiento
void realizar_movimiento(int fila_origen, int columna_origen, int fila_destino, int columna_destino) {
    int fila_media = fila_origen + (fila_destino - fila_origen) / 2;
    int columna_media = columna_origen + (columna_destino - columna_origen) / 2;
    
    // Mover ficha
    tablero[fila_destino][columna_destino] = 'X';
    tablero[fila_origen][columna_origen] = '.';
    tablero[fila_media][columna_media] = '.';
    
    // Guardar movimiento en solución
    if (contador_solucion < MAX_MOVIMIENTOS) {
        solucion[contador_solucion].fila_origen = fila_origen + 1; // +1 para índice base 1
        solucion[contador_solucion].columna_origen = columna_origen + 1;
        solucion[contador_solucion].fila_destino = fila_destino + 1;
        solucion[contador_solucion].columna_destino = columna_destino + 1;
        contador_solucion++;
    }
}

// Deshace un movimiento
void deshacer_movimiento(int fila_origen, int columna_origen, int fila_destino, int columna_destino) {
    int fila_media = fila_origen + (fila_destino - fila_origen) / 2;
    int columna_media = columna_origen + (columna_destino - columna_origen) / 2;
    
    // Restaurar fichas
    tablero[fila_origen][columna_origen] = 'X';
    tablero[fila_destino][columna_destino] = '.';
    tablero[fila_media][columna_media] = 'X';
    
    // Remover movimiento de solución
    contador_solucion--;
}

// Cuenta cuántas fichas quedan en el tablero
int contar_fichas() {
    int contador = 0;
    for (int i = 0; i < TAMANIO_TABLERO; i++) {
        for (int j = 0; j < TAMANIO_TABLERO; j++) {
            if (casilla_valida[i][j] && tablero[i][j] == 'X') {
                contador++;
            }
        }
    }
    return contador;
}

// Función recursiva de backtracking para resolver el solitario
void resolver_solitario() {
    if (solucion_encontrada) return;
    
    // Verificar si hemos llegado a la solución
    if (contar_fichas() == 1 && tablero[3][3] == 'X') {
        solucion_encontrada = true;
        return;
    }
    
    // Probar todos los movimientos posibles
    for (int i = 0; i < TAMANIO_TABLERO && !solucion_encontrada; i++) {
        for (int j = 0; j < TAMANIO_TABLERO && !solucion_encontrada; j++) {
            if (tablero[i][j] == 'X') {
                // Probar todas las direcciones
                for (int direccion = 0; direccion < 4 && !solucion_encontrada; direccion++) {
                    int destino_i = i + 2 * dx[direccion];
                    int destino_j = j + 2 * dy[direccion];
                    
                    if (es_movimiento_valido(i, j, destino_i, destino_j)) {
                        realizar_movimiento(i, j, destino_i, destino_j);
                        resolver_solitario();
                        if (!solucion_encontrada) {
                            deshacer_movimiento(i, j, destino_i, destino_j);
                        }
                    }
                }
            }
        }
    }
}

// Función para imprimir el tablero (útil para depuración)
void imprimir_tablero() {
    printf("Estado del tablero:\n");
    for (int i = 0; i < TAMANIO_TABLERO; i++) {
        for (int j = 0; j < TAMANIO_TABLERO; j++) {
            if (casilla_valida[i][j]) {
                printf("%c ", tablero[i][j]);
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    // Leer el tablero de entrada
    printf("Leyendo configuracion del tablero...\n");
    for (int i = 0; i < TAMANIO_TABLERO; i++) {
        char linea[8];
        scanf("%7s", linea);
        for (int j = 0; j < TAMANIO_TABLERO; j++) {
            tablero[i][j] = linea[j];
            casilla_valida[i][j] = (linea[j] == 'X');
        }
    }
    
    // Configurar estado inicial: todas las posiciones válidas tienen fichas excepto el centro
    for (int i = 0; i < TAMANIO_TABLERO; i++) {
        for (int j = 0; j < TAMANIO_TABLERO; j++) {
            if (casilla_valida[i][j]) {
                if (i == 3 && j == 3) {
                    tablero[i][j] = '.'; // Centro vacío
                } else {
                    tablero[i][j] = 'X'; // Ficha
                }
            } else {
                tablero[i][j] = ' '; // Posición inválida
            }
        }
    }
    
    printf("Configuracion inicial establecida.\n");
    printf("Fichas iniciales: %d\n", contar_fichas());
    
    // Resolver el solitario
    printf("Resolviendo solitario...\n");
    resolver_solitario();
    
    // Mostrar solución
    if (solucion_encontrada) {
        printf("En %d movimientos se encontro la solucion\n", contador_solucion);
        for (int i = 0; i < contador_solucion; i++) {
            printf("%d: posicion <%d,%d> a posicion <%d,%d>\n", 
                   i + 1, 
                   solucion[i].fila_origen, solucion[i].columna_origen,
                   solucion[i].fila_destino, solucion[i].columna_destino);
        }
    } else {
        printf("No se encontro solucion\n");
    }
    
    return 0;
}