// -*- C++ -*-

//=============================================================================
/**
 *  @file    UDP_i.h
 *
 *  This class implements the server functionality of the UDP test.
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
 */
class UDP_i: public POA_UDP
{
public:
  /// Constructor
  UDP_i () = default;

  /// Destructor
  ~UDP_i () override = default;

  UDP_i (UDP_i const &) = delete;
  UDP_i &operator= (UDP_i const &) = delete;
  UDP_i (UDP_i &&) = delete;
  UDP_i &operator= (UDP_i &&) = delete;

  void setResponseHandler (UDP_ptr udpHandler) override;

  void invoke (char const *client_name,
               CORBA::Long request_id) override;

  void reset (char const *client_name) override;

  /// Shutdown the server.
  void shutdown () override;

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

  /// Return the number of lost messages
  ACE_UINT32 getMessagesCount ();

  /// Return the number of lost messages
  ACE_UINT32 getWrongMessagesCount ();

private:
  /// ORB pointer.
  CORBA::ORB_var orb_ {};

  ACE_Hash_Map_Manager_Ex < CORBA::String_var,
                            CORBA::Long,
                            ACE_Hash < const char * >,
                            ACE_Equal_To < const char * >,
                            ACE_Null_Mutex > request_id_table_;

  ACE_UINT32 messages_count_ {};

  ACE_UINT32 wrong_messages_count_ {};

  UDP_var responseHandler_ {};
};

#endif /* UDP_I_H */
