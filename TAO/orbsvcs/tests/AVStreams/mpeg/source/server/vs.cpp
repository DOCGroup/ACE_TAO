// $Id$

#include "mpeg_server/Video_Server.h"
#include "vs.h"
 
ACE_RCSID(server, vs, "$Id$")

int
Video_Child_Process::init (int argc,
                           char **argv)
{
  if (TAO_AV_Child_Process_B<Video_Server_StreamEndPoint,TAO_VDev,Video_Control_i>::init (argc,argv) == -1)
    return -1;

  int max_pkt_size = -INET_SOCKET_BUFFER_SIZE;
  VIDEO_SINGLETON::instance ()->serviceSocket = -1;
    
  VIDEO_SINGLETON::instance ()->conn_tag = max_pkt_size;
  
  if (max_pkt_size > 0) 
    VIDEO_SINGLETON::instance ()->msgsize 
      = max_pkt_size;
  else 
    if (max_pkt_size < 0) 
      VIDEO_SINGLETON::instance ()->msgsize 
        = - max_pkt_size;
    else 
      VIDEO_SINGLETON::instance ()->msgsize 
        = 1024 * 1024;
  /*
    SFprintf(stderr, "VS VIDEO_SINGLETON::instance ()->msgsize = %d\n", VIDEO_SINGLETON::instance ()->msgsize);
  */
  VIDEO_SINGLETON::instance ()->msgsize -= sizeof(VideoMessage);
  
  VIDEO_SINGLETON::instance ()->start_time =ACE_OS::time (NULL);

  VIDEO_SINGLETON::instance ()->lastRef [0] 
    = VIDEO_SINGLETON::instance ()->lastRef[1] 
    = -1;

  VIDEO_SINGLETON::instance ()->lastRefPtr = 0;

  
}
int
Video_Child_Process::make_mediactrl (Video_Control_i *& media_ctrl)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Video_Child_Process::make_mediactrl ()\n"));
  media_ctrl = VIDEO_CONTROL_I::instance ();
  return 0;
}

int
main (int argc, char **argv)
{
  Video_Child_Process video_child;

  if (video_child.init (argc,argv) == -1)
    return 1;
//   while (1)
//     {
      video_child.run ();
//       if (errno == EINTR)
//         continue;
//       else
//         break;
//     }

  return 0;
}
