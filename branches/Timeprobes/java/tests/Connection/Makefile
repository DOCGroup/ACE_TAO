# Makefile
 
.SUFFIXES: .java .class

JACE_WRAPPER = ../..
CLASSDIR = $(JACE_WRAPPER)/classes

CLASSPATH := $(CLASSDIR):$(CLASSPATH)
 
all:
	javac -d ${JACE_WRAPPER}/classes $(files)
doc:
	javadoc -d ${JACE_WRAPPER}/doc $(files) $(packages)


files =	 ServerHandler.java \
	 ClientHandler.java \
	 ConnectorTest.java \
	 AcceptorTest.java

packages = tests.Connection

realclean:
	find ${JACE_WRAPPER}/classes/tests/Connection -name '*.class' -print | xargs ${RM}
