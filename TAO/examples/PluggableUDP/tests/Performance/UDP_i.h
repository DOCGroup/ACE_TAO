// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/examples/PluggableUDP/tests/Performance
//
// = FILENAME
//    UDP_i.h
//
// = DESCRIPTION
//    This class implements the server functionality of the UDP test.
//
// = AUTHOR
//    Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================

#ifndef UDP_I_H
#define UDP_I_H

#include "UDPS.h"
#include "ace/Hash_Map_Manager_T.h"

class UDP_i: public POA_UDP
{
  // = TITLE
  //    UDP Object Implementation
  //
  // = DESCRIPTION
  //
public:
  // = Initialization and termination methods.
  UDP_i (void);
  // Constructor

  ~UDP_i (void);
  // Destructor

  virtual void setResponseHandler (UDP_ptr udpHandler
                                   TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void invoke (const char *client_name,
                       CORBA::Long request_id
                       TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void reset (const char * client_name
                      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));


  virtual void shutdown (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Shutdown the server.

  void orb (CORBA::ORB_ptr o);
  // Set the ORB pointer.

  ACE_UINT32 getMessagesCount ();
  // Return the number of lost messages

  ACE_UINT32 getWrongMessagesCount ();
  // Return the number of lost messages


private:
  CORBA::ORB_var orb_;
  // ORB pointer.

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
