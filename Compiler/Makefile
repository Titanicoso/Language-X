all:
	flex lex.l
	yacc -dv yacc.y
	gcc -o parser y.tab.c lex.yy.c nodes.c ../Utilities/translateNodes.c ../Utilities/typeChecks.c -ly
	rm lex.yy.c y.tab.c y.tab.h y.output

