# Makefile
 
.SUFFIXES: .java .class

JACE_WRAPPER = ../..
CLASSDIR = $(JACE_WRAPPER)/classes

CLASSPATH := $(CLASSDIR):$(CLASSPATH)
 
all:
	javac -d ${JACE_WRAPPER}/classes $(files)
doc:
	javadoc -d ${JACE_WRAPPER}/doc $(files) $(packages)


files =	 ParseArgsTest.java

packages = tests.Misc

realclean:
	find ${JACE_WRAPPER}/classes/tests/Misc -name '*.class' -print | xargs ${RM}
