// $Id$

// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "Notify_Push_Supplier.h"
#include "controlS.h"
#include "Notify_Test_Client.h"
#include "tao/debug.h"

// ******************************************************************
// Data Section
// ******************************************************************

static CORBA::Boolean go = 0;
static CORBA::Short n_events = 1;
static const char* ior_file = "supplier.ior";
static CORBA::Boolean isFilter = 0;
static Notify_Push_Supplier* supplier = 0;
static CORBA::Short op = 0; // 0 = AND_OP; 1 = OR_OP

#define GRAMMAR "TCL"

// ******************************************************************
// Subroutine Section
// ******************************************************************

class supplier_ctrl_i : public POA_supplier_ctrl
{
public:
  void ctrl (CORBA::Boolean flag,
             CORBA::Short num_events
             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
};


void
supplier_ctrl_i::ctrl (CORBA::Boolean flag,
                       CORBA::Short num_events
                       ACE_ENV_ARG_DECL_NOT_USED /*ACE_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  go = flag;
  n_events = num_events;
}


class Supplier_Client : public Notify_Test_Client
{
public:
  virtual int parse_args (int argc, char* argv[]);
};


int
Supplier_Client::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:f");
  int x;

  while ((x = get_opts ()) != -1)
    switch (x)
      {
      case 'o':
        if (!ACE_OS::strcmp ("OR_OP", get_opts.optarg))
          op = 1;
        break;

      case 'f':
        isFilter = 1;
        break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "[-f] -o <AND_OP | OR_OP>"
                           " -ORBInitRef <Naming Service Location>"
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0; // successful parse
}


static CosNotifyChannelAdmin::SupplierAdmin_ptr
create_supplieradmin (CosNotifyChannelAdmin::EventChannel_ptr ec
                      ACE_ENV_ARG_DECL)
{
  CosNotifyChannelAdmin::AdminID adminid = 0;
  CosNotifyChannelAdmin::SupplierAdmin_var admin =
    ec->new_for_suppliers ((op ? CosNotifyChannelAdmin::OR_OP :
                                 CosNotifyChannelAdmin::AND_OP),
                           adminid
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return CosNotifyChannelAdmin::SupplierAdmin::_duplicate (admin.in ());
}

int
setup_event ()
{
  static int count = 0;

  CosNotification::StructuredEvent event;

   event.header.fixed_header.event_type.domain_name =
     CORBA::string_dup ("TAO Test Suite");
   event.header.fixed_header.event_type.type_name =
     CORBA::string_dup ("Filtered Structured Event Notification Svc test");

   if (count % 6 == 0)
    {
      event.header.fixed_header.event_name = CORBA::string_dup ("String test event");
      event.header.variable_header.length (1);

      event.filterable_data.length (2);
      event.filterable_data[0].name = CORBA::string_dup ("String");
      event.filterable_data[0].value <<= "One Hundred";
      event.filterable_data[1].name = CORBA::string_dup ("seq");
      event.filterable_data[1].value <<= (CORBA::Short) count;
    }
   else if (count % 6 == 1)
    {
      event.header.fixed_header.event_name = CORBA::string_dup ("Short test event");
      event.header.variable_header.length (1);

      event.filterable_data.length (2);
      event.filterable_data[0].name = CORBA::string_dup ("Number");
      event.filterable_data[0].value <<= (CORBA::Short) 100;
      event.filterable_data[1].name = CORBA::string_dup ("seq");
      event.filterable_data[1].value <<= (CORBA::Short) count;
    }
   else if (count % 6 == 2)
    {
      event.header.fixed_header.event_name = CORBA::string_dup ("String test event");
      event.header.variable_header.length (1);

      event.filterable_data.length (2);
      event.filterable_data[0].name = CORBA::string_dup ("String");
      event.filterable_data[0].value <<= "Fifty";
      event.filterable_data[1].name = CORBA::string_dup ("seq");
      event.filterable_data[1].value <<= (CORBA::Short) count;
    }
   else if (count % 6 == 3)
    {
      event.header.fixed_header.event_name = CORBA::string_dup ("String test event");
      event.header.variable_header.length (1);

      event.filterable_data.length (2);
      event.filterable_data[0].name = CORBA::string_dup ("Number");
      event.filterable_data[0].value <<= (CORBA::Short) 50;
      event.filterable_data[1].name = CORBA::string_dup ("seq");
      event.filterable_data[1].value <<= (CORBA::Short) count;
    }
   else if (count % 6 == 4)
    {
      event.header.fixed_header.event_name = CORBA::string_dup ("String test event");
      event.header.variable_header.length (1);

      event.filterable_data.length (2);
      event.filterable_data[0].name = CORBA::string_dup ("String");
      event.filterable_data[0].value <<= "Two Hundred";
      event.filterable_data[1].name = CORBA::string_dup ("seq");
      event.filterable_data[1].value <<= (CORBA::Short) count;
    }
   else
    {
      event.header.fixed_header.event_name = CORBA::string_dup ("Short test event");
      event.header.variable_header.length (1);

      event.filterable_data.length (2);
      event.filterable_data[0].name = CORBA::string_dup ("Number");
      event.filterable_data[0].value <<= (CORBA::Short) 200;
      event.filterable_data[1].name = CORBA::string_dup ("seq");
      event.filterable_data[1].value <<= (CORBA::Short) count;
    }

  count++;

  ACE_TRY_NEW_ENV
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG, "%d sent \n", count));

      supplier->send_event (event ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::Exception, e)
    {
      ACE_PRINT_EXCEPTION (e, "Supplier Signal Exception: ");
    }
  ACE_ENDTRY;

  return 0;
}

static void create_supplier (CosNotifyChannelAdmin::SupplierAdmin_ptr admin,
                             CosNotifyChannelAdmin::EventChannel_ptr ec,
                             PortableServer::POA_ptr poa
                             ACE_ENV_ARG_DECL)
{
  ACE_NEW_THROW_EX (supplier,
                    Notify_Push_Supplier (),
                    CORBA::NO_MEMORY ());

  supplier->init (poa ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  supplier->connect (admin, ec, isFilter ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void add_filter (CosNotifyChannelAdmin::SupplierAdmin_ptr admin,
                 CosNotifyChannelAdmin::EventChannel_ptr notify_channel
                 ACE_ENV_ARG_DECL)
{
    // add supplier side filtering
  CosNotifyFilter::FilterFactory_var ffact =
    notify_channel->default_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyFilter::Filter_var filter =
    ffact->create_filter (GRAMMAR ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (filter.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  " (%P|%t) Consumer unable to initialize filter.\n"));
      exit (1);
    }

  CosNotifyFilter::ConstraintExpSeq constraint_list (1);
  constraint_list.length (1);

  constraint_list[0].event_types.length (0);
  constraint_list[0].constraint_expr = CORBA::string_dup ("String == 'Fifty'");

  filter->add_constraints (constraint_list ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // apply filter
  admin->add_filter (filter.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

// ******************************************************************
// Main Section
// ******************************************************************

int main (int argc, char * argv[])
{
  int status = 0;

  ACE_TRY_NEW_ENV;
    {
      Supplier_Client client;
      status = client.init (argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (status == 0)
        {
          PortableServer::POA_ptr poa = client.root_poa ();

          CosNotifyChannelAdmin::EventChannel_var ec =
                   client.create_event_channel ("NotifyEventChannelFactory",
                                                0
                                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // register control with naming service
          supplier_ctrl_i controller;

          CosNaming::Name ctrl_name (1);
          ctrl_name.length (1);
          ctrl_name[0].id = CORBA::string_dup ("Supplier");

          PortableServer::ObjectId_var oid =
            poa->activate_object (&controller);

          CosNaming::NamingContext_ptr naming_context =
                                          client.naming_context ();

          CORBA::Object_var controller_obj = poa->id_to_reference (oid.in ());
          naming_context->bind (ctrl_name, controller_obj.in ());

          int done = 0;

          CosNotifyChannelAdmin::SupplierAdmin_var admin =
            create_supplieradmin (ec.in () ACE_ENV_ARG_PARAMETER);

          if (isFilter)
            {
              add_filter (admin.in (), ec.in () ACE_ENV_ARG_PARAMETER);
            }

          if (!CORBA::is_nil (admin.in ()))
            {
              create_supplier (admin.in (),
                               ec.in (),
                                client.root_poa ()
                               ACE_ENV_ARG_PARAMETER);

              // Ready. So Write ior to file
              CORBA::ORB_ptr orb = client.orb ();
              CORBA::String_var ior = orb->object_to_string (controller_obj.in ());
              FILE *fd = ACE_OS::fopen (ior_file, "w");
              if (fd == 0)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "Cannot open output file for "
                                   "writing IOR: %s",
                                   ior_file),
                                   1);
              ACE_OS::fprintf (fd, "%s", ior.in ());
              ACE_OS::fclose (fd);

              if (TAO_debug_level)
                ACE_DEBUG ((LM_DEBUG, "Supplier Ready.\n"));

              while ((!done) && (n_events > 0))
                {
                  if (go)
                    {
                      done = setup_event ();
                      n_events--;
                    }

                  if (orb->work_pending ())
                    orb->perform_work ();
                }

              orb->destroy ();
            }
        }
    }
  ACE_CATCH (CORBA::Exception, e)
    {
      ACE_PRINT_EXCEPTION (e, "Supplier Exception: ");
      status =  1;
    }
  ACE_ENDTRY;

 return status;
}
