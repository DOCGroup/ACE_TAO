#include "ace/Get_Opt.h"
#include "ace/Monitor_Point_Registry.h"

#include <memory>

#include "orbsvcs/Notify/MonitorControl/NotificationServiceMCC.h"
#include "orbsvcs/Notify/MonitorControlExt/NotifyMonitoringExtC.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control;

#include "MonitorTestInterfaceS.h"

extern "C" int
sorter (void const* a, void const* b)
{
  char const* left = *static_cast<char const* const*> (a);
  char const* right = *static_cast<char const* const*> (b);
  return ACE_OS::strcmp (left, right);
}

// It's reasonable to assume that there is only one event channel
// factory and that we know its name.
class MonitorTestInterface_i: public virtual POA_MonitorTestInterface
{
public:
  explicit MonitorTestInterface_i (
    CosNotification::NotificationServiceMonitorControl_ptr nsm)
    : nsm_ (CosNotification::NotificationServiceMonitorControl::_duplicate (nsm))
  {
  }

  MonitorTestInterface_i (MonitorTestInterface_i const&) = delete;
  MonitorTestInterface_i (MonitorTestInterface_i&&) = delete;
  MonitorTestInterface_i& operator= (MonitorTestInterface_i const&) = delete;
  MonitorTestInterface_i& operator= (MonitorTestInterface_i&&) = delete;
  ~MonitorTestInterface_i () override = default;

  void running (MonitorTestInterface::Which proc) override;

  void finished (MonitorTestInterface::Which proc) override;

private:
  void brain_dump (char const* context);
  void stat_dump (char const* stat_name);
  void consumer_stats_check ();
private:
  ACE_CString base_ {};
  CosNotification::NotificationServiceMonitorControl_var nsm_ {};
};

void
MonitorTestInterface_i::running (MonitorTestInterface::Which proc)
{
  ACE_CString str {};
  Monitor::Data_var data {};
  Monitor::NameList list {};
  Monitor::Numeric num {};

  switch (proc)
    {
    case MonitorTestInterface::NotifyService:
      {
      data = this->nsm_->get_statistic (NotifyMonitoringExt::EventChannelFactoryNames);
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
      data = this->nsm_->get_statistic (str.c_str ());
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
      data = this->nsm_->get_statistic (str.c_str ());
      num = data->data_union.num ();
      if (!ACE::is_equal (num.dlist[0].value, 1.0))
        ACE_ERROR ((LM_ERROR, "Monitor: ERROR: There should be only one ConsumerAdmin\n"));

      str = this->base_ + NotifyMonitoringExt::EventChannelQueueElementCount;
      data = this->nsm_->get_statistic (str.c_str ());
      num = data->data_union.num ();
      if (!ACE::is_equal (num.dlist[0].value, 0.0))
        ACE_ERROR ((LM_ERROR, "Monitor: ERROR: There should be no events queued\n"));

      brain_dump ("Running Consumer");

      break;
      }
    case MonitorTestInterface::Supplier:
      {
      str = this->base_ + NotifyMonitoringExt::EventChannelSupplierCount;
      data = this->nsm_->get_statistic (str.c_str ());
      num = data->data_union.num ();

      if (!ACE::is_equal (num.dlist[0].value, 1.0))
        ACE_ERROR ((LM_ERROR, "Monitor: ERROR: There should be only one Supplier\n"));

      str = this->base_ + NotifyMonitoringExt::EventChannelSupplierAdminCount;
      data = this->nsm_->get_statistic (str.c_str ());
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
MonitorTestInterface_i::consumer_stats_check ()
{
  bool found_consumer_stats {};
  Monitor::NameList_var const names = this->nsm_->get_statistic_names ();
  CORBA::ULong const length = names->length ();
  for (CORBA::ULong i {}; i < length; ++i)
    {
      char const* const name = names[i];
      size_t slash_count {};
      bool is_consumer_queue_size {};
      for (size_t character_index {};
           name[character_index] != 0 && slash_count < 3;
           ++character_index)
        {
          if (name[character_index] == '/')
            {
              ++slash_count;
              if (slash_count == 3)
                is_consumer_queue_size = 0 == ACE_OS::strcmp (
                  &name[character_index + 1],
                  NotifyMonitoringExt::EventChannelQueueSize);
            }
        }
      if (is_consumer_queue_size)
        {
          found_consumer_stats = true;
          // We have a consumer queue
          try
            {
              Monitor::Data_var const queue_size_data = this->nsm_->get_statistic (name);
              Monitor::Numeric const queue_size_num = queue_size_data->data_union.num ();
              ACE_DEBUG ((LM_DEBUG,
                          "Monitor: %s: Average: %f, Maximum: %f, Most recent: %f\n",
                          name,
                          queue_size_num.average,
                          queue_size_num.maximum,
                          queue_size_num.last));
              if (queue_size_num.average <= 0.0 || queue_size_num.average > 2000.0)
                ACE_ERROR ((LM_ERROR,
                            "Monitor: ERROR: %s average queue size [%f] should be greater than "
                            "zero and less than 2000.\n",
                            name,
                            queue_size_num.average));
              if (queue_size_num.last > 2000.0)
                ACE_ERROR ((LM_ERROR,
                            "Monitor: ERROR: %s most recent queue size [%f] should not be "
                            "greater than 2000.\n",
                            name,
                            queue_size_num.last));
            }
          catch (CORBA::Exception const& ex)
            {
              ex._tao_print_exception (name);
            }
        }
    }
  if (!found_consumer_stats)
    {
      ACE_ERROR ((LM_ERROR, "Monitor: ERROR: No consumer queue size statistics found.\n"
                ));
    }
}

void
MonitorTestInterface_i::brain_dump (char const* /*context*/)
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
MonitorTestInterface_i::stat_dump (char const* stat_name)
{
  try
    {
      Monitor::Data_var const data = this->nsm_->get_statistic (stat_name);
      switch (data->data_union._d ())
        {
      case Monitor::DATA_NUMERIC:
        {
          ACE_DEBUG ((LM_DEBUG, "Numeric: %s\n", stat_name));
          Monitor::Numeric const num = data->data_union.num ();
          ACE_DEBUG ((LM_DEBUG, "   count: %d, average: %f; sumsq: %f, min: %f, max: %f: last %f\n",
                      static_cast<unsigned int> (num.count), num.average, num.sum_of_squares,
                      num.minimum, num.maximum, num.last));
          break;
        }
      default:
        {
          Monitor::NameList const list = data->data_union.list ();
          size_t const len = list.length ();
          ACE_DEBUG ((LM_DEBUG, "Text[%d]: %s\n", static_cast<int> (len), stat_name));

          for (size_t i {}; i < len; ++i)
            {
              ACE_CString const str = list[i].in ();
              ACE_DEBUG ((LM_DEBUG, "   %d: %s\n", static_cast<int> (i), str.c_str ()));
            }
          break;
        }
        }
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception (stat_name);
    }
}

void
MonitorTestInterface_i::finished (MonitorTestInterface::Which proc)
{
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
        ACE_CString const consumer_count_name =
          this->base_ + NotifyMonitoringExt::EventChannelConsumerCount;
        Monitor::Data_var const consumer_count_data =
          this->nsm_->get_statistic (consumer_count_name.c_str ());
        Monitor::Numeric const consumer_count_num = consumer_count_data->data_union.num ();
        if (!ACE::is_equal (consumer_count_num.last, 1.0))
          ACE_ERROR ((LM_ERROR, "Monitor: ERROR: There should still be one Consumer\n"));

        ACE_CString const queue_element_name =
          this->base_ + NotifyMonitoringExt::EventChannelQueueElementCount;
        Monitor::Data_var const queue_element_data =
          this->nsm_->get_statistic (queue_element_name.c_str ());
        Monitor::Numeric const queue_element_num = queue_element_data->data_union.num ();
        if (ACE::is_equal (queue_element_num.last, 0.0))
          ACE_ERROR ((LM_ERROR,
                      "Monitor: ERROR: There should be at least one event queued\n"));

        ACE_CString const admin_names_name =
          this->base_ + NotifyMonitoringExt::EventChannelConsumerAdminNames;
        Monitor::Data_var const admin_names_data =
          this->nsm_->get_statistic (admin_names_name.c_str ());
        Monitor::NameList const name_list = admin_names_data->data_union.list ();
        for (CORBA::ULong i {}; i < name_list.length (); ++i)
          {
            ACE_CString queue_size_name = name_list[i].in ();
            queue_size_name += "/";
            queue_size_name += NotifyMonitoringExt::EventChannelQueueSize;
            Monitor::Data_var const queue_size_data =
              this->nsm_->get_statistic (queue_size_name.c_str ());
            Monitor::Numeric const queue_size_num = queue_size_data->data_union.num ();
            ACE_DEBUG ((LM_DEBUG,
                        "Monitor: Queue Size: Average: %f, Maximum: %f, Most recent: %f\n",
                        queue_size_num.average, queue_size_num.maximum, queue_size_num.last));
            if (queue_size_num.average <= 0.0 || queue_size_num.average > 2000.0)
              ACE_ERROR ((LM_ERROR,
                          "Monitor: ERROR: The average queue size [%f] should be greater "
                          "than zero and less than 2000.\n",
                          queue_size_num.average));
            if (queue_size_num.last > 2000.0)
              ACE_ERROR ((LM_ERROR,
                          "Monitor: ERROR: The most recent queue size [%f] should not be "
                          "greater than 2000.\n",
                          queue_size_num.last));
          }

        this->consumer_stats_check ();
        this->brain_dump ("Finished Supplier");
        break;
      }
    default:
      ACE_ERROR ((LM_ERROR, "Impossible enum value %d\n", proc));
      break;
    }
}

struct Monitor_Options
{
  ACE_TCHAR const* ior_output_file { ACE_TEXT ("test_monitor.ior") };
  ACE_TCHAR const* monitor_ior {};
};

static int
parse_args (int argc, ACE_TCHAR* argv[], Monitor_Options& options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    {
      switch (c)
      {
      case 'k':
        options.monitor_ior = get_opts.opt_arg ();
        break;
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
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
    }
  return 0;
}

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  int status {};

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      Monitor_Options options {};

      if (parse_args (argc, argv, options) != 0)
        {
          return 1;
        }

      CORBA::Object_var obj = orb->string_to_object (options.monitor_ior);
      CosNotification::NotificationServiceMonitorControl_var nsm =
        CosNotification::NotificationServiceMonitorControl::_narrow (obj.in ());

      if (CORBA::is_nil (nsm.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Monitor: Unable to locate the "
                             "Notification Service Monitor\n"),
                            1);
        }

      MonitorTestInterface_i* mti {};
      ACE_NEW_RETURN (mti, MonitorTestInterface_i (nsm.in ()), 1);
      PortableServer::ServantBase_var owner_transfer (mti);

      CORBA::Object_var poa_object = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::ObjectId_var id = root_poa->activate_object (mti);
      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      MonitorTestInterface_var test =
        MonitorTestInterface::_narrow (object.in ());
      CORBA::String_var ior = orb->object_to_string (test.in ());

      // Test the case where there are no consumers or suppliers first
      // before we write out our IOR
      mti->running (MonitorTestInterface::NotifyService);

      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_output_file, ACE_TEXT ("w")), ACE_OS::fclose);
        if (!output_file)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot open output file for writing IOR\n"),
                              1);
          }

        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();
      poa_manager->activate ();

      orb->run ();
      orb->destroy ();
    }
  catch (CORBA::Exception const& ex)
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

