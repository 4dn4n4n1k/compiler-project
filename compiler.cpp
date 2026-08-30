#include "compiler.h"
#include <bits/stdc++.h>

using namespace std;

bool isKeyword(string word)
{
    string keywords[] =
    {
        "int", "float", "double", "char",
        "bool", "if", "else", "for",
        "while", "return", "class"
    };

    for(string keyword : keywords)
    {
        if(word == keyword)
            return true;
    }

    return false;
}

bool isIdentifier(string word)
{
    if(word.empty() ||
       !(isalpha(word[0]) || word[0] == '_'))
        return false;

    for(int i = 1; i < word.length(); i++)
    {
        if(!(isalnum(word[i]) || word[i] == '_'))
            return false;
    }

    return !isKeyword(word);
}

bool isNumber(string word)
{
    if(word.empty())
        return false;

    for(char c : word)
    {
        if(!isdigit(c))
            return false;
    }

    return true;
}

vector<string> tokenize(string input)
{
    vector<string> tokens;

    int i = 0;

    while(i < input.length())
    {
        if(input[i] == ' ' ||
           input[i] == '\t' ||
           input[i] == '\n' ||
           input[i] == '\r')
        {
            i++;
        }

        else if(isalpha(input[i]) || input[i] == '_')
        {
            string word = "";

            while(i < input.length() &&
                  (isalnum(input[i]) || input[i] == '_'))
            {
                word += input[i++];
            }

            tokens.push_back(word);
        }

        else if(isdigit(input[i]))
        {
            string number = "";

            while(i < input.length() &&
                  isdigit(input[i]))
            {
                number += input[i++];
            }

            tokens.push_back(number);
        }

        else if(input[i] == '+' ||
                input[i] == '-' ||
                input[i] == '*' ||
                input[i] == '/' ||
                input[i] == '=')
        {
            tokens.push_back(string(1, input[i]));
            i++;
        }

        else if(input[i] == ';' ||
                input[i] == ',' ||
                input[i] == '(' ||
                input[i] == ')' ||
                input[i] == '{' ||
                input[i] == '}')
        {
            tokens.push_back(string(1, input[i]));
            i++;
        }

        else
        {
            return {};
        }
    }

    return tokens;
}


// =========================
// Parser
// =========================

class Parser
{
    vector<string> tokens;
    vector<Symbol>& symbolTable;

    // NEW:
    // Stores errors so GUI can display them
    vector<string>& errors;

    int index;

public:

    // CHANGED:
    // Added errors parameter
    Parser(
        vector<string> t,
        vector<Symbol>& s,
        vector<string>& e
    )
        : tokens(t),
          symbolTable(s),
          errors(e),
          index(0)
    {
    }

    bool isDeclared(string name)
    {
        for(Symbol s : symbolTable)
        {
            if(s.name == name)
                return true;
        }

        return false;
    }

    // CHANGED:
    // Error is now stored instead of printed with cout
    void error(string msg)
    {
        errors.push_back(
            "Line " +
            to_string(index + 1) +
            ": Error: " +
            msg
        );
    }

    bool parseExpression()
    {
        bool valid = false;

        while(index < tokens.size())
        {
            if(isNumber(tokens[index]))
            {
                valid = true;
            }

            else if(isIdentifier(tokens[index]))
            {
                if(!isDeclared(tokens[index]))
                {
                    error(
                        "Undefined variable " +
                        tokens[index]
                    );
                }

                valid = true;
            }

            else if(tokens[index] == "+" ||
                    tokens[index] == "-" ||
                    tokens[index] == "*" ||
                    tokens[index] == "/")
            {
                index++;
                continue;
            }

            else
            {
                break;
            }

            index++;
        }

        return valid;
    }

    bool parseStatement()
    {
        if(index >= tokens.size())
            return true;

        // Variable declaration
        if(isKeyword(tokens[index]) &&
           (tokens[index] == "int" ||
            tokens[index] == "float" ||
            tokens[index] == "double" ||
            tokens[index] == "char" ||
            tokens[index] == "bool"))
        {
            index++;

            if(index >= tokens.size() ||
               !isIdentifier(tokens[index]))
            {
                error("Invalid variable declaration");
                return false;
            }

            index++;

            if(index < tokens.size() &&
               tokens[index] == "=")
            {
                index++;

                parseExpression();
            }

            if(index >= tokens.size() ||
               tokens[index] != ";")
            {
                error("Missing Semicolon");
                return false;
            }

            index++;

            return true;
        }

        // Assignment
        if(isIdentifier(tokens[index]))
        {
            string var = tokens[index];

            if(!isDeclared(var))
            {
                error(
                    "Undefined variable " +
                    var
                );
            }

            index++;

            if(index < tokens.size() &&
               tokens[index] == "=")
            {
                index++;

                parseExpression();
            }

            if(index >= tokens.size() ||
               tokens[index] != ";")
            {
                error("Missing Semicolon");
                return false;
            }

            index++;

            return true;
        }

        error("Invalid syntax");

        index++;

        return false;
    }

    bool parse()
    {
        bool result = true;

        while(index < tokens.size())
        {
            if(!parseStatement())
            {
                result = false;
            }
        }

        return result;
    }
};


// =========================
// Compile
// =========================

CompilationResult compile(string sourceCode)
{
    CompilationResult result;

    result.success = false;


    // =========================
    // 1. Tokenization
    // =========================

    result.tokens = tokenize(sourceCode);

    if(result.tokens.empty())
    {
        result.errors.push_back(
            "Lexical Error: Invalid input."
        );

        return result;
    }


    // =========================
    // 2. Token Classification
    // =========================

    for(string token : result.tokens)
    {
        if(isKeyword(token))
        {
            result.classifications.push_back(
                token + " -> Keyword"
            );
        }

        else if(isIdentifier(token))
        {
            result.classifications.push_back(
                token + " -> Identifier"
            );
        }

        else if(isNumber(token))
        {
            result.classifications.push_back(
                token + " -> Number"
            );
        }

        else if(token == "+" ||
                token == "-" ||
                token == "*" ||
                token == "/" ||
                token == "=")
        {
            result.classifications.push_back(
                token + " -> Operator"
            );
        }

        else
        {
            result.classifications.push_back(
                token + " -> Symbol"
            );
        }
    }


    // =========================
    // 3. Symbol Table
    // =========================

    string currentType = "";

    for(string token : result.tokens)
    {
        if(token == "int" ||
           token == "float" ||
           token == "double" ||
           token == "char" ||
           token == "bool")
        {
            currentType = token;
        }

        else if(isIdentifier(token) &&
                currentType != "")
        {
            result.symbolTable.push_back(
                {token, currentType, "Variable"}
            );

            currentType = "";
        }
    }


    // =========================
    // 4. Syntax Analysis
    // =========================

    // CHANGED:
    // Pass result.errors to Parser
    Parser parser(
        result.tokens,
        result.symbolTable,
        result.errors
    );

    result.success = parser.parse();


    // =========================
    // 5. Final Result
    // =========================

    // If there are errors, compilation must fail
    if(!result.errors.empty())
    {
        result.success = false;
    }

    return result;
}
