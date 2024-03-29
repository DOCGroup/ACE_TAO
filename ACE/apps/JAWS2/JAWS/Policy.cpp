#include "JAWS/Policy.h"
#include "JAWS/Concurrency.h"
#include "JAWS/IO_Handler.h"
#include "JAWS/IO_Acceptor.h"


JAWS_Dispatch_Policy::JAWS_Dispatch_Policy ()
{
}

JAWS_Dispatch_Policy::~JAWS_Dispatch_Policy ()
{
}

JAWS_Default_Dispatch_Policy::JAWS_Default_Dispatch_Policy ()
  : ratio_ (1),
    concurrency_ (JAWS_Thread_Pool_Singleton::instance ()),
    ioh_factory_ (JAWS_Synch_IO_Handler_Factory_Singleton::instance ()),
    acceptor_ (JAWS_IO_Synch_Acceptor_Singleton::instance ()),
    io_ (JAWS_Synch_IO_Singleton::instance ())
{
}

JAWS_Default_Dispatch_Policy::~JAWS_Default_Dispatch_Policy ()
{
}

int
JAWS_Default_Dispatch_Policy::ratio ()
{
  return this->ratio_;
}

JAWS_IO *
JAWS_Default_Dispatch_Policy::io ()
{
  return this->io_;
}

JAWS_IO_Handler_Factory *
JAWS_Default_Dispatch_Policy::ioh_factory ()
{
  return this->ioh_factory_;
}

JAWS_IO_Acceptor *
JAWS_Default_Dispatch_Policy::acceptor ()
{
  return this->acceptor_;
}

JAWS_Concurrency_Base *
JAWS_Default_Dispatch_Policy::concurrency ()
{
  return this->concurrency_;
}

void
JAWS_Default_Dispatch_Policy::ratio (int r)
{
  this->ratio_ = r;
}

void
JAWS_Default_Dispatch_Policy::io (JAWS_IO *io)
{
  this->io_ = io;
}

void
JAWS_Default_Dispatch_Policy::ioh_factory (JAWS_IO_Handler_Factory *iohf)
{
  this->ioh_factory_ = iohf;
}

void
JAWS_Default_Dispatch_Policy::acceptor (JAWS_IO_Acceptor *acceptor)
{
  this->acceptor_ = acceptor;
}

void
JAWS_Default_Dispatch_Policy::concurrency (JAWS_Concurrency_Base *concp)
{
  this->concurrency_ = concp;
}
