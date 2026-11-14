// -*- C++ -*-

//

#ifndef LOG_WALKER_PEER_OBJECT_H
#define LOG_WALKER_PEER_OBJECT_H

#include "ace/SString.h"
#include "ace/streams.h"
#include <vector>

class PeerProcess;
class Invocation;

using Invocations = std::vector<Invocation *>;

class PeerObject
{
public:
  PeerObject (long ident, const char *name, PeerProcess * );
  virtual ~PeerObject ();

  void add_invocation (Invocation *inv);
  void dump_detail (ostream &strm);
  size_t num_invocations ();
  long ident ();
  ACE_CString &name ();

private:
  long ident_;
  ACE_CString name_;
  //PeerProcess *parent_;
  Invocations invocations_;
};

#endif // LOG_WALKER_PEER_OBJECT_H
