// $Id$

#include "mpeg_server/Audio_Server.h"

int
main (int argc,char **argv)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
  
  Audio_Server audio_server;

  if (audio_server.init (argc,
                         argv,
                         0,
                         -INET_SOCKET_BUFFER_SIZE) == -1)
    return 1;

  audio_server.run ();
  return 0;
}
