# Makefile
# $Id$
 
.SUFFIXES: .java .class

VERSION = 1.5
JACE_WRAPPER = ..
CLASSDIR = $(JACE_WRAPPER)/classes
DOCDIR = $(JACE_WRAPPER)/doc

JC = javac
JCOPTS = -d $(CLASSDIR)
JD = javadoc
JDOPTS = -classpath $(CLASSDIR) -sourcepath $(JACE_WRAPPER) -windowtitle \
         "Java ACE $(VERSION) Documentation" -bottom \
         "Java ACE Version $(VERSION)<BR> \
         <A HREF="http://www.cs.wustl.edu/~schmidt"> \
         http://www.cs.wustl.edu/~schmidt</A>" \
         -doctitle "Java ACE Version $(VERSION) Documentation" -d $(DOCDIR) \
         -overview overview.html

COMPILE.java = $(JC) $(JCOPTS) $(filter %.java,$?)

CLASSPATH := $(CLASSDIR):$(CLASSPATH)

all:	asx os concurrency connection timers misc reactor svcconfig socksap netsvcs

pkg_all = $(pkg_asx_timestuff) $(pkg_asx) $(pkg_os) $(pkg_concurrency) \
          $(pkg_connection) $(pkg_timers) $(pkg_misc) $(pkg_reactor) \
          $(pkg_socksap) $(pkg_svcconfig) $(pkg_netsvcs)

doc:
	$(JD) $(JDOPTS) $(packages)

docclean:
	find ${JACE_WRAPPER}/doc -name '*.html' -print | xargs ${RM}

clean:
	find ${JACE_WRAPPER}/classes/JACE -name '*.class' -print | xargs ${RM}

realclean:	clean docclean


pkg_asx_timestuff = \
	TimeValue \
	TimeoutException \
	TimedWait

asx_timestuff:	$(addprefix ASX/,$(addsuffix .java,$(pkg_asx_timestuff)))
	$(COMPILE.java)

pkg_asx = \
	IOCntlCmds \
	IOCntlMsg \
	Task \
	TaskFlags \
	ThruTask \
	Module \
	MessageType \
	MessageBlock \
	MessageQueue \
	StreamHead \
	StreamTail \
	Stream

asx:	os asx_timestuff reactor concurrency \
        $(addprefix ASX/,$(addsuffix .java,$(pkg_asx)))
	$(COMPILE.java)

pkg_os = \
	OS \
	ACE

os:	$(addprefix OS/,$(addsuffix .java,$(pkg_os)))
	$(COMPILE.java)

pkg_concurrency = \
	AbstractLock \
	LockAdapter \
	LockException \
	WaitObject \
	RenewObject \
	Condition \
	Mutex \
	RWMutex \
	Semaphore \
	ThreadManager \
	Token

concurrency:	$(addprefix Concurrency/,$(addsuffix .java,\
                $(pkg_concurrency))) asx_timestuff os
	$(COMPILE.java)

pkg_connection = \
	SvcHandler \
	Acceptor \
	Connector \
	AcceptStrategy \
	ActivateStrategy \
	CreationStrategy \
	StrategyAcceptor \
	Blob \
	BlobHandler \
	BlobReader \
	BlobWriter \
	HTTPHelper

connection:	os socksap svcconfig $(addprefix Connection/,\
                $(addsuffix .java,$(pkg_connection)))
	$(COMPILE.java)

pkg_timers = \
	ProfileTimer

timers:	$(addprefix Timers/,$(addsuffix .java,$(pkg_timers)))
	$(COMPILE.java)

pkg_misc = \
	GetOpt

misc:	$(addprefix Misc/,$(addsuffix .java,$(pkg_misc)))
	$(COMPILE.java)

pkg_reactor = \
	EventHandler \
	TimerQueue

reactor:	asx_timestuff $(addprefix Reactor/,\
                $(addsuffix .java,$(pkg_reactor)))
	$(COMPILE.java)

pkg_socksap = \
	INETAddr \
	SOCKStream \
	SOCKAcceptor \
	SOCKConnector

socksap:	os $(addprefix SOCK_SAP/,\
                $(addsuffix .java,$(pkg_socksap)))
	$(COMPILE.java)

pkg_svcconfig = \
        Service \
        ServiceLoader \
        ServiceObject \
        ServiceConfig \
	ServiceRepository

svcconfig:	os reactor misc $(addprefix ServiceConfigurator/,\
                $(addsuffix .java,$(pkg_svcconfig)))
	$(COMPILE.java)

netsvcs:	netsvcs_base netsvcs_logger netsvcs_naming netsvcs_time \
                netsvcs_token

pkg_netsvcs = $(pkg_netsvcs_base) $(pkg_netsvcs_logger) \
              $(pkg_netsvcs_time) $(pkg_netsvcs_naming) $(pkg_netsvcs_token)

pkg_netsvcs_base = \
        Server \
        Handler

netsvcs_base:	os asx concurrency connection socksap svcconfig \
		$(addprefix netsvcs/,$(addsuffix .java,$(pkg_netsvcs_base)))
	$(COMPILE.java)

pkg_netsvcs_logger = \
        LogRecord \
        LoggingStrategy \
        StderrStrategy \
        ServerLoggingHandler \
        ServerLoggingAcceptor

netsvcs_logger:	netsvcs_base $(addprefix netsvcs/Logger/,\
		$(addsuffix .java,$(pkg_netsvcs_logger)))
	$(COMPILE.java)

pkg_netsvcs_naming = \
        NameRequest \
        NameReply \
        NameProxy \
        NameHandler \
        NameAcceptor

netsvcs_naming:	netsvcs_base $(addprefix netsvcs/Naming/,\
		$(addsuffix .java,$(pkg_netsvcs_naming)))
	$(COMPILE.java)

pkg_netsvcs_time = \
        TimeInfo \
        TimeRequest \
        TSClerkHandler \
        TSClerkProcessor \
        TSServerHandler \
        TSServerAcceptor

netsvcs_time:	netsvcs_base $(addprefix netsvcs/Time/,\
		$(addsuffix .java,$(pkg_netsvcs_time)))
	$(COMPILE.java)

pkg_netsvcs_token = \
        TokenRequest \
        TokenReply \
        TokenRequestHandler \
        LockHandler \
        LockHandlerAdapter \
        LockOperations \
        LockTypes \
        TokenAcceptor \
        RemoteLock \
        RemoteMutex \
        RemoteRWMutex \
        MutexHandler \
        RWMutexHandler

netsvcs_token:	netsvcs_base $(addprefix netsvcs/Token/,\
		$(addsuffix .java,$(pkg_netsvcs_token)))
	$(COMPILE.java)

packages = \
	   JACE.ASX \
	   JACE.Connection \
	   JACE.Concurrency \
	   JACE.Misc \
	   JACE.OS \
	   JACE.Reactor \
	   JACE.SOCK_SAP \
	   JACE.ServiceConfigurator \
	   JACE.Timers \
           JACE.netsvcs \
           JACE.netsvcs.Logger \
           JACE.netsvcs.Naming \
           JACE.netsvcs.Time \
           JACE.netsvcs.Token
