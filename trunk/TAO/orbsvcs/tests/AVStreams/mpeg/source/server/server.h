
#if !defined (_MPEG_SERVER_H)
#define      _MPEG_SERVER_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <time.h>
#include <sys/time.h>
#include "../include/common.h"
#include "../mpeg_server/proto.h"
#include "fileio.h"
#include "routine.h"
#include "com.h"
#include "ace/Get_Opt.h"

#ifdef NATIVE_ATM
#include "atmcom.h"
#endif

class Mpeg_Server
{
  // =TITLE
  //   Defines a class that abstracts the functionality of a mpeg
  //   video and audio server.
public:
  Mpeg_Server ();
  int init (int argc,
            char **argv);
  // Initialize the mpeg_server

  int run ();
  // Run the Mpeg_Server

  static  void int_handler (int sig);
  static  void on_exit_routine (void);
  static  void clear_child (int sig);
  // various signal handlers
  // we need them to be static 
  static void init_static (void);

  ~Mpeg_Server ();
private:
  int parse_args (int argcs,
                  char **argv);

  int set_signals (void);

  // data members
  int inet_port_;

  char *unix_port_;
  char* atm_port_;

  struct FdTable
  {
    int fd;   /* -1 -- free item, >=0 -- fd value */
    int type;
    unsigned stime;  /* setup time, in seconds */
    int state;
  } * fdTable_;

  int size_;
  int rttag_;         
};

#endif // _MPEG_SERVER_H
