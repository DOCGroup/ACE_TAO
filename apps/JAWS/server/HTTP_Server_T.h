/* -*- c++ -*- */
// Hey, Emacs!  This is a C++ file.
// $Id$

// ============================================================================
//
// = LIBRARY
//   jaws
// 
// = FILENAME
//    HTTP_Server_T.h
//
// = AUTHOR
//    James Hu
// 
// ============================================================================

#if !defined (HTTP_SERVER_T_H)
#define HTTP_SERVER_T_H

#include "ace/SOCK_Acceptor.h"

template <class LOCK>
class LOCK_SOCK_Acceptor : public ACE_SOCK_Acceptor
  // = TITLE
  // Specialize ACE_SOCK_Acceptor to lock around <accept>;
{
public:
  int accept (ACE_SOCK_Stream &new_stream,
	      ACE_Addr *remote_addr = 0,
	      ACE_Time_Value *timeout = 0,
	      int restart = 1) const;
  
private:
  LOCK lock_;
  // Type of locking mechanism.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "HTTP_Server_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("HTTP_Server_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* HTTP_SERVER_T_H */
