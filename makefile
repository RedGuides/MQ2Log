!include "../global.mak"

ALL : "$(OUTDIR)\MQ2Log.dll"

CLEAN :
	-@erase "$(INTDIR)\MQ2Log.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\MQ2Log.dll"
	-@erase "$(OUTDIR)\MQ2Log.exp"
	-@erase "$(OUTDIR)\MQ2Log.lib"
	-@erase "$(OUTDIR)\MQ2Log.pdb"


LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(DETLIB) ..\Release\MQ2Main.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\MQ2Log.pdb" /debug /machine:I386 /out:"$(OUTDIR)\MQ2Log.dll" /implib:"$(OUTDIR)\MQ2Log.lib" /OPT:NOICF /OPT:NOREF 
LINK32_OBJS= \
	"$(INTDIR)\MQ2Log.obj" \
	"$(OUTDIR)\MQ2Main.lib"

"$(OUTDIR)\MQ2Log.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("MQ2Log.dep")
!INCLUDE "MQ2Log.dep"
!ELSE 
!MESSAGE Warning: cannot find "MQ2Log.dep"
!ENDIF 
!ENDIF 


SOURCE=.\MQ2Log.cpp

"$(INTDIR)\MQ2Log.obj" : $(SOURCE) "$(INTDIR)"

