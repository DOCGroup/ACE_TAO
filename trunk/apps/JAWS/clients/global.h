#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

#include <ace/ACE.h>
#include <ace/OS.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Connector.h>
#include <ace/Pipe.h>
#include <ace/SOCK_Stream.h>
#include <ace/INET_Addr.h>
#include <ace/Profile_Timer.h>
#include <ace/Thread.h>
#include <ace/Thread_Manager.h>
#include <ace/Service_Config.h>
#include <ace/Log_Msg.h>

#include <math.h>

#define ERRLOG "http_tester.errors"

#ifndef extern_c_symbols
#define extern_c_symbols
extern "C" {

       int usleep(unsigned int useconds);
}
#endif










