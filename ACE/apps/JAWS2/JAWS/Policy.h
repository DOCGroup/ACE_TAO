/* -*- c++ -*- */
#ifndef JAWS_POLICY_H
#define JAWS_POLICY_H

#include "JAWS/Export.h"
#include "JAWS/Concurrency.h"

class JAWS_IO;
class JAWS_IO_Handler;
class JAWS_IO_Handler_Factory;

/**
 * Policy mechanism for choosing different concurrency models.
 *
 * Given some (unspecified) state, decides what the concurrency
 * model should be.  (For now, we always return the same model.)
 */
class JAWS_Export JAWS_Dispatch_Policy
{
public:
  JAWS_Dispatch_Policy ();
  virtual ~JAWS_Dispatch_Policy ();

  virtual int ratio () = 0;
  virtual JAWS_IO * io () = 0;
  virtual JAWS_IO_Handler_Factory *ioh_factory () = 0;
  virtual JAWS_IO_Acceptor *acceptor () = 0;
  virtual JAWS_Concurrency_Base * concurrency () = 0;

  virtual void ratio (int r) = 0;
  virtual void io (JAWS_IO *iop) = 0;
  virtual void ioh_factory (JAWS_IO_Handler_Factory *factoryp) = 0;
  virtual void acceptor (JAWS_IO_Acceptor *acceptorp) = 0;
  virtual void concurrency (JAWS_Concurrency_Base *concp) = 0;
};

class JAWS_Export JAWS_Default_Dispatch_Policy : public JAWS_Dispatch_Policy
{
public:
  JAWS_Default_Dispatch_Policy ();
  virtual ~JAWS_Default_Dispatch_Policy ();

  int ratio () override;
  JAWS_IO *io () override;
  JAWS_IO_Handler_Factory *ioh_factory () override;
  JAWS_IO_Acceptor *acceptor () override;
  JAWS_Concurrency_Base *concurrency () override;

  void ratio (int r) override;
  void io (JAWS_IO *iop) override;
  void ioh_factory (JAWS_IO_Handler_Factory *factoryp) override;
  void acceptor (JAWS_IO_Acceptor *acceptorp) override;
  void concurrency (JAWS_Concurrency_Base *concp) override;

private:
  int ratio_;
  JAWS_Concurrency_Base *concurrency_;
  JAWS_IO_Handler_Factory *ioh_factory_;
  JAWS_IO_Acceptor *acceptor_;
  JAWS_IO *io_;
};

#endif /* !defined (JAWS_POLICY_H) */
