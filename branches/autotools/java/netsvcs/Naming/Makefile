# Makefile
# $Id$

.SUFFIXES: .java .class

JACE_WRAPPER = $(WRAPPER_ROOT)/java

all:
	javac -d ${JACE_WRAPPER}/classes $(files)
doc:
	javadoc -d ${JACE_WRAPPER}/doc $(files) $(packages)

files = NameAcceptor.java \
	NameHandler.java \
	NameRequest.java \
	NameReply.java \
	NameProxy.java \

packages = netsvcs \
	   netsvcs.Naming

realclean: 
	/bin/rm -rf ${JACE_WRAPPER}/classes/netsvcs/Naming

