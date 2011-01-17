// $Id$

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Sched_Params.h"
#include "ace/ACE.h"
#include "ace/OS_NS_unistd.h"

#include "tao/Timeprobe.h"
#include "tao/CDR.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/CosNamingC.h"
#include "ECM_Supplier.h"
#include "ECM_Data.h"
#include "ace/OS_NS_errno.h"



ECMS_Driver::ECMS_Driver (void)
  : n_suppliers_ (1),
    event_count_ (100),
    event_period_ (100),
    event_size_ (32),
    event_a_ (ACE_ES_EVENT_UNDEFINED),
    event_b_ (ACE_ES_EVENT_UNDEFINED + 1),
    pid_file_name_ (0)
{
}

// ****************************************************************

int
ECMS_Driver::run (int argc, ACE_TCHAR* argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");
      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (this->parse_args (argc, argv))
        return 1;

      ACE_DEBUG ((LM_DEBUG,
                  "Execution parameters:\n"
                  "  suppliers = <%d>\n"
                  "  event count = <%d>\n"
                  "  event period = <%d>\n"
                  "  event size = <%d>\n"
                  "  supplier Event A = <%d>\n"
                  "  supplier Event B = <%d>\n"
                  "  pid file name = <%s>\n",

                  this->n_suppliers_,
                  this->event_count_,
                  this->event_period_,
                  this->event_size_,
                  this->event_a_,
                  this->event_b_,

                  this->pid_file_name_?this->pid_file_name_:ACE_TEXT("nil")) );

      if (this->pid_file_name_ != 0)
        {
          FILE* pid = ACE_OS::fopen (this->pid_file_name_, "w");
          if (pid != 0)
            {
              ACE_OS::fprintf (pid, "%ld\n",
                               static_cast<long> (ACE_OS::getpid ()));
              ACE_OS::fclose (pid);
            }
        }

      int min_priority =
        ACE_Sched_Params::priority_min (ACE_SCHED_FIFO);
        // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.

      if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                                  min_priority,
                                                  ACE_SCOPE_PROCESS)) != 0)
        {
          if (ACE_OS::last_error () == EPERM)
            ACE_DEBUG ((LM_DEBUG,
                        "%s: user is not superuser, "
                        "so remain in time-sharing class\n", argv[0]));
          else
            ACE_ERROR ((LM_ERROR,
                        "%s: ACE_OS::sched_params failed\n", argv[0]));
        }

      if (ACE_OS::thr_setprio (min_priority) == -1)
        {
          ACE_ERROR ((LM_ERROR, "(%P|%t) main thr_setprio failed,"
                      "no real-time features\n"));
        }

      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the Naming Service.\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ());

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup ("EventService");

      CORBA::Object_var ec_obj =
        naming_context->resolve (name);

      RtecEventChannelAdmin::EventChannel_var channel;
      if (CORBA::is_nil (ec_obj.in ()))
        channel = RtecEventChannelAdmin::EventChannel::_nil ();
      else
        channel = RtecEventChannelAdmin::EventChannel::_narrow (ec_obj.in ());

      poa_manager->activate ();

      this->connect_suppliers (channel.in ());

      ACE_DEBUG ((LM_DEBUG, "connected supplier(s)\n"));

      this->activate_suppliers ();

      ACE_DEBUG ((LM_DEBUG, "suppliers are active\n"));

      // Wait for the supplier threads...
      if (ACE_Thread_Manager::instance ()->wait () == -1)
        {
          ACE_ERROR ((LM_ERROR, "Thread_Manager wait failed\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "suppliers finished\n"));

      this->disconnect_suppliers ();

      // @@ Deactivate the suppliers (as CORBA Objects?)
    }
  catch (const CORBA::SystemException& sys_ex)
    {
      sys_ex._tao_print_exception ("SYS_EX in Supplier");
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("NON SYS EX in Supplier");
    }
  return 0;
}

int
ECMS_Driver::supplier_task (Test_Supplier *supplier,
                            void* /* cookie */)
{
  try
    {
      ACE_Time_Value tv (0, this->event_period_);

      CORBA::ULong n = this->event_size_;

      ECM_IDLData::Info info;
      info.mobile_name = CORBA::string_dup ("test");
      info.mobile_speed = 1;
      info.trajectory.length (n);

      ECM_Data other;
      other.description = CORBA::string_dup ("some data");

      for (CORBA::ULong j = 0; j < n; ++j)
        {
          info.trajectory[j].x = j;
          info.trajectory[j].y = j * j;
          other.inventory.bind (j, j + 1);
        }

      ACE_DEBUG ((LM_DEBUG,
                  "The inventory contains (%d) elements\n",
                  other.inventory.current_size ()));

      // We have to make it big enough so we get a contiguous block,
      // otherwise the octet sequence will not work correctly.
      // NOTE: we could pre-allocate enough memory in the CDR stream
      // but we want to show that chaining works!
      TAO_OutputCDR cdr;

      CORBA::Boolean byte_order = TAO_ENCAP_BYTE_ORDER;
      cdr << CORBA::Any::from_boolean (byte_order);

      // The typecode name standard, the encode method is not (in
      // general the CDR interface is not specified).
      if (!(cdr << info))
        throw CORBA::MARSHAL ();

      // Here we marshall a non-IDL type.
      cdr << other;

      if (!cdr.good_bit ())
        ACE_ERROR ((LM_ERROR, "Problem marshalling C++ data\n"));

      const ACE_Message_Block* mb = cdr.begin ();
      // NOTE: total_length () return the length of the complete
      // chain.
      CORBA::ULong mblen = cdr.total_length ();

      for (CORBA::Long i = 0; i < this->event_count_; ++i)
        {
          RtecEventComm::EventSet event (1);
          event.length (1);
          event[0].header.source = supplier->supplier_id ();
          event[0].header.ttl = 1;

          ACE_hrtime_t t = ACE_OS::gethrtime ();
          ORBSVCS_Time::hrtime_to_TimeT (event[0].header.creation_time, t);

          if (i == static_cast<CORBA::Long> (this->event_count_) - 1)
            event[0].header.type = ACE_ES_EVENT_SHUTDOWN;
          else if (i % 2 == 0)
            event[0].header.type = this->event_a_;
          else
            event[0].header.type = this->event_b_;

          // We use replace to minimize the copies, this should result
          // in just one memory allocation;
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
          event[0].data.payload.replace (mblen, mb);
#else
          // If the replace method is not available, we will need
          // to do the copy manually.  First, set the octet sequence length.
          event[0].data.payload.length (mblen);

          // Now copy over each byte.
          char* base = mb->data_block ()->base ();
          for(CORBA::ULong i = 0; i < mblen; i++)
            {
              event[0].data.payload[i] = base[i];
            }
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */

          supplier->consumer_proxy ()->push(event);

          // ACE_DEBUG ((LM_DEBUG, "(%t) supplier push event\n"));

          ACE_OS::sleep (tv);
        }
    }
  catch (const CORBA::SystemException& sys_ex)
    {
      sys_ex._tao_print_exception ("SYS_EX");
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("NON SYS EX");
    }
  return 0;
}

void
ECMS_Driver::connect_suppliers (RtecEventChannelAdmin::EventChannel_ptr channel)
{
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      char buf[BUFSIZ];
      ACE_OS::sprintf (buf, "supplier_%02d", i);

      ACE_NEW (this->suppliers_[i], Test_Supplier (this));

      this->suppliers_[i]->connect (buf,
                                    this->event_a_,
                                    this->event_b_,
                                    channel);
    }
}

void
ECMS_Driver::activate_suppliers (void)
{
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      this->suppliers_[i]->activate ();
    }
}

void
ECMS_Driver::disconnect_suppliers (void)
{
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      this->suppliers_[i]->disconnect ();
    }
}

int
ECMS_Driver::parse_args (int argc, ACE_TCHAR *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("ds:n:t:h:p:b:"));
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 's':
          this->n_suppliers_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 'n':
          this->event_count_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 't':
          this->event_period_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 'b':
          this->event_size_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 'h':
          {
            char* aux;
                char* arg = ACE_OS::strtok_r (ACE_TEXT_ALWAYS_CHAR(get_opt.opt_arg ()), ",", &aux);

            this->event_a_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->event_b_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
          }
          break;

        case 'p':
          this->pid_file_name_ = get_opt.opt_arg ();
          break;

        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "[ORB options] "
                      "-s <nsuppliers> "
                      "-n <event count> "
                      "-t <event period (usecs)> "
                      "-h <eventa,eventb> "
                      "-p <pid file name> "
                      "\n",
                      argv[0]));
          return -1;
        }
    }

  if (this->event_count_ <= 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s: event count (%d) is out of range, "
                  "reset to default (%d)\n",
                  argv[0], this->event_count_,
                  100));
      this->event_count_ = 100;
    }

  if (this->event_size_ < 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s: event size (%d) is out of range, "
                  "reset to default (%d)\n",
                  argv[0], this->event_size_,
                  32));
      this->event_count_ = 32;
    }

  if (this->n_suppliers_ <= 0)
    {
      this->n_suppliers_ = 1;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s: number of suppliers out of range, "
                         "reset to default (%d)\n",
                         argv[0], 1), -1);
    }

  return 0;
}

Test_Supplier::Test_Supplier (ECMS_Driver *driver)
  :  driver_ (driver),
     supplier_ (this)
{
}

void
Test_Supplier::connect (const char* name,
                        int event_a,
                        int event_b,
                        RtecEventChannelAdmin::EventChannel_ptr ec)
{
  this->supplier_id_ = ACE::crc32 (name);
  ACE_DEBUG ((LM_DEBUG,
              "ID for <%s> is %04.4x\n",
              name,
              this->supplier_id_));

  ACE_SupplierQOS_Factory qos;
  qos.insert (this->supplier_id_,
              event_a,
              0, 1);
  qos.insert (this->supplier_id_,
              event_b,
              0, 1);
  qos.insert (this->supplier_id_,
              ACE_ES_EVENT_SHUTDOWN,
              0, 1);

  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    ec->for_suppliers ();

  this->consumer_proxy_ =
    supplier_admin->obtain_push_consumer ();

  RtecEventComm::PushSupplier_var objref =
    this->supplier_._this ();

  this->consumer_proxy_->connect_push_supplier (objref.in (),
                                                qos.get_SupplierQOS ());
}

void
Test_Supplier::disconnect (void)
{
  if (CORBA::is_nil (this->consumer_proxy_.in ()))
    return;

  RtecEventChannelAdmin::ProxyPushConsumer_var proxy =
    this->consumer_proxy_._retn ();

  try
    {
      proxy->disconnect_push_consumer ();
    }
  catch (const CORBA::OBJECT_NOT_EXIST&)
    {
      // Ignore, the EC can shutdown before we get a chance to
      // disconnect
    }
  catch (const CORBA::TRANSIENT&)
    {
      // Ignore, the EC can shutdown before we get a chance to
      // disconnect
    }
  catch (const CORBA::Exception&)
    {
      throw;
    }
}

int
Test_Supplier::svc ()
{
  return this->driver_->supplier_task (this, this->cookie_);
}

void
Test_Supplier::disconnect_push_supplier (void)
{
  this->consumer_proxy_ =
    RtecEventChannelAdmin::ProxyPushConsumer::_nil ();
}

int Test_Supplier::supplier_id (void) const
{
  return this->supplier_id_;
}

RtecEventChannelAdmin::ProxyPushConsumer_ptr
Test_Supplier::consumer_proxy (void)
{
  return this->consumer_proxy_.in ();
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ECMS_Driver driver;
  return driver.run (argc, argv);
}
