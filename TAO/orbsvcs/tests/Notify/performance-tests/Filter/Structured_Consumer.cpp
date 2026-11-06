// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"
#include "ace/SString.h"

#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "Notify_Structured_Push_Consumer.h"
#include "goC.h"
#include "tao/debug.h"
#include "Notify_Test_Client.h"
#include "ace/OS_NS_sys_time.h"

// ******************************************************************
// Data Section
// ******************************************************************

struct Consumer_Options
{
  ACE_TString ior { ACE_TEXT ("file://supplier.ior") };
  bool filter {};
  unsigned int consumers {2};
  unsigned int expected {1000};
};

static constexpr char grammar[] = "TCL";

// ******************************************************************
// Subroutine Section
// ******************************************************************

class Consumer_Client : public Notify_Test_Client
{
public:
  explicit Consumer_Client (Consumer_Options& options)
    : options_ (options)
  {
  }

  Consumer_Client (Consumer_Client const&) = delete;
  Consumer_Client (Consumer_Client&&) = delete;
  Consumer_Client& operator= (Consumer_Client const&) = delete;
  Consumer_Client& operator= (Consumer_Client&&) = delete;

  int parse_args (int argc, ACE_TCHAR *argv[]) override;

private:
  Consumer_Options& options_;
};


int
Consumer_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:e:c:f"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        this->options_.ior = get_opts.optarg;
        break;

      case 'c':
        this->options_.consumers = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'e':
        this->options_.expected = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'f':
        this->options_.filter = true;
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-c <# of consumers> "
                           "-e <expected events> "
                           "-f "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}


static CosNotifyChannelAdmin::ConsumerAdmin_ptr
create_consumeradmin (CosNotifyChannelAdmin::EventChannel_ptr ec,
                      bool filter_enabled)
{
  CosNotifyChannelAdmin::AdminID adminid {};
  CosNotifyChannelAdmin::ConsumerAdmin_var admin =
    ec->new_for_consumers (CosNotifyChannelAdmin::AND_OP,
                           adminid);


  if (filter_enabled)
    {
      // Filter Section
      CosNotifyFilter::FilterFactory_var ffact =
          ec->default_filter_factory ();

      // One Filter
      CosNotifyFilter::Filter_var filter =
          ffact->create_filter (grammar);

      CosNotifyFilter::ConstraintExpSeq constraint_list (1);
      constraint_list.length (1);

      constraint_list[0].event_types.length (0);
      constraint_list[0].constraint_expr =
                                 CORBA::string_dup ("type == 'even'");

      filter->add_constraints (constraint_list);

      admin->add_filter (filter.in ());
      // End One Filter
    }

  return CosNotifyChannelAdmin::ConsumerAdmin::_duplicate (admin.in ());
}


static void
create_consumers (CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
                  Notify_Test_Client& client,
                  Consumer_Options const& options)
{
  for (unsigned int i {}; i < options.consumers; ++i)
    {
      // startup the first consumer
      char name[64] {};
      ACE_OS::sprintf (name, "consumer%u", i);
      Notify_Structured_Push_Consumer* consumer {};
      ACE_NEW_THROW_EX (consumer,
                        Notify_Structured_Push_Consumer (
                                         name,
                                         options.expected,
                                         client),
                        CORBA::NO_MEMORY ());

      PortableServer::ServantBase_var consumer_owner (consumer);
      consumer->init (client.root_poa ());

      consumer->_connect (admin);
    }
}

// ******************************************************************
// Main Section
// ******************************************************************

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int status {};
  Consumer_Options options;
  try
    {
      Consumer_Client client (options);

      status = client.init (argc, argv);

      if (status == 0)
        {
          // If we are filtering, we will get half as many as originally
          // expected
          if (options.filter)
            {
              options.expected /= 2;
            }

          CosNotifyChannelAdmin::EventChannel_var ec =
          client.create_event_channel ("MyEventChannel", 1);

          CORBA::ORB_ptr orb = client.orb ();
          CORBA::Object_var object =
                              orb->string_to_object (options.ior.c_str ());

          sig_var sig = sig::_narrow (object.in ());

          if (CORBA::is_nil (sig.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Object reference <%s> is nil\n",
                                 options.ior.c_str ()),
                                1);
            }

          CosNotifyChannelAdmin::ConsumerAdmin_var admin =
            create_consumeradmin (ec.in (), options.filter);

          if (!CORBA::is_nil (admin.in ()))
            {
              create_consumers (admin.in (), client, options);

              // Tell the supplier to go
              sig->go ();

              client.ORB_run();
              ACE_DEBUG((LM_DEBUG, "Consumer done.\n"));

              sig->done ();
            }
        }
    }
  catch (CORBA::Exception const& e)
    {
      e._tao_print_exception ("Error: Consumer exception: ");
      status = 1;
    }

  return status;
}
