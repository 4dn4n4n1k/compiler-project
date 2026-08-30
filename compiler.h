#ifndef COMPILER_H
#define COMPILER_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;


// =============================
// Symbol Table
// =============================

struct Symbol
{
    string name;
    string type;
    string category;
};


// =============================
// Compilation Result
// =============================

struct CompilationResult
{
    vector<string> tokens;
    vector<string> classifications;
    vector<Symbol> symbolTable;
    vector<string> errors;

    bool success;
};


// =============================
// Lexical Analysis Functions
// =============================

bool isKeyword(string word);

bool isIdentifier(string word);

bool isNumber(string word);

vector<string> tokenize(string input);


// =============================
// Compiler Function
// =============================

CompilationResult compile(string sourceCode);


#endif
