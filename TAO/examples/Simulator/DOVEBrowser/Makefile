#----------------------------------------------------------------------------
#
#       $Id$
#
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

VISIJAVA_BIN=/project/doc/pkg/VisiJava-3.2/bin

#### Make sure that VisiJava is installed and visible.
ifeq ($(shell type idl2java),)
  #### Try again, appending $(VISIJAVA_BIN) to the PATH.  Use := instead
  #### of += because there was an embedded space with +=.
  PATH:=$(PATH):$(VISIJAVA_BIN)
  ifeq ($(findstring No such, \
                     $(shell ls $(VISIJAVA_BIN)/idl2java 2>&1)), \
        No such)
    ERROR:
	  @echo idl2java was not found.  Please specify its path in
	  @echo your VISIJAVA_BIN environment variable.
  endif
endif

DOVE_BROWSER_JAVA = \
        CosNaming.java \
        CosTimeBase.java \
        RtecEventComm.java \
        RtecScheduler.java \
        RtecEventChannelAdmin.java \
        Navigation.java \
        Weapons.java

DOVE_BROWSER_CLASS = Navigation.class

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

BUILD = $(DOVE_BROWSER_JAVA) $(DOVE_BROWSER_CLASS)

ifeq ($(CROSS-COMPILE),)
  include $(ACE_ROOT)/include/makeinclude/macros.GNU
  include $(TAO_ROOT)/rules.tao.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
  include $(TAO_ROOT)/taoconfig.mk
else
  all default_embedded:
	@echo This module is not built on CROSS-COMPILE platforms.

  clean realclean:
	@echo Nothing to clean on CROSS-COMPILE platforms.
endif # ! CROSS-COMPILE

#### No need to create object directories.
OBJDIRS =

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

.SUFFIXES:
.SUFFIXES: .class .idl .java

#### Don't remove the .idl and .java files after building the .class files.
.PRECIOUS: $(DOVE_BROWSER_JAVA) $(patsubst %.java,%.idl,$(DOVE_BROWSER_JAVA))

%.java: %.idl
	@idl2java $<

%.class: %.java
	@vbjc *.java

NavWeap.idl: ../NavWeap.idl
	@cp -p $< .

Navigation.java Weapons.java: NavWeap.idl
	@idl2java $<

%.idl: $(TAO_ROOT)/orbsvcs/orbsvcs/%.idl
	@cp -p $< .

realclean: clean
	-/bin/rm -rf $(DOVE_BROWSER_JAVA) \
         $(patsubst %.java,%.idl,$(DOVE_BROWSER_JAVA)) NavWeap.idl \
         Navigation.java NavigationH*.java \
         Weapons.java WeaponsH*.java *.class CosNaming \
	 RtecEventChannelAdmin RtecEventComm RtecScheduler TimeBase

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
