#
# COS Naming Service Implementation
#

include /home/cs/student/marina/CONFIG_MT/orb.mk

LOGGER_OBJS     = loggerS.o logger_tie.o logger-main.o CosNamingC.o
SERVER_OBJS	= CosNamingS.o NS_CosNaming.o server-main.o 
CLIENT_OBJS	= CosNamingC.o loggerC.o Client.o 
C++FLAGS	+= -I$(ACE_ROOT) 
LDFLAGS 	+= -lACE

all:	nameclient nameserver logger_factory

logger_factory: $(LOGGER_OBJS)
	$(C++) $(C++FLAGS) -o logger_factory $(LOGGER_OBJS) $(LDFLAGS) $(ITSRV)\
					$(SYSLIBS)


nameclient: $(CLIENT_OBJS)
	$(C++) $(C++FLAGS) -o nameclient $(CLIENT_OBJS) $(LDFLAGS) $(ITCLT)\
					$(SYSLIBS)

nameserver: $(SERVER_OBJS)
	$(C++) $(C++FLAGS) -o nameserver $(SERVER_OBJS) $(LDFLAGS) $(ITSRV) \
					$(SYSLIBS)

#purify -cache-dir=/tmp
clean:
	rm -rf CosNamingC.* CosNamingS.* CosNaming.hh logger.hh loggerC.* loggerS.*
	rm -rf Templates.DB core *.o nameclient nameserver logger_factory *~



