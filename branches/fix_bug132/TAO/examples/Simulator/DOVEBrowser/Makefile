#----------------------------------------------------------------------------
#
#       $Id$
#
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

#### VisiBroker
ifdef VBROKER_ADM
  JAVA_DIR = $(VBROKER_ADM)/../bin/
  CLASSPATH := $(VBROKER_ADM)/../lib/:$(VBROKER_ADM)/../bin/:$(CLASSPATH)
else  # ! VBROKER_ADM
  JAVA_DIR = /project/doc/pkg/VisiJava3.4/bin/
endif # ! VBROKER_ADM
IDL2JAVA = $(JAVA_DIR)idl2java
JAVAC = $(JAVA_DIR)vbjc

#### JDK 1.2
#### JAVA_DIR = /pkg/java_1.2b4/bin/
#### IDL2JAVA = $(JAVA_DIR)idltojava
#### JAVAC = $(JAVA_DIR)javac

DOVE_BROWSER_JAVA = \
        CosNaming.java \
        TimeBase.java \
        RtecDefaultEventData.java \
        RtecBase.java \
        RtecEventComm.java \
        RtecScheduler.java \
        RtecEventChannelAdmin.java \
        Navigation.java \
        Weapons.java \
        Persian.java

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

  realclean: clean
	-$(RM) -r $(DOVE_BROWSER_JAVA) \
         $(patsubst %.java,%.idl,$(DOVE_BROWSER_JAVA)) NavWeap.idl \
         Navigation.java NavigationH*.java \
         Weapons.java WeaponsH*.java *.class CosNaming PersianRecursion \
         RtecBase RtecEventChannelAdmin RtecEventComm RtecScheduler TimeBase \
         NavWeapTerminator*.java _*.java \
         EventPayloadH*.java RtecEventData*.java

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
	@$(IDL2JAVA) -I$(TAO_ROOT)/orbsvcs/orbsvcs -I$(TAO_ROOT) $<

#### VisiBroker 3.2 idl2java creates references to a _long type,
#### but never defines it.  Just change it to long if it's present.
files=$(find . -name '*.java' | xargs egrep -l ' _long')
ifneq (,$(files))
  %.class: %.java
	perl -pi -e 's/\b_long\b/long/g' $(files)
else  # ! vbjc
  %.class: %.java
endif # vbjc
	@$(JAVAC) *.java

Navigation.java Weapons.java: NavWeap.idl
	@$(IDL2JAVA) -I$(TAO_ROOT)/orbsvcs/orbsvcs -I$(TAO_ROOT) $<

%.idl: ../%.idl
	@cp -p $< .

%.idl: $(TAO_ROOT)/orbsvcs/orbsvcs/%.idl
	@cp -p $< .

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
