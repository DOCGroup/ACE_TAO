// $Id$

#if !defined (TAO_AV_BENCH_SERVER_H)
#define TAO_AV_BENCH_SERVER_H


#include "ace/Get_Opt.h"
#include "ace/Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_CODgram.h"
#include "ace/Select_Reactor.h"
#include "orbsvcs/orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "client.h"

class Server
{
public:
  Server (void);
  // Default constructor
  
  int init (int argc,
            char **argv,
            CORBA::Environment& env);
  
  int run (CORBA::Environment& env);
  
  ~Server (void);
private:
  TAO_ORB_Manager orb_manager_;
  // the TAO ORB manager.

  CosNaming::NamingContext_var naming_context_;
  // The root naming context of the naming service

  ACE_Process_Options process_options_;
  // The process options for the process to be spawned by the process strategy

  TAO_AV_Endpoint_Process_Strategy_B process_strategy_;
  // The proces strategy for the video.

  TAO_MMDevice *mmdevice_;
  // The video server multimedia device
};

#endif /* TAO_AV_BENCH_SERVER_H */
