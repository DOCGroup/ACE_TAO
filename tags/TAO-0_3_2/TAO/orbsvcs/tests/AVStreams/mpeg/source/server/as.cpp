// $Id$

#include "mpeg_server/Audio_Server.h"
#include "as.h"

ACE_RCSID(server, as, "$Id$")

int
Audio_Child_Process::init (int argc,
                           char **argv)
{
  if (TAO_AV_Child_Process_B<Audio_Server_StreamEndPoint,TAO_VDev,Audio_Control_i>::init (argc,argv) == -1)
    return -1;

  int max_pkt_size = -INET_SOCKET_BUFFER_SIZE;
  AUDIO_GLOBAL::instance ()->serviceSocket = -1;
    
  AUDIO_GLOBAL::instance ()->conn_tag = max_pkt_size;

  if (max_pkt_size < 0) 
    max_pkt_size = -max_pkt_size;
  else if (max_pkt_size == 0)
    max_pkt_size = 1024 * 1024;
    
  AUDIO_GLOBAL::instance ()->start_time = time(NULL);

  AUDIO_GLOBAL::instance ()->fbpara = (AudioFeedBackPara *)malloc(FBBUF_SIZE);
  if (AUDIO_GLOBAL::instance ()->fbpara == NULL) {
    perror("AS failed to allocate mem for fbpara");
    return (-1);
  }

  AUDIO_GLOBAL::instance ()->databuf_size = max_pkt_size - sizeof(AudioPacket);
  if (AUDIO_GLOBAL::instance ()->databuf_size > DATABUF_SIZE)
    AUDIO_GLOBAL::instance ()->databuf_size = DATABUF_SIZE;
  
  AUDIO_GLOBAL::instance ()->pktbuf = (AudioPacket *)malloc(sizeof(AudioPacket) + 
                                                     AUDIO_GLOBAL::instance ()->databuf_size);
  if (AUDIO_GLOBAL::instance ()->pktbuf == NULL) {
    perror("AS failed to allocate mem for pktbuf");
    return(-1);
  }
}

int
Audio_Child_Process::make_mediactrl (Audio_Control_i *& media_ctrl)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Audio_Child_Process::make_mediactrl ()\n"));
  media_ctrl = AUDIO_CONTROL_I::instance ();
  return 0;
}


int
main (int argc,char **argv)
{
  Audio_Child_Process audio_child;


  if (audio_child.init (argc,argv) == -1)
    return 1;
//   while  (1)
//     {
      int result = audio_child.run () ;
//       if (errno == EINTR)
//         continue;
//       else
//         break;
//     }
  return 0;
}
