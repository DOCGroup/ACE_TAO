#if !defined (MPEG_GLOBAL_H)
#define MPEG_GLOBAL_H

// Global symbols, these need to become enums eventually.

#define SESSION_NUM     4
#define CONN_INET	1
#define CONN_UNIX	2
#define CONN_ATM	3

#define SERVER_FDTABLE_SIZE	50
#define CLIENT_FDTABLE_SIZE	10

#define STATE_PENDING	1
#define STATE_CONTROL	2
#define STATE_DATA	3
#define STATE_SINGLE	4

// Global definitions

#include <sys/types.h>
#include <sys/socket.h>

class Mpeg_Global
{
public:
  static int parentpid;

  static int listenSocketIn;
  static int listenSocketUn;

  static struct linger linger;

  static int live_audio;
  static int live_video;  /* 0 - no,  1 - to open,  2 - opened */

  static int drift_ppm;  /* clock drift in ppm */

  static int session_limit;
  static int session_num;
};


#endif /* define MPEG_GLOBAL_H */

