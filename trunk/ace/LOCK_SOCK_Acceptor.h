// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    LOCK_SOCK_Acceptor.h
//
// = AUTHOR
//    James Hu and Irfan Pyarali
// 
// ============================================================================

#if !defined (LOCK_SOCK_ACCEPTOR_H)
#define LOCK_SOCK_ACCEPTOR_H

#include "ace/SOCK_Acceptor.h"

template <class LOCK>
class ACE_LOCK_SOCK_Acceptor : public ACE_SOCK_Acceptor
  // = TITLE
  // 
  //     Specialize ACE_SOCK_Acceptor to lock around <accept>;
{
public:
  virtual int accept (ACE_SOCK_Stream &new_stream,
                      ACE_Addr *remote_addr = 0,
                      ACE_Time_Value *timeout = 0,
                      int restart = 1,
                      int reset_new_handle = 0) const;
protected:
  typedef ACE_SOCK_Acceptor SUPER;
  // Useful typedef

  LOCK lock_;
  // Type of locking mechanism.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "LOCK_SOCK_Acceptor.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("LOCK_SOCK_Acceptor.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* LOCK_SOCK_ACCEPTOR_H */
