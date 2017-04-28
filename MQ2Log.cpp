// MQ2Log.cpp : Logs mq2 output to logs/servername_charname.log
// Will create logs folder if it does not exist
// /mlog on|off
// Saves toggle setting to ini under 
//
// [MQ2Log]
// Enabled=1
//
// v1.0 - Sym - 04-18-2012

#include "../MQ2Plugin.h"
#include <time.h>

PreSetup("MQ2Log");
PLUGIN_VERSION(1.0);
bool bLog = false;
bool bInit = false;
CHAR Filename[MAX_STRING] = {0};

bool DirectoryExists(LPCTSTR lpszPath) {
	DWORD dw = ::GetFileAttributes(lpszPath);
	return (dw != INVALID_FILE_ATTRIBUTES && (dw & FILE_ATTRIBUTE_DIRECTORY) != 0);
}

void Update_INIFileName() {
	sprintf_s(INIFileName,"%s\\%s_%s.ini",gszINIPath, EQADDR_SERVERNAME, GetCharInfo()->Name);
}

VOID SaveINI(VOID) {
    char szTemp[MAX_STRING];
    Update_INIFileName();
    sprintf_s(szTemp,"MQ2Log");
    WritePrivateProfileSection(szTemp, "", INIFileName);
    WritePrivateProfileString(szTemp,"Enabled",bLog ? "1" : "0",INIFileName);

}

VOID LoadINI(VOID)
{
	char szTemp[MAX_STRING];
    Update_INIFileName();
    sprintf_s(szTemp,"MQ2Log");
    bLog = GetPrivateProfileInt(szTemp, "Enabled", 1, INIFileName) > 0 ? true : false;
	sprintf_s(Filename,"%s\\%s_%s.log",gszLogPath,EQADDR_SERVERNAME,GetCharInfo()->Name);
	if (!DirectoryExists(gszLogPath)) CreateDirectory(gszLogPath, NULL);
}

void LogCommand(PSPAWNINFO pChar, PCHAR szLine) {

    CHAR szArg1[MAX_STRING] = {0};
	GetArg(szArg1, szLine, 1);
    if(!_strcmpi(szArg1, "")) {
		WriteChatf("MQ2Log Usage :: /mlog on|off");
		return;
	}
	if(!_strcmpi(szArg1, "on")) {
		bLog = true;
    } else if(!_strcmpi(szArg1, "off")) {
		bLog = false;
    }
	SaveINI();
	WriteChatf("MQ2Log :: Logging is %s", bLog?"\agON\ax":"\arOFF\ax");
}

VOID MacroLogClean(PSPAWNINFO pChar, PCHAR szLine)
{
    FILE *fOut = NULL;
    CHAR Filename[MAX_STRING] = {0};
    CHAR szBuffer[MAX_STRING] = {0};
    bRunNextCommand = TRUE;

    if (gszMacroName[0]==0) {
        sprintf_s(Filename,"%s\\MacroQuest.log",gszLogPath);
    } else {
        sprintf_s(Filename,"%s\\%s.log",gszLogPath, gszMacroName);
    }

    if (!_stricmp(szLine,"clear")) {
        errno_t err = fopen_s(&fOut,Filename,"wt");
        if (err) {
            MacroError("Couldn't open log file: %s",Filename);
            return;
        }
        WriteChatColor("Cleared log.",USERCOLOR_DEFAULT);
        fclose(fOut);
        return;
    }

    errno_t err = fopen_s(&fOut,Filename,"at");
    if (err) {
        MacroError("Couldn't open log file: %s",Filename);
        return;
    }

    sprintf_s(szBuffer, "%s",szLine);
    ParseMacroParameter(pChar,szBuffer);
    fprintf(fOut,"%s\n", szBuffer);
    DebugSpew("MacroLog - %s", szBuffer);

    fclose(fOut);

}

PLUGIN_API VOID InitializePlugin(VOID)
{
	DebugSpewAlways("Initializing MQ2Log");
	AddCommand("/mlog",LogCommand);
	AddCommand("/mqlogclean",MacroLogClean);
}

PLUGIN_API VOID ShutdownPlugin(VOID)
{
	DebugSpewAlways("Shutting down MQ2Log");
	RemoveCommand("/mlog");
	RemoveCommand("/mqlogclean");
}

PLUGIN_API VOID SetGameState(DWORD GameState) {
    if(GameState==GAMESTATE_INGAME) {
        if(!bInit) {
            bInit=true;
			LoadINI();
		}
	}
}

PLUGIN_API DWORD OnWriteChatColor(PCHAR Line, DWORD Color, DWORD Filter)
{
	if (!bLog || !bInit) return 0;

	FILE *fOut = NULL;
	CHAR szBuffer[MAX_STRING] = {0};
	DWORD i;

	for (i=0;i<strlen(Filename);i++) {
		if (Filename[i]=='\\') {
			strncpy_s(szBuffer,Filename,i);
		}
	}

	errno_t err = fopen_s(&fOut,Filename,"at");
	if (err) {
		sprintf_s(szBuffer,"Couldn't open log file: %s",Filename);
		WriteChatColor(szBuffer,CONCOLOR_RED);
		return 0;;
	}
	char tmpbuf[128];
	tm today = {0};
	time_t tm;
	tm = time(NULL);
	localtime_s(&today,&tm);
	strftime( tmpbuf, 128, "%Y/%m/%d %H:%M:%S", &today );
	CHAR PlainText[MAX_STRING]={0};
	StripMQChat(Line,PlainText);
	sprintf_s(szBuffer, "[%s] %s",tmpbuf,PlainText);
	for(unsigned int i=0; i < strlen(szBuffer); i++) {
		if(szBuffer[i] == 0x07) {
			if((i+2) < strlen(szBuffer))
				strcpy_s(&szBuffer[i], MAX_STRING, &szBuffer[i+2]);
			else
				szBuffer[i] = 0;
		}
	}
	fprintf(fOut,"%s\n", szBuffer);
	fclose(fOut);
	return 0;
}
