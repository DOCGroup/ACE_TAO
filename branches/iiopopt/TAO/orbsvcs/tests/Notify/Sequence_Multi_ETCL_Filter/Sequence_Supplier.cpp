// $Id$

// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"

#include "tao/ORB_Core.h"

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/TimeBaseC.h"
#include "Notify_SequencePushSupplier.h"
#include "goS.h"

#include "Notify_Test_Client.h"

#include "ace/OS_NS_unistd.h"

// ******************************************************************
// Data Section
// ******************************************************************
const int PER_BATCH = 16;
static TAO_Notify_Tests_SequencePushSupplier* supplier_1 = 0;
static int num_batches = 6;  // 6 sets of 16
static const char* ior_output_file = "supplier.ior";

// ******************************************************************
// Subroutine Section
// ******************************************************************
class sig_i : public POA_sig
{
public:
  sig_i(CORBA::ORB_ptr orb)
    : orb_(orb)
    , started_(false)
  {
  }

  void go (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    started_ = true;
  }

  void done (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    started_ = false;
  }

  void wait_for_startup()
  {
    while (! started_) {
      ACE_Time_Value tv(0, 100 * 1000); // 100ms
      orb_->run(tv);
    }
  }

  void wait_for_completion()
  {
    while (started_) {
      ACE_Time_Value tv(0, 100 * 1000); // 100ms
      orb_->run(tv);
    }
  }

private:
  CORBA::ORB_ptr orb_;
  bool started_;
};

class Supplier_Client : public Notify_Test_Client
{
public:
  virtual int parse_args (int argc, char* argv[]);
};


int
Supplier_Client::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:e:d");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
  {
    case 'e':
      num_batches = ACE_OS::atoi (get_opts.optarg);
      break;

    case 'o':
      ior_output_file = get_opts.optarg;
      break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR,
        "usage:  %s "
        "-o <iorfile> -e <# of events>"
        "\n",
        argv [0]),
        -1);
  }

  // Indicates sucessful parsing of the command line
  return 0;
}


static CosNotifyChannelAdmin::SupplierAdmin_ptr
create_supplieradmin (CosNotifyChannelAdmin::EventChannel_ptr ec
                      ACE_ENV_ARG_DECL)
{
  CosNotifyChannelAdmin::AdminID adminid = 0;
  CosNotifyChannelAdmin::SupplierAdmin_var admin =
    ec->new_for_suppliers (CosNotifyChannelAdmin::AND_OP,
    adminid
    ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (0);

  return CosNotifyChannelAdmin::SupplierAdmin::_duplicate (admin.in ());
}


static void
SendEvents (int id ACE_ENV_ARG_DECL)
{
  static const char* types[] = { "good", "bad", "ugly" };
  CosNotification::EventBatch events;

  CosNotification::StructuredEvent event;

  event.header.fixed_header.event_type.domain_name =
    CORBA::string_dup ("Orbix 2000 Demos");
  event.header.fixed_header.event_type.type_name =
    CORBA::string_dup ("Sequence Notification Push Demo Event");

  event.header.fixed_header.event_name = CORBA::string_dup ("test event");

  event.header.variable_header.length (2);
  event.header.variable_header[0].name =
    CORBA::string_dup (CosNotification::Priority);
  event.header.variable_header[1].name =
    CORBA::string_dup (CosNotification::Timeout);

  event.filterable_data.length (3);
  event.filterable_data[0].name = CORBA::string_dup ("objectId");
  event.filterable_data[1].name = CORBA::string_dup ("type");
  event.filterable_data[2].name = CORBA::string_dup ("enum");

  events.length (PER_BATCH);

  for (int z = 0; z < PER_BATCH; ++z)
  {
    event.header.variable_header[0].value <<= (CORBA::Short)
      (id > num_batches / 2 ?
      -id : id);

    event.filterable_data[0].value <<= (CORBA::Long)z;
    event.filterable_data[1].value <<= types[2 - (id % 3)];
    event.filterable_data[2].value <<= (CORBA::Long)
      (id > num_batches / 2 ?
      -id : id);

    events[z] = event;
  }

  ACE_TRY_NEW_ENV
  {
    supplier_1->send_events (events ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCH (CORBA::Exception, e)
  {
    ACE_PRINT_EXCEPTION (e, "Error: Supplier exception: ");
  }
  ACE_ENDTRY;
}

static void
create_suppliers (CosNotifyChannelAdmin::SupplierAdmin_ptr admin,
                  PortableServer::POA_ptr poa
                  ACE_ENV_ARG_DECL)
{
  // start up the supplier
  ACE_NEW_THROW_EX (supplier_1,
    TAO_Notify_Tests_SequencePushSupplier (),
    CORBA::NO_MEMORY ());

  supplier_1->init (poa ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  supplier_1->connect (admin ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


// ******************************************************************
// Main Section
// ******************************************************************

int main (int argc, char* argv[])
{
  ACE_Auto_Ptr< sig_i > sig_impl;
  int status = 0;
  ACE_TRY_NEW_ENV
  {
    Supplier_Client client;
    status = client.init (argc, argv ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (status == 0)
    {
      CosNotifyChannelAdmin::EventChannel_var ec =
        client.create_event_channel ("MyEventChannel", 0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ORB_ptr orb = client.orb ();

      sig_impl.reset( new sig_i( orb ) );
      sig_var sig = sig_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (sig.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
      {
        FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
        if (output_file == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
          "Cannot open output file for "
          "writing IOR: %s",
          ior_output_file),
          1);
        ACE_OS::fprintf (output_file, "%s", ior.in ());
        ACE_OS::fclose (output_file);
      }

      CosNotifyChannelAdmin::SupplierAdmin_var admin =
        create_supplieradmin (ec.in () ACE_ENV_ARG_PARAMETER);
      if (!CORBA::is_nil (admin.in ()))
      {
        create_suppliers (admin.in (), client.root_poa () ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        sig_impl->wait_for_startup();

        ACE_DEBUG((LM_DEBUG, " 1 supplier sending %d batches of %d events...\n", num_batches, PER_BATCH));
        for (int i = 0; i < num_batches; ++i)
        {
          ACE_DEBUG((LM_DEBUG, "+"));
          SendEvents (i ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
        ACE_DEBUG((LM_DEBUG, "\nSupplier waiting for consumer...\n"));

        sig_impl->wait_for_completion();

        ACE_DEBUG((LM_DEBUG, "\nSupplier done.\n"));

        ACE_OS::unlink (ior_output_file);

        ec->destroy(ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
    }
  }
  ACE_CATCH (CORBA::Exception, e)
  {
    ACE_PRINT_EXCEPTION (e, "Error: Supplier exception: ");
    status = 1;
  }
  ACE_ENDTRY;

  return status;
}
