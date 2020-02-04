# Compiler
  Semester project on making a compiler, starting with Lexical Analysis, then Syntax Anaylsis, Intermediate Code Generation, Optimization, and finally Object Code Generation.

  Written in C++, the compilier is written to compile an assigned sudo-code defined as:
  
TOKENS			Example Lexemes

KEYWORDS  	=	int, float, bool, true, false, if, else, then, endif, while, whileend, do, doend, for, forend, input, output,
              and, or, not
              
IDENTIFIERS =	legal identifiers must start with alphabetic character follow by digits, alpha, or $

SEPARATORS 	=	'(){}[],.:; sp(space)

OPERATORS 	=	*+-=/><%

Additional examples:

Valid IDENTIFIERS	:  	num, num1, large$, num$1, num2, num2$, a9, ab, ab2, etc...

Valid Numbers		:	integers whole numbers (1,2,3,...) and reals floats (5.0, 0.9, 1.75, ...)

Valid Block Comments	:	!  this is a block comment to be ignored by the Lexical Analyzer !
