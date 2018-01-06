#include "Console.h"

CConsole::CConsole()
{
	this->hConsole = NULL;
	this->hFile = NULL;

	this->IsConsole = false;
	this->IsLogFile = false;

	defColor = 0;

	sys_time = SYSTEMTIME();

	//clear buffer
	memset(buffer, 0, sizeof(buffer));
}

CConsole::~CConsole()
{
	this->hConsole = NULL;
	this->hFile = NULL;

	this->IsConsole = false;
	this->IsLogFile = false;

	defColor = 0;

	sys_time = SYSTEMTIME();

	//clear buffer
	memset(buffer, 0, sizeof(buffer));

	//free console
	FreeConsole();

	//Close console HANDLE
	CloseHandle(this->hConsole);

	//End of file
	SetEndOfFile(this->hFile);

	//close file HANDLE
	CloseHandle(this->hFile);
}

void CConsole::Init(bool Console, bool LogFile)
{
	this->IsConsole = Console;
	this->IsLogFile = LogFile;

	if (this->IsConsole)
	{
		//Enable Console
		AllocConsole();

		//Set default console color
		SetDefaultColor(ConsoleColor::Gray);

		//Get Console Handle
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	if (this->IsLogFile)
	{
		TCHAR ModulePath[0xff];

		//get full module path and length
		DWORD length = GetModuleFileName(NULL, ModulePath, 0xff);

		//changer exterion type
		ModulePath[length - 1] = TEXT('t');
		ModulePath[length - 2] = TEXT('x');
		ModulePath[length - 3] = TEXT('t');

		//create output log in module directory
		hFile = CreateFile(ModulePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}
}

bool CConsole::SetTitle(LPCTSTR Title)
{
	return SetConsoleTitle(Title);
}

void CConsole::SetDefaultColor(WORD Color)
{
	//use ternary operator
	this->defColor = Color ? Color : Gray;

	this->SetConsoleColor(this->defColor);
}

bool CConsole::SetConsoleColor(WORD Color)
{
	return SetConsoleTextAttribute(this->hConsole, Color);
}

void CConsole::Log(LPCTSTR format, ...)
{
	if (!IsConsole && !IsLogFile)
		return;

	//Variadic function
	va_list args;
	va_start(args, format);
	_vstprintf_s(buffer, format, args);
	va_end(args);

	//get current time
	LPTSTR bufftime = this->GetTimeNow();

	if (IsConsole)
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
	if (!IsConsole && !IsLogFile)
		return;

	//Variadic function
	va_list args;
	va_start(args, format);
	_vstprintf_s(buffer, format, args);
	va_end(args);

	//get current time
	LPTSTR bufftime = this->GetTimeNow();

	if (IsConsole)
	{
		//write time
		this->WriteInConsole(bufftime);

		//set console color atributs
		this->SetConsoleColor(Color);

		//write buffer
		this->WriteInConsole(buffer);

		//tab and break line
		this->WriteInConsole(LITERALS);

		//restore color
		this->SetConsoleColor(this->defColor);
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

bool CConsole::WriteInConsole(LPCTSTR buffer)
{
	return WriteConsole(this->hConsole, buffer, STRLEN(buffer), NULL, NULL);
}

#ifdef UNICODE
bool CConsole::WriteInFile(LPCTSTR buffer)
{
	if (this->hFile == INVALID_HANDLE_VALUE)
		return false;

	unsigned char Header[2];

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
	if (this->hFile == INVALID_HANDLE_VALUE)
		return false;

	return WriteFile(this->hFile, (LPCVOID)buffer, STRLEN(buffer) * sizeof(TCHAR), NULL, NULL);
}
#endif

LPTSTR CConsole::GetTimeNow()
{
	TCHAR tbuffer[0xff];

	GetLocalTime(&sys_time);

	_stprintf_s(tbuffer, TEXT("<%02d:%02d:%02d> "), sys_time.wHour, sys_time.wMinute, sys_time.wSecond);

	sys_time = SYSTEMTIME();

	return tbuffer;
}