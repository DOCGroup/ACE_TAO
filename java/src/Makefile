# Makefile
 
.SUFFIXES: .java .class

JACE_WRAPPER = ..
CLASSDIR = $(JACE_WRAPPER)/classes
DOCDIR = $(JACE_WRAPPER)/doc

CLASSPATH := $(CLASSDIR):$(CLASSPATH)

all:
	javac -d ${JACE_WRAPPER}/classes $(files)
doc:
	javadoc -d ${JACE_WRAPPER}/doc $(files) $(packages)

files = \
         ACE.java \
         AcceptStrategy.java \
         Acceptor.java \
         ActivateStrategy.java \
         Condition.java \
         Connector.java \
         CreationStrategy.java \
         EventHandler.java \
         GetOpt.java \
         MessageBlock.java \
         MessageQueue.java \
         MessageType.java \
         Mutex.java \
         OS.java \
         ProfileTimer.java \
         RWMutex.java \
         SOCKAcceptor.java \
         SOCKConnector.java \
         SOCKStream.java \
         Semaphore.java \
         ServiceConfig.java \
         ServiceObject.java \
         ServiceRepository.java \
         StrategyAcceptor.java \
         SvcHandler.java \
         Task.java \
         TaskFlags.java \
         ThreadManager.java \
         TimeValue.java \
         TimedWait.java \
         TimeoutException.java 

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


clean:
	find ${JACE_WRAPPER}/classes/ACE -name '*.class' -print | xargs ${RM}

docclean:
	find ${JACE_WRAPPER}/doc -name '*.html' -print | xargs ${RM}

realclean:	clean docclean
