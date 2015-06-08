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
#define TAMANIO_BLOQUE (20 * 1024 * 1024)


int armarBloque(char *bloque_completo);

int main(void) {

	struct stat file;
	int fd;
	char *data;
//	char *datos_a_guardar;
//	char *datos_a_reciclar;
	data = malloc (TAMANIO_BLOQUE);

	FILE *prueba_restantes = fopen ("pruebaRestantes.txt","w");

	FILE *archivo_de_20 = fopen ("de20Megas.txt","w");

	/*obtengo el descriptor del fichero*/
	fd = open("/home/utnso/Escritorio/201301hourly.txt", O_RDONLY);
	if(fd == -1){
		perror("Error al obtener el descriptor de fichero del archivo");
		exit(1);
	}
	fflush(stdout);

	fstat(fd, &file);


	if((mmap((caddr_t)0, file.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED){

		perror("mmap");
		exit(1);
	}
	else {

		memcpy(data, mmap((caddr_t)0, TAMANIO_BLOQUE , PROT_READ, MAP_SHARED, fd, 0), TAMANIO_BLOQUE);
	}

	fwrite (data, sizeof(char), TAMANIO_BLOQUE, archivo_de_20);

	int sobran = armarBloque(data);

	fwrite (data ,sizeof(char), TAMANIO_BLOQUE ,prueba_restantes);


	printf("%d", sobran);
	fflush(stdout);
	//Buenas practicas segun foro
	munmap(data,file.st_size);
	close(fd);

	free (data);

	fclose(archivo_de_20);
	fclose(prueba_restantes);
	//free (datos_a_guardar);
	//free (datos_a_reciclar);

	return 0;
}

int armarBloque(char *bloque_completo){

	int restante = TAMANIO_BLOQUE - 1;

	while(restante > 0 && bloque_completo[restante] != '\n')
	{
		if(bloque_completo[restante]== '\n')break;

		bloque_completo[restante]= '0';

		restante--;

	}

	return TAMANIO_BLOQUE - (restante + 1);
}
