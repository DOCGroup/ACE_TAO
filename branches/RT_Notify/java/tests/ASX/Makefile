# Makefile
 
.SUFFIXES: .java .class

JACE_WRAPPER = ../..
CLASSDIR = $(JACE_WRAPPER)/classes

CLASSPATH := $(CLASSDIR):$(CLASSPATH)
 
all:
	javac -d ${JACE_WRAPPER}/classes $(files)
doc:
	javadoc -d ${JACE_WRAPPER}/doc $(files) $(packages)


files =	 MessageQueueTest.java \
	 TaskTest.java \
	 PriorityBufferTest.java \
	 ThreadPoolTest.java \
	 BufferStreamTest.java

packages = tests.ASX

realclean:
	find ${JACE_WRAPPER}/classes/tests/ASX -name '*.class' -print | xargs ${RM}

