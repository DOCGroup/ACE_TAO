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
	rm -rf $(JACE_WRAPPER)/classes/imaging/framework

files =	ImageFilterFactory.java \
	ImageProcessor.java \
	DialogType.java \
	URLFrame.java \
	DialogManager.java \
	ImageCanvas.java \
	ImageApp.java \
	BaseButton.java \
	FilePanel.java \
	StatusDisplay.java \
	Icon.java \
	Hierarchy.java \
	HierarchyAdapter.java \
	FileBrowser.java \
	ListFiles.java
