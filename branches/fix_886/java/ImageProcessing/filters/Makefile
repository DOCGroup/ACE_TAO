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
	rm -rf $(JACE_WRAPPER)/classes/imaging/filters/*.class

files =	MedFilter.java \
	Assert.java \
	BleachFilter.java \
	BlurFilter.java \
	DarkenFilter.java \
	DeSpeckleFilter.java \
	DissolveFilter.java \
	EdgeDetectFilter.java \
	EmbossFilter.java \
	OilPaintFilter.java \
	PixelizeFilter.java \
	RotateFilter.java \
	SharpenFilter.java \
	SpatialFilter.java \
	SpreadFilter.java \
	Timer.java 

