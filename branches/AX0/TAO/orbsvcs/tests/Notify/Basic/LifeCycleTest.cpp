//$Id$

#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include <orbsvcs/CosNamingC.h>
#include <orbsvcs/CosNotifyCommC.h>
#include <orbsvcs/CosNotifyChannelAdminC.h>

#include "LifeCycleTest.h"

ACE_RCSID (Notify_Tests, LifeCycleTest, "$Id$")

LifeCycleTest::LifeCycleTest (void)
  : count_ (10)
{
}

LifeCycleTest::~LifeCycleTest (void)
{
}

int
LifeCycleTest::parse_args (int argc,
                           char *argv[])
{
    ACE_Arg_Shifter arg_shifter (argc,
                                 argv);

    const char *current_arg = 0;

    while (arg_shifter.is_anything_left ())
    {
      if ((current_arg = arg_shifter.get_the_parameter ("-count")))
        {
          this->count_ = ACE_OS::atoi (current_arg);
          // The number of times to create and destroy.
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-?") == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "usage: %s "
                      "-count testcount \n",
                      argv[0],
                      argv[0]));

          arg_shifter.consume_arg ();

          return -1;
        }
      else
        {
          arg_shifter.ignore_arg ();
        }
    }

  return 0;
}

void
LifeCycleTest::init (int argc,
                     char* argv[]
                     ACE_ENV_ARG_DECL)
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                        argv,
                                        ""
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var rootObj =
    orb->resolve_initial_references ("NameService"
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (rootObj.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  " (%P|%t) Unable to resolve naming service !\n"));
      return;
    }

  CosNaming::NamingContext_var rootNC =
    CosNaming::NamingContext::_narrow (rootObj.in ()
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup ("NotifyEventChannelFactory");

  CORBA::Object_var obj = rootNC->resolve (name
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  notify_factory_ =
    CosNotifyChannelAdmin::EventChannelFactory::_narrow (
        obj.in()
        ACE_ENV_ARG_PARAMETER
      );
  ACE_CHECK;

  if (CORBA::is_nil (notify_factory_.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  " (%P|%t) Unable to locate Notify_Service \n"));

      return;
    }
}

void
LifeCycleTest::run_test(ACE_ENV_SINGLE_ARG_DECL)
{
  for (int i = 0; i < this->count_; ++i)
    {
     this->create_ec (ACE_ENV_SINGLE_ARG_PARAMETER);
     ACE_CHECK;

     this->create_supplier_admin (ACE_ENV_SINGLE_ARG_PARAMETER);
     ACE_CHECK;

     this->create_consumer_admin (ACE_ENV_SINGLE_ARG_PARAMETER);
     ACE_CHECK;

     this->destroy_consumer_admin (ACE_ENV_SINGLE_ARG_PARAMETER);
     ACE_CHECK;

     this->destroy_supplier_admin (ACE_ENV_SINGLE_ARG_PARAMETER);
     ACE_CHECK;

     this->destroy_ec (ACE_ENV_SINGLE_ARG_PARAMETER);
     ACE_CHECK;
  }
}

void
LifeCycleTest::create_ec (ACE_ENV_SINGLE_ARG_DECL)
{
    CosNotifyChannelAdmin::ChannelID id;
    CosNotification::QoSProperties initial_qos;
    CosNotification::AdminProperties initial_admin;

    this->ec_ = notify_factory_->create_channel (initial_qos,
                                                 initial_admin,
                                                 id
                                                 ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (CORBA::is_nil (ec_.in ())) {
      ACE_ERROR ((LM_ERROR,
                  " (%P|%t) Unable to create event channel\n"));
      return;
    }

    ACE_DEBUG ((LM_DEBUG,
                "created event channel\n"));
}

void
LifeCycleTest::create_supplier_admin (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotifyChannelAdmin::AdminID adminid;
  CosNotifyChannelAdmin::InterFilterGroupOperator ifgop =
          CosNotifyChannelAdmin::OR_OP;

  supplier_admin_ = this->ec_->new_for_suppliers (ifgop,
                                                  adminid
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (supplier_admin_.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  " (%P|%t) Unable to create supplier admin\n"));
      return;
    }

  ACE_DEBUG ((LM_DEBUG,
              "created supplier admin\n"));
}

void
LifeCycleTest::create_consumer_admin (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotifyChannelAdmin::AdminID adminid;
  CosNotifyChannelAdmin::InterFilterGroupOperator ifgop =
    CosNotifyChannelAdmin::OR_OP;

  consumer_admin_ = ec_->new_for_consumers (ifgop, adminid ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (consumer_admin_.in()))
    {
      ACE_ERROR ((LM_ERROR,
                         " (%P|%t) Unable to find supplier admin\n"));
      return;
    }

   ACE_DEBUG ((LM_DEBUG,
               "created consumer admin\n"));
}

void
LifeCycleTest::destroy_supplier_admin (ACE_ENV_SINGLE_ARG_DECL)
{
  this->supplier_admin_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "destroyed supplier admin\n"));
}

void
LifeCycleTest::destroy_consumer_admin (ACE_ENV_SINGLE_ARG_DECL)
{
  this->consumer_admin_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "destroyed consumer admin\n"));
}

void
LifeCycleTest::destroy_ec (ACE_ENV_SINGLE_ARG_DECL)
{
  this->ec_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  ACE_DEBUG ((LM_DEBUG,
              "destroyed event channel\n"));
}


int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      LifeCycleTest test;

      test.parse_args (argc,
                       argv);

      test.init (argc,
                 argv
                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test.run_test (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH(CosNotification::UnsupportedAdmin, ex)
   {
     ACE_PRINT_EXCEPTION (ex, "Unsupported Admin Properties");
   }
  ACE_CATCH(CosNotification::UnsupportedQoS, ex)
   {
     ACE_PRINT_EXCEPTION (ex, "Unsupported QoS Properties");
   }
  ACE_CATCH(CosNotifyChannelAdmin::AdminNotFound, ex)
   {
     ACE_PRINT_EXCEPTION (ex, "destroying admin object");
   }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
