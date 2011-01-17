// $Id$

#include "ace/Get_Opt.h"
#include "ace/Monitor_Point_Registry.h"

#include "orbsvcs/Notify/MonitorControl/NotificationServiceMCC.h"
#include "orbsvcs/Notify/MonitorControlExt/NotifyMonitoringExtC.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control;

#include "MonitorTestInterfaceS.h"
extern "C" int
sorter (const void* a, const void* b)
{
  const char* left = *(reinterpret_cast<const char* const*> (a));
  const char* right = *(reinterpret_cast<const char* const*> (b));
  return ACE_OS::strcmp (left, right);
}



// It's reasonable to assume that there is only one event channel
// factory and that we know its name.
class MonitorTestInterface_i: public virtual POA_MonitorTestInterface
{
public:
  MonitorTestInterface_i (
    CosNotification::NotificationServiceMonitorControl_ptr nsm)
    : nsm_ (CosNotification::NotificationServiceMonitorControl::_duplicate (nsm))
  {
  }

  virtual void
  running (MonitorTestInterface::Which proc);

  virtual void
  finished (MonitorTestInterface::Which proc);

private:
  void brain_dump (const char * context);
  void stat_dump (const char * statName);
  void consumer_stats_check (void);
private:
  ACE_CString base_;
  CosNotification::NotificationServiceMonitorControl_var nsm_;
};

void
MonitorTestInterface_i::running (MonitorTestInterface::Which proc)
{
  ACE_CString str;
  Monitor::Data_var data;
  Monitor::NameList list;
  Monitor::Numeric num;

  switch (proc)
    {
    case MonitorTestInterface::NotifyService:
      {
      data =
        nsm_->get_statistic (NotifyMonitoringExt::EventChannelFactoryNames);
      list = data->data_union.list ();

      if (list.length () != 1)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: There should be only one Event "
                      "Channel Factory\n"));
        }

      this->base_ = list[0];
      this->base_ += "/";
      str = this->base_ + NotifyMonitoringExt::ActiveEventChannelCount;
      data = this->nsm_->get_statistic (str.c_str ());
      num = data->data_union.num ();

      if (!ACE::is_equal (num.dlist[0].value, 0.0))
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: There should be no active Event "
                      "Channels\n"));
        }

      str = this->base_ + NotifyMonitoringExt::InactiveEventChannelCount;
      data = this->nsm_->get_statistic (str.c_str ());
      num = data->data_union.num ();

      if (!ACE::is_equal (num.dlist[0].value, 0.0))
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: There should be no inactive Event "
                      "Channels\n"));
        }

      break;
      }
    case MonitorTestInterface::Consumer:
      {
      str = this->base_ + NotifyMonitoringExt::ActiveEventChannelNames;
      data = nsm_->get_statistic (str.c_str ());
      list = data->data_union.list ();

      if (list.length () != 1)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: There should be only one active "
                      "Event Channel\n"));
        }

      // Base will now be the factory plus the event channel.
      this->base_ = list[0];
      this->base_ += "/";

      str = this->base_ + NotifyMonitoringExt::EventChannelConsumerCount;
      data = this->nsm_->get_statistic (str.c_str ());
      num = data->data_union.num ();
      if (!ACE::is_equal (num.dlist[0].value, 1.0))
        ACE_ERROR ((LM_ERROR, "Monitor: ERROR: There should be only one Consumer\n"));

      str = this->base_ + NotifyMonitoringExt::EventChannelConsumerAdminCount;
      data = nsm_->get_statistic(str.c_str ());
      num = data->data_union.num ();
      if (!ACE::is_equal (num.dlist[0].value, 1.0))
        ACE_ERROR ((LM_ERROR,
                        "Monitor: ERROR: There should be only one ConsumerAdmin\n"));

      str = this->base_ + NotifyMonitoringExt::EventChannelQueueElementCount;
      data = nsm_->get_statistic(str.c_str ());
      num = data->data_union.num ();
      if (!ACE::is_equal (num.dlist[0].value, 0.0))
        ACE_ERROR ((LM_ERROR, "Monitor: ERROR: There should be no events queued\n"));

        brain_dump ("Running Consumer");

      break;
      }
    case MonitorTestInterface::Supplier:
      {
      str = this->base_ + NotifyMonitoringExt::EventChannelSupplierCount;
      data = nsm_->get_statistic(str.c_str ());
      num = data->data_union.num ();

      if (!ACE::is_equal (num.dlist[0].value, 1.0))
        ACE_ERROR ((LM_ERROR, "Monitor: ERROR: There should be only one Supplier\n"));

      str = this->base_ + NotifyMonitoringExt::EventChannelSupplierAdminCount;
      data = nsm_->get_statistic(str.c_str ());
      num = data->data_union.num ();
      if (!ACE::is_equal (num.dlist[0].value, 1.0))
        ACE_ERROR ((LM_ERROR,
              "Monitor: ERROR: There should be only one SupplierAdmin\n"));
        brain_dump ("Running Supplier");
      break;
      }
    default:
      ACE_ERROR ((LM_ERROR, "Monitor: ERROR: Impossible enum value %d\n", proc));
      break;
    }
}

void
MonitorTestInterface_i::consumer_stats_check()
{
  bool foundConsumerStats = false;
  Monitor::NameList_var names = nsm_->get_statistic_names ();
  CORBA::ULong length = names->length ();
  for(CORBA::ULong i = 0; i < length; i++)
    {
      const char * name = names[i].in ();
      size_t slashcount = 0;
      bool isConsumerQueueSize = false;
      for (size_t nCh = 0; name[nCh] != 0 && slashcount < 3; ++nCh)
        {
          if (name[nCh] == '/')
            {
              slashcount += 1;
              if(slashcount == 3)
              {
                isConsumerQueueSize = 0 == ACE_OS::strcmp(
                  &name[nCh + 1],
                  NotifyMonitoringExt::EventChannelQueueSize);
              }
            }
        }
      if (isConsumerQueueSize)
      {
        foundConsumerStats = true;
        // We have a consumer queue
        try
          {
            Monitor::Data_var queueSizeData =
              nsm_->get_statistic(name);

            Monitor::Numeric queueSizeNum = queueSizeData->data_union.num ();
            ACE_DEBUG ((LM_DEBUG, "Monitor: %s: Average: %f, Maximum: %f, Most recent: %f\n",
                name,
                queueSizeNum.average, queueSizeNum.maximum, queueSizeNum.last));
            if (queueSizeNum.average <= 0.0 || queueSizeNum.average > 2000.0)
              ACE_ERROR ((LM_ERROR, "Monitor: ERROR: %s average queue size [%f] should be greater than zero and less than 2000.\n",
                name,
                queueSizeNum.average));
            if (queueSizeNum.last > 2000.0)
              ACE_ERROR ((LM_ERROR, "Monitor: ERROR: %s most recent queue size [%f] should not be greater than 2000.\n",
                name,
                queueSizeNum.last));
          }
        catch (const CORBA::Exception& ex)
          {
            ex._tao_print_exception (name);
          }
      }

    }
  if(! foundConsumerStats)
    {
      ACE_ERROR ((LM_ERROR, "Monitor: ERROR: No consumer queue size statistics found.\n"
                ));
    }
}

void
MonitorTestInterface_i::brain_dump(const char * /*context*/)
{
#if 0 // verbose output should be controlled via a command line option
  ACE_DEBUG ((LM_DEBUG, "\nStatistics as of: %s\n", context));
  // Temporary::Dale: Dump known names
  CosNotification::NotificationServiceMonitorControl::NameList_var names =
    nsm_->get_statistic_names ();
  CORBA::ULong length = names->length ();
  ACE_DEBUG ((LM_DEBUG, "Statistic names [%d]\n", (int)length));

  // It's much easier to read once it's sorted
  const char** narray = 0;
  ACE_NEW_THROW_EX (narray,
                    const char* [length],
                    CORBA::NO_MEMORY ());
  for(CORBA::ULong i = 0; i < length; i++)
    narray[i] = names[i].in ();
  ACE_OS::qsort (narray, length,
                 sizeof (const char*), sorter);

  for(CORBA::ULong i = 0; i < length; i++)
    {
      stat_dump(narray[i]);
    }
  delete [] narray;
#endif // verbose option
}

void
MonitorTestInterface_i::stat_dump (const char * statName)
{
  try
    {
      Monitor::Data_var data = nsm_->get_statistic(statName);
      switch (data->data_union._d())
      {
      case Monitor::DATA_NUMERIC:
        {
        ACE_DEBUG ((LM_DEBUG, "Numeric: %s\n", statName));
        Monitor::Numeric num = data->data_union.num();
        ACE_DEBUG ((LM_DEBUG, "   count: %d, average: %f; sumsq: %f, min: %f, max: %f: last %f\n",
          (unsigned int)num.count, num.average, num.sum_of_squares, num.minimum, num.maximum, num.last));
        break;
        }
      default:
        {
        Monitor::NameList list = data->data_union.list ();
        size_t len = list.length ();
        ACE_DEBUG ((LM_DEBUG, "Text[%d]: %s\n", (int)len, statName));

        for (size_t i = 0; i < len; i++)
        {
          ACE_CString str = list[i].in ();
          ACE_DEBUG ((LM_DEBUG, "   %d: %s\n", (int)i, str.c_str()));
        }
        break;
        }
      }
    }
  catch (const CORBA::Exception& ex)
    {

      ex._tao_print_exception (statName);
    }
}


void
MonitorTestInterface_i::finished (MonitorTestInterface::Which proc)
{
//  ACE_CString str;
//  Monitor::Data_var data;
//  Monitor::NameList list;
//  Monitor::Numeric num;

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
      {
      ACE_CString consumerCountName = this->base_ + NotifyMonitoringExt::EventChannelConsumerCount;
      Monitor::Data_var consumerCountData =
        nsm_->get_statistic(consumerCountName.c_str ());
      Monitor::Numeric consumerCountNum =
        consumerCountData->data_union.num ();
      if (!ACE::is_equal (consumerCountNum.last, 1.0))
      ACE_ERROR ((LM_ERROR, "Monitor: ERROR: There should still be one Consumer\n"));

      ACE_CString queueElementName = this->base_ + NotifyMonitoringExt::EventChannelQueueElementCount;
      Monitor::Data_var queueElementData =
        nsm_->get_statistic(queueElementName.c_str ());
      Monitor::Numeric queueElementNum =
        queueElementData->data_union.num ();
      if (ACE::is_equal (queueElementNum.last, 0.0))
      ACE_ERROR ((LM_ERROR, "Monitor: ERROR: There should be at least one "
                            "event queued\n"));

      ACE_CString adminNamesName  = this->base_ + NotifyMonitoringExt::EventChannelConsumerAdminNames;
      Monitor::Data_var adminNamesData =
        nsm_->get_statistic(adminNamesName.c_str ());
      Monitor::NameList nameList =
        adminNamesData->data_union.list ();
      for (CORBA::ULong i = 0; i < nameList.length (); i++)
        {
          ACE_CString queueSizeName = nameList[i].in ();
          queueSizeName += "/";
          queueSizeName += NotifyMonitoringExt::EventChannelQueueSize;
          Monitor::Data_var queueSizeData =
            nsm_->get_statistic(queueSizeName.c_str ());
          Monitor::Numeric queueSizeNum =
            queueSizeData->data_union.num ();
        ACE_DEBUG ((LM_DEBUG, "Monitor: Queue Size: Average: %f, Maximum: %f, Most recent: %f\n",
            queueSizeNum.average, queueSizeNum.maximum, queueSizeNum.last));
          if (queueSizeNum.average <= 0.0 || queueSizeNum.average > 2000.0)
            ACE_ERROR ((LM_ERROR, "Monitor: ERROR: The average queue size [%f] should be greater than zero and less than 2000.\n",
              queueSizeNum.average));
          if (queueSizeNum.last > 2000.0)
            ACE_ERROR ((LM_ERROR, "Monitor: ERROR: The most recent queue size [%f] should not be greater than 2000.\n",
              queueSizeNum.last));
      }

      consumer_stats_check();

      brain_dump ("Finished Supplier");
      break;
      }
    default:
      ACE_ERROR ((LM_ERROR, "Impossible enum value %d\n", proc));
      break;
    }
}

static const ACE_TCHAR* ior_output_file = ACE_TEXT ("test_monitor.ior");
static const ACE_TCHAR* monitor_ior = 0;

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:o:"));
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
          "Monitor: usage: %s "
                           "-k <ior> "
                           "-o <file> "
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  int status = 0;

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      CORBA::Object_var obj =
        orb->string_to_object (monitor_ior);
      CosNotification::NotificationServiceMonitorControl_var nsm =
        CosNotification::NotificationServiceMonitorControl::_narrow (obj.in ());

      if (CORBA::is_nil (nsm.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Monitor: Unable to locate the "
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

      FILE *output_file= ACE_OS::fopen (ACE_TEXT_ALWAYS_CHAR(ior_output_file), ACE_TEXT ("w"));

      if (output_file == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s\n",
                             ior_output_file),
                             1);
        }

      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();
      poa_manager->activate ();

      orb->run ();
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_monitor: ");
      status++;
    }

#else
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

  return status;
}

