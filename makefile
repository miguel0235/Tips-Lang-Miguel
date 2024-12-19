# variables used in the following rules
LEX      = flex
CXX      = g++
CC       = gcc
RM       = rm
# -g generate debug information for gdb
# -Wno-c++11-extensions silence the c++11 error warnings
# -std=c++11 assert that we are using c++11
CXXFLAGS = -g
CXXFLAGS = -g -Wno-c++11-extensions
CXXFLAGS = -g -std=c++11
CCFLAGS  = -g
# don't ever remove these file types
.PRECIOUS = *.l *.h *.cpp [Mm]akefile

parse: lex.yy.o driver.o parser.o node.o
	$(CXX) $(CXXFLAGS) -o parse lex.yy.o driver.o parser.o node.o

#     -o flag specifies the output file
#
#     The above rule could be written with macros as
#        $(CXX) $(CXXFLAGS) -o $@ $^

driver.o: driver.cpp parser.h node.h lexer.h
	$(CXX) $(CXXFLAGS) -o driver.o -c driver.cpp
#      -c flag specifies stop after compiling, do not link

parser.o: parser.cpp parser.h lexer.h node.h
	$(CXX) $(CXXFLAGS) -o parser.o -c parser.cpp

node.o: node.cpp lexer.h
	$(CXX) $(CXXFLAGS) -o node.o -c node.cpp

lex.yy.o: lex.yy.c lexer.h
	$(CC) $(CCFLAGS) -o lex.yy.o -c lex.yy.c

lex.yy.c: rules.l lexer.h
	$(LEX) -o lex.yy.c rules.l

clean: 
	$(RM) *.o lex.yy.c parse.exe
#   delete all generated files	

ring:
	ls $(.PRECIOUS)
#   tell us what is precious here
