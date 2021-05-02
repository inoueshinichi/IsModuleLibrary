#pragma once

#include <utils/defs.hpp>

#include <Windows.h>


#include <cstdio>

namespace Is
{
    namespace utils
    {
        namespace win32
        {
/* WindowsコンソールのANSI エスケープシーケンス対応のプリプロセッサ */
// Windows SDKが古いバージョンだとENABLE_VIRTUAL_TERMINAL_INPUT / ENABLE_VIRTUAL_TERMINAL_PROCESSING
// は，宣言されていない.
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#ifndef ENABLE_VIRTUAL_TERMINAL_INPUT
#define ENABLE_VIRTUAL_TERMINAL_INPUT 0x0004
#endif

#ifndef ENABLE_LVB_GRID_WORLDWIDE
#define ENABLE_LVB_GRID_WORLDWIDE 0x0010
#endif

            class IS_UTILS_API Win32Console
            {
                enum
                {
                    CNSL_OK                     = 0,
                    CNSL_ERR_WINDOW_HANDLE      = 1,
                    CNSL_ERR_INPUT_MODE         = 2,
                    CNSL_ERR_SCREEN_MODE        = 3,
                    CNSL_ERR_NO_STD_INPUT       = 4,
                    CNSL_ERR_NO_STD_OUTPUT      = 5,
                    CNSL_ERR_NO_STD_ERROR       = 6,
                    CNSL_ERR_ANSI_ESCAPE_INPUT  = 7,
                    CNSL_ERR_ANSI_ESCAPE_SCREEN = 8,
                };

                HWND  m_hWnd;       // Windows Handler
                FILE* m_fpIn;       // Input File Pointer
                FILE* m_fpOut;      // Output File Pointer
                FILE* m_fpErr;      // Error FIle Pointer
                int   m_fdIn;       // Input File Descriptor
                int   m_fdOut;      // Output File Descriptor
                int   m_fdErr;      // Erro FIle Descriptor
                HANDLE m_hIn;       // Input Handle
                HANDLE m_hOut;      // Output Handle
                HANDLE m_hErr;      // Error Handle

                DWORD m_inputConsoleMode;
                DWORD m_outputConsoleMode;
                BOOL  m_isStartUp;

                constexpr int WINDOW_TITLE_LENGTH = 2048;

                BOOL InitConsole();

            public:
                Win32Console();
                virtual ~Win32Console();

                static BOOL WINAPI ConsoleSignalHandler(DWORD dwCtrlType);

                int GetMode();
                int EnableANSIEscapeSequence();
                void ShowConsoleMode();
            };
        }
    }
}