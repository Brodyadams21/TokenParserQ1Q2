#include <iostream>
#include <fstream>
#include <cstdlib> 
using namespace std;

ifstream infp;
enum Tokens { A, B, C, ENDFILE=80, UNKNOWN=99 };
			
const int SIZE = 100;
Tokens nextToken;
string lexeme;
char nextChar;

int errors = 0; // counter for error messages
int line = 1;   // variable to keep track of the line number from the source code

/******************************************************/
/*   Helping function to display the token as a string */
void prt (Tokens nt) {
	switch (nt) {
		// Type your code here
		case A: cout << "<A>"; break;
		case B: cout << "<B>"; break;
		case C: cout << "<C>"; break;
		case ENDFILE: cout << "<ENDFILE>"; break;
		case UNKNOWN: cout << "<UNKNOWN>"; break;
	}
};

/* errMsg - function to display the error message with the line number of the error detected. */
void errMsg (string msg) {
	cout << "Error at line: " << line << ": " << msg << endl;
	errors++;
	
}

/*****************************************************/
/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar(char nextChar) {
	lexeme += nextChar;
	if (lexeme.size() > 99) {
		errMsg ("Lexeme size exceeds 100 characters");
		exit(0);
	}
}

char getChar() {
	char ch = infp.get();
	if (ch == '\n') {		
		line++;
		
	}
	return ch;
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it
 returns a non-whitespace character */
char getNonSpaceChar() {
	char ch = ' ';
	while (isspace(ch)) {
		ch = getChar();
	}
	return ch;
}

/*******************************************************************
LookupKeyword - a simple lookup code for keywords in the language: */
Tokens lookupKeywords (string lexeme) {
	// return keyword token
	// Type your code here
	Tokens token = UNKNOWN;
	if (lexeme == "a") token = A;
	else if (lexeme == "b") token = B;
	else if (lexeme == "c") token = C;
	return token;
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic 
 expressions */
Tokens tokenizer() {
	Tokens nextToken = ENDFILE;
	lexeme = "";
	if (isspace(nextChar)) nextChar = getNonSpaceChar();
	
	if (nextChar == 'a') {
		// Type your code here
		addChar(nextChar);
        nextToken = A;
        nextChar = getChar();  // Move to the next character
        while (nextChar == 'a') {
            addChar(nextChar);
            nextChar = getChar();
		}
	}
	else if (nextChar == 'b') {
		// Type your code here
		addChar(nextChar);
        nextToken = B;
        nextChar = getChar();
        while (nextChar == 'b') {
            addChar(nextChar);
            nextChar = getChar();
		}
	}
	else if (nextChar == 'c') {
		// Type your code here
		addChar(nextChar);
        nextToken = C;
        nextChar = getChar();
	}
	else if (nextChar == EOF) {
			nextToken = ENDFILE;
			lexeme = "EOF";

	}
  
  return nextToken;
}

/*************************************************/
/* <C> -> c
*/
Tokens CParser (Tokens nextToken) {
	// Type your code here
	if (nextToken == C) {
        cout << "Parsed C" << endl;
        nextToken = tokenizer();  // Move to the next token
    } else {
        errMsg("Expected 'c', but found something else.");
    }
	return nextToken;
}

/*************************************************/
/* <B> -> b<B> | b
*/
Tokens BParser (Tokens nextToken) {
	// Type your code here
	if (nextToken == B) {
        cout << "Parsed B" << endl;
        nextToken = tokenizer();  // Move to the next token
        if (nextToken == B) {
            nextToken = BParser(nextToken);  // Recursive call for b<B>
        }
    } else {
        errMsg("Expected 'b', but found something else.");
    }
    return nextToken;
}

/***************************************************/
/* <A> -> a<A> | a
*/
Tokens AParser (Tokens nextToken) {
	// Type your code here
	 if (nextToken == A) {
        cout << "Parsed A" << endl;
        nextToken = tokenizer();  // Move to the next token
        if (nextToken == A) {
            nextToken = AParser(nextToken);  // Recursive call for a<A>
        }
    } else {
        errMsg("Expected 'a', but found something else.");
    }
    return nextToken;
}

/********************************************/
/* stmt - statement parser by the rule:
   <stmt> -> <A><C><B> | <A>
*/
Tokens S (Tokens nextToken) {
	// Type your code here
	nextToken = AParser(nextToken);  // Always start by parsing A
    
    // Check if the next token is C, which indicates we should parse <C><B>
    if (nextToken == C) {
        nextToken = CParser(nextToken);  // Parse C
        nextToken = BParser(nextToken);  // Parse B
    }

    return nextToken;  // Return the current token to continue parsing
}

/******************************************* End of Syntax Analyzer (parser) **************************************************************************/

/******************************************************/
/* main driver */
int main() {
	infp.open(".\\prg.txt");
	if (!infp) {
		cout << "ERROR - cannot open file \n";
		errors++;
	}
	else {
		nextChar = ' ';
		Tokens nextToken = tokenizer();
		do {
			nextToken = S (nextToken);
			if (errors > 10) break;
		} while (nextToken != ENDFILE);
	}
	cout << "Total number of errors: " << errors << endl;
	return 0;
}
