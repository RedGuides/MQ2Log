// MQ2Log.cpp : Logs mq2 output to logs/servername_charname.log
// Will create logs folder if it does not exist
// /mlog on|off
// Saves toggle setting to ini under 
//
// [MQ2Log]
// Enabled=1
//
// v1.0 - Sym - 04-18-2012
// v2.1 - Eqmule 07-22-2016 - Added string safety.
// v2.2 - wired420 10-22-2019 - Added /mqlogcustom for custom naming file

#include <mq/Plugin.h>

#include <ctime>

PreSetup("MQ2Log");
PLUGIN_VERSION(2.2);

bool bLog = false;
bool bInit = false;
CHAR Filename[MAX_STRING] = { 0 };

void Update_INIFileName()
{
	sprintf_s(INIFileName, 260, "%s\\%s_%s.ini", gPathConfig, EQADDR_SERVERNAME, GetCharInfo()->Name);
}

void SaveINI() {
	CHAR szTemp[MAX_STRING] = { 0 };
	Update_INIFileName();
	sprintf_s(szTemp, "MQ2Log");
	WritePrivateProfileSection(szTemp, "", INIFileName);
	WritePrivateProfileString(szTemp, "Enabled", bLog ? "1" : "0", INIFileName);

}

void LoadINI()
{
	char szTemp[MAX_STRING];
	Update_INIFileName();
	sprintf_s(szTemp, "MQ2Log");
	bLog = GetPrivateProfileInt(szTemp, "Enabled", 1, INIFileName) > 0 ? true : false;
	sprintf_s(Filename, "%s\\%s_%s.log", gPathLogs, EQADDR_SERVERNAME, GetCharInfo()->Name);
}

void LogCommand(PSPAWNINFO pChar, PCHAR szLine) {

	CHAR szArg1[MAX_STRING] = { 0 };
	GetArg(szArg1, szLine, 1);
	if (!_strcmpi(szArg1, "")) {
		WriteChatf("MQ2Log Usage :: /mlog on|off");
		return;
	}
	if (!_strcmpi(szArg1, "on")) {
		bLog = true;
	}
	else if (!_strcmpi(szArg1, "off")) {
		bLog = false;
	}
	SaveINI();
	WriteChatf("MQ2Log :: Logging is %s", bLog ? "\agON\ax" : "\arOFF\ax");
}

void MacroLogClean(PSPAWNINFO pChar, PCHAR szLine)
{
	FILE* fOut = NULL;
	CHAR Filename2[MAX_STRING] = { 0 };
	CHAR szBuffer[MAX_STRING] = { 0 };
	bRunNextCommand = TRUE;

	if (gszMacroName[0] == 0) {
		sprintf_s(Filename2, "%s\\MacroQuest.log", gPathLogs);
	}
	else {
		sprintf_s(Filename2, "%s\\%s.log", gPathLogs, gszMacroName);
	}

	if (!_stricmp(szLine, "clear")) {
		errno_t err = fopen_s(&fOut, Filename2, "wt");
		if (err) {
			MacroError("Couldn't open log file: %s", Filename2);
			return;
		}
		WriteChatColor("Cleared log.", USERCOLOR_DEFAULT);
		fclose(fOut);
		return;
	}

	errno_t err = fopen_s(&fOut, Filename2, "at");
	if (err) {
		MacroError("Couldn't open log file: %s", Filename2);
		return;
	}

	sprintf_s(szBuffer, "%s", szLine);
	ParseMacroParameter(pChar, szBuffer);
	fprintf(fOut, "%s\n", szBuffer);
	DebugSpew("MacroLog - %s", szBuffer);

	fclose(fOut);
}

void MacroLogCustom(PSPAWNINFO pChar, PCHAR szLine)
{
	FILE* fOut = NULL;
	CHAR Arg1[MAX_STRING] = { 0 };
	CHAR Arg2[MAX_STRING] = { 0 };
	CHAR Filename2[MAX_STRING] = { 0 };
	CHAR szBuffer[MAX_STRING] = { 0 };
	bRunNextCommand = TRUE;

	GetArg(Arg1, szLine, 1);
	GetArg(Arg2, szLine, 2);

	sprintf_s(Filename2, "%s\\%s.log", gPathLogs, Arg1);

	if (!_stricmp(szLine, "clear")) {
		errno_t err = fopen_s(&fOut, Filename2, "wt");
		if (err) {
			MacroError("Couldn't open log file: %s", Filename2);
			return;
		}
		WriteChatColor("Cleared log.", USERCOLOR_DEFAULT);
		fclose(fOut);
		return;
	}

	errno_t err = fopen_s(&fOut, Filename2, "at");
	if (err) {
		MacroError("Couldn't open log file: %s", Filename2);
		return;
	}

	sprintf_s(szBuffer, "%s", Arg2);
	ParseMacroParameter(pChar, szBuffer);
	fprintf(fOut, "%s\n", szBuffer);
	DebugSpew("MacroLog - %s", szBuffer);

	fclose(fOut);
}

PLUGIN_API VOID InitializePlugin(VOID)
{
	DebugSpewAlways("Initializing MQ2Log");
	AddCommand("/mlog", LogCommand);
	AddCommand("/mqlogclean", MacroLogClean);
	AddCommand("/mqlogcustom", MacroLogCustom);
}

PLUGIN_API VOID ShutdownPlugin(VOID)
{
	DebugSpewAlways("Shutting down MQ2Log");
	RemoveCommand("/mlog");
	RemoveCommand("/mqlogclean");
	RemoveCommand("/mqlogcustom");
}

PLUGIN_API VOID SetGameState(DWORD GameState) {
	if (GameState == GAMESTATE_INGAME) {
		if (!bInit) {
			bInit = true;
			LoadINI();
		}
	}
}

PLUGIN_API DWORD OnWriteChatColor(PCHAR Line, DWORD Color, DWORD Filter)
{
	if (!bLog || !bInit) return 0;

	FILE* fOut = NULL;
	CHAR* szBuffer = new char[MAX_STRING];

	for (DWORD i = 0; i < strlen(Filename); i++) {
		if (Filename[i] == '\\') {
			strncpy_s(szBuffer, MAX_STRING, Filename, i);
		}
	}

	errno_t err = fopen_s(&fOut, Filename, "atc");
	if (err) {
		sprintf_s(szBuffer, MAX_STRING, "Couldn't open log file: %s", Filename);
		//if we do this we will run out of stack when we keep getting called by ourself over and over, so no, just no.
		//instead we call the chatwindow OnWriteChatColor directly...
		//WriteChatColor(szBuffer,CONCOLOR_RED);
		if (HMODULE hMod = GetModuleHandle("MQ2ChatWnd"))
		{
			if (fMQWriteChatColor mq2chatWriteChatColor = (fMQWriteChatColor)GetProcAddress(hMod, "OnWriteChatColor"))
			{
				mq2chatWriteChatColor(szBuffer, CONCOLOR_RED, Filter);
			}
		}
		else {
			OutputDebugString(szBuffer);
		}
		delete[] szBuffer;
		return 0;;
	}
	char* tmpbuf = new char[128];
	struct tm today = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	time_t tm = time(nullptr);
	localtime_s(&today, &tm);
	strftime(tmpbuf, 128, "%Y/%m/%d %H:%M:%S", &today);
	CHAR* PlainText = new char[MAX_STRING];
	StripMQChat(Line, PlainText);
	sprintf_s(szBuffer, MAX_STRING, "[%s] %s", tmpbuf, PlainText);
	delete[] tmpbuf;
	delete[] PlainText;
	for (unsigned int i = 0; i < strlen(szBuffer); i++) {
		if (szBuffer[i] == 0x07) {
			if ((i + 2) < strlen(szBuffer))
				strcpy_s(&szBuffer[i], sizeof(szBuffer) - i, &szBuffer[i + 2]);
			else
				szBuffer[i] = 0;
		}
	}
	fprintf(fOut, "%s\r\n", szBuffer);
	_flushall();
	fclose(fOut);
	delete[] szBuffer;
	return 0;
}
