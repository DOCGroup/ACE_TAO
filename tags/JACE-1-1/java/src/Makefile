# Makefile
# $Id$
 
.SUFFIXES: .java .class

JACE_WRAPPER = ..
CLASSDIR = $(JACE_WRAPPER)/classes
DOCDIR = $(JACE_WRAPPER)/doc

JC = javac_g
JCOPTS = -g -d $(CLASSDIR)
JD = javadoc
JDOPTS = -d $(DOCDIR)

COMPILE.java = $(JC) $(JCOPTS) $(filter %.java,$?)

CLASSPATH := $(CLASSDIR):$(CLASSPATH)

all:	asx os concurrency connection timers misc reactor svcconfig socksap

pkg_all = $(pkg_asx_timestuff) $(pkg_asx) $(pkg_os) $(pkg_concurrency) \
          $(pkg_connection) $(pkg_timers) $(pkg_misc) $(pkg_reactor) \
          $(pkg_socksap) $(pkg_svcconfig)

doc:
	$(JD) $(JDOPTS) $(addsuffix .java,$(pkg_all)) $(packages)

clean:
	find ${JACE_WRAPPER}/classes/ACE -name '*.class' -print | xargs ${RM}

docclean:
	find ${JACE_WRAPPER}/doc -name '*.html' -print | xargs ${RM}

realclean:	clean docclean


pkg_asx_timestuff = \
	TimeValue \
	TimeoutException \
	TimedWait

asx_timestuff:	$(addsuffix .java,$(pkg_asx_timestuff))
	$(COMPILE.java)

pkg_asx = \
	Task \
	TaskFlags \
	MessageType \
	MessageBlock \
	MessageQueue

asx:	os asx_timestuff reactor concurrency $(addsuffix .java,$(pkg_asx))
	$(COMPILE.java)

pkg_os = \
	OS \
	ACE

os:	$(addsuffix .java,$(pkg_os))
	$(COMPILE.java)

pkg_concurrency = \
	Condition \
	Mutex \
	RWMutex \
	Semaphore \
	ThreadManager \
	Token

concurrency:	$(addsuffix .java,$(pkg_concurrency)) asx_timestuff os
	$(COMPILE.java)

pkg_connection = \
	SvcHandler \
	Acceptor \
	Connector \
	AcceptStrategy \
	ActivateStrategy \
	CreationStrategy \
	StrategyAcceptor

connection:	os socksap svcconfig $(addsuffix .java,$(pkg_connection))
	$(COMPILE.java)

pkg_timers = \
	ProfileTimer

timers:	$(addsuffix .java,$(pkg_timers))
	$(COMPILE.java)

pkg_misc = \
	GetOpt

misc:	$(addsuffix .java,$(pkg_misc))
	$(COMPILE.java)

pkg_reactor = \
	EventHandler \
	TimerQueue

reactor:	asx_timestuff $(addsuffix .java,$(pkg_reactor))
	$(COMPILE.java)

pkg_socksap = \
	INETAddr \
	SOCKStream \
	SOCKAcceptor \
	SOCKConnector

socksap:	os $(addsuffix .java,$(pkg_socksap))
	$(COMPILE.java)

pkg_svcconfig = \
	ServiceObject \
	ServiceConfig \
	ServiceRepository

svcconfig:	os reactor misc $(addsuffix .java,$(pkg_svcconfig))
	$(COMPILE.java)

packages = ACE \
	   ACE.ASX \
	   ACE.Connection \
	   ACE.Concurrency \
	   ACE.Misc \
	   ACE.OS \
	   ACE.Reactor \
	   ACE.SOCK_SAP \
	   ACE.Service_Configurator \
	   ACE.Timers


