#include <windows.h>
#include <string>
#include <sstream>
#include "compiler.h"

using namespace std;

// =============================
// Global GUI Controls
// =============================

HWND hEditor;
HWND hOutput;
HWND hCompileButton;
HWND hClearButton;
HWND hStatus;

HFONT hFont;
HFONT hTitleFont;


// =============================
// Helper Function
// =============================

void SetControlFont(HWND control, HFONT font)
{
    SendMessage(
        control,
        WM_SETFONT,
        (WPARAM)font,
        TRUE
    );
}


// =============================
// Compile Function
// =============================

void CompileCode()
{
    // Get source code length
    int length = GetWindowTextLength(hEditor);

    if(length == 0)
    {
        SetWindowText(
            hOutput,
            "Please enter some source code."
        );

        SetWindowText(
            hStatus,
            "Status: No source code"
        );

        return;
    }


    // Get source code
    string sourceCode(length, '\0');

    GetWindowText(
        hEditor,
        &sourceCode[0],
        length + 1
    );


    // Call existing compiler
    CompilationResult result =
        compile(sourceCode);


    stringstream output;


    // =============================
    // TOKENS
    // =============================

    output << "===== TOKENS =====\r\n\r\n";

    if(result.tokens.empty())
    {
        output << "No tokens found.\r\n";
    }
    else
    {
        for(string token : result.tokens)
        {
            output << token << "\r\n";
        }
    }


    // =============================
    // CLASSIFICATION
    // =============================

    output << "\r\n";
    output << "===== TOKEN CLASSIFICATION =====\r\n\r\n";

    for(string classification :
        result.classifications)
    {
        output << classification << "\r\n";
    }


    // =============================
    // SYMBOL TABLE
    // =============================

    output << "\r\n";
    output << "===== SYMBOL TABLE =====\r\n\r\n";

    if(result.symbolTable.empty())
    {
        output << "No symbols found.\r\n";
    }
    else
    {
        for(Symbol s : result.symbolTable)
        {
            output << "Name: "
                   << s.name
                   << " | Type: "
                   << s.type
                   << " | Category: "
                   << s.category
                   << "\r\n";
        }
    }


    // =============================
    // ERRORS
    // =============================

    if(!result.errors.empty())
    {
        output << "\r\n";
        output << "===== ERRORS =====\r\n\r\n";

        for(string error :
            result.errors)
        {
            output << error << "\r\n";
        }
    }


    // =============================
    // FINAL RESULT
    // =============================

    output << "\r\n";
    output << "===== COMPILATION RESULT =====\r\n\r\n";

    if(result.success)
    {
        output << "Compilation Successful!";

        SetWindowText(
            hStatus,
            "Status: Compilation Successful"
        );
    }
    else
    {
        output << "Compilation Failed!";

        SetWindowText(
            hStatus,
            "Status: Compilation Failed"
        );
    }


    // Display output
    SetWindowText(
        hOutput,
        output.str().c_str()
    );
}


// =============================
// Clear Function
// =============================

void ClearAll()
{
    SetWindowText(
        hEditor,
        ""
    );

    SetWindowText(
        hOutput,
        ""
    );

    SetWindowText(
        hStatus,
        "Status: Ready"
    );
}


// =============================
// Window Procedure
// =============================

LRESULT CALLBACK WindowProcedure(
    HWND hwnd,
    UINT msg,
    WPARAM wp,
    LPARAM lp
)
{
    switch(msg)
    {

        // =============================
        // CREATE WINDOW
        // =============================

        case WM_CREATE:
        {
            // -------------------------
            // Fonts
            // -------------------------

            hFont = CreateFont(
                18,
                0,
                0,
                0,
                FW_NORMAL,
                FALSE,
                FALSE,
                FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY,
                FIXED_PITCH | FF_MODERN,
                "Consolas"
            );


            hTitleFont = CreateFont(
                26,
                0,
                0,
                0,
                FW_BOLD,
                FALSE,
                FALSE,
                FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_SWISS,
                "Arial"
            );


            // -------------------------
            // Title
            // -------------------------

            HWND hTitle = CreateWindow(
                "STATIC",
                "MINI COMPILER",
                WS_VISIBLE | WS_CHILD |
                SS_CENTER,
                20,
                15,
                840,
                40,
                hwnd,
                NULL,
                NULL,
                NULL
            );

            SetControlFont(
                hTitle,
                hTitleFont
            );


            // -------------------------
            // Source Code Label
            // -------------------------

            HWND hSourceLabel = CreateWindow(
                "STATIC",
                "SOURCE CODE",
                WS_VISIBLE | WS_CHILD,
                30,
                70,
                300,
                25,
                hwnd,
                NULL,
                NULL,
                NULL
            );

            SetControlFont(
                hSourceLabel,
                hTitleFont
            );


            // -------------------------
            // Source Editor
            // -------------------------

            hEditor = CreateWindowEx(
                WS_EX_CLIENTEDGE,
                "EDIT",
                "",
                WS_VISIBLE |
                WS_CHILD |
                ES_MULTILINE |
                ES_AUTOVSCROLL |
                ES_WANTRETURN |
                WS_VSCROLL |
                WS_HSCROLL,
                30,
                105,
                400,
                430,
                hwnd,
                NULL,
                NULL,
                NULL
            );

            SetControlFont(
                hEditor,
                hFont
            );


            // -------------------------
            // Output Label
            // -------------------------

            HWND hOutputLabel = CreateWindow(
                "STATIC",
                "COMPILATION OUTPUT",
                WS_VISIBLE | WS_CHILD,
                460,
                70,
                350,
                25,
                hwnd,
                NULL,
                NULL,
                NULL
            );

            SetControlFont(
                hOutputLabel,
                hTitleFont
            );


            // -------------------------
            // Output Box
            // -------------------------

            hOutput = CreateWindowEx(
                WS_EX_CLIENTEDGE,
                "EDIT",
                "",
                WS_VISIBLE |
                WS_CHILD |
                ES_MULTILINE |
                ES_AUTOVSCROLL |
                ES_READONLY |
                WS_VSCROLL |
                WS_HSCROLL,
                460,
                105,
                400,
                430,
                hwnd,
                NULL,
                NULL,
                NULL
            );

            SetControlFont(
                hOutput,
                hFont
            );


            // -------------------------
            // Compile Button
            // -------------------------

            hCompileButton = CreateWindow(
                "BUTTON",
                "COMPILE",
                WS_VISIBLE |
                WS_CHILD |
                BS_PUSHBUTTON,
                300,
                555,
                120,
                40,
                hwnd,
                (HMENU)1,
                NULL,
                NULL
            );

            SetControlFont(
                hCompileButton,
                hTitleFont
            );


            // -------------------------
            // Clear Button
            // -------------------------

            hClearButton = CreateWindow(
                "BUTTON",
                "CLEAR",
                WS_VISIBLE |
                WS_CHILD |
                BS_PUSHBUTTON,
                460,
                555,
                120,
                40,
                hwnd,
                (HMENU)2,
                NULL,
                NULL
            );

            SetControlFont(
                hClearButton,
                hTitleFont
            );


            // -------------------------
            // Status
            // -------------------------

            hStatus = CreateWindow(
                "STATIC",
                "Status: Ready",
                WS_VISIBLE |
                WS_CHILD |
                SS_LEFT,
                30,
                620,
                830,
                30,
                hwnd,
                NULL,
                NULL,
                NULL
            );

            SetControlFont(
                hStatus,
                hFont
            );

            break;
        }


        // =============================
        // BUTTON EVENTS
        // =============================

        case WM_COMMAND:
        {
            // Compile
            if(LOWORD(wp) == 1)
            {
                CompileCode();
            }

            // Clear
            else if(LOWORD(wp) == 2)
            {
                ClearAll();
            }

            break;
        }


        // =============================
        // CLOSE WINDOW
        // =============================

        case WM_DESTROY:
        {
            DeleteObject(hFont);
            DeleteObject(hTitleFont);

            PostQuitMessage(0);

            break;
        }


        default:
        {
            return DefWindowProc(
                hwnd,
                msg,
                wp,
                lp
            );
        }
    }

    return 0;
}


// =============================
// Windows Entry Point
// =============================

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
)
{
    const char CLASS_NAME[] =
        "MiniCompilerGUI";


    // -------------------------
    // Window Class
    // -------------------------

    WNDCLASS wc = {};

    wc.lpfnWndProc =
        WindowProcedure;

    wc.hInstance =
        hInstance;

    wc.lpszClassName =
        CLASS_NAME;

    wc.hCursor =
        LoadCursor(
            NULL,
            IDC_ARROW
        );

    wc.hbrBackground =
        (HBRUSH)(COLOR_WINDOW + 1);


    RegisterClass(&wc);


    // -------------------------
    // Create Window
    // -------------------------

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Mini Compiler",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        920,
        720,
        NULL,
        NULL,
        hInstance,
        NULL
    );


    if(hwnd == NULL)
    {
        return 0;
    }


    // -------------------------
    // Show Window
    // -------------------------

    ShowWindow(
        hwnd,
        nCmdShow
    );

    UpdateWindow(hwnd);


    // -------------------------
    // Message Loop
    // -------------------------

    MSG msg = {};

    while(
        GetMessage(
            &msg,
            NULL,
            0,
            0
        )
    )
    {
        TranslateMessage(&msg);

        DispatchMessage(&msg);
    }


    return 0;
}
