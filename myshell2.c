#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

/* a mini-shell
 * read a command line and exec the commands
 * with help on parsing from: https://www.codingame.com/playgrounds/14213/how-to-play-with-strings-in-c/string-split
 *
 * skeleton - you must fill in the fork/exec calls
 */

void
main()
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    char *prompt = "$ ";
    char delim[] = " \n";
    char *cptr;
    char *words[8];		// BFMI for holding args
    int nargs=0;
    int i;
    int pid;

    printf("%s", prompt);
    while ((nread = getline(&line, &len, stdin)) != -1) {
	// line is length: nread
	// debug: fwrite(line, nread, 1, stdout);
	// split line into words:

	words[0] = cptr = strtok(line, delim);
	for(nargs=1; (cptr != NULL) ; nargs++)
	{
		words[nargs] = cptr = strtok(NULL, delim);
	}

	// code goes here to fork/exec the command entered via stdin
	// using the words in the words[] array as the arg list for execl
	pid = fork();
	if(pid)
	{
		waitpid(-1, 0, 0);		
	}else{
		execvp(words[0],words, NULL);
	}

        for(i=0; i<nargs; i++) {
	    printf("words[%d] = %s\n", i, words[i]);
	}


	// get ready to loop back around for another line of input
        free(line);
	line = NULL;
	printf("%s", prompt);
    }
} // main
