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
  : ec_impl_ (0,
             ACE_DEFAULT_EVENT_CHANNEL_TYPE,
             &module_factory_)
{
  // No-Op.
}

CosEC_Basic::~CosEC_Basic (void)
{
  // No-Op.
}

int
CosEC_Basic::init  (int argc, char *argv [])
{
  // @@ Pradeep, this method is too long!  Please split it into
  // several smaller methods, each of which handles a particular
  // aspect of the initialization process.
  TAO_TRY
    {
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    "",
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      CORBA::Object_var poa_object  =
        this->orb_->resolve_initial_references("RootPOA",
                                               TAO_TRY_ENV);
      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                           -1);
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (),
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      RtecScheduler::Scheduler_var scheduler =
        this->scheduler_impl_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      CORBA::String_var str =
        this->orb_->object_to_string (scheduler.in (),
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      ACE_DEBUG ((LM_DEBUG,
                  "EC_Basic: The (local) scheduler IOR is <%s>\n",
                  str.in ()));

      if (ACE_Scheduler_Factory::server (scheduler.in ()) == -1)
        return -1;

      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      this->rtec_ = this->ec_impl_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      str = this->orb_->object_to_string (this->rtec_.in (),
                                          TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      ACE_DEBUG ((LM_DEBUG,
                  "EC_Basic: The RTEC IOR is <%s>\n",
                  str.in ()));

      this->ec_impl_.activate ();

      this->supplier_qos_.insert (1,
                                  ACE_ES_EVENT_ANY,
                                  scheduler->create ("supplier",
                                                     TAO_TRY_ENV),
                                  1);

      this->consumer_qos_.start_disjunction_group ();
      this->consumer_qos_.insert_source (1,
                                         scheduler->create ("consumer",
                                                            TAO_TRY_ENV));
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      const RtecEventChannelAdmin::ConsumerQOS &consumerqos =
        this->consumer_qos_.get_ConsumerQOS ();

      const RtecEventChannelAdmin::SupplierQOS &supplierqos =
        this->supplier_qos_.get_SupplierQOS ();

      if (this->ec_i_.init (consumerqos,
                            supplierqos,
                            this->rtec_,
                            TAO_TRY_ENV) != 0)
        return -1;

      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      this->cos_ec_ = this->ec_i_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      return 0;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Exception in CosEC_Basic::initEC");
      return -1;
    }
  TAO_ENDTRY;
}

void
CosEC_Basic::shutdown (void)
{
  TAO_TRY
    {
      this->supplier_.close (TAO_TRY_ENV);
      this->consumer_.close (TAO_TRY_ENV);

      this->cos_ec_->destroy (TAO_TRY_ENV);
      this->ec_impl_.shutdown ();
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Exception in CosEC_Basic::finish");
    }
  TAO_ENDTRY;
}

void
CosEC_Basic::run (void)
{
  TAO_TRY
    {
      // Create an Any type to pass to the Cos EC.
      CORBA_Any cany;
      cany <<= CORBA::Long (50);
      CORBA::Any any;
      cany >>= any;

      this->consumer_.open (this->cos_ec_,
                            TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

      this->consumer_.connect (TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

      this->supplier_.open (this->cos_ec_,
                            TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

      this->supplier_.connect (TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

      this->supplier_.send_event (any,
                                  TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("exception in CosEC_Basic::runtest\n");
    }
  TAO_ENDTRY;
}
