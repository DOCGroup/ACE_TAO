// The following code illustrates how the ACE_Dumpable mechanisms are
// $Id$

// integrated into ACE components like the SOCK_Acceptor and
// SOCK_Stream.

#include "ace/Dump.h"

class SOCK
{
public:
  SOCK (void) { ACE_REGISTER_OBJECT (SOCK); }
  ~SOCK (void) { ACE_REMOVE_OBJECT; }

  void dump (void) const {
    cerr << "hello from SOCK = " << (u_long) this << endl;
  }

  // ...
};

class SOCK_Acceptor : public SOCK
{
public:
  SOCK_Acceptor (void) { ACE_REGISTER_OBJECT (SOCK_Acceptor); }
  ~SOCK_Acceptor (void) { ACE_REMOVE_OBJECT; }

  void dump (void) const {
    cerr << "hello from SOCK_Acceptor = " << (u_long) this << endl;
  }

  // ...
};

class SOCK_Stream : public SOCK
{
public:
  SOCK_Stream (void) { ACE_REGISTER_OBJECT (SOCK_Stream); }
  ~SOCK_Stream (void) { ACE_REMOVE_OBJECT; }

  void dump (void) const {
    cerr << "hello from SOCK_Stream = " << (u_long) this << endl;
  }

  // ...
};

int
main (void)
{
  SOCK outer_sock;
  // Note that the SOCK superclass is *not* printed.
  SOCK_Stream outer_stream;
  SOCK_Acceptor outer_acceptor;
  ACE_ODB::instance ()->dump_objects ();
  {
    SOCK inner_sock;
    // Note that the SOCK superclass is *not* printed.
    SOCK_Stream inner_stream;
    SOCK_Acceptor inner_acceptor;
    ACE_ODB::instance ()->dump_objects ();
  }
  ACE_ODB::instance ()->dump_objects ();
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Dumpable_Adapter<SOCK_Stream>;
template class ACE_Dumpable_Adapter<SOCK>;
template class ACE_Dumpable_Adapter<SOCK_Acceptor>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Dumpable_Adapter<SOCK_Stream>
#pragma instantiate ACE_Dumpable_Adapter<SOCK>
#pragma instantiate ACE_Dumpable_Adapter<SOCK_Acceptor>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

