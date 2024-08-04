#pragma once

class Console
{
public:
	enum EColors : int
	{
		ECOLOR_DEFAULT,
		ECOLOR_DARK_BLUE,
		ECOLOR_DARK_GREEN,
		ECOLOR_DARK_TEAL,
		ECOLOR_DARK_RED,
		ECOLOR_DARK_PINK,
		ECOLOR_DARK_YELLOW,
		ECOLOR_DARK_WHITE,
		ECOLOR_DARK_GRAY,
		ECOLOR_BLUE,
		ECOLOR_GREEN,
		ECOLOR_TEAL,
		ECOLOR_RED,
		ECOLOR_PINK,
		ECOLOR_YELLOW,
		ECOLOR_WHITE,
	};

public:
	static inline FILE* p_OutStream{ nullptr };
	static inline bool m_IsInit{ false };
	bool m_IsShow{ false };
	bool m_IsVerbose{ false };

public:
	void Initialize(const char* ConsoleName, bool bShowWindow = true);
	void cLog(const char* fmt, const EColors Color = ECOLOR_DEFAULT, ...);
	void LogError(const char* fmt, ...);
	void DestroyConsole();
	void SetConsoleVisibility(bool bShow);
	HANDLE GetHandle();
	HWND GetWindowHandle();

public:
	static void Log(const char* fmt, ...);
	static void Clear();

public:
	explicit Console();
	Console(const char* title);
	Console(const char* title, bool bShow);
	~Console();

private:
	HANDLE p_Handle{};
	HWND p_Hwnd{};
} extern g_Console;