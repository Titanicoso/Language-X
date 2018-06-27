./Compiler/parser < $1;
gcc -o executable.out ./Compiler/compiled.c ./Utilities/linkedList.c;
rm ./Compiler/compiled.c;
