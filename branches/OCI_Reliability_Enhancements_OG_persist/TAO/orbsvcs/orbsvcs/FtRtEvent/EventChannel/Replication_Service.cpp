// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/Replication_Service.h"
#include "orbsvcs/FtRtEvent/EventChannel/AMI_Replication_Strategy.h"
#include "orbsvcs/FtRtEvent/EventChannel/Basic_Replication_Strategy.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_ORBInitializer.h"
#include "../Utils/Log.h"

#include "tao/ORBInitializer_Registry.h"
#include "tao/CDR.h"

#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_strings.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace FTRTEC
{
  namespace {
    auto_ptr<Replication_Strategy> replication_strategy;
    int threads = 1;
    Replication_Service* service;
  }

  Replication_Service* Replication_Service::instance()
  {
    return service;
  }

  Replication_Service::Replication_Service()
  {
    service = this;
  }

  Replication_Service::~Replication_Service()
  {
  }

  int Replication_Service::init (int argc, ACE_TCHAR* argv[])
  {
    static int initialized = 0;

    // Only allow initialization once.
    if (initialized)
      return 0;

    initialized = 1;
    bool ami = false;

    // Parse any service configurator parameters.
    while (argc > 0) {
      if (ACE_OS::strcasecmp (argv[0], ACE_TEXT("AMI")) ==0 )
        ami = true;
      if (ACE_OS::strcasecmp (argv[0], ACE_TEXT("-threads")) ==0  && argc > 1) {
        FTRTEC::threads = ACE_OS::atoi(argv[1]);
        if (FTRTEC::threads ==0 )
          FTRTEC::threads = 1;
        ++argv; --argc;
      }
      ++argv; --argc;
    }

    Replication_Strategy* strategy;
    if (ami) {
      ACE_NEW_RETURN (strategy, AMI_Replication_Strategy(threads() > 1), -1);
      TAO_FTRTEC::Log(3, ACE_TEXT("AMI replication strategy\n"));
    }
    else {
      ACE_NEW_RETURN (strategy, Basic_Replication_Strategy(threads() > 1), -1);
      TAO_FTRTEC::Log(3, ACE_TEXT("Basic replication strategy\n"));
    }

     ACE_auto_ptr_reset (replication_strategy, strategy);

      try
      {
        PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
          PortableInterceptor::ORBInitializer::_nil ();
        PortableInterceptor::ORBInitializer_var orb_initializer;

        /// Register the RTCORBA ORBInitializer.
        ACE_NEW_THROW_EX (temp_orb_initializer,
          FTEC_ORBInitializer,
          CORBA::NO_MEMORY ());
        orb_initializer = temp_orb_initializer;

        PortableInterceptor::register_orb_initializer (orb_initializer.in ());
      }
      catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception (
          "Unexpected exception caught while ""initializing the TransactionDepth");
        return -1;
      }
    return 0;
  }


  void Replication_Service::become_primary()
  {
    TAO_FTRTEC::Log(3, ACE_TEXT("become_primary\n"));

    Replication_Strategy* strategy =
      replication_strategy->make_primary_strategy();

    ACE_ASSERT(strategy);

    if (replication_strategy.get() != strategy) {
      ACE_auto_ptr_reset(replication_strategy, strategy);
    }
  }

  void Replication_Service::check_validity(void)
  {
    replication_strategy->check_validity();
  }


  void Replication_Service::replicate_request(const FtRtecEventChannelAdmin::Operation& update,
    RollbackOperation rollback)
  {
    TAO_OutputCDR cdr;
    cdr << update;

    ACE_Message_Block mb;
    ACE_CDR::consolidate(&mb, cdr.begin());
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
    FTRT::State state(mb.length(), &mb);
#else
    // If the form of the constructor is not available, we will need
    // to do the copy manually.  First, set the octet sequence length.
    FTRT::State state;
    CORBA::ULong length = mb.length ();
    state.length (length);

    // Now copy over each byte.
    char* base = mb.data_block ()->base ();
    for(CORBA::ULong i = 0; i < length; i++)
      {
        state[i] = base[i];
      }
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */


    replication_strategy->replicate_request(
      state,
      rollback,
      update.object_id);
  }

  void Replication_Service::add_member(const FTRT::ManagerInfo & info,
                                       CORBA::ULong object_group_ref_version)
  {
    replication_strategy->add_member(info, object_group_ref_version);
  }

  int  Replication_Service::acquire_read (void)
  {
    int r =  replication_strategy->acquire_read();
    TAO_FTRTEC::Log(3, ACE_TEXT("Read Lock acquired %d\n"), r);
    return r;
  }

  int  Replication_Service::acquire_write (void)
  {
    int r= replication_strategy->acquire_write();
    TAO_FTRTEC::Log(3, ACE_TEXT("Write Lock acqured %d\n"), r);
    return r;
  }

  int  Replication_Service::release (void)
  {
    int r= replication_strategy->release();
    TAO_FTRTEC::Log(3, ACE_TEXT("Lock Released %d\n"), r);
    return r;
  }

  int Replication_Service::threads() const {
    return FTRTEC::threads;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_NAMESPACE_DEFINE (
  TAO_FTRTEC,
  Replication_Service,
  FTRTEC::Replication_Service)

ACE_STATIC_SVC_DEFINE (
  Replication_Service,
  ACE_TEXT ("FTRTEC_Replication"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (Replication_Service),
  ACE_Service_Type::DELETE_THIS
  | ACE_Service_Type::DELETE_OBJ,
  0)
