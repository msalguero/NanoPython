bison --defines=tokens.h -o expr_parser.cpp expr.y
flex -o expr_lexer.cpp expr.l
g++ -std=c++11 -c -o tree.o tree.cpp
g++ -std=c++11 -c -o expr_lexer.o expr_lexer.cpp
g++ -std=c++11 -c -o expr_parser.o expr_parser.cpp

g++ -std=c++11 -o expr expr_lexer.o expr_parser.o tree.o