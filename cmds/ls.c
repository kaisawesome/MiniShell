#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>	// For getpwuid() - mapping uid to user name
#include <grp.h>	// For getgrgid() - mapping gid to group name
#include <time.h>	// For strftime() - converting mtime in stat() to string
#include <ctype.h>	// For tolower()

// My implementation of case insensitive string compare
int strcmpi(char const *a, char const *b)
{
	for (;; a++, b++){
		int d = tolower(*a) - tolower(*b);
		if (d != 0 || !*a)
			return d;
	}
}

int main(int argc, char ** argv){

	DIR * dir;

	// An array of directory entries makes it easier to apply the options
	// Assumumption that no directory has more than 1024 entries
	struct dirent * entry[1024];
	int directory = 1;

	// By default, the second argument will be the directory
	directory = 1;
	// Check if any directories are specified
	if ( argv[directory] == NULL ){
		argv[directory] = ".";
		argc++;
	}

	int i = directory;
	for ( i = directory ; i < argc ; i++ ){

		// Open directory stream
		if ( (dir = opendir(argv[i])) == NULL ){
			fprintf(stderr, "\nminsh: %s - No such file or directory\n\n", argv[i]);
			return 0;
		}

		printf("\n%s:\n", argv[i]);

		// Read every directory entry and print it

		errno = 0;	// Distinguish between end of directory
				// and error

		int entno = 0;	// Entry number - keep track of position in the array of entries
		while ( 1 ){
			// Adds the entry to the array of entries
			// Automatically adds NULL to specify the end too!!
			entry[entno] = readdir(dir);
			if (entry[entno] == NULL){
				if (errno == 0){	// End of directory
					break;
				}
				else{	// Error has occurred
					perror("error in ls");
					return 1;
				}
			}
			entno++;
		}

		// Sort the directory entries based on name
		int j = 0;
		int k = 0;
		struct dirent * temp;
		for ( j = 0 ; j < entno-1 ; j++ ){
			for ( k = j+1 ; k < entno ; k++ ){
				if ( strcmpi(entry[j]->d_name, entry[k]->d_name) > 0 ){
					temp = entry[j];
					entry[j] = entry[k];
					entry[k] = temp;
				}
			}
		}


        for (entno = 0 ; entry[entno] != NULL ; entno++){
            if ( (entry[entno]->d_name)[0] != '.'){
                printf("\n");
                printf("%s", entry[entno]->d_name);
            }
        }
	}
	printf("\n\n");
	return 0;
}

