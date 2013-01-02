//Thomas Zaorski
//EMAIL: Zaorst@rpi.edu
//RIN: 660747712

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int special = 0;  //Is 1 if argc is != 1
char sfile[1000]; //The file that will contain the cmd argument for part 2

//Uses the value of buf.st_mode to print the permissions
void print_perm(int mode)
{
	//These first tree if statements print the owner's permissions
        if ((mode & S_IRUSR) && (mode & S_IREAD))
          printf("r");
        else
          printf("-");
        if ( (mode & S_IWUSR) && (mode & S_IWRITE))
          printf("w");
        else
          printf("-");
        if ( (mode & S_IXUSR) && (mode & S_IEXEC) )
          printf("x");
        else
          printf("-");
	//These next three if statements print the Group's perissions
        if ((mode & S_IRGRP) &&  (mode & S_IREAD))
          printf("r");
        else
          printf("-");
        if((mode & S_IWGRP) && (mode & S_IWRITE ))
          printf("w");
        else
          printf("-");
        if ( (mode & S_IXGRP) && (mode & S_IEXEC))
          printf("x");
        else
          printf("-");
	//These final three if statements print the Other permissions
        if ((mode & S_IROTH) && (mode & S_IREAD))
          printf("r");
        else
          printf("-");
        if ((mode & S_IWOTH ) && (mode & S_IWRITE))
          printf("w");
        else
          printf("-");
        if ((mode & S_IXOTH)  && (mode & S_IEXEC))
          printf("x] \n");
	else
	  printf("-] \n");
}

int list_directory(const char *current_path, int spacing)
{
	DIR * dir = opendir((char*)current_path); //Opens the current directory
	if (dir == NULL)  //Detects failure to open directory
	{
	    perror("opendir() Failed");
	    return EXIT_FAILURE;
	}

	struct dirent *current; 
	int rc = 0;

	//readdir sets current = to the items in the directory one at a time until none are found (NULL)
	while ((current = readdir(dir)) != NULL)
	{
	    struct stat buf;
	    char new[10000]; //Used to create the complete path name
	
	    //Next few lines create the complete path name required for Stat()
	    strcpy(new, current_path);
	    strcat(new, "/");
	    strcat(new, (char*)current->d_name);
	    rc = stat(new, &buf);
	    //rc = stat(current -> d_name, &buf);
	    if (rc < 0) //Detects stat() failure
	    {
		perror("stat() Failed");
		return EXIT_FAILURE;
	    }
	    else
	    {
		if (S_ISREG(buf.st_mode)) //If directory entry is a regular file
		{
		  if (special == 0) //If argc == 1, prints all regular files
		{
		    int i;
                    for(i = 0; i < spacing * 4; i++) //Prints the correct spacing for subdirectories
		    {
                    printf(" ");
                    }

                printf( "%s ", current->d_name );
                printf( "(%lld bytes) [-", (long long) buf.st_size ); //Ends in [- to start off printing the permissions
		print_perm(buf.st_mode);  //I used a function to print the permissions using buf.st_mode as an arguement
		}
		else //If argc != 1, only reg files printed are those equivalent to argv[1]
		{
		  if(strcmp(sfile, current->d_name) == 0) //Compares the current file name with argv[1]
		   {
		    int i;
                    for(i = 0; i < spacing * 4; i++) //Prints the correct spacing for subdirectories
		    {
                    printf(" ");
                    }

                printf( "%s ", current->d_name );
		printf( "(%lld bytes) [-", (long long) buf.st_size );
		print_perm(buf.st_mode);
		}
		}
            }
	    if (S_ISDIR(buf.st_mode)) //If current directory item is another directory (subdirectory)
            {
                if ( strcmp(".", current->d_name) == 0 || strcmp("..", current->d_name) == 0 )
                    continue; //Used in skipping the . and .. directories

                int i;
                for(i = 0; i < spacing * 4; i++) //Prints the correct spacing for subdirectories
		{
                    printf(" ");
                }

		//The next few lines build the full path name to the subdirectory
                printf( "%s (subdirectory) ", current->d_name );
		printf( "[d");  //Prints the d part of the permissions to signify that it is a directory
		print_perm(buf.st_mode); //Prints directory permissions
		int next_spacing = spacing + 1;
		char destFile[10000];
		strcpy(destFile, current_path);
		strcat(destFile, "/");
		strcat(destFile, (char*)current->d_name);
		//Recursively Explores the subdirectory
		list_directory(destFile, next_spacing);
            }
	}
}

 //closes the open directory
 closedir(dir);
 return EXIT_SUCCESS;
}


int main (int argc, char **argv)
{
   if (argc == 1) //For part 1, when no cmd line argument is given
   {
	list_directory(".", 0);
    }
    else
      {
	 //When argc == 2
	 special = 1; //Updates the global variable special, used in part 2
	 strcpy(sfile, argv[1]); //copies the cmd line agrument to an array of chars. Used in strcmp later
	 list_directory(".", 0);
      }

return 0;
}

