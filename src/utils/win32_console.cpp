#include "utils/win32_console.hpp"
#include "utils/win32_exception.hpp"
#include "utils/format_string.hpp"

#include <io.h>     // _isatty

namespace Is
{
    namespace utils
    {
        using std::string;

        Win32Console::Win32Console()
            : m_hWnd(NULL)
            , m_fpIn(nullptr)
            , m_fpOut(nullptr)
            , m_fpErr(nullptr)
            , m_fdIn(NULL)
            , m_fdOut(NULL)
            , m_fdErr(NULL)
            , m_hIn(NULL)
            , m_hOut(NULL)
            , m_hErr(NULL)
            , m_inputConsoleMode(0)
            , m_isStartUp(false)
        {
            // 初期化
            if (InitConsole())
            {
                CString msg("[Open] Win32 Console\n");
                ::OutputDebugString(static_cast<LPCTSTR>(msg));
                m_isStartUp = true;
            }

            // コンソールモードの取得
            if (GetMode() != 0)
            {
                CString msg("[Error] Win32 Console Mode\n")
                ::OutputDebugString(static_cast<LPCTSTR>(msg));
            }

            // ANSI エスケープシーケンスを有効化
            EnableANSIEscapeSequence();

            // コンソールモードの確認
	        ShowConsoleMode();
        }


        Win32Console::~Win32Console()
        {
            if (m_isStartUp)
            {
                // プロセスに割当て済みのWin32コンソールを解放する.
			    FreeConsole();
                CString msg("[Close] Win32 Console\n")
                ::OutputDebugString(static_cast<LPCTSTR>(msg));
            }
        }


        /* コンソールへのシグナルハンドラ */
        BOOL WINAPI Win32Console::ConsoleSignalHander(DWORD dwCtrlType)
        {
            if (dwCtrlType == CTRL_SHUTDOWN_EVENT)
            {
                // PCをシャットダウンする場合
                // ここに特定の処理を記述
            }
            else if (dwCtrlType == CTRL_LOGOFF_EVENT)
            {
                // PCをログオフする場合
                // ここに特定の処理を記述
            }
            else if (dwCtrlType == CTRL_CLOSE_EVENT)
            {
                // コンソールを消す場合(×ボタン)
                // ここに特定の処理を記述
            }
            else if (dwCtrlType == CTRL_BREAK_EVENT)
            {
                // Ctrl + Breakが押された場合
                // ここに特定の処理を記述
            }
            else if (dwCtrlType == CTRL_C_EVENT)
            {
                // Ctrl + Cが押された場合
                // ここに特定の処理を記述
            }
            return true;
        }


        /* 入力/スクリーンバッファのモードを取得. */
        int Win32Console::GetMode()
        {
            int iRet = CNSL_OK;

            if (m_hWnd != NULL)
            {
                // コンソール入力バッファのモードを取得
                if (!GetConsoleMode(m_hIn, &m_inputConsoleMode))
                {
                    // 失敗
                    CString msg = show_win32api_error(); // Win32 API エラーメッセージを取得
                    CString full_msg;
                    full_msg.Format("入力バッファモードの取得失敗: %s\n", (LPCTSTR)msg);
                    iRet = CNSL_ERR_INPUT_MODE;
                }

                // コンソールスクリーンバッファのモードを取得
                if (!GetConsoleMode(m_hOut, &m_outputConsoleMode))
                {
                    // 失敗
                    CString msg = show_win32api_error(); // Win32 API エラーメッセージを取得
                    CString full_msg;
                    full_msg.Format("スクリーンバッファモードの取得失敗: %s\n", (LPCTSTR)msg);
                    iRet = CNSL_ERR_SCREEN_MODE;
                }
            }
            else
            {
                CString msg = show_win32api_error(); // Win32 API エラーメッセージを取得
                CString full_msg;
                full_msg.Format("Windows Handle Error: %s\n", (LPCTSTR)msg);
                iRet = CNSL_ERR_WINDOW_HANDLE;
            }

            ::OutputDebugString(full_msg);
            return iRet;
        }


        int Win32Console::EvableANSIEscapeSequence()
        {
            /* コンソールでANSIエスケープを有効にする */
            CString full_msg;
            int iRet = CNSL_OK;

            // Windows 10（TH2/1511以降）では条件付きでANSIエスケープシーケンスに対応
            if (m_hWnd != NULL)
            {
                /*
                ANSIエスケープを有効にするために
                GetConsoleModeのmodeに定数ENABLE_VIRTUAL_TERMINAL_PROCESSING / ENABLE_VIRTUAL_TERMINAL_INPUTを指定する
                新しいWindowsコンソールではANSIエスケープシーケンスが有効になる
                古いWindowsコンソールではAPI呼び出しが失敗し、GetLastError()はERROR_INVALID_PARAMETER(0x57)を返す
                */
                
                // 標準入力に繋がっているかチェック
                if (_isatty(m_fdIn))
                {
                    if (!SetConsoleMode(m_hIn, m_inputConsoleMode | ENABLE_VIRTUAL_TERMINAL_INPUT))
                    {
                        // 成功
                        full_msg.Format("[Success] Input: Enable ANSI-Escape Sequence\n");
                    }
                    else
                    {
                        // 失敗
                        CString msg = show_win32api_error(); // Win32 API エラーメッセージを取得
                        full_msg.Format("[Error] 入力バッファモードのANSI ESCAPEの有効化失敗: %s\n", (LPCTSTR)msg);
                        iRet = CNSL_ERR_ANSI_ESCAPE_INPUT;
                    }
                }
                else
                {
                    // 標準入力につながっていない
                    CString msg = show_win32api_error(); // Win32 API エラーメッセージを取得
                    full_msg.Format("標準入力に未接続: %s\n", (LPCTSTR)msg);
                    iRet = CNSL_ERR_NO_STD_INPUT;
                }


                // 標準出力に繋がっているかチェック
                if (_isatty(m_fdOut))
                {
                    // ANSI Escape Sequenceを有効化
                    if (!SetConsoleMode(m_hOut, m_outputConsoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
                    {
                        // 成功
                        full_msg.Format("[Success] Output: Enable ANSI-Escape Sequence\n");
                    }
                    else
                    {
                        // 失敗
                        CString msg = show_win32api_error(); // Win32 API エラーメッセージを取得
                        full_msg.Format("[Error] スクリーンバッファモードのANSI ESCAPEの有効化失敗: %s\n", (LPCTSTR)msg);
                        iRet = CNSL_ERR_ANSI_ESCAPE_SCREEN;
                    }
                }
                else
                {
                    // 標準出力につながっていない
                    CString msg = show_win32api_error(); // Win32 API エラーメッセージを取得
                    full_msg.Format("標準出力に未接続: %s\n", (LPCTSTR)msg);
                    iRet = CNSL_ERR_NO_STD_OUTPUT;
                }

            }
            else
            {
                CString msg = show_win32api_error(); // Win32 API エラーメッセージを取得
                CString full_msg;
                full_msg.Format("Windows Handle Error: %s\n", (LPCTSTR)msg);
                iRet = CNSL_ERR_WINDOW_HANDLE;
            }

            ::OutputDebugString(full_msg);
            return iRet;
        }


        /* 現在のコンソールモードの状態を表示 */
        void Win32Console::ShowConsoleMode()
        {
            if (m_hWnd != NULL)
            {
                /*
                BOOL GetConsoleMode(HANDLE hConsoleHandle, LPDWORD lpMode)関数

                [hConsoleHandleに入力バファfのハンドルを指定した場合，lpModeが指す変数に以下の値の組み合わせが格納される]
                ENABLE_LINE_INPUT				0x0002 : ReadFile関数またはReadConsole関数はキャリッジリターン文字を読み取ったときだけ制御を戻す。
                                                         このモードが無効な場合，これらの関数は１つ以上の文字を読取ると制御を戻す。

                ENABLE_EXTENDED_FLAGS			0x0080 :


                ENABLE_INSERT_MODE				0x0020 :


                ENABLE_ECHO_INPUT				0x0004 : ReadFile関数またはReadConsole関数で文字を読取るたびに，その文字がアクティブなスクリーンバッファに書き込まれる。
                このモードを有効にできるのは, ENABLE_LINE_INPUTモードが有効な場合だけ。

                ENABLE_PROCESSED_INPUT			0x0001 : CTRL+Cがシステムによって処理され，入力バッファに格納されない。ReadFile関数またはReadConsole関数で入力バッファの読み取りを行う場合，
                                                         他の制御キーはシステムによって処理され，ReadFile関数またはReadConsole関数のバッファに返されない。
                                                         ENABLE_LINE_INPUTモードも有効な場合，バックスペース文字，キャリッジ・リターン文字，ラインフィード文字がシステムによって処理される。

                ENABLE_WINDOW_INPUT				0x0008 : コンソールスクリーンバッファのサイズを変更するためのユーザーとの対話がコンソールの入力バッファに報告される。これらのイベントに関する情報は，
                                                         ReadConsoleInput関数を使って入力バッファから読み取ることができる。ReadFile関数とReadConsole関数は読み取ることができない。

                ENABLE_MOUSE_INPUT				0x0010 : マウスポインタがコンソールウィンドウの境界内にあり，ウィンドウがキーボードフォーカスを持っている場合，マウスを移動したりマウスボタンを押すことに
                　　　　　　　　　　　　　　　				よって生成されるマウスイベントが入力バッファに置かれる。これらのイベントは，このモードが有効な場合でも，ReadFile関数やReadConsole関数によって破棄される。

                ENABLE_QUICK_EDIT_MODE			0x0040 :


                ENABLE_VIRTUAL_TERMINAL_INPUT	0x0200 :
                */

                if (m_hIn != NULL)
                {
                    ::OutputDebugString("[InputConsoleBuffer Mode]\n");
                    CString isFlags;
                    isFlags.Format("ENABLE_LINE_INPUT -> %s\n",
                        ((m_inputConsoleMode & ENABLE_LINE_INPUT) == ENABLE_LINE_INPUT) ? "Enable" : "Didable");
                    ::OutputDebugString(isFlags);
                    isFlags.Format("ENABLE_EXTEDNED_FLAGS -> %s\n",
                        ((m_inputConsoleMode & ENABLE_EXTENDED_FLAGS) == ENABLE_EXTENDED_FLAGS) ? "Enable" : "Didable");
                    ::OutputDebugString(isFlags);
                    isFlags.Format("ENABLE_INSERT_MODE -> %s\n",
                        ((m_inputConsoleMode & ENABLE_INSERT_MODE) == ENABLE_INSERT_MODE) ? "Enable" : "Didable");
                    ::OutputDebugString(isFlags);
                    isFlags.Format("ENABLE_ECHO_INPUT -> %s\n",
                        ((m_inputConsoleMode & ENABLE_ECHO_INPUT) == ENABLE_ECHO_INPUT) ? "Enable" : "Didable");
                    ::OutputDebugString(isFlags);
                    isFlags.Format("ENABLE_PROCESSED_INPUT -> %s\n",
                        ((m_inputConsoleMode & ENABLE_PROCESSED_INPUT) == ENABLE_PROCESSED_INPUT) ? "Enable" : "Didable");
                    ::OutputDebugString(isFlags);
                    isFlags.Format("ENABLE_MOUSE_INPUT -> %s\n",
                        ((m_inputConsoleMode & ENABLE_MOUSE_INPUT) == ENABLE_MOUSE_INPUT) ? "Enable" : "Didable");
                    ::OutputDebugString(isFlags);
                    isFlags.Format("ENABLE_QUICK_EDIT_MODE -> %s\n",
                        ((m_inputConsoleMode & ENABLE_QUICK_EDIT_MODE) == ENABLE_QUICK_EDIT_MODE) ? "Enable" : "Didable");
                    ::OutputDebugString(isFlags);
                    isFlags.Format("ENABLE_VIRTUAL_TERMINAL_INPUT -> %s\n",
                        ((m_inputConsoleMode & ENABLE_VIRTUAL_TERMINAL_INPUT) == ENABLE_VIRTUAL_TERMINAL_INPUT) ? _TEXT("Enable") : _TEXT("Didable"));
                    ::OutputDebugString(isFlags);
                }

                /*
                [hConsoleHandleにスクリーンバッファのハンドルを指定した場合，lpModeが指す変数に以下の値の組み合わせが格納される]
                ENABLE_PROCESSED_OUTPUT				0x0001 : WriteFile関数またはWriteConsole関数で書き込む文字，ReadFile関数またはReadConsole関数でエコーする文字がASCII文字か解析され，
                                                             正しい動作が実行される。バックスペース文字、タブ文字、ベル文字、キャリッジリターン文字、ラインフィード文字が処理される。
                ENABLE_WRAP_AT_EOL_OUTPUT			0x0002 : WriteFile関数またはWriteConsole関数で書き込みをする時，あるいはReadFile関数またはReadConsole関数でエコーする時，カーソルが
                　　　　　　　　　　　　　　　　　			    現在の行の終端に到達すると次の行の先頭に移動する。カーソルがウィンドウの最後の行から次の行に進む時，コンソールウィンドウに表示されている
                                                             行が自動的に上にスクロールする。また，カーソルがスクリーンバッファの最後の行から次の行に進む時，スクリーンバッファの内容が上にスクロールする。
                                                             (スクリーンバッファの一番上の行が破棄される)。このモードが無効になっていると，業の最後の文字にそれ以降の文字が上書きされる

                ENABLE_VIRTUAL_TERMINAL_PROCESSING	0x0004 :

                DISABLE_NEWLINE_AUTO_RETURN			0x0008 :

                ENABLE_LVB_GRID_WORLDWIDE			0x0010 :
                */

                if (m_hOut != NULL)
                {
                    ::OutputDebugString("[ScreenConsoleBuffer Mode]\n");
                    CString isFlags;
                    isFlags.Format("ENABLE_PROCESSED_OUTPUT -> %s\n",
                        ((m_inputConsoleMode & ENABLE_PROCESSED_OUTPUT) == ENABLE_PROCESSED_OUTPUT) ? "Enable" : "Didable");
                    ::OutputDebugString(isFlags);
                    isFlags.Format("ENABLE_WRAP_AT_EOL_OUTPUT -> %s\n",
                        ((m_inputConsoleMode & ENABLE_WRAP_AT_EOL_OUTPUT) == ENABLE_WRAP_AT_EOL_OUTPUT) ? "Enable" : "Didable");
                    ::OutputDebugString(isFlags);
                    isFlags.Format("ENABLE_VIRTUAL_TERMINAL_PROCESSING -> %s\n",
                        ((m_inputConsoleMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) == ENABLE_VIRTUAL_TERMINAL_PROCESSING) ? "Enable" : "Didable");
                    ::OutputDebugString(isFlags);
                    isFlags.Format("ENABLE_LVB_GRID_WORLDWIDE -> %s\n",
                        ((m_inputConsoleMode & ENABLE_LVB_GRID_WORLDWIDE) == ENABLE_LVB_GRID_WORLDWIDE) ? "Enable" : "Didable");
                    ::OutputDebugString(isFlags);
                }
            }
        }


        /* コンソールの初期化 */
        BOOL Win32Console::InitConsole()
        {
            // コンソールに標準入力・標準出力・標準エラー出力を割り当てる
            if (!AllocConsole())
            {
                return FALSE;
            }

            // (C/C++) FILE Pointerの取得
            freopen_s(&m_fpIn, "CONIN$", "r", stdin);
            freopen_s(&m_fpOut, "CONOUT$", "w+", stdout); // "w+": 読み取りと書き込みの両方のモードでオープン
            freopen_s(&m_fpErr, "CONOUT$", "w", stderr);

            // (Unix) File Descriptorの取得
            m_fdIn = _fileno(m_fpIn);
            m_fdOut = _fileno(m_fpOut);
            m_fdErr = _fileno(m_fpErr);

            // (Windows) HANDLEの取得
            m_hIn  = (HANDLE)_get_osfhandle(m_fdIn);
            m_hOut = (HANDLE)_get_osfhandle(m_fdOut);
            m_hErr = (HANDLE)_get_osfhandle(m_fdErr);


            // コンソールへのシグナルハンドラを設定
            ::SetConsoleCtrlHandler(&CStartUpConsole::ConsoleSignalHander, TRUE);

            // コンソールウィンドウのタイトルを取得
            char cnslTitle[FILESYSTEM_SOLIDMAXLENGTH];
            ::GetConsoleTitle(cnslTitle, sizeof(cnslTitle));


            // タイトルからウィンドウを検索してウィンドウハンドルを取得
            m_hWnd = ::FindWindow(NULL, cnslTitle);

            // 現在のウィンドウ位置を取得
            RECT cnslWinRect;
            ::GetWindowRect(m_hWnd, &cnslWinRect);


            // ウィンドウ位置を変更
            int cnslLeft = CW_USEDEFAULT; //::GetSystemMetrics(SM_CXSCREEN) / m_divideX;
            int cnslTop = CW_USEDEFAULT; //::GetSystemMetrics(SM_CYSCREEN) / m_divideY;
            int cnslWidth = (cnslWinRect.right - cnslWinRect.left);
            int cnslHeight = (cnslWinRect.bottom - cnslWinRect.top);
            ::MoveWindow(m_hWnd, cnslLeft, cnslTop, cnslWidth, cnslHeight, TRUE);

            return TRUE;
        }
    }
}