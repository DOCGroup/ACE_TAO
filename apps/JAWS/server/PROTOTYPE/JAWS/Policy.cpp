// $Id$

#include "JAWS/Policy.h"

JAWS_Dispatch_Policy::JAWS_Dispatch_Policy (void)
{
}

JAWS_Dispatch_Policy::~JAWS_Dispatch_Policy (void)
{
}

ACE_INET_Addr *
JAWS_Synch_Dispatch_Policy::addr (void)
{
  return this->addr_;
}

JAWS_IO *
JAWS_Synch_Dispatch_Policy::io (void)
{
  return this->io_;
}

JAWS_IO_Acceptor *
JAWS_Synch_Dispatch_Policy::acceptor (void)
{
  return this->acceptor_;
}

JAWS_Concurrency_Base *
JAWS_Synch_Dispatch_Policy::concurrency (void)
{
  return this->concurrency_;
}

void
JAWS_Synch_Dispatch_Policy::addr (ACE_INET_Addr *addrp)
{
  this->addr_ = addrp;
}

void
JAWS_Synch_Dispatch_Policy::io (JAWS_IO *iop)
{
  this->io_ = iop;
}

void
JAWS_Synch_Dispatch_Policy::acceptor (JAWS_IO_Acceptor *acceptorp)
{
  this->acceptor_ = acceptorp;
}

void
JAWS_Synch_Dispatch_Policy::concurrency (JAWS_Concurrency_Base *concp)
{
  this->concurrency_ = concp;
}
