/* go.c: utility to send to some dir
         - my projects bench or 
	 - some project itself, under bench

Goal: Intent to be used with aliases (.bash_alises)

Use: go [some basename of project]

if "some basename" not exist, ask me if want to create
if not, goto the bench

Author: marez
Created: qui, 26 de dez de 2019 11:02:36
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>
#include <time.h>

void prep_repo(char *prj, char *togo)
{
  FILE *f;
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  char buf[64];
  
  /* create README.md */
  strcpy(buf, togo);
  strcat(buf, "/README.md");
  f = fopen(buf,"w");
  /* insert header in ROADMAP */
  fprintf(f, "Project: %s\n", prj);
  fprintf(f, "Goal: \n");
  fprintf(f, "Author: marez <mace.murez@gmail.com\n");
  strftime(buf, 64, "Created: %c\n", &tm);
  fprintf(f,  buf);
  fclose(f);
  
  /* create .gitignore */
  strcpy(buf, togo);
  strcat(buf, "/.gitignore");
  f = fopen(buf,"w");
  /* insert initial lines in gitignore */
  fprintf(f, "**/*.code\n");
  fprintf(f, "**/#*\n");
  fprintf(f, "**/.#*\n");
  fprintf(f, "**/*.o\n");
  fprintf(f, "**/*.obj\n");
  fprintf(f, "**/*.map\n");
  fprintf(f, "**/*.asm\n");
  fprintf(f, "**/*.exe\n");
  fprintf(f, "**/*.stackdump\n");
  fprintf(f, "**/*.core\n");
  fprintf(f, "**/*~\n");
  fclose(f);

  /* create initial Makefile */
  strcpy(buf, togo);
  strcat(buf, "/Makefile");
  f = fopen(buf,"w");
  /* insert clean section in Makefile */
  fprintf(f, "# Makefile for %s\n\n", prj);
  fprintf(f, "CC = gcc\n");
  fprintf(f, "CFLAGS = -g3 -O0 -Wall -Wextra\n\n");
  fprintf(f, "ARGS =\n\n");
  fprintf(f, "INSTALDIR = /usr/local/bin\n\n");
  fprintf(f, "all: %s\n\n",prj);
  fprintf(f, "run:\n\t./%s $(ARGS)\n\n",prj);
  fprintf(f, "install: %s\n\tcp ./%s $(INSTALDIR)/\n\n",prj,prj);
  fprintf(f, "uninstall: $(INSTALDIR)/%s\n\trm -i $(INSTALDIR)/%s\n\n",prj,prj);
  fprintf(f, "%s: %s.o\n\n",prj,prj);
  fprintf(f, "%s.o: %s.c\n\n",prj,prj);
  fprintf(f, ".PHONY: clean\nclean:\n");
  fprintf(f, "\t$(RM) *.o\n");
  fprintf(f, "\t$(RM) *.exe\n");
  fprintf(f, "\t$(RM) *.asm\n");
  fprintf(f, "\t$(RM) *.map\n");
  fprintf(f, "\t$(RM) *.stackdump\n");
  fprintf(f, "\t$(RM) *.core\n");
  fclose(f);

  /* create pilot */
  strcpy(buf, togo);
  strcat(buf, "/");
  strcat(buf, prj);
  strcat(buf, ".c");
  f = fopen(buf,"w");
  /* insert main in pilot */
  fprintf(f, "#include <stdio.h>\n");
  fprintf(f, "#include <stdlib.h>\n");
  fprintf(f, "#include <strings.h>\n\n");
  fprintf(f, "int main(int argc, char *argv[])\n");
  fprintf(f, "{\n\tint i;\n");
  fprintf(f, "\n\tif(argc > 1)\n");
  fprintf(f, "\t\tfor (i=0; i < argc; i++)\n");
  fprintf(f, "\t\t\tprintf(\"Hello, %%s!\\n\", argv[i]);\n");
  fprintf(f, "\telse\n");
  fprintf(f, "\t\tprintf(\"Hello, %%s!\\n\", argv[0]);\n");
  fprintf(f, "\texit(0);\n}");
  fclose(f);

  fprintf(stderr, "%s created.\n", togo);
}

char *get_bench(void)
{
  return "/cygdrive/h/bench/src/";
}

int main(int argc, char *argv[])
{
  struct stat sb;
  const char *bench = get_bench();
  char togo[64];

  strcpy(togo, bench);
  
  if (argc > 1)	{
    strcat(togo, argv[1]); /* add dir name to path where to go 
			      and test if dir exist */
    if (stat(togo, &sb) == 0) {  /* path exist ? */
      if (!(S_ISDIR(sb.st_mode)))  /* exist, but is not a dir */
	strcpy(togo,"."); /* stays the same */
    }
    else {
      /* dir not exist */
      /* inform dir not exist and ask if want to create */
      fprintf(stderr, "%s not exist. Create? (y/n)", togo);
      if ('Y' == toupper((char) getchar_unlocked())) { /* yeap */
	/* create */
	int status;
	status = mkdir(togo, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (status == 0) 
	  prep_repo(argv[1], togo);
	else {
	  fprintf(stderr, "%s not created.\n", togo);
	  strcpy(togo,"."); /* stays at . */
	  exit(1);
	}
      }
      else  /* nope */
	strcpy(togo,"."); /* stays at . */
    }
  }
  /* chdir to bench + dir to go */
  fprintf(stderr, "chdir to %s\n", togo);
  printf("%s", togo);
  exit(0);
}
