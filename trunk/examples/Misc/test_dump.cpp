// The following code illustrates how the ACE_Dumpable mechanisms are
// @(#)test_dump.cpp	1.1	10/18/96

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
  SOCK sock;
  // Note that the SOCK superclass is *not* printed.
  SOCK_Stream stream;
  SOCK_Acceptor acceptor;
  ACE_ODB::instance ()->dump_objects ();
  {
    SOCK sock;
    // Note that the SOCK superclass is *not* printed.
    SOCK_Stream stream;
    SOCK_Acceptor acceptor;
    ACE_ODB::instance ()->dump_objects ();
  }
  ACE_ODB::instance ()->dump_objects ();  
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Dumpable_Adapter<SOCK_Stream>;
template class ACE_Dumpable_Adapter<SOCK>;
template class ACE_Dumpable_Adapter<SOCK_Acceptor>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
