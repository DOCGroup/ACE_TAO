# Makefile
 
.SUFFIXES: .java .class

JACE_WRAPPER = ../..
CLASSDIR = $(JACE_WRAPPER)/classes

CLASSPATH := $(CLASSDIR):$(CLASSPATH)
 
all:
	javac -d ${JACE_WRAPPER}/classes $(files)
doc:
	javadoc -d ${JACE_WRAPPER}/doc $(files) $(packages)


files =	 SOCKConnectorTest.java \
	 SOCKAcceptorTest.java

packages = tests.SOCK_SAP

realclean:
	find ${JACE_WRAPPER}/classes/tests/SOCK_SAP -name '*.class' -print | xargs ${RM}
