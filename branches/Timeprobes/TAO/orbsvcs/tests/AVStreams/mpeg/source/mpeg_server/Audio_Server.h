/* -*- C++ -*- */
//$Id$

#if !defined (_AUDIO_SERVER_H)
#define _AUDIO_SERVER_H

#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include "include/common.h"
#include "mpeg_shared/fileio.h"
#include "mpeg_shared/routine.h"
#include "mpeg_shared/com.h"
#include "server_proto.h"

#include "ace/SOCK_CODgram.h"
#include "ace/SOCK_Stream.h"
#include "tao/TAO.h"

#define DATABUF_SIZE 500

#define SPEEDUP_SCALE 5
#define MAX_RESEND_REQS 10
#define FBBUF_SIZE (sizeof(AudioFeedBackPara) + \
                    (MAX_RESEND_REQS - 1) * sizeof(APdescriptor))


  class Audio_Global
  {
  public:
    Audio_Global (void);
    // Default constructor.
    int CmdRead(char *buf, int psize);
    void CmdWrite(char *buf, int size);
    int INITaudio(void);
    int send_packet(int firstSample, int samples);
    int SendPacket(void);
    void ResendPacket(int firstsample, int samples);
    int PLAYaudio(void);
    int play_audio (void);
    int send_audio (void);
    void on_exit_routine(void);

  public:

    enum audio_state {AUDIO_WAITING = 0,AUDIO_PLAY =1};

    audio_state state;
    // The state of the audio server.

    int addSamples;
    unsigned nextTime;
    int upp;  /* micro-seconds per packet */
    int delta_sps ; 
    // The members previously in PLAY audio.

    int bytes_sent ;
    time_t start_time;

    int conn_tag;

    int serviceSocket;
    int audioSocket ;

    char audioFile[PATH_SIZE];
    int fd;
    AudioParameter audioPara;
    int totalSamples;
    int fileSize;
    unsigned char cmd;
    int live_source ;

    int databuf_size;
    int cmdsn;
    int nextsample;
    int sps; /* audio play speed given by the client: samples per second */
    int spslimit;
    int spp; /* samples per packet */
    AudioPacket * pktbuf ;
    AudioFeedBackPara * fbpara ;
  };

class Audio_Sig_Handler 
  : public virtual ACE_Event_Handler
{
public:
  Audio_Sig_Handler (Audio_Global *audio_global);
  // Constructor.

  virtual ACE_HANDLE get_handle (void) const;

  int register_handler (void);
  // this will register this sig_handler
  // with the reactor for SIGALRM

  virtual int shutdown (ACE_HANDLE, 
                        ACE_Reactor_Mask);

  virtual int handle_input (ACE_HANDLE);

  virtual int handle_signal (ACE_HANDLE signum,
                             siginfo_t * = 0,
                             ucontext_t* = 0);
private:
  ACE_HANDLE handle_;
  // my handle
  Audio_Global *audio_global_;
};

class Audio_Data_Handler : public virtual ACE_Event_Handler
{
public:
  Audio_Data_Handler (ACE_HANDLE data_fd,
                      Audio_Global *audio_global);
                      
  // Constructor

  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // Called when data shows up.

  virtual ACE_HANDLE get_handle (void) const;
  // Get the handle used by this event handler

private:
  ACE_HANDLE data_fd_;
  Audio_Global *audio_global_;
};

class Audio_Control_Handler : public virtual ACE_Event_Handler
{
public:
  Audio_Control_Handler (ACE_HANDLE fd,
                         Audio_Global *audio_global);
  // Constructor

  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // Called when data shows up.

  virtual ACE_HANDLE get_handle (void) const;
  // Get the handle used by this event handler

private:
  ACE_HANDLE control_fd_;
  // The underlying TCP socket.
  Audio_Global *audio_global_;
  // Pointer to the global object.
};


class Audio_Server
{
public:
  Audio_Server (void);
  // Default constructor
  Audio_Server (int argc,
                char **argv,
                //ACE_SOCK_Stream& control,
               int rttag,
               int max_pkt_size);
  // Taking the control,data fd's and rttag.
  int init (int argc,
            char **argv,
            // ACE_SOCK_Stream& control,
            int rttag,
            int max_pkt_size);
  // inits the Audio_Server
  int run (void);
  // runs the loop.
  int set_peer (void);
  // sets the peer endpoint.
private:
  int parse_args (int argc,
                  char **argv);
  // Parse the arguments.

  TAO_ORB_Manager orb_manager_;
  // The ORB Manager.

  Audio_Global *audio_global_;
  // The globals object.

  Audio_Control_Handler *control_handler_;
  // The audio control handler

  Audio_Data_Handler *data_handler_;
  // The audio data i.e feedback handler.

  Audio_Sig_Handler *sig_handler_;
  // The signal handler.

  ACE_SOCK_Stream control;
  // The control stream.
  ACE_SOCK_CODgram dgram_;
  // The data socket.
};

#endif /*_AUDIO_SERVER_H */
