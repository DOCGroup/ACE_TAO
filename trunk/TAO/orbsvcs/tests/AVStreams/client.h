/* -*- C++ -*- */

/* $Id$ */

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams
//
// = FILENAME
//    client.h
//
// = DESCRIPTION 
//    Test client for the AVStreams demo
//   
// = AUTHORS
//   Sumedh Mungee <sumedh@cs.wustl.edu>
//
// ============================================================================

#if !defined (AVSTREAMS_CLIENT_H)
#define AVSTREAMS_CLIENT_H

#include "ace/Synch.h"
#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "ace/Get_Opt.h"
#include "tao/tao_util.h"

class Client
{
public:
  Client (void);
  int init (int argc, 
            char **argv,
            CORBA::Environment &env);
  int run (CORBA::Environment &env);
protected:
  int bind_to_remote_mmdevice (int argc,
                               char **argv,
                               CORBA::Environment &env);
  TAO_ORB_Manager manager_;
  AVStreams::MMDevice_var local_mmdevice_;
  AVStreams::MMDevice_var remote_mmdevice_;
  AVStreams::StreamCtrl_var stream_ctrl_;
};

#endif /* AVSTREAMS_CLIENT_H */
