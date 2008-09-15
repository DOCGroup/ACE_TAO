/* -*- C++ -*- */
// $Id$

#include "Basic.h"
#include "CosEvent_Utilities.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Basic basic;

  try
    {
      basic.init (argc, argv);

      basic.run ();

      basic.shutdown ();
    }
  catch (const CORBA::UserException& ue)
    {
      ue._tao_print_exception ("CosEC_Basic user exception: ");
      return 1;
    }
  catch (const CORBA::SystemException& se)
    {
      se._tao_print_exception ("CosEC_Basic system exception: ");
      return 1;
    }

  return 0;
}

Basic::Basic (void)
{
  // No-Op.
}

Basic::~Basic (void)
{
  // No-Op.
}

void
Basic::init (int argc, char *argv[])
{
  this->init_ORB (argc, argv);

  this->init_CosEC ();
}

void
Basic::init_ORB  (int argc, char *argv [])
{
  this->orb_ = CORBA::ORB_init (argc, argv);

  CORBA::Object_var poa_object  =
    this->orb_->resolve_initial_references("RootPOA");

  if (CORBA::is_nil (poa_object.in ()))
    ACE_ERROR ((LM_ERROR,
                " (%P|%t) Unable to initialize the POA.\n"));

  root_poa_ =
    PortableServer::POA::_narrow (poa_object.in ());

  PortableServer::POAManager_var poa_manager =
    root_poa_->the_POAManager ();

  poa_manager->activate ();
}

void
Basic::init_CosEC (void)
{
  CosEC_ServantBase *ec = 0;

  ACE_NEW_THROW_EX (ec,
                    CosEC_ServantBase (),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var ec_var (ec);

  ec->init (this->root_poa_.in(),
            this->root_poa_.in(),
            0,0,0);

  int retval = ec->activate ();

  if (retval == -1)
    throw CORBA::UNKNOWN ();
  // @@ look for more descriptive exception to throw here

  CORBA::Object_var obj =
    this->root_poa_->servant_to_reference (ec);

  this->cos_ec_ =
    CosEventChannelAdmin::EventChannel::_narrow (obj._retn ());
}

void
Basic::run (void)
{
  // Create an Any type to pass to the Cos EC.
  CORBA::Any any;
  any <<= (CORBA::Long)50;

  this->consumer_.open (this->cos_ec_.in (),
                        this->orb_.in ());

  this->consumer_.connect ();

  this->supplier_.open (this->cos_ec_.in ());

  this->supplier_.connect ();

  this->supplier_.send_event (any);

  // this->orb_->run ();
  // @@ commenting out the run-shutdown mechanism for now because it gives
  // an poa "unknown location" exception otherwise.

  // wait here.
  // When the consumer gets the event we pushed, it will shutdown the ORB.
  // We will return from <run> then.
}

void
Basic::shutdown (void)
{
  this->supplier_.close ();

  this->consumer_.close ();

  this->cos_ec_->destroy ();
}
