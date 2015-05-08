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

int espacioVacioBloque = 0;
char*   string_substring(char* text, int start, int length) {
	char* new_string = calloc(1, length + 1);
	strncpy(new_string, text + start, length);
	return new_string;

}

char *string_substring_from(char *text, int start) {
	return string_substring(text, start, strlen(text) - start);
}

int main(void) {

	struct stat file;
	int fd;
	const char *data;

	/*obtengo el descriptor del fichero*/
	fd = open("PruebaMMap.txt", O_RDONLY);
	if(fd == -1){
		perror("Error al obtener el descriptor de fichero del archivo");
		exit(1);
	}

	fstat(fd, &file);
	if((data = mmap((caddr_t)0, file.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED){
		perror("mmap");
		exit(1);
	}
	printf("%s\n",data);
	int i = string_length(data);
	for(i ;i > 0; i--){
		if(data[i]=='\n')
			{
			printf("Encontre un barraene posicion %d", i);
			break;
			}
	}
	printf("esto estaba antes del barraene %s\n",string_substring_until(data, i));
	printf("esto estaba despues del barraene %s\n",string_substring_from(data, i));

	//Mostrar por pantalla las palabras
	/*for(i=0;i<file.st_size;i++){
		printf("%c", bloque[i]);
	}*/
	//Buenas practicas segun foro
	munmap(data,file.st_size);
	close(fd);
	return 0;
}
