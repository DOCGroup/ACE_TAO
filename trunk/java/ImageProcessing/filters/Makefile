# Makefile
 
.SUFFIXES: .java .class

JACE_WRAPPER = ../..
CLASSDIR = $(JACE_WRAPPER)/classes
DOCDIR = $(JACE_WRAPPER)/doc

CLASSPATH := $(CLASSDIR):$(CLASSPATH)

all:
	javac -d ${JACE_WRAPPER}/classes $(files)

clean:
	rm -rf *~

realclean: clean  
	rm -rf $(JACE_WRAPPER)/classes/*.class

files =	MedFilter.java \
	Assert.java \
	BleachFilter.java \
	DarkenFilter.java \
	DissolveFilter.java \
	EmbossFilter.java \
	FivebyfiveFilter.java \
	MeanFilter.java \
        NinebynineFilter.java \
	RotateFilter.java \
	SharpenFilter.java \
	SobelFilter.java \
	SpatialFilter.java \
	Timer.java \
	ThreebythreeFilter.java \
	UnsharpFilter.java

