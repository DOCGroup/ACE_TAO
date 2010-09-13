// -*- C++ -*-

// $Id$
//

#ifndef LOG_WALKER_PEER_OBJECT_H
#define LOG_WALKER_PEER_OBJECT_H

#include "ace/SString.h"
#include "ace/Unbounded_Queue.h"
#include "ace/streams.h"

class PeerProcess;
class Invocation;

typedef ACE_Unbounded_Queue<Invocation*> Invocations;
typedef ACE_Unbounded_Queue_Iterator<Invocation*> InvocationIter;

class PeerObject
{
public:
  PeerObject (long ident, const char *name, PeerProcess *parent);
  virtual ~PeerObject (void);

  void add_invocation (Invocation *inv);
  void dump_detail (ostream &strm);
  size_t num_invocations (void);
  long ident (void);
  ACE_CString &name (void);

private:
  long ident_;
  ACE_CString name_;
  PeerProcess *parent_;
  Invocations invocations_;
};

#endif // LOG_WALKER_PEER_OBJECT_H
