#include "Console.h"

#ifdef UNICODE
#define STRLEN(s) wcslen(s)
#else
#define STRLEN(s) strlen(s)
#endif //You can use _tcslen

#define LITERALS	TEXT("\r\n")

void CConsole::Release()
{
	this->IsLogConsole = false;
	this->IsLogFile = false;

	default_color = 0;

	//clear module path
	memset(ModulePath, 0, sizeof(ModulePath));

	//free console
	FreeConsole();

	//End of file
	SetEndOfFile(this->hFile);

	//Close console HANDLE
	if (this->hConsole)
	{
		this->hConsole = nullptr;
	}

	//close file HANDLE
	if (this->hFile)
	{
		CloseHandle(this->hFile);
		this->hFile = nullptr;
	}
}

void CConsole::Init(bool Console, bool LogFile)
{
	this->IsLogConsole = Console;
	this->IsLogFile = LogFile;

	if (Console)
	{
		//Enable Console
		AllocConsole();

		//Set default console color
		SetDefaultColor(CONSOLE_DEFAULT_COLOR);

		//Get Console Handle
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	if (LogFile)
	{
		//get full module path and length
		DWORD length = GetModuleFileName(NULL, this->ModulePath, 0xff);

		//changer exterion type
		this->ModulePath[length - 1] = FILE_TYPE[0];
		this->ModulePath[length - 2] = FILE_TYPE[1];
		this->ModulePath[length - 3] = FILE_TYPE[2];

		//create output log in module directory
		hFile = CreateFile(this->ModulePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}
}

bool CConsole::SetTitle(LPCTSTR Title)
{
	return SetConsoleTitle(Title);
}

bool CConsole::SetDefaultColor(WORD Color)
{
	//use ternary operator
	this->default_color = Color;

	return SetConsoleColor(this->default_color);
}

bool CConsole::SetConsoleColor(WORD Color)
{
	return SetConsoleTextAttribute(this->hConsole, Color);
}

bool CConsole::SetConsoleFont(LPCTSTR Name, SHORT Size, UINT Weight)
{
	TCHAR font_name[32];

#ifdef UNICODE
	wcscpy_s(font_name, 32, Name);
#else
	strcpy_s(font_name, 32, Name);
#endif

	CONSOLE_FONT_INFOEX fInfo;

	fInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	fInfo.nFont = 0;
	fInfo.dwFontSize = { 0, Size };
	fInfo.FontFamily = TMPF_VECTOR;
	fInfo.FontWeight = Weight; //FW_LIGHT, FW_NORMAL, FW_BOLD
	std::copy(font_name, font_name + (sizeof(font_name) / sizeof(TCHAR)), fInfo.FaceName);

	return SetCurrentConsoleFontEx(this->hConsole, false, &fInfo);
}

void CConsole::Log(LPCTSTR format, ...)
{
	if (!IsLogConsole && !IsLogFile)
		return;

	//Text buffer
	TCHAR buffer[0xff] = { 0 };

	//Variadic function
	va_list args;
	va_start(args, format);
	_vstprintf_s(buffer, format, args);
	va_end(args);

	//get current time
	LPTSTR bufftime = this->GetTimeNow();

	if (IsLogConsole)
	{
		//write time
		this->WriteInConsole(bufftime);

		//write buffer
		this->WriteInConsole(buffer);

		//tab and break line
		this->WriteInConsole(LITERALS);
	}

	if (IsLogFile)
	{
		//write time in log file
		this->WriteInFile(bufftime);

		//write buffer in log file
		this->WriteInFile(buffer);

		//tab and break line in log file
		this->WriteInFile(LITERALS);
	}

	//clear buffer
	memset(buffer, 0, sizeof(buffer));

	//clear buffer
	memset(bufftime, 0, sizeof(bufftime));
}

void CConsole::Log(ConsoleColor Color, LPCTSTR format, ...)
{
	if (!IsLogConsole && !IsLogFile)
		return;

	//Text buffer
	TCHAR buffer[0xff] = { 0 };

	//Variadic function
	va_list args;
	va_start(args, format);
	_vstprintf_s(buffer, format, args);
	va_end(args);

	SetConsoleColor(Color);

	this->Log(buffer);

	SetConsoleColor(this->default_color);

	//clear buffer
	memset(buffer, 0, sizeof(buffer));
}

bool CConsole::WriteInConsole(LPCTSTR buffer)
{
	if (this->hConsole == NULL)
		return false;

	if (this->hConsole == INVALID_HANDLE_VALUE)
		return false;

	return WriteConsole(this->hConsole, buffer, (DWORD)STRLEN(buffer), NULL, NULL);
}

#ifdef UNICODE
bool CConsole::WriteInFile(LPCTSTR buffer)
{
	if (this->hFile == NULL)
		return false;

	if (this->hFile == INVALID_HANDLE_VALUE)
		return false;

	unsigned char Header[2]; //UTF-16 BOM

	Header[0] = 0xFF;

	Header[1] = 0xFE;

	if (!WriteFile(this->hFile, Header, 2, NULL, NULL))
		return false;

	if (!WriteFile(this->hFile, (LPCVOID)buffer, STRLEN(buffer) * sizeof(TCHAR), NULL, NULL))
		return false;

	return true;
}
#else
bool CConsole::WriteInFile(LPCTSTR string)
{
	if (this->hFile == NULL)
		return false;

	if (this->hFile == INVALID_HANDLE_VALUE)
		return false;

	return WriteFile(this->hFile, string, (DWORD)STRLEN(string), NULL, NULL);
}
#endif

LPTSTR CConsole::GetTimeNow()
{
	static TCHAR tbuffer[0xff];

	//Time struct
	SYSTEMTIME time = SYSTEMTIME();

	//Get local time
	GetLocalTime(&time);

	//formart time text
	_stprintf_s(tbuffer, sizeof(tbuffer), TEXT("<%02d:%02d:%02d> "), time.wHour, time.wMinute, time.wSecond);

	//return buffer
	return tbuffer;
}
