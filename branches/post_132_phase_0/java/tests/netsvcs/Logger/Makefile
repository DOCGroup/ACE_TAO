# $Id$

.SUFFIXES: .java .class

JACE_WRAPPER = $(ACE_ROOT)/java

all:
	javac -d ${JACE_WRAPPER}/classes $(files)
doc:
	javadoc -d ${JACE_WRAPPER}/doc $(files) $(packages)

files = LoggerTest.java

packages = netsvcs \
	   netsvcs.Logger

realclean:
	/bin/rm -rf ${JACE_WRAPPER}/classes/tests/netsvcs/Logger

