
# Makefile
 
.SUFFIXES: .java .class

JACE_WRAPPER = $(WRAPPER_ROOT)/java

all:
	javac -d ${JACE_WRAPPER}/classes $(files)
doc:
	javadoc -d ${JACE_WRAPPER}/doc $(files) $(packages)

files =  TSClerkProcessor.java \
	 TSClerkHandler.java \
	 TSRequestAcceptor.java \
	 TSRequestHandler.java \
	 TSServerAcceptor.java \
	 TSServerHandler.java \
	 Clerk.java \
	 Server.java

packages = netsvcs \
	   netsvcs.Time

realclean: 
	/bin/rm -rf ${JACE_WRAPPER}/classes/netsvcs/Time

