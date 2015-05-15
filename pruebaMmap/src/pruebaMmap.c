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

	//Buenas practicas segun foro
	munmap(data,file.st_size);
	close(fd);
	return 0;
}
