/* $Id$ */

// ============================================================================
//
// = LIBRARY
//    as
//
// = FILENAME
//    as.h
//
// = DESCRIPTION
//   @@ Please add a synopsis of this file.
//
// = AUTHORS
//    Sumedh Mungee (sumedh@cs.wustl.edu)
//    Nagarajan Surendran (naga@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_AV_AS_H
#define TAO_AV_AS_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_CODgram.h"
#include "ace/Select_Reactor.h"

#include "include/common.h"
#include "mpeg_server/server_proto.h"
#include "mpeg_shared/fileio.h"
#include "mpeg_shared/routine.h"
#include "mpeg_shared/com.h"
#include "mpeg_server/Audio_Control_i.h"
#include "orbsvcs/orbsvcs/Naming/Naming_Utils.h"

#include "mpeg_server/Audio_Server.h"
#include "orbsvcs/AV/AVStreams_i.h"

class Audio_Child_Process
  :public TAO_AV_Child_Process_B <Audio_Server_StreamEndPoint,TAO_VDev,Audio_Control_i>
{
public:
  int init (int argc,
            char **argv);
  // init the Video Singleton.

  int make_mediactrl (Audio_Control_i *&media_ctrl);
};


#endif /* TAO_AV_AS_H */
