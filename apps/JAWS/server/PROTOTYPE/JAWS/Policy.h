/* -*- c++ -*- */
// $Id$

#if !defined (JAWS_POLICY_H)
#define JAWS_POLICY_H

#include "JAWS/Concurrency.h"

class JAWS_IO;
class JAWS_IO_Handler;
class JAWS_IO_Handler_Factory;

class JAWS_Dispatch_Policy
  // = TITLE
  //     Policy mechanism for choosing different concurrency models.
  //
  // = DESCRIPTION
  //     Given some (unspecified) state, decides what the concurrency
  //     model should be.  (For now, we always return the same model.)
{
public:
  JAWS_Dispatch_Policy (void);
  virtual ~JAWS_Dispatch_Policy (void);

  virtual ACE_INET_Addr *addr (void);
  virtual JAWS_IO * io (void) = 0;
  virtual JAWS_IO_Acceptor *acceptor (void) = 0;
  virtual JAWS_Concurrency_Base * concurrency (void) = 0;

  virtual void addr (ACE_INET_Addr *addrp) = 0;
  virtual void io (JAWS_IO *iop) = 0;
  virtual void acceptor (JAWS_IO_Acceptor *acceptorp) = 0;
  virtual void concurrency (JAWS_Concurrency_Base *concp) = 0;
};

class JAWS_Synch_Dispatch_Policy : public JAWS_Dispatch_Policy
{
public:
  virtual ACE_INET_Addr *addr (void);
  virtual JAWS_IO *io (void);
  virtual JAWS_IO_Acceptor *acceptor (void);
  virtual JAWS_Concurrency_Base *concurrency (void);

  virtual void addr (ACE_INET_Addr *addrp);
  virtual void io (JAWS_IO *iop);
  virtual void acceptor (JAWS_IO_Acceptor *acceptorp);
  virtual void concurrency (JAWS_Concurrency_Base *concp);

private:
  ACE_INET_Addr *addr_;

  JAWS_IO_Acceptor *acceptor_;
  JAWS_IO_Handler_Factory *factory_;
  JAWS_IO_Handler *ioh_;
  JAWS_IO *io_;

  JAWS_Concurrency_Base *concurrency_;
};

#endif /* !defined (JAWS_POLICY_H) */
