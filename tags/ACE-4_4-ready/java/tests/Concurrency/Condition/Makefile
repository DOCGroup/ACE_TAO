# Makefile
 
.SUFFIXES: .java .class

JACE_WRAPPER = ../../..
CLASSDIR = $(JACE_WRAPPER)/classes

CLASSPATH := $(CLASSDIR):$(CLASSPATH)

all:
	javac -d ${JACE_WRAPPER}/classes $(files)
doc:
	javadoc -d ${JACE_WRAPPER}/doc $(files) $(packages)

files =	\
	QueueTest.java \
	JoinableThreadGroup.java \
	SimpleMessageQueue.java \
	Producer.java \
	Consumer.java 

packages = tests.Concurrency.Condition;

realclean:
	find ${JACE_WRAPPER}/classes/tests/Concurrency/Condition -name '*.class' -print | xargs ${RM}
