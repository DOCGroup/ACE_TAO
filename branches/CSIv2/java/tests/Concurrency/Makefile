# Makefile
 
.SUFFIXES: .java .class

JACE_WRAPPER = ../..
CLASSDIR = $(JACE_WRAPPER)/classes

CLASSPATH := $(CLASSDIR):$(CLASSPATH)
 
all:
	javac -d ${JACE_WRAPPER}/classes $(files)
doc:
	javadoc -d ${JACE_WRAPPER}/doc $(files) $(packages)

files =	 MutexTest.java \
	 SemaphoreTest.java \
	 RWMutexTest.java \
	 TokenTest.java

packages = tests.Concurrency

realclean:
	find ${JACE_WRAPPER}/classes/tests/Concurrency -name '*.class' -print | xargs ${RM}
