// $Id$

// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"

#include "tao/ORB_Core.h"

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "Notify_StructuredPushSupplier.h"
#include "goS.h"

#include "Notify_Test_Client.h"

// ******************************************************************
// Data Section
// ******************************************************************

static const unsigned int supplier_max = 32;
static TAO_Notify_StructuredPushSupplier* suppliers[supplier_max] = {0};
static unsigned int supplier_count = 1;
static CORBA::Boolean done = 0;
static CORBA::Boolean start = 0;
static CORBA::Boolean filter = 0;
static int max_events = 12;
static const char* ior_output_file = "supplier.ior";

#define GRAMMAR "TCL"

// ******************************************************************
// Subroutine Section
// ******************************************************************

class sig_i : public POA_sig
{
public:
  void go (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
};


void
sig_i::go (TAO_ENV_SINGLE_ARG_DECL_NOT_USED /*TAO_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  start = 1;
}


class Consumer_Client : public Notify_Test_Client
{
public:
  virtual int parse_args (int argc, char* argv[]);
};


int
Consumer_Client::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:e:fc:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':
        supplier_count = ACE_OS::atoi (get_opts.optarg);
        if (supplier_count > supplier_max)
          {
            supplier_count = supplier_max;
          }
        break;

      case 'e':
        max_events = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'f':
        filter = 1;
        break;

      case 'o':
        ior_output_file = get_opts.optarg;
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile> -e <# of events> -f"
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates sucessful parsing of the command line
  return 0;
}


static CosNotifyChannelAdmin::SupplierAdmin_ptr
create_supplieradmin (CosNotifyChannelAdmin::EventChannel_ptr ec
                      TAO_ENV_ARG_DECL)
{
  CosNotifyChannelAdmin::AdminID adminid = 0;
  CosNotifyChannelAdmin::SupplierAdmin_var admin =
    ec->new_for_suppliers (CosNotifyChannelAdmin::AND_OP,
                           adminid
                           TAO_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (0);

  // Filter Section
  if (filter)
    {
      CosNotifyFilter::FilterFactory_var ffact =
        ec->default_filter_factory (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CosNotifyFilter::Filter_var filter =
        ffact->create_filter (GRAMMAR TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CosNotifyFilter::ConstraintExpSeq constraint_list (1);
      constraint_list.length (1);

      constraint_list[0].event_types.length (0);
      constraint_list[0].constraint_expr = CORBA::string_dup ("type == 'odd'");

      filter->add_constraints (constraint_list TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      admin->add_filter (filter.in () TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  return CosNotifyChannelAdmin::SupplierAdmin::_duplicate (admin.in ());
}


static void
SendEvent (void)
{
  if (start)
    {
      static int count = 0;
      static const char* types[] = { "odd", "even" };
      CosNotification::StructuredEvent event;

      event.header.fixed_header.event_type.domain_name =
                            CORBA::string_dup ("Orbix 2000 Demos");
      event.header.fixed_header.event_type.type_name =
          CORBA::string_dup ("Sequence Notification Push Demo Event");

      event.header.fixed_header.event_name = CORBA::string_dup ("test event");


      event.filterable_data.length (2);
      event.filterable_data[0].name = CORBA::string_dup ("type");
      event.filterable_data[0].value <<= ((count & 1) == 1 ?
                                              types[0] : types[1]);
      event.filterable_data[1].name = CORBA::string_dup ("count");
      event.filterable_data[1].value <<= (CORBA::ULong)count;

      count++;

      if (count >= max_events)
        {
          done = 1;
        }

      ACE_TRY_NEW_ENV
        {
          for (unsigned int i = 0; i < supplier_count; i++)
            {
              suppliers[i]->send_event (event TAO_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }
      ACE_CATCH (CORBA::Exception, e)
        {
          ACE_PRINT_EXCEPTION (e,
                               "Supplier exception: ");
        }
      ACE_ENDTRY;
  }
}

static void
create_suppliers (CosNotifyChannelAdmin::SupplierAdmin_ptr admin,
                  PortableServer::POA_ptr poa
                  TAO_ENV_ARG_DECL)
{
  for (unsigned int i = 0; i < supplier_count; i++)
    {
      // startup the supplier
      ACE_NEW_THROW_EX (suppliers[i],
                        TAO_Notify_StructuredPushSupplier (),
                        CORBA::NO_MEMORY ());
      suppliers[i]->init (poa TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      suppliers[i]->connect (admin TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}


// ******************************************************************
// Main Section
// ******************************************************************

int main (int argc, char* argv[])
{
  int status = 0;
  ACE_TRY_NEW_ENV
    {
      Consumer_Client client;
      status = client.init (argc, argv TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (status == 0)
        {
          CosNotifyChannelAdmin::EventChannel_var ec =
            client.create_event_channel ("MyEventChannel", 0 TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          sig_i sig_impl;
          sig_var sig = sig_impl._this (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::ORB_ptr orb = client.orb ();
          CORBA::String_var ior =
                   orb->object_to_string (sig.in () TAO_ENV_ARG_PARAMETER);
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
                            create_supplieradmin (ec.in () TAO_ENV_ARG_PARAMETER);
          if (!CORBA::is_nil (admin.in ()))
            {
              create_suppliers (admin.in (), client.root_poa () TAO_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              while (!done)
                {
                  SendEvent ();
                  if (orb->work_pending ())
                    {
                      orb->perform_work ();
                    }
                }
            }
        }
    }
  ACE_CATCH (CORBA::Exception, e)
    {
      ACE_PRINT_EXCEPTION (e,
                           "Supplier exception: ");
      status = 1;
    }
  ACE_ENDTRY;

  return status;
}
