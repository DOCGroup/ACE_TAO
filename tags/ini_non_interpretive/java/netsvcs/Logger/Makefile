# $Id$

.SUFFIXES: .java .class

JACE_WRAPPER = $(ACE_ROOT)/java

all:
	javac -d ${JACE_WRAPPER}/classes $(files)
doc:
	javadoc -d ${JACE_WRAPPER}/doc $(files) $(packages)

files = LogMessageReceiver.java \
	DefaultLMR.java \
	LogRecord.java \
	ServerLoggingAcceptor.java \
	ServerLoggingHandler.java

packages = netsvcs \
	   netsvcs.Logger

realclean:
	/bin/rm -rf ${JACE_WRAPPER}/classes/netsvcs/Logger

