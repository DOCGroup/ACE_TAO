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
    void on_exit_routine(void);

  public:
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

class Audio_Server
{
public:
  Audio_Server (void);
  // Default constructor
  Audio_Server (ACE_SOCK_Stream& control,
               int rttag,
               int max_pkt_size);
  // Taking the control,data fd's and rttag.
  int init (ACE_SOCK_Stream& control,
            int rttag,
            int max_pkt_size);
  // inits the Audio_Server
  int run (void);
  // runs the loop.
  int set_peer (void);
  // sets the peer endpoint.
private:
  Audio_Global audio_global_;
  ACE_SOCK_CODgram dgram_;
};
#endif
