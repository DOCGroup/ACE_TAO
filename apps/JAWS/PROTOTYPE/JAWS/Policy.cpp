// $Id$

#include "JAWS/Policy.h"
#include "JAWS/Concurrency.h"
#include "JAWS/IO_Handler.h"
#include "JAWS/IO_Acceptor.h"

ACE_RCSID(JAWS, Policy, "$Id$")

JAWS_Dispatch_Policy::JAWS_Dispatch_Policy (void)
{
}

JAWS_Dispatch_Policy::~JAWS_Dispatch_Policy (void)
{
}

JAWS_Default_Dispatch_Policy::JAWS_Default_Dispatch_Policy (void)
  : concurrency_ (JAWS_Thread_Pool_Singleton::instance ()),
    ioh_factory_ (JAWS_Synch_IO_Handler_Factory_Singleton::instance ()),
    acceptor_ (JAWS_IO_Synch_Acceptor_Singleton::instance ()),
    io_ (JAWS_Synch_IO_Singleton::instance ())
{
}

JAWS_Default_Dispatch_Policy::~JAWS_Default_Dispatch_Policy (void)
{
}

JAWS_IO *
JAWS_Default_Dispatch_Policy::io (void)
{
  return this->io_;
}

JAWS_IO_Handler_Factory *
JAWS_Default_Dispatch_Policy::ioh_factory (void)
{
  return this->ioh_factory_;
}

JAWS_IO_Acceptor *
JAWS_Default_Dispatch_Policy::acceptor (void)
{
  return this->acceptor_;
}

JAWS_Concurrency_Base *
JAWS_Default_Dispatch_Policy::concurrency (void)
{
  return this->concurrency_;
}

void
JAWS_Default_Dispatch_Policy::io (JAWS_IO *)
{
}

void
JAWS_Default_Dispatch_Policy::ioh_factory (JAWS_IO_Handler_Factory *)
{
}

void
JAWS_Default_Dispatch_Policy::acceptor (JAWS_IO_Acceptor *)
{
}

void
JAWS_Default_Dispatch_Policy::concurrency (JAWS_Concurrency_Base *concp)
{
  this->concurrency_ = concp;
}
