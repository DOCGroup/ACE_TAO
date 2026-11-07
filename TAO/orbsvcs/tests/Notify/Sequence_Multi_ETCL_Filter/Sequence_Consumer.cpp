// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"

#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "Notify_Sequence_Push_Consumer.h"
#include "goC.h"

#include "Notify_Test_Client.h"

// ******************************************************************
// Data Section
// ******************************************************************

struct Consumer_Options
{
  ACE_TCHAR const* ior { ACE_TEXT ("file://supplier.ior") };
  unsigned int low { 32 };
  unsigned int high { 32 };
};

// ******************************************************************
// Subroutine Section
// ******************************************************************

class Consumer_Client : public Notify_Test_Client
{
public:
  int parse_args (int argc, ACE_TCHAR *argv[]) override;

  Consumer_Options const& options () const
  {
    return this->options_;
  }

private:
  Consumer_Options options_;
};


int
Consumer_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:l:h:d:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        this->options_.ior = get_opts.optarg;
        break;

      case 'l':
        this->options_.low = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'h':
        this->options_.high = ACE_OS::atoi (get_opts.optarg);
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -k <ior> -l <low expected events> "
                                     "-h <high expected events>\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}


static CosNotifyChannelAdmin::ConsumerAdmin_ptr
create_consumeradmin (CosNotifyChannelAdmin::EventChannel_ptr ec)
{
  CosNotifyChannelAdmin::AdminID adminid {};
  CosNotifyChannelAdmin::ConsumerAdmin_var admin =
    ec->new_for_consumers (CosNotifyChannelAdmin::AND_OP,
                           adminid);


  CosNotifyFilter::FilterFactory_var ffact =
    ec->default_filter_factory ();

  CosNotifyFilter::Filter_var filter =
    ffact->create_filter ("EXTENDED_TCL");

  if (CORBA::is_nil (filter.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  " (%P|%t) Consumer unable to initialize filter.\n"));
      return CosNotifyChannelAdmin::ConsumerAdmin::_nil ();
    }

  CosNotifyFilter::ConstraintExpSeq constraint_list (1);
  constraint_list.length (1);

  constraint_list[0].event_types.length (0);
  constraint_list[0].constraint_expr = CORBA::string_dup ("$.enum < 0");

  CosNotifyFilter::ConstraintInfoSeq_var cons_info = filter->add_constraints (constraint_list);

  // apply filter
  admin->add_filter (filter.in ());

  return CosNotifyChannelAdmin::ConsumerAdmin::_duplicate (admin.in ());
}


static void
create_consumers (CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
                  Notify_Test_Client& client,
                  Consumer_Options const& options)
{
  // startup the consumer
  Notify_Sequence_Push_Consumer* consumer_1 {};
  ACE_NEW_THROW_EX (consumer_1,
                    Notify_Sequence_Push_Consumer (options.low,
                                                   options.high,
                                                   client),
                    CORBA::NO_MEMORY ());

  consumer_1->init (client.root_poa ());

  consumer_1->_connect (admin);
}

// ******************************************************************
// Main Section
// ******************************************************************

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int status {};
  try
    {
      Consumer_Client client;

      status = client.init (argc, argv);

      if (status == 0)
        {
          CosNotifyChannelAdmin::EventChannel_var ec =
            client.create_event_channel ("MyEventChannel", 1);

          CORBA::ORB_ptr orb = client.orb ();
          CORBA::Object_var object = orb->string_to_object (client.options ().ior);

          sig_var sig = sig::_narrow (object.in ());

          if (CORBA::is_nil (sig.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("Object reference <%s> is nil\n"),
                                 client.options ().ior),
                                1);
            }

          CosNotifyChannelAdmin::ConsumerAdmin_var admin =
            create_consumeradmin (ec.in ());

          if (!CORBA::is_nil (admin.in ()))
            {
              create_consumers (admin.in (), client, client.options ());

              // Tell the supplier to go
              sig->go ();

              client.ORB_run ();
              ACE_DEBUG ((LM_DEBUG, "Consumer done.\n"));

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
