// -*- C++ -*-

//=============================================================================
/**
 *  @file    UDP_i.h
 *
 *  $Id$
 *
 *  This class implements the server functionality of the UDP test.
 *  We support only single threaded servers, as we have a null mutex
 *  for locking our state.
 *
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


#ifndef UDP_I_H
#define UDP_I_H

#include "UDPS.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"

/**
 * @class UDP_i:
 *
 * @brief UDP Object Implementation
 *
 * This class implements the UDP object, which accepts two methods.
 * The first, <invoke> accepts a string identifying the client
 * and a request_id so that the server can verify if the requests
 * arrived in order.
 */
class UDP_i: public POA_UDP
{
public:
  // = Initialization and termination methods.
  /// Constructor
  UDP_i (CORBA::ORB_ptr o);

  /// Destructor
  ~UDP_i (void);

  virtual void invoke (const char *client_name,
                       UDP_ptr udpHandler,
                       CORBA::Long request_id);

  /// Shutdown the server.
  virtual void shutdown (void);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;

  ACE_Hash_Map_Manager_Ex < CORBA::String_var,
                            CORBA::Long,
                            ACE_Hash < const char * >,
                            ACE_Equal_To < const char * >,
                            ACE_Null_Mutex > request_id_table_;
};

#endif /* UDP_I_H */
