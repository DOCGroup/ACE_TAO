# $Id$

# Makefile

.SUFFIXES: .idl .module .java .class 

.java.class:
	$(VISIJAVA_ROOT)/bin/vbjc $<

.idl.module:
	$(VISIJAVA_ROOT)/bin/idl2java -DTAO_HAS_DYNAMIC_PROPERTY_BUG \
	-no_tie -no_skel -no_bind -no_comments -no_examples $<
	touch $@

default: all

clean: 
	rm -rf CosTrading CosTradingDynamic CosTradingRepos TAO_Sequences TAO_VR
	rm -f *.class *.tmp *.module *~

IDLS = \
	$(TAO_ROOT)/orbsvcs/orbsvcs/CosTrading.idl \
	$(TAO_ROOT)/orbsvcs/orbsvcs/Sequences.idl \
	$(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/server/Video_Repository.idl

MODULES = $(IDLS:.idl=.module)

SRCS = \
	TS_Resolve.java \
	Property_Evaluator.java \
	Discover_Server.java

CLASSES = $(SRCS:.java=.class) 

all:	$(MODULES) $(CLASSES)
