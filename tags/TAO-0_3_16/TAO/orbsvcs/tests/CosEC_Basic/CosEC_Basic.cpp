/* -*- C++ -*- */
// $Id$

#include "CosEC_Basic.h"

int
main (int argc, char *argv [])
{
  CosEC_Basic basic;

  if (basic.init (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) CosEC_Basic::init failed.\n"),
                      -1);
  basic.run ();
  basic.shutdown ();
  return 0;
}

CosEC_Basic::CosEC_Basic (void)
  : ec_impl_ (0)
{
  // No-Op.
}

CosEC_Basic::~CosEC_Basic (void)
{
  // No-Op.
}

int
CosEC_Basic::init_ORB  (int argc, char *argv [])
{
  ACE_DECLARE_NEW_CORBA_ENV
  ACE_TRY
    {
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    "",
                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object  =
        this->orb_->resolve_initial_references("RootPOA",
                                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          -1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
       ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                            "Exception in CosEC_Basic::init_ORB");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  ACE_NOTREACHED (return 0;)
}

int
CosEC_Basic::init_RtEC (void)
{
  ACE_DECLARE_NEW_CORBA_ENV
    ACE_TRY
    {
      this->scheduler_ =
        this->scheduler_impl_._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::String_var str =
        this->orb_->object_to_string (this->scheduler_.in (),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "EC_Basic: The (local) scheduler IOR is <%s>\n",
                  str.in ()));

      if (ACE_Scheduler_Factory::server (this->scheduler_.in ()) == -1)
        return -1;

      ACE_NEW_RETURN(this->ec_impl_, ACE_EventChannel (0,
                                                       ACE_DEFAULT_EVENT_CHANNEL_TYPE,
                                                       &module_factory_), -1);
      this->rtec_ = this->ec_impl_->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      str = this->orb_->object_to_string (this->rtec_.in (),
                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "EC_Basic: The RTEC IOR is <%s>\n",
                  str.in ()));

      this->ec_impl_->activate ();

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in CosEC_Basic::init_RtEC");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  ACE_NOTREACHED (return 0;)
}

int
CosEC_Basic::init_CosEC (void)
{
  ACE_DECLARE_NEW_CORBA_ENV
    ACE_TRY
    {
      // Setup the QOS params..
      this->supplier_qos_.insert (1,
                                  ACE_ES_EVENT_ANY,
                                  this->scheduler_->create ("supplier",
                                                            ACE_TRY_ENV),
                                  1);
      ACE_TRY_CHECK;

      this->consumer_qos_.start_disjunction_group ();
      this->consumer_qos_.insert_source (1,
                                         this->scheduler_->create ("consumer",
                                                                   ACE_TRY_ENV));
      ACE_TRY_CHECK;

      const RtecEventChannelAdmin::ConsumerQOS &consumerqos =
        this->consumer_qos_.get_ConsumerQOS ();

      const RtecEventChannelAdmin::SupplierQOS &supplierqos =
        this->supplier_qos_.get_SupplierQOS ();

      if (this->ec_i_.init (consumerqos,
                            supplierqos,
                            this->rtec_.in (),
                            ACE_TRY_ENV) != 0)
        return -1;

      ACE_TRY_CHECK;

      this->cos_ec_ = this->ec_i_._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in CosEC_Basic::init_CosEC");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  ACE_NOTREACHED (return 0;)
}

int
CosEC_Basic::init  (int argc, char *argv [])
{
  if (init_ORB (argc, argv) == -1
      || init_RtEC () == -1
      || init_CosEC () == -1)
    return -1;
  else
    return 0;
}

void
CosEC_Basic::shutdown (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->supplier_.close (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->consumer_.close (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->cos_ec_->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->ec_impl_->shutdown ();
      delete this->ec_impl_;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in CosEC_Basic::shutdown");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
CosEC_Basic::run (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Create an Any type to pass to the Cos EC.
      CORBA_Any cany;
      cany <<= CORBA::Long (50);
      CORBA::Any any;
      cany >>= any;

      this->consumer_.open (this->cos_ec_,
                            this->orb_,
                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->consumer_.connect (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->supplier_.open (this->cos_ec_,
                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->supplier_.connect (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->supplier_.send_event (any,
                                  ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "exception in CosEC_Basic::runtest\n");
    }
  ACE_ENDTRY;
  ACE_CHECK;

  this->orb_->run ();
  // wait here.
  // When the consumer gets the event we pushed, it will shutdown the ORB.
  // We will return from <run> then.
}
