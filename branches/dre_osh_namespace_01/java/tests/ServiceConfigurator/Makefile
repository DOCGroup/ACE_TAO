# Makefile
 
.SUFFIXES: .java .class

JACE_WRAPPER = ../..
CLASSDIR = $(JACE_WRAPPER)/classes

CLASSPATH := $(CLASSDIR):$(CLASSPATH)
 
all:
	javac -d ${JACE_WRAPPER}/classes $(files)
doc:
	javadoc -d ${JACE_WRAPPER}/doc $(files) $(packages)


files =	 TestService.java \
	 reloadTest.java \
	 suspendTest.java

packages = tests.Service_Configurator

realclean:
	find ${JACE_WRAPPER}/classes/tests/ServiceConfigurator -name '*.class' -print | xargs ${RM}


