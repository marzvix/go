/*
NAME
  go - move (a.k.a chdir) to my c projects directory.

SYNOPSIS
  go [project-directory]

DESCRIPTION
  go is an utility to send (chdir) to some dir where
  project lives

   - usuallly under bench or 
   - some project itself, under bench

  go read its own configuration
  
  go will test project-directory exists.

  if exists. Chdir there.

  Otherwise, creates a brand new dir project containing
  with some dummy files (hello) to start a new c project.
    
  - at command "go foo", it will be created a dir underneath basepath (/cygdrive/h/bench/src) a dir with "foo" name

  - that dir "foo" will contain:
    - README.md (with some data - date, author, etc)
    - Makefile (with commands - all, run, clean and labels to build foo)
    - foo.t2t: skelton of man page to be compiled with txt2tags (https://txt2tags.org)
    - .gitignore - contain initial files for git ignore (duh!)
    - foo.c (where foo is the name choose for project dir)
    - foo.h

DEPENDENCIES
- ./bash_aliases: have to contain following line:

    alias go='function _go(){ cd "`/usr/local/bin/go $1`"; };_go'

EXAMPLE

$ ./go foo
/cygdrive/h/bench/src/foo not exist. Create? (y/n)


WISHLIST
- prepare relocatable configuration under .config
- /etc/config

BUGS
- go presupposes existence of ~/.config/go/go.conf
	       
AUTHOR
  2019,20 Written by MarzVix (marzvix@gmail.com)
  Created: qui, 26 de dez de 2019 11:02:36
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <time.h>
#include <fcntl.h>
#include <libgen.h>

#include "toolkit.h"
#include "filetools.h"
#include "txtools.h"

#define MAX_PAD 64

static void create_readme(void);
static void create_gitignore(void);
static void create_header(void);
static void create_pilot(void);
static void create_makefile(void);

char pad[MAX_PAD];
FILE *fp;

static struct {
  char *prj;
  char *goal;
  char *bench;
  char *author;
} conf =
  {
   NULL,
   NULL,
   "/cygdrive/h/bench",
   "MarzVix (marzvix@gmail.com)"
  };

int main(int argc, char *argv[])
{
  strcpy(pad, conf.bench);  

  if (argc > 1)  {        /* default - go to bench root */
    conf.prj = argv[1];
    strcpy(pad, adddirname(pad, conf.prj));

    if(!exist(pad)) {        /* if dir not exist */
      if (argc > 2) /* goal defined */
	conf.goal = argv[2];
      /* create bench [+ dir] */
      create_dir(pad);
      create_readme();
      create_gitignore();
      create_header();
      create_pilot();
      create_makefile();
      strcpy(pad, conf.bench);  
      strcpy(pad, adddirname(pad, argv[1]));
    }
  }

  /* string to bench [+ dir] */
  puts(pad);
  return EXIT_SUCCESS;
}

static void create_readme(void)
{
  time_t result = time(NULL);
  
  /* create README.md */
  sprintf(pad, "%s/%s/README.md", conf.bench, conf.prj);
  fp = fopen(pad,"w");
  /* insert header in ROADMAP */
  fprintf(fp, "Project: %s\n", conf.prj);
  fprintf(fp, "Goal: %s\n", conf.goal);
  fprintf(fp, "Author: %s\n", conf.author);
  fprintf(fp, "%s", asctime(localtime(&result)));
  fclose(fp);
}

static char *ignore[] =
  {
   "*~",
   "#*",
   ".#*",
   "*.o",
   "*.obj",
   "*.map",
   "*.asm",
   "*.exe",
   "*.code",
   "*.core",
   "*.stackdump",
   NULL
  };

static void create_gitignore(void)
{
  char **ig = ignore;

  /* create .gitignore */
  sprintf(pad, "%s/%s/.gitignore", conf.bench, conf.prj);
  fp = fopen(pad,"w");

  /* insert initial lines in gitignore */
  while(*ig)
    fprintf(fp, "\\*\\*\\/%s\n", *ig++);
  fclose(fp);
}
  
static void create_header(void)
{
  char *h = (char *) pad;

  /* create pilot header */
  sprintf(pad, "%s/%s/%s.h", conf.bench, conf.prj, conf.prj);
  fp = fopen(pad,"w");
  txtupper(h, conf.prj);
  fprintf(fp, "#ifndef _%s_H\n", h);
  fprintf(fp, "#define _%s_H\n", h);
  fprintf(fp, "void %s(int, char **)\n", conf.prj);
  fprintf(fp, "#endif /* _%s_H */\n", h);
  fclose(fp);
}

static void create_pilot(void)
{
  /* create pilot header */
  sprintf(pad, "%s/%s/%s.c", conf.bench, conf.prj, conf.prj);
  fp = fopen(pad,"w");
  /* create pilot c */
  fprintf(fp, "#include <stdio.h>\n");
  fprintf(fp, "#include <stdlib.h>\n");
  fprintf(fp, "#include <strings.h>\n");
  fprintf(fp, "#include <libgen.h>\n\n");

  fprintf(fp, "void %s(int c, char **v) {\n", conf.prj);
  fprintf(fp, "\n\tint i;\n");
  fprintf(fp, "\n\tprintf(\"Hello, %%s!\\n\", basename(v[0]));\n");
  fprintf(fp, "\tfor (i=0; i < c; i++)\n");
  fprintf(fp, "\t\tprintf(\"Hello, argc[%%i] %%s!\\n\", i, v[i]);\n}\n\n");

  /* insert main in pilot */
  fprintf(fp, "int main(int argc, char *argv[])\n");
  fprintf(fp, "{\n");
  fprintf(fp, "\t%s(argc, argv);\n", conf.prj );
  fprintf(fp, "\treturn 0;\n}");
  fclose(fp);
}

static void create_makefile(void)
{
  static char pad[MAX_PAD];
  char **rm = ignore;

  /* create initial Makefile */
  sprintf(pad, "%s/%s/Makefile", conf.bench, conf.prj);
  fp = fopen(pad,"w");

  /* insert clean section in Makefile */
  fprintf(fp, "# Makefile for %s\n\n", conf.prj);
  fprintf(fp, "CC = gcc\n");
  fprintf(fp, "CFLAGS = -g3 -O0 -Wall -Wextra\n\n");
  fprintf(fp, "ARGS =\n\n");
  fprintf(fp, "INSTALDIR = /usr/local/bin\n\n");
  fprintf(fp, "all: %s\n\n",conf.prj);
  fprintf(fp, "run:\n\t./%s $(ARGS)\n\n",conf.prj);
  fprintf(fp, "install: %s\n\tcp ./%s $(INSTALDIR)/\n\n",
	  conf.prj, conf.prj);
  fprintf(fp, "uninstall: $(INSTALDIR)/%s\n\trm -i $(INSTALDIR)/%s\n\n",conf.prj, conf.prj);
  fprintf(fp, "%s: %s.o\n\n",conf.prj,conf.prj);
  fprintf(fp, "%s.o: %s.c\n\n",conf.prj,conf.prj);
  fprintf(fp, ".PHONY: clean\nclean:\n");
  while(*rm)
    fprintf(fp, "\t$(RM) %s\n", *rm++);
  fclose(fp);
}
