# filename: VC50_DIR.MAK
#    $Id$
#
# Description:  This file defines rules to recursively step into
#               subdirectories and MAKE it's auto build file.
#		The file will recurse into directories in DIR_LIST 
#		first then do the makefiles in MAK_LIST. 
#
# Notice: Because of lack of features of Microsoft NMAKE, you must
#   	  append directory names with an extension so that they
#	  can't be found in the directory structures.  Also, you can't
#	  specify a makefile with its full name.  Remove its extension 
#	  name when put into MAK_LIST.
#	  For example, if we wants to recurse into a directory named
#	  "ace", you must put it in DIR_LIST as "ace.dir" where there
#	  is no file or directory named "ace.dir".
#
# @@ This file currently only supports VC 5.0.


!IF DEFINED (ACE_PLATFORM) && DEFINED (ACE_TARGET)

ALL: $(DIR_LIST) $(MAK_LIST)

MYMAKEFLAGS = /$(MAKEFLAGS)
total = "$(DIR_LIST)$(MAK_LIST)"

!IFDEF DIR_LIST
$(DIR_LIST):
	@cd $(@B)
	@Echo --------------------------------------------------
	@Echo +++++ Entering directory: +++++
	@CD
	@Echo --------------------------------------------------
#	@Echo NMake $(MYMAKEFLAGS) ACE_PLATFORM="$(ACE_PLATFORM)" ACE_TARGET="$(ACE_TARGET)" /F VCAUTO.MAK
	@NMake $(MYMAKEFLAGS) ACE_PLATFORM="$(ACE_PLATFORM)" ACE_TARGET="$(ACE_TARGET)" ACE_ACTION="$(ACE_ACTION)" /NOLOGO /F VCAUTO.MAK
	@Echo --------------------------------------------------
	@Echo ----- Leaving directory: -----
	@CD
	@Echo --------------------------------------------------
	@cd ..
	@Echo.
!ENDIF

!IFDEF MAK_LIST
$(MAK_LIST) : 
!IFDEF ACE_CHECK_DIR
!IF ! EXIST ($(ACE_CHECK_DIR))
	@mkdir $(ACE_CHECK_DIR)
!ENDIF	
!ENDIF
!IF	"$(MSVC_VER)" == "5.0"
#       When using MSVC 5.0
	@Echo NMake $(MYMAKEFLAGS) CFG="$@ - $(ACE_PLATFORM) $(ACE_TARGET)" /f $@.MAK
	@NMake $(MYMAKEFLAGS) CFG="$@ - $(ACE_PLATFORM) $(ACE_TARGET)" /NOLOGO /f $@.MAK $(ACE_ACTION)
!ELSE IF "$(MSVC_VER)" == "4.2"
#       When using MSVC 4.2
	@Echo NMake $(MYMAKEFLAGS) CFG="$@ - $(ACE_PLATFORM) $(ACE_TARGET)" /f $(MAK_NAME).MAK
	@NMake $(MYMAKEFLAGS) CFG="$@ - $(ACE_PLATFORM) $(ACE_TARGET)" /NOLOGO /f $(MAK_NAME).MAK $(ACE_ACTION)
!ELSE
!MESSAGE "You must define the version number of MSVC you are using."
!ENDIF	
!ENDIF

!ELSE
!MESSAGE "YOU MUST DEFINE ACE_CFG BEFORE I CAN BUILD ANYTHING!!
!ENDIF
