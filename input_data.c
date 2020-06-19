#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *fp;
	char line[256];

	fp = fopen("testdata", "r");

	while (fgets(line, sizeof(line), fp)){
		printf("%s", line);

	}

	fclose(fp);
}
