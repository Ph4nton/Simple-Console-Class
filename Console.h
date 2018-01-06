#pragma once
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <tchar.h>

#ifdef UNICODE
#define STRLEN(s) wcslen(s)
#else
#define STRLEN(s) strlen(s)
#endif //You can use _tcslen

#define LITERALS	TEXT("\r\n")

//used only in the console
enum ConsoleColor : WORD 
{
	Black = 0,
	DarkBlue = 1,
	DarkGreen = 2,
	DarkCyan = 3,
	DarkRed = 4,
	DarkMagenta = 5,
	DarkYellow = 6,
	Gray = 7,
	DarkGray = 8,
	Blue = 9,
	Green = 10,
	Cyan = 11,
	Red = 12,
	Magenta = 13,
	Yellow = 14,
	White = 15,
};

class CConsole
{
public:
	//Constructor
	CConsole();

	//Destructor
	~CConsole();

	//Initilize console and log file
	void Init(bool Console, bool LogFile);

	//Set console title
	bool SetTitle(LPCTSTR Title);

	//Set console default color
	void SetDefaultColor(WORD Color);

	//Log in console and log file
	void Log(LPCTSTR format, ...);

	//Log with color
	void Log(ConsoleColor Color, LPCTSTR format, ...);

private:
	//Edit console attribute color
	bool SetConsoleColor(WORD Color);

	//write in console
	bool WriteInConsole(LPCTSTR buffer);

	//Write in log file
	bool WriteInFile(LPCTSTR buffer);

	//Get current time
	LPTSTR GetTimeNow();

protected:
	//Text buffer
	TCHAR buffer[0xff];

	//Default color value
	WORD defColor;

	//Console and file HANDLE
	HANDLE hConsole, hFile;

	//Console and file boolean 
	bool IsConsole, IsLogFile;

	//Time struct
	SYSTEMTIME sys_time;
};