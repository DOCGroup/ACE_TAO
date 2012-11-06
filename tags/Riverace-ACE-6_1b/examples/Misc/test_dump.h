// $Id$

// Define the classes used with templates in test_dump.cpp

#ifndef __TEST_DUMP_H
#define __TEST_DUMP_H

#include "ace/Dump.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"

class SOCK
{
public:
  SOCK (void) { ACE_REGISTER_OBJECT (SOCK); }
  ~SOCK (void) { ACE_REMOVE_OBJECT; }

  void dump (void) const {
    ACE_DEBUG ((LM_DEBUG, "hello from SOCK = %@\n", this));
  }

  // ...
};

class SOCK_Acceptor : public SOCK
{
public:
  SOCK_Acceptor (void) { ACE_REGISTER_OBJECT (SOCK_Acceptor); }
  ~SOCK_Acceptor (void) { ACE_REMOVE_OBJECT; }

  void dump (void) const {
    ACE_DEBUG ((LM_DEBUG, "hello from SOCK_Acceptor = %@\n", this));
  }

  // ...
};

class SOCK_Stream : public SOCK
{
public:
  SOCK_Stream (void) { ACE_REGISTER_OBJECT (SOCK_Stream); }
  ~SOCK_Stream (void) { ACE_REMOVE_OBJECT; }

  void dump (void) const {
    ACE_DEBUG ((LM_DEBUG, "hello from SOCK_Stream = %@\n", this));
  }

  // ...
};

#endif /* __TEST_DUMP_H */
