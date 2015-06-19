/*
 ============================================================================
 Name        : pruebaMmap.c
 Author      : Lucas Vergñory, Leandro Wagner
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#define TAMANIO_BLOQUE 20*1024*1024


int armarBloque(char *bloque_completo,int tamanio_aux);

void armarBloqueFinal(char *bloque_final, int cantidad_escrita);

void agregarArchivoAlMDFS(char *archivo_a_agregar, int indice);

void agregarArchivoAlMDFS(char *archivo_a_agregar, int indice, t_list *nodos_sistema) {
	struct stat file;
	int file_descriptor, tamanio_archivo;
	int desplazamiento = 0;
	int bytes_sobrantes = 0;
	int tamanio_final = 0;

	char *datos_a_copiar;
	void *archivo_mapeado;

	datos_a_copiar = malloc (TAMANIO_BLOQUE);

	/*obtengo el descriptor del fichero*/
	file_descriptor = open(archivo_a_agregar, O_RDONLY);
	if(file_descriptor == -1){
		perror("Error al obtener el descriptor de fichero del archivo");
		exit(1);
	}

	fstat(file_descriptor, &file);

	tamanio_archivo = file.st_size;

	archivo_mapeado = mmap((caddr_t)0, tamanio_archivo , PROT_READ, MAP_PRIVATE, file_descriptor, 0);


	if( archivo_mapeado == MAP_FAILED){

		perror("mmap");
		exit(1);
	}

	while (1){

		if( (tamanio_archivo - desplazamiento) < TAMANIO_BLOQUE  ){

			tamanio_final = (tamanio_archivo - desplazamiento);

			memcpy(datos_a_copiar, archivo_mapeado + desplazamiento, tamanio_final); //cambio tamanio_bloque por tamanio_final

			armarBloqueFinal(datos_a_copiar, tamanio_final ); //La que tengo que enviar es datos_a_copiar

			enviarBloqueCortado(datos_a_copiar, nodos_sistema);// La variable que falta es la lista de nodos

			free (datos_a_copiar);

			break;

		}

		memcpy(datos_a_copiar , archivo_mapeado + desplazamiento, TAMANIO_BLOQUE);

		bytes_sobrantes = armarBloque(datos_a_copiar, TAMANIO_BLOQUE);

		enviarBloqueCortado(datos_a_copiar, nodos_sistema); //La variable que falta es la lista de nodos

		desplazamiento = desplazamiento + (TAMANIO_BLOQUE - bytes_sobrantes);

	}

	munmap(archivo_mapeado,file.st_size);

	close(file_descriptor);

}

int armarBloque(char *bloque_completo , int tamanio_aux){

	int restante = tamanio_aux - 1;

	while(restante >= 0 && bloque_completo[restante] != '\n')
	{
		bloque_completo[restante] ='0';

		restante--;

	}

	return TAMANIO_BLOQUE - (restante + 1);
}

void armarBloqueFinal(char *bloque_final, int cantidad_escrita){

	int contador = TAMANIO_BLOQUE - 1;

	while (contador >= cantidad_escrita ){

		bloque_final[contador] = '0';

		contador --;
	}
}
