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

files =	ImageProcessor.java \
	DialogType.java \
	LoadURLFrame.java \
	SaveURLFrame.java \
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
	ListFiles.java \
	IntHashtable.java \
	ImageEncoder.java \
	GifEncoder.java \
	ImageByteCounter.java \
	StatusIndicator.java \
	GIFHandler.java \
	ImageSender.java \
	ServerTest.java \
	ServerHandler.java \
	Tester.java \
	TestHandler.java
