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

files =  ACE.java \
	 OS.java \
	 TimeValue.java \
	 EventHandler.java \
	 ProfileTimer.java \
	 TimerQueue.java \
	 GetOpt.java \
	 MessageType.java \
	 MessageBlock.java \
	 MessageQueue.java \
	 ThreadManager.java \
	 Semaphore.java \
	 Mutex.java \
	 RWMutex.java \
	 TaskFlags.java \
	 Task.java \
	 SOCKStream.java \
	 SOCKAcceptor.java \
	 SOCKConnector.java \
	 SvcHandler.java \
	 CreationStrategy.java \
	 AcceptStrategy.java \
	 ActivateStrategy.java \
	 Acceptor.java \
	 StrategyAcceptor.java \
	 Connector.java \
	 ServiceRepository.java \
	 ServiceObject.java \
	 ServiceConfig.java \
	 TimeoutException.java \
	 TimedWait.java

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
