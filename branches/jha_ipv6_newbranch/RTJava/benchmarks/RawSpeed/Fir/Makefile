#$Id$

JAVAC = javac
JAVADOC = javadoc
JAVAFLAGS += -classpath ../ -O
RM = rm
RMFLAGS = -f

DOC_HOME = doc
CLASS_PATH = .

SOURCE := $(wildcard *.java)
CLASSES := $(patsubst %.java,%.class,$(SOURCE))

PKG = Fir

default: all

all: $(CLASSES)

%.class: %.java
	$(JAVAC) $(JAVAFLAGS) $<

Doc:
	cd ..; $(JAVADOC) -d $(DOC_HOME) -classpath $(CLASS_PATH)  $(PKG); cd $(PKG)
clean:
	$(RM) *.class; $(RM) *~

removetmp:
	$(RM) $(RMFLAGS) *~
