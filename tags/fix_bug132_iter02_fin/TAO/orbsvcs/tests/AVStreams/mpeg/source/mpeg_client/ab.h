//  $Id$

// ============================================================================
//
// = LIBRARY
//    mpeg_client
//
// = FILENAME
//    ab.h
//
// = DESCRIPTION
//     Defines the audio packet buffering process.
//
// = AUTHORS
//    Nagarajan Surendran (naga@cs.wustl.edu)
//
// ============================================================================

#if !defined (AV_AB_H)
#define AV_AB_H

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
#ifdef __svr4__
#include <stropts.h>
#include <sys/conf.h>
#endif

#include "include/common.h"
#include "newproto.h"
#include "global.h"
#include "mpeg_shared/filters.h"
#include "mpeg_shared/fileio.h"
#include "mpeg_shared/com.h"
#include "ace/OS.h"
#include "ace/Event_Handler.h"
#include "ace/Reactor.h"
#include "vb.h"

extern VideoBuffer *vbuffer;

/* magic number -- deviation is considered
   caused by clock drift only if rate <= 1/MAX_CLOCK_DRIFT.
   */
#define MAX_CLOCK_DRIFT 50

#define max(a,b) ((a)>(b) ? (a) : (b))
#define min(a,b) ((a)<(b) ? (a) : (b))
#define PACKET_SIZE 8192
#define STARTUP_WAIT 10000000
#define ACTION_WAIT 5000000

typedef struct {
  int bufsize; /* number of bytes for the buffer pointed by 'buf' */
  char * buf;  /* pointer to the data buffer area */
  int bps; /* current byte-per-sample */
  int size;  /* number of samples the buffer can hold */
  int samples; /* number of samples in the buffer; */
  int stuff; /* number of stuff samples to be read by ABgetSamples() */
  int ts;  /* tail-sample: the next sample to be comsumed by CTR */
  int hs;  /* head-sample: the next sample to be expected from the network */
  int tind; /* index of the ts-sample in the buf */
} ABBuffer;


class Notification_Handler;

class AudioBuffer :public ACE_Event_Handler
{
public:  
  AudioBuffer (void);
  // constructor.

  int ABprocess (int dataSocket);
  // start the buffering process.

  ~AudioBuffer (void);
  // destructor.

  virtual ACE_HANDLE get_handle (void) const;
  // get the underlying I/O handle.

  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // called when input arrives on the datasocket.

  virtual int handle_output (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // Called to send feedback.

  void set_silence(char *buf, int samples);

  void ABinitBuf(int size);

  void ABflushBuf(int nextSample);
  // flush the whole buffer 

  int ABcheckSamples(void);
  // returns # of samples in ABbuf

  int ABgetSamples(char * buf, int samples);
  // read at most given number of samples from AB to buf, returns
  // number of sample actually read 

  int ABskipSamples(int samples);
  // if samples < 0; then stuff |samples| silient samples to ABgetSamples(),
  //   otherwise wipe out this number of samples from AB. 

  void ABdeleteBuf(void);

  void ABdeleteSem(void);

  static void exit_on_kill(void);

  static void usr1_handler(int sig);

  static void usr2_handler(int sig);

  void feedback (void);

private:
  char *temp;
  // temporary pointer.
  int bytes;
  // temporary placeholder for no.of bytes to be read.
  ABBuffer *abuf;
  int sid;
  int exit_tag;
  int savedSocket;
  AudioPacket *packet;
  char *pkt_data;
  int conn_tag;
  /* following are for feedback */
  int fbstate;
  unsigned waketime;
  int pcmdsn;

  int dataSocket;
  // data socket handle.

  Notification_Handler *handler_;
  
  enum MODE {INVALID = -1,READ_HEADER,READ_DATA,WRITE_FEEDBACK1,WRITE_FEEDBACK2};
  int mode_;
};

class Audio_Notification_Handler :public Notification_Handler
{
public:
  virtual ACE_HANDLE get_handle (void) const;
  // Returns the audio notification handle.
};

#endif /* AV_AB_H */
