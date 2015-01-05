/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Conn_Test.h
 *
 *  $Id$
 *
 *  Define class needed for generating templates. IBM C++ requires this to
 *  be in its own file for auto template instantiation.
 *
 *
 *  @author Irfan Pyarali  <irfan@cs.wustl.edu>
 */
//=============================================================================


#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Null_Condition.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Stream.h"

/**
 * @class Svc_Handler
 *
 * @brief This class is the product created by both <ACE_Connector>
 * and <ACE_Acceptor> objects.
 *
 * This class gets its own header file to work around AIX C++
 * compiler "features" related to template instantiation...  It is
 * only used by Conn_Test.cpp.
 */
class Svc_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  /// Do-nothing constructor.
  Svc_Handler (ACE_Thread_Manager * = 0);

  /// Initialization hook.
  virtual int open (void *);

  /// Prepare for recycling.
  virtual int recycle (void * = 0);

  /// Send data to server.
  void send_data (void);

  /// Recv data from client.
  void recv_data (void);

  /// Shutdown the <Svc_Handler>.
  int close (u_long = 0);

  /// Set <this> to idle.
  int idle (u_long flags);
};
