# Makefile for JACE tests
# $Id$
 
.SUFFIXES: .java .class

JACE_WRAPPER = ../..
CLASSDIR = $(JACE_WRAPPER)/classes

JC = javac
JCOPTS = -d $(CLASSDIR)

COMPILE.java = $(JC) $(JCOPTS) $(filter %.java,$?)

CLASSPATH := $(CLASSDIR):$(CLASSPATH)

all:	asx concurrency condition connection misc reactor socksap svcconfig netsvcs

pkg_all = $(pkg_asx) $(pkg_concurrency) $(pkg_condition) $(pkg_connection) \
          $(pkg_misc) $(pkg_reactor) $(pkg_socksap) $(pkg_svcconfig) $(pkg_netsvcs)

clean:	
	find ${JACE_WRAPPER}/classes/JACE/tests -name '*.class' -print | xargs ${RM}

pkg_asx = \
	BufferStreamTest \
	MessageQueueTest \
	PriorityBufferTest \
	TaskTest \
	ThreadPoolTest

asx:	$(addprefix ASX/,$(addsuffix .java,$(pkg_asx)))
	$(COMPILE.java)

pkg_concurrency = \
	IterationTest \
	RWMutexTest \
	TimingTest \
	TokenTest

concurrency:	$(addprefix Concurrency/,$(addsuffix .java,$(pkg_concurrency)))
	$(COMPILE.java)

pkg_condition = \
	Consumer \
	JoinableThreadGroup \
	Producer \
	QueueTest \
	SimpleMessageQueue

condition:	$(addprefix Concurrency/Condition/,$(addsuffix .java,$(pkg_condition)))
	$(COMPILE.java)

pkg_connection = \
	AcceptorTest \
	ClientHandler \
	ConnectorTest \
	HTTPHelperTest \
	ServerHandler

connection:	$(addprefix Connection/,$(addsuffix .java,$(pkg_connection)))
	$(COMPILE.java)

pkg_misc = \
	ParseArgsTest

misc:	$(addprefix Misc/,$(addsuffix .java,$(pkg_misc)))
	$(COMPILE.java)

pkg_reactor = \
	TimeValueTest \
	TimerQueueTest

reactor:	$(addprefix Reactor/,$(addsuffix .java,$(pkg_reactor)))
	$(COMPILE.java)

pkg_socksap = \
	SOCKAcceptorTest \
	SOCKConnectorTest

socksap:	$(addprefix SOCK_SAP/,$(addsuffix .java,$(pkg_socksap)))
	$(COMPILE.java)

pkg_svcconfig = \
	Main

svcconfig:	$(addprefix ServiceConfigurator/,$(addsuffix .java,$(pkg_svcconfig)))
	$(COMPILE.java)

netsvcs:	logger_tests naming_tests time_tests token_tests

pkg_logger_tests = \
	LoggerTest

logger_tests:	$(addprefix netsvcs/Logger/,$(addsuffix .java,$(pkg_logger_tests)))
	$(COMPILE.java)

pkg_naming_tests = \
	ClientTest

naming_tests:	$(addprefix netsvcs/Naming/,$(addsuffix .java,$(pkg_naming_tests)))
	$(COMPILE.java)

pkg_time_tests = \
	TSClient

time_tests:	$(addprefix netsvcs/Time/,$(addsuffix .java,$(pkg_time_tests)))
	$(COMPILE.java)

pkg_token_tests = \
	ProxyClientTest

token_tests:	$(addprefix netsvcs/Token/,$(addsuffix .java,$(pkg_token_tests)))
	$(COMPILE.java)


