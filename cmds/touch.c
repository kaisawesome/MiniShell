#include<stdio.h>

int main(int argc, char **argv)
{
    if (argc == 1){
		fprintf(stderr, "error in touch: Argument required\n");
		return 1;
	}

    FILE *new;
    new=fopen(argv[1],"w+");
    fclose(new);

    return 0;
}
