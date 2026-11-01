// Define the classes used with templates in test_dump.cpp

#ifndef __TEST_DUMP_H
#define __TEST_DUMP_H

#include "ace/Dump.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"

class SOCK
{
public:
  SOCK () { ACE_REGISTER_OBJECT (SOCK); }
  virtual ~SOCK () { ACE_REMOVE_OBJECT; }

  SOCK (SOCK const &) = delete;
  SOCK (SOCK &&) = delete;
  SOCK &operator= (SOCK const &) = delete;
  SOCK &operator= (SOCK &&) = delete;

  void dump () const {
    ACE_DEBUG ((LM_DEBUG, "hello from SOCK = %@\n", this));
  }

  // ...
};

class SOCK_Acceptor : public SOCK
{
public:
  SOCK_Acceptor () { ACE_REGISTER_OBJECT (SOCK_Acceptor); }
  ~SOCK_Acceptor () override { ACE_REMOVE_OBJECT; }

  SOCK_Acceptor (SOCK_Acceptor const &) = delete;
  SOCK_Acceptor (SOCK_Acceptor &&) = delete;
  SOCK_Acceptor &operator= (SOCK_Acceptor const &) = delete;
  SOCK_Acceptor &operator= (SOCK_Acceptor &&) = delete;

  void dump () const {
    ACE_DEBUG ((LM_DEBUG, "hello from SOCK_Acceptor = %@\n", this));
  }

  // ...
};

class SOCK_Stream : public SOCK
{
public:
  SOCK_Stream () { ACE_REGISTER_OBJECT (SOCK_Stream); }
  ~SOCK_Stream () override { ACE_REMOVE_OBJECT; }

  SOCK_Stream (SOCK_Stream const &) = delete;
  SOCK_Stream (SOCK_Stream &&) = delete;
  SOCK_Stream &operator= (SOCK_Stream const &) = delete;
  SOCK_Stream &operator= (SOCK_Stream &&) = delete;

  void dump () const {
    ACE_DEBUG ((LM_DEBUG, "hello from SOCK_Stream = %@\n", this));
  }

  // ...
};

#endif /* __TEST_DUMP_H */
