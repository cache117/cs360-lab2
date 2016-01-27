#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

main(int argc, char **argv)
{
	struct stat filestat;

	if(stat(argv[1], &filestat)) {
		cout <<"ERROR in stat\n";
	}
	if(S_ISREG(filestat.st_mode)) {
		cout << argv[1] << " is a regular file \n";
		cout << "File size = " << filestat.st_size << "\n";
		FILE *file = fopen(argv[1], "r");
		char *buffer = (char *) malloc(filestat.st_size);
		fread(buffer, filestat.st_size, 1, file);
		printf("File: \n\n%s", buffer);
		//write to socket instead of printf (sprintf)
		free(buffer);
		fclose(file);
	}
	if(S_ISDIR(filestat.st_mode)) {
		cout << argv[1] << " is a directory \n";
		DIR *dirp;
		struct dirent *dp;

		dirp = opendir(".");
		while ((dp = readdir(dirp)) != NULL)
  			printf("name %s\n", dp->d_name);
		(void)closedir(dirp);
	}

	
}

