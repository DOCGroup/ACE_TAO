/* -*- C++ -*- */
// $Id$

#include "CosEC_Basic.h"

int
main (int argc, char *argv [])
{
  CosEC_Basic basic;
  basic.initEC(argc, argv);
  basic.runtest ();
  basic.finish ();
}

CosEC_Basic::CosEC_Basic (void)
  : ec_impl (0,
             ACE_DEFAULT_EVENT_CHANNEL_TYPE,
             &module_factory)
{
  // No-Op.
}

CosEC_Basic::~CosEC_Basic ()
{
  // No-Op.
}

int
CosEC_Basic::initEC  (int argc, char *argv [])
{
  TAO_TRY
    {
      this->orb_ =
        CORBA::ORB_init (argc, argv, "", TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      CORBA::Object_var poa_object  =
        this->orb_->resolve_initial_references("RootPOA", TAO_TRY_ENV);

      if (CORBA::is_nil (poa_object.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      " (%P|%t) Unable to initialize the POA.\n"));
          return -1;
        }

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      RtecScheduler::Scheduler_var scheduler =
        this->scheduler_impl._this (TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      CORBA::String_var str =
        this->orb_->object_to_string (scheduler.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      ACE_DEBUG ((LM_DEBUG,
                  "EC_Basic: The (local) scheduler IOR is <%s>\n",
                  str.in ()));

      if (ACE_Scheduler_Factory::server (scheduler.in ()) == -1)
        return -1;

      rtec = ec_impl._this (TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      str = this->orb_->object_to_string (rtec.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      ACE_DEBUG ((LM_DEBUG,
                  "EC_Basic: The (local) EC IOR is <%s>\n",
                  str.in ()));

      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      RtecEventChannelAdmin::EventChannel_var local_ec =
        ec_impl._this (TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      ec_impl.activate ();

      supplier_qos.insert (1,
                           ACE_ES_EVENT_ANY,
                           scheduler->create ("supplier", TAO_TRY_ENV),
                           1);

      consumer_qos.start_disjunction_group ();
      consumer_qos.insert_source (1,
                                  scheduler->create ("consumer", TAO_TRY_ENV));
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      const RtecEventChannelAdmin::ConsumerQOS &consumerqos
        = consumer_qos.get_ConsumerQOS ();

      const RtecEventChannelAdmin::SupplierQOS &supplierqos
        = supplier_qos.get_SupplierQOS ();

      if (ec_i.init (consumerqos, supplierqos, local_ec, TAO_TRY_ENV) != 0)
        {
          return -1;
        }

      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      cos_ec = ec_i._this (TAO_TRY_ENV);
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
CosEC_Basic::finish ()
{
  TAO_TRY
    {
      this->supplier.close (TAO_TRY_ENV);
      this->consumer.close (TAO_TRY_ENV);

      this->cos_ec->destroy (TAO_TRY_ENV);

      this->ec_impl.shutdown ();
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Exception in CosEC_Basic::finish");
    }
  TAO_ENDTRY;
}

void
CosEC_Basic::runtest ()
{
  TAO_TRY
    {
      // create an Any type to pass to the Cos EC.
      CORBA_Any cany;
      cany <<= (CORBA::Long)50;
      CORBA::Any any;
      cany >>= any;

      this->consumer.open (this->cos_ec, TAO_TRY_ENV);
      this->consumer.connect (TAO_TRY_ENV);

      this->supplier.open (this->cos_ec, TAO_TRY_ENV);
      this->supplier.connect (TAO_TRY_ENV);

      this->supplier.send_event (any, TAO_TRY_ENV);
    }
 TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("exception in CosEC_Basic::runtest\n");
    }
  TAO_ENDTRY;
}
