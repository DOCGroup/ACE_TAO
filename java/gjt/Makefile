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
	rm -rf $(JACE_WRAPPER)/classes/gjt

files =	Assert.java \
	Border.java \
	BulletinLayout.java \
	CardPanel.java \
	ChoiceCardPanel.java \
	ColumnLayout.java \
	ComponentScroller.java \
	DialogClient.java \
	DrawingPanel.java \
	DrawnRectangle.java \
	EtchedBorder.java \
	EtchedRectangle.java \
	Etching.java \
	ExclusiveImageButtonPanel.java \
	FontDialog.java \
	GJTDialog.java \
	IconCardPanel.java \
	ImageButton.java \
	ImageButtonController.java \
	ImageButtonEvent.java \
	ImageButtonPanel.java \
	ImageButtonPanelController.java \
	ImageCanvas.java \
	ImageScroller.java \
	LabelCanvas.java \
	MessageDialog.java \
	MouseController.java \
	Orientation.java \
	ProgressDialog.java \
	QuestionDialog.java \
	RadioImageButtonPanelController.java \
	RowLayout.java \
	Scroller.java \
	ScrollerLayout.java \
	SelectionEvent.java \
	Separator.java \
	SpringyImageButtonController.java \
	StateButton.java \
	StateButtonController.java \
	StickyImageButtonController.java \
	Stopwatch.java \
	StopwatchClient.java \
	ThreeDBorder.java \
	ThreeDBorderStyle.java \
	ThreeDRectangle.java \
	Toolbar.java \
	Util.java \
	YesNoDialog.java
