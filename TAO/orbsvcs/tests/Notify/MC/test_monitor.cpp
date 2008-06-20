// $Id$

#include "ace/Get_Opt.h"
#include "ace/Monitor_Point_Registry.h"

#include "orbsvcs/Notify/MonitorControl/NotificationServiceMCC.h"
#include "orbsvcs/Notify/MonitorControlExt/NotifyMonitoringExtC.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control;

#include "MonitorTestInterfaceS.h"

// It's reasonable to assume that there is only one event channel
// factory and that we know its name.
class MonitorTestInterface_i: public virtual POA_MonitorTestInterface
{
public:
  MonitorTestInterface_i (
    CosNotification::NotificationServiceMonitorControl_ptr nsm)
    : base_ ("NotifyEventChannelFactory")
    , nsm_ (CosNotification::NotificationServiceMonitorControl::_duplicate (nsm))
  {
  }

  virtual void
  running (MonitorTestInterface::Which proc);

  virtual void
  finished (MonitorTestInterface::Which proc);
  
private:
  Monitor::NameList
  ec_names (Monitor::NameList_var &reg_names);
            
private:
  ACE_CString base_;
  CosNotification::NotificationServiceMonitorControl_var nsm_;
};

void
MonitorTestInterface_i::running (MonitorTestInterface::Which proc)
{
  ACE_CString str;
  CosNotification::NotificationServiceMonitorControl::Data_var data;
  CosNotification::NotificationServiceMonitorControl::Numeric num; 
  Monitor::NameList_var reg_names;
  Monitor::NameList ec_names;
      
  switch (proc)
    {
    case MonitorTestInterface::NotifyService:
      str = this->base_
            + "/"
            + NotifyMonitoringExt::ActiveEventChannelCount;
            
      data = this->nsm_->get_statistic (str.c_str ());
      num = data->num ();
      
      if (num.last != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: There should be no active Event "
                      "Channels\n"));
        }

      str = this->base_
            + "/"
            + NotifyMonitoringExt::InactiveEventChannelCount;
            
      data = this->nsm_->get_statistic (str.c_str ());
      num = data->num ();
      
      if (num.last != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: There should be no inactive Event "
                      "Channels\n"));
        }
        
      break;
    case MonitorTestInterface::Consumer:
      reg_names = this->nsm_->get_statistic_names ();
      ec_names = this->ec_names (reg_names);
      
      // Length should be 0 or 1.
      if (ec_names.length () == 0)
        {
          break;
        }
      
      str = this->base_
            + "/"
            + ec_names[0].in ()
            + "/"
            + NotifyMonitoringExt::EventChannelConsumerCount;
            
      data = this->nsm_->get_statistic (str.c_str ());
      num = data->num ();
      
      if (num.last != 1)
        {
          ACE_ERROR ((LM_ERROR, "There should be only one Consumer\n"));
        }

      str = this->base_
            + "/"
            + ec_names[0].in ()
            + "/"
            + NotifyMonitoringExt::EventChannelConsumerAdminCount;
            
      data = this->nsm_->get_statistic (str.c_str ());
      num = data->num ();
      
      if (num.last != 1)
        {
          ACE_ERROR ((LM_ERROR, "There should be only one ConsumerAdmin\n"));
        }

      str = this->base_
            + "/"
            + ec_names[0].in ()
            + "/"
            + NotifyMonitoringExt::EventChannelQueueElementCount;
            
      data = this->nsm_->get_statistic (str.c_str ());
      num = data->num ();
      
      if (num.last != 0)
        {
          ACE_ERROR ((LM_ERROR, "There should be no events queued\n"));
        }
        
      break;
    case MonitorTestInterface::Supplier:
      reg_names = this->nsm_->get_statistic_names ();
      ec_names = this->ec_names (reg_names);
      
      // Length should be 0 or 1.
      if (ec_names.length () == 0)
        {
          break;
        }
      
      str = this->base_
            + "/"
            + ec_names[0].in ()
            + "/"
            + NotifyMonitoringExt::EventChannelSupplierCount;
            
      data = this->nsm_->get_statistic (str.c_str ());
      num = data->num ();
      
      if (num.last != 1)
        {
          ACE_ERROR ((LM_ERROR, "There should be only one Supplier\n"));
        }

      str = this->base_
            + "/"
            + ec_names[0].in ()
            + "/"
            + NotifyMonitoringExt::EventChannelSupplierAdminCount;
            
      data = this->nsm_->get_statistic (str.c_str ());
      num = data->num ();
      
      if (num.last != 1)
        {
          ACE_ERROR ((LM_ERROR, "There should be only one SupplierAdmin\n"));
        }
        
      break;
    default:
      ACE_ERROR ((LM_ERROR, "Impossible enum value %d\n", proc));
      break;
    }
}

void
MonitorTestInterface_i::finished (MonitorTestInterface::Which proc)
{
  ACE_CString str;
  CosNotification::NotificationServiceMonitorControl::Data_var data;
  Monitor::NameList list;
  CosNotification::NotificationServiceMonitorControl::Numeric num;
  Monitor::NameList_var reg_names;
  Monitor::NameList ec_names;

  switch (proc)
    {
    case MonitorTestInterface::NotifyService:
      ACE_ERROR ((LM_ERROR, "This should not be called\n"));
      break;
    case MonitorTestInterface::Consumer:
      // By the time the consumer finishes, it should have destroyed
      // the event channel.
      this->running (MonitorTestInterface::NotifyService);
      break;
    case MonitorTestInterface::Supplier:
      reg_names = this->nsm_->get_statistic_names ();
      ec_names = this->ec_names (reg_names);
      
      // Length should be 0 or 1.
      if (ec_names.length () == 0)
        {
          break;
        }
      
      str = this->base_
            + "/"
            + ec_names[0].in ()
            + "/"
            + NotifyMonitoringExt::EventChannelConsumerCount;
            
      data = this->nsm_->get_statistic (str.c_str ());
      num = data->num ();
      
      if (num.last != 1)
        {
          ACE_ERROR ((LM_ERROR, "There should still be one Consumer\n"));
        }

      str = this->base_
            + "/"
            + ec_names[0].in ()
            + "/"
            + NotifyMonitoringExt::EventChannelQueueElementCount;
            
      data = this->nsm_->get_statistic (str.c_str ());
      num = data->num ();
      
      if (num.last == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "There should be at least one event queued\n"));
        }

      str = this->base_
            + "/"
            + ec_names[0].in ()
            + "/"
            + NotifyMonitoringExt::EventChannelQueueSize;
            
      data = this->nsm_->get_statistic (str.c_str ());
      num = data->num ();
      
      ACE_DEBUG ((LM_DEBUG, "Average Queue Size: %f\n", num.average));
      
      if (num.average == 0.0)
        {
          ACE_ERROR ((LM_ERROR, "The average should be non-zero\n"));
        }

      break;
    default:
      ACE_ERROR ((LM_ERROR, "Impossible enum value %d\n", proc));
      break;
    }
}

Monitor::NameList
MonitorTestInterface_i::ec_names (Monitor::NameList_var &reg_names)
{
  // The correctness of this code depends on 2 things:
  // 1. all the monitors for each event channel are added at once
  // 2. the monitor registry iterator preserves order of addition
  // If either of those things ever changes, the code below will
  // have to change as well.
  
  Monitor::NameList retval (0);
  CORBA::ULong index = 0UL;
  retval.length (index);
  ACE_CString current;
  
  ACE_CString::size_type len = this->base_.length ();

  for (CORBA::ULong i = 0UL; i < reg_names->length (); ++i)
    {
      ACE_CString reg_name = reg_names[i].in ();
      
      if (reg_name.find (this->base_) == 0 && reg_name[len] == '/')
        {
          ACE_CString tail = reg_name.substr (len + 1);
          ACE_CString::size_type pos = tail.find ('/');
          
          if (pos != ACE_CString::npos)
            {
              ACE_CString ec_name = tail.substr (0, pos);
              
              if (ec_name != current)
                {
                  retval.length (index + 1);
                  retval[index++] = ec_name.c_str ();
                  current = ec_name;
                }
            }
        }
    }
    
  return retval;
}

static const ACE_TCHAR* ior_output_file = ACE_TEXT ("test_monitor.ior");
static const ACE_TCHAR* monitor_ior = 0;

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        monitor_ior = get_opts.opt_arg ();
        break;
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage: %s "
                           "-k <ior> "
                           "-o <file> "
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  int status = 0;
  
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      CORBA::Object_var obj =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (monitor_ior));
      CosNotification::NotificationServiceMonitorControl_var nsm =
        CosNotification::NotificationServiceMonitorControl::_narrow (obj.in ());

      if (CORBA::is_nil (nsm.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to locate the "
                             "Notification Service Monitor\n"),
                            1);
        }

      MonitorTestInterface_i* mti = 0;
      ACE_NEW_RETURN (mti, MonitorTestInterface_i (nsm.in ()), 1);
      PortableServer::ServantBase_var owner_transfer (mti);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::ObjectId_var id = root_poa->activate_object (mti);
      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      MonitorTestInterface_var test =
        MonitorTestInterface::_narrow (object.in ());
      CORBA::String_var ior = orb->object_to_string (test.in ());   

      // Test the case where there are no consumers or suppliers first
      // before we write out our IOR
      mti->running (MonitorTestInterface::NotifyService);

      FILE *output_file= ACE_OS::fopen (ior_output_file, ACE_TEXT ("w"));
      
      if (output_file == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s\n",
                             ior_output_file),
                             1);
        }
        
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();
      poa_manager->activate ();

      orb->run ();
      orb->destroy ();
    }
  catch (const CosNotification::NotificationServiceMonitorControl::InvalidName& e)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "test_monitor: caught invalid name %s\n",
                  e.names[0].in ()));
      status++;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_monitor: ");
      status++;
    }

  return status;
}

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

