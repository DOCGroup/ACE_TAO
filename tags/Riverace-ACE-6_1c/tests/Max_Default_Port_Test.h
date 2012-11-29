//=============================================================================
/**
 *  @file    Max_Default_Port_Test.h
 *
 *  $Id$
 *
 *   This is a test to verify ACE_MAX_DEFAULT_PORT is correct.
 *
 *
 *  @author Chanaka Liyanaarachchi <chanaka@ociweb.com>
 */
//=============================================================================



#ifndef ACE_TESTS_MAX_DEFAULT_PORT_TEST
#define ACE_TESTS_MAX_DEFAULT_PORT_TEST

#include "ace/Event_Handler.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/INET_Addr.h"

/**
 * @class My_Accept_Handler
 = Title
 * A simple event handler
 */
class My_Accept_Handler : public ACE_Event_Handler
{

public:
  My_Accept_Handler (ACE_INET_Addr &addr);
  ~My_Accept_Handler ();

  int open (ACE_INET_Addr &addr);

  ACE_HANDLE get_handle () const;

  int handle_input (ACE_HANDLE handle);

  /// Returns the port the event handler is listening at.
  u_short port ();

private:
  ACE_SOCK_Acceptor peer_acceptor_;

  ACE_SOCK_Stream stream_;

  ACE_INET_Addr addr_;
};

#endif  /* ACE_TESTS_MAX_DEFAULT_PORT_TEST */
