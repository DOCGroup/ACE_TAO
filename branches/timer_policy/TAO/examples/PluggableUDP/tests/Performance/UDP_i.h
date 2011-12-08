// -*- C++ -*-

//=============================================================================
/**
 *  @file    UDP_i.h
 *
 *  $Id$
 *
 *  This class implements the server functionality of the UDP test.
 *
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


#ifndef UDP_I_H
#define UDP_I_H

#include "UDPS.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Null_Mutex.h"

/**
 * @class UDP_i:
 *
 * @brief UDP Object Implementation
 *
 */
class UDP_i: public POA_UDP
{
public:
  // = Initialization and termination methods.
  /// Constructor
  UDP_i (void);

  /// Destructor
  ~UDP_i (void);

  virtual void setResponseHandler (UDP_ptr udpHandler);

  virtual void invoke (const char *client_name,
                       CORBA::Long request_id);

  virtual void reset (const char * client_name);

  /// Shutdown the server.
  virtual void shutdown (void);

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

  /// Return the number of lost messages
  ACE_UINT32 getMessagesCount ();

  /// Return the number of lost messages
  ACE_UINT32 getWrongMessagesCount ();

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;

  ACE_Hash_Map_Manager_Ex < CORBA::String_var,
                            CORBA::Long,
                            ACE_Hash < const char * >,
                            ACE_Equal_To < const char * >,
                            ACE_Null_Mutex > request_id_table_;

  ACE_UINT32 messages_count_;

  ACE_UINT32 wrong_messages_count_;

  UDP_var responseHandler_;
};

#endif /* UDP_I_H */
