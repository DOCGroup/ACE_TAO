.SUFFIXES: .java .class

CLASSDIR = .
DOCDIR = .

JC = javac_g
JCOPTS = -g -d $(CLASSDIR)
JD = javadoc
JDOPTS = -d $(DOCDIR)

COMPILE.java = $(JC) $(JCOPTS) $<
DOCCOMP.java = $(JD) $(JDOPTS) $<

CLASSPATH := $(CLASSDIR):$(CLASSPATH)

CLASSES = LoggerConstants LogRecord LoggingHandler \
          LoggingAcceptor SimpleLogger LoggingClient
CLASSFILES = $(addsuffix .class,$(CLASSES))
DOCFILES = $(addsuffix .html,$(CLASSES))

%.class:	%.java
	$(COMPILE.java)

%.html:		%.java
	$(DOCCOMP.java)

all:	classes doc

classes:	$(CLASSFILES)
doc:		$(DOCFILES)

clean:
	$(RM) *~ $(CLASSFILES) $(DOCFILES)

