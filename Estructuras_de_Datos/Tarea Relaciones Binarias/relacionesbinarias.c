#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//      Estructura de la relación binaria.
// Una matriz de valores booleanos para marcar verdadero en donde exista la relación
// binaria
typedef struct {
    bool **relacion;
    int cantidad,ancho,alto;
} RelacionBinaria;
// Estructura de una imagen a color en el formato PPM
typedef struct {
    int ancho, alto;
    int **rojo, **verde, **azul; // tres matrices para almacenar el color en formato RGB
}ImagenPPM;
// Construir el espacio en memoria para almacenar la imagen PPM almacenada en el
// archivo cuyo nombre se encuentra almacenado en la variable nombreArchivo
void inicializarImagen(ImagenPPM *I,char *nombreArchivo){
    FILE *archivo = fopen(nombreArchivo,"r"); // Abrir el archivo para lectura
    int ancho, alto, escala, tipo;char letra;
    fscanf(archivo,"%c%d %d %d %d",&letra,&tipo,&ancho, &alto, &escala);
    I->ancho=ancho;I->alto=alto;
    I->rojo = (int **)malloc(alto * sizeof(int*));
    I->verde = (int **)malloc(alto * sizeof(int*));
    I->azul = (int **)malloc(alto * sizeof(int*));
    for(int y=0;y<alto;y++){
        I->rojo[y] = (int *)calloc(ancho,sizeof(int));
        I->verde[y] = (int *)calloc(ancho,sizeof(int));
        I->azul[y] = (int *)calloc(ancho,sizeof(int));
        for(int x=0;x<ancho;x++){
            fscanf(archivo,"%d %d %d",&I->rojo[y][x],&I->verde[y][x],&I->azul[y][x]);
        }
    }
    fclose(archivo);
}
void ImprimirImagenPPM(ImagenPPM *I){
    printf("P3\n%d %d\n255\n",I->ancho,I->alto);
    for(int y=0;y<I->alto;y++){
        for(int x=0;x<I->ancho;x++){
            printf("%d %d %d ",I->rojo[y][x],I->verde[y][x],I->azul[y][x]);
        }
        printf("\n");
    }
}
// Inicializar la relación binaria, todos los espacios de la matriz deben contener el valor false
void inicializarRelacion(RelacionBinaria *r, int ancho, int alto) {
    r->cantidad = 0;r->alto=alto;r->ancho = ancho;
    r->relacion = (bool **)calloc(r->alto,sizeof(bool *)); // Memoria dinámica para una matriz
    for(int i = 0; i < r->alto; i++) {
        r->relacion[i] = (bool *)calloc(r->ancho,sizeof(bool)); // Memoria para un renglón
        for(int j = 0; j < ancho; j++) {
            r->relacion[i][j] = false;
        }
    }
}
// La relación binaria se refiere a las coincidencia con el color RGB y las variables de
// entrada: rojo, verde y azul. Se marca con true la posición o píxel en la matriz existente en
// la variable tipo relaciónBinaria que coincide con el color que se recibe como argumento en
// la función y los colores en la variable del tipo ImagenPPM.
void construirRelacionCoincidencia(RelacionBinaria *r,ImagenPPM *I, int rojo, int verde, int
azul) {
    for(int i = 0; i < I->alto; i++) {
        for(int j = 0; j < I->ancho; j++) {
            if(I->rojo[i][j] == rojo && I->verde[i][j] == verde && I->azul[i][j] == azul) {
                r->relacion[i][j] = true;
                r->cantidad++;
            }
        }
    }
}
// Se imprime la relación binaria como una imagen PPM en el formato binario
void imprimirRelacion(const RelacionBinaria *r) {
    printf("P1\n%d %d\n",r->ancho,r->alto);
    for(int i = 0; i < r->alto; i++) {
        for(int j = 0; j < r->ancho; j++) {
            printf("%d ", r->relacion[i][j]);
        }
        printf("\n");
    }
}

int contarColores(ImagenPPM *I)
{
    int totalColores = 0;
    
    // RECORRER TODOS LOS PIXELES
    for(int y = 0; y < I->alto; y++)
    {
        for(int x = 0; x < I->ancho; x++)
        {
            int duplicado = 0;
            for(int y2 = 0; y2 <= y && !duplicado; y2++)
            {
                for(int x2 = 0; x2 < (y2 == y ? x : I->ancho) && !duplicado; x2++)
                {
                    if(I->rojo[y][x] == I->rojo[y2][x2] && I->verde[y][x] == I->verde[y2][x2] && I->azul[y][x] == I->azul[y2][x2])
                        {
                            duplicado = 1;
                        }
                }
            }
            
            if(!duplicado) 
                {
                    totalColores++;
                }
        }
    }
    return totalColores;
}
// Ejemplo de uso de las estructuras y sus propiedades.
int main(int argc, char *argv[]) {
    ImagenPPM IMG;
    inicializarImagen(&IMG,argv[1]);
    RelacionBinaria r;
    inicializarRelacion(&r, IMG.ancho,IMG.alto);
    construirRelacionCoincidencia(&r, &IMG, 253,152,39);
    imprimirRelacion(&r);
    int colores = contarColores(&IMG);
    printf("Hay %d colores diferentes\n", colores);
    return 0;
}