./Compiler/parser < $1;
if [[ $? -ne 2 ]] ; then
  gcc -o executable.out ./Compiler/compiled.c ./Utilities/linkedList.c;
fi
rm ./Compiler/compiled.c;
