#include "pch.h"

Console::Console() {}

Console::Console(const char* Title) { Initialize(Title); }

Console::Console(const char* Title, bool bShow) { Initialize(Title, bShow); }

Console::~Console() { DestroyConsole(); }

HANDLE Console::GetHandle() { return this->p_Handle; }

HWND Console::GetWindowHandle() { return this->p_Hwnd; }

void Console::Clear() { system(skCrypt("cls")); }

void Console::Initialize(const char* ConsoleName, bool bShowWindow)
{
	if (Console::m_IsInit)
	{
		LogError(skCrypt("[!] [Console::InitializeConsole] Failed To Initialize Console.\n"));
		return;
	}

	AllocConsole();
	p_Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	p_Hwnd = GetConsoleWindow();
	freopen_s(&p_OutStream, "CONOUT$", "w", stdout);

	char titleBuff[256];
	sprintf_s(titleBuff, skCrypt("[DEBUG] %s"), ConsoleName);
	SetConsoleTitleA(titleBuff);

	Console::m_IsInit = true;
	Console::m_IsShow = bShowWindow;

	ShowWindow(this->p_Hwnd, Console::m_IsShow ? SW_SHOW : SW_HIDE);
}

void Console::cLog(const char* fmt, EColors color, ...)
{
	SetConsoleTextAttribute(p_Handle, color);
	va_list arg;
	va_start(arg, color);
	vfprintf(p_OutStream, fmt, arg);
	va_end(arg);
	SetConsoleTextAttribute(p_Handle, ECOLOR_DEFAULT);
}

void Console::LogError(const char* fmt, ...)
{
	SetConsoleTextAttribute(p_Handle, ECOLOR_RED);
	va_list arg;
	va_start(arg, fmt);
	vfprintf(p_OutStream, fmt, arg);
	va_end(arg);
	SetConsoleTextAttribute(p_Handle, ECOLOR_DEFAULT);
}

void Console::DestroyConsole()
{
	fclose(p_OutStream);
	DestroyWindow(p_Hwnd);
	FreeConsole();
	m_IsInit = false;
}

void Console::ToggleConsole()
{
	this->m_IsShow = !this->m_IsShow;
	ShowWindow(p_Hwnd, this->m_IsShow ? SW_SHOW : SW_HIDE);
}

void Console::Log(const char* fmt, ...)
{
	if (!p_OutStream)
		return;

	va_list arg;
	va_start(arg, fmt);
	vfprintf(p_OutStream, fmt, arg);
	va_end(arg);
}