#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

int main(void) {
	char characters;

	int iFds = open("./input.txt", O_RDONLY); // inputFD
	int oFds = open("./output.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR); //outputFD

	close(1);
	dup(oFds);

	while(read(iFds, &characters,1)){
		cout<<characters;   
	}

	close(iFds);
	close(oFds);

	return EXIT_SUCCESS;
}
