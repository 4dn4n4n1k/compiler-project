#include <iostream>
#include "compiler.h"

using namespace std;

int main()
{
    string sourceCode;

    cout << "Enter your code: ";
    getline(cin, sourceCode);

    CompilationResult result = compile(sourceCode);

    cout << "\n===== TOKENS =====\n";

    for(string token : result.tokens)
        cout << token << endl;

    cout << "\n===== CLASSIFICATION =====\n";

    for(string c : result.classifications)
        cout << c << endl;

    cout << "\n===== SYMBOL TABLE =====\n";

    for(Symbol s : result.symbolTable)
    {
        cout << "Name: " << s.name
             << " | Type: " << s.type
             << " | Category: " << s.category
             << endl;
    }

    cout << "\n===== RESULT =====\n";

    if(result.success)
        cout << "Compilation Successful!" << endl;
    else
        cout << "Compilation Failed!" << endl;

    return 0;
}
