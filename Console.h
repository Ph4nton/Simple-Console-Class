#pragma once
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <tchar.h>

#define CONSOLE_DEFAULT_COLOR ConsoleColor::Gray

#define FILE_TYPE TEXT("txt")

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
	//Initilize console and log file
	void Init(bool Console = false, bool LogFile = false);

	//Set console title
	bool SetTitle(LPCTSTR Title);

	//Set console default color
	bool SetDefaultColor(WORD Color);

	//Set console font
	bool SetConsoleFont(LPCTSTR Name, SHORT Size = 16, UINT Weight = FW_NORMAL);

	//Log in console and log file
	void Log(LPCTSTR format, ...);

	//Log with color
	void Log(ConsoleColor Color, LPCTSTR format, ...);

	//Release
	void Release();

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
	//Module Path
	TCHAR ModulePath[0xff];

	//Default color value
	WORD default_color;

	//Console and file HANDLE
	HANDLE hConsole, hFile;

	//Console and file boolean 
	bool IsLogConsole, IsLogFile;
};
