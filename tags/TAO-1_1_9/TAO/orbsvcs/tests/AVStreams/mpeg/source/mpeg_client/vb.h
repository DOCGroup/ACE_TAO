// $Id$

// ============================================================================
//
// = LIBRARY
//    mpeg_client
//
// = FILENAME
//    vb.h
//
// = DESCRIPTION
//     Defines the video packet buffering process.
//
// = AUTHORS
//    Nagarajan Surendran (naga@cs.wustl.edu)
//
// ============================================================================

#if !defined (AV_VB_H)
#define AV_VB_H

#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>
#if defined(__svr4__) || defined(IRIX)
#include <stropts.h>
#include <sys/conf.h>
#endif
#include "include/common.h"
#include "newproto.h"
#include "global.h"
#include "mpeg_shared/filters.h"
#include "mpeg_shared/fileio.h"
#include "mpeg_shared/com.h"
#include "ace/Event_Handler.h"
#include "ace/Reactor.h"

/* magic number -- deviation is considered
   caused by clock drift only if rate <= 1/MAX_CLOCK_DRIFT.
   */
#define MAX_CLOCK_DRIFT 50
#define SHCODE 0x000001b3
#define max(a,b) ((a)>(b) ? (a) : (b))

struct block
{
  unsigned shcode;
  struct block * next;
  int full;
};

struct header
{
  struct block *h, *t;
};

class Notification_Handler;

class VideoBuffer :public ACE_Event_Handler
{
public:
  VideoBuffer (void);
  // constructor.

  virtual ~VideoBuffer (void);
  // destructor.

  virtual int handle_input (ACE_HANDLE fd);
  // callback when data arrives on the video socket.

  virtual int handle_output (ACE_HANDLE fd);
  // callback when data can be written on the video socket.

  ACE_HANDLE get_handle (void) const;
  // gets the video socket.
 
  void VBinitBuf (int size);
  // size in byte.

  char * VBgetBuf (int size);
  // block version.

  int VBcheckBuf (int size);
  // non-block check, return True/False

  void VBputMsg (char * msgPtr);

  char * VBgetMsg ();
  // block version.

  int  VBcheckMsg();
  // non-block check, return Number of Msgs in buffer.

  int VBbufEmpty(void);

  void VBreclaimMsg(char * msgPtr);

  static void VBdeleteBuf(void);

  static void VBdeleteSem(void);

  static void exit_on_kill(void);

  int  VBprocess(int initSocket, int normalSocket);

private:

  int sync_feedback (void);
  int feedback_action (void);

  static block ** head;
  static block ** tail;
  static char * buf;
  static int bufsize;
  static int sid, countid;
  static int exit_tag;
  static int conn_tag;
  static int savedSocket;

  VideoMessage *msg, msghd;
  VideoPacket *packet;
  int len, bsize, msgsn;
  int psize, poffset;
  char * ptr, *ptr1;
  char *tmp_buf;
  int dataSocket;
  int initSocket,normalSocket;
  
  // following variables are for feedback 
  int pcmdsn, pcmd; 
  // to store the cmdsn and cmd of previous frame,
  // to detect new commands 

  int pfid, pgop;
  // frame-id of gopid of previous frame 

  int cmdsn;
  // cmdsn of previous command 

  int fb_state;
  // state of the control law/regulator 

  int startpos;
  // gopid or frameid when the regulator is in 'start' state 1 

  int delay;
  // #frames to delay after 'start' 

  int qosRecomputes;
  // indicate if qos has been recomputed recently 

  int action_delay;
  // microseconds to delay after feedback action 

  int action_time;
  // the time when feedback action is taken, used to enforce
  // action_delay 

  int upf, cupf;
  // upf -- currentUPF as carried in all frames,
  // cupf -- real current UPF, being adjusted by FB 

  double fv;
  // current buffer fill level in term of #frames 	

  double init_fv;
  // init_fv and init_pos record the fv value and position
  // (frameid or gopid) when fv is very close to med 

  double init_pos;
  int high, low, med;
  // median, high and low water marks, in term of #frames 

  int period;
  // interms of #frames, (high-med, period) defined the max
  // buffer fill drift which can be assumed as caused by
  // clock drift 

  Filter * f;
  // buffer fill level filter 

  int advance, min_advance;
  // in microseconds, advance holds the
  // current target buffer-fill-level as
  // determined by jitter level
  // estimate. min_advance sets a absolute
  // minimun level, no matter how low jitter
  // level is.'advance' does not change when
  // play-speed changes, but med/high/low
  // will be updated upon speed change 

  double fav;
  // #frames, jitter level output by the filter.
 
  Filter * fa;
  // jitter filter.
 
  int reach_limit;
  // indicate that the target 'advance' has reached client VB
  // buffer limit, and can not further increase, even if
  // jitter increases. 

  int not_action;
  // indicates entering state=3 is not caused by
  // feedback action. If not_action = 1 entering state=3
  // is caused either by 'start' or speed change, and
  // the jitter filter needs to be reset. 

#ifdef STAT
  int to_count;
  int gap_msgsn;
#endif

  Notification_Handler *handler_;
  // handler to be used for exit notification.

  enum state {INVALID = -1,
              READ_HEADER = 0,
              READ_NEXT_HEADER = 1,
              READ_MESSAGE = 2,
              SKIP_MESSAGE = 3,
              SKIP_NEXT_MESSAGE = 4,
              WRITE_FEEDBACK1 = 5,
              WRITE_FEEDBACK2 = 6};
  state state_;

  char *temp;
  // temporary pointer.
  int bytes;
  // temporary placeholder for no.of bytes to be read.

  int fb_addupf;
  int fb_addf;
  int fb_advance;
  
  int socket_flag_;
  // flag to indicate which socket to be used.
};

class Notification_Handler :public ACE_Event_Handler
{
public:
  virtual ACE_HANDLE get_handle (void) const;
  // Get the Notification handle.

  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // called when input events occur.

};  

class Video_Notification_Handler :public Notification_Handler
{
public:
  virtual ACE_HANDLE get_handle (void) const;
  // Returns the video notification handle.
};
#endif /* AV_VB_H */
