Project: go
Goal: move (chdir) to my c project

Depends: ./bash_aliases
         
Author: marez <mace.murez@gmail.com

Created: Thu Dec 26 19:04:10 2019

Notes:

- ./bash_aliases have to contain following line:

    alias go='function _go(){ cd "`/usr/local/bin/go $1`"; };_go'

- executable of go is in /usr/local/bin (which is better is in path)

- at command "go xxx", it will be created a dir bellow basepath (/home/marez/bench/src)
  a dir with "xxx" name

- that dir "xxx" will contain:
  - README.md (with some data - date, author, etc)
  - Makefile (with commands - all, run, clean and labels to build xxx)
  - .gitignore - contain initial files for git ignore (duh!)


* qui,  9 de jan de 2020 23:10:47

- split txt, files & dirs and toolkit (others) function in a lib
  libtoolkit and this project have to be linked to libtoolkit

- reestructured

TODO:

- prepare relocatable configuration
- .config
- /etc/config
