//$Id$

#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "tao/debug.h"
#include <orbsvcs/CosNamingC.h>
#include <orbsvcs/CosNotifyCommC.h>
#include <orbsvcs/CosNotifyChannelAdminC.h>

#include "LifeCycle.h"

LifeCycle::LifeCycle (void)
  : count_ (10)
{
}

LifeCycle::~LifeCycle (void)
{
}

int
LifeCycle::parse_args (int argc,
                           ACE_TCHAR *argv[])
{
    ACE_Arg_Shifter arg_shifter (argc,
                                 argv);

    const ACE_TCHAR *current_arg = 0;

    while (arg_shifter.is_anything_left ())
    {
      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-count"))))
        {
          this->count_ = ACE_OS::atoi (current_arg);
          // The number of times to create and destroy.
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-?")) == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "usage: %s "
                      "-count testcount\n",
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
LifeCycle::init (int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  CORBA::Object_var rootObj =
    orb->resolve_initial_references ("NameService");

  if (CORBA::is_nil (rootObj.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  " (%P|%t) Unable to resolve naming service !\n"));
      return;
    }

  CosNaming::NamingContext_var rootNC =
    CosNaming::NamingContext::_narrow (rootObj.in ());

  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup ("NotifyEventChannelFactory");

  CORBA::Object_var obj = rootNC->resolve (name);

  notify_factory_ =
    CosNotifyChannelAdmin::EventChannelFactory::_narrow (
        obj.in()
      );

  if (CORBA::is_nil (notify_factory_.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  " (%P|%t) Unable to locate Notify_Service\n"));

      return;
    }
}

void
LifeCycle::run_test(void)
{
  for (int i = 0; i < this->count_; ++i)
    {
     this->create_ec ();

     this->create_supplier_admin ();

     this->create_consumer_admin ();

     this->destroy_consumer_admin ();

     this->destroy_supplier_admin ();

     this->destroy_ec ();
  }
}

void
LifeCycle::create_ec (void)
{
  CosNotifyChannelAdmin::ChannelID id;
  CosNotification::QoSProperties initial_qos;
  CosNotification::AdminProperties initial_admin;

  this->ec_ = notify_factory_->create_channel (initial_qos,
                                               initial_admin,
                                               id);

  if (CORBA::is_nil (ec_.in ())) {
    ACE_ERROR ((LM_ERROR,
                " (%P|%t) Unable to create event channel\n"));
    return;
  }

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
                "created event channel\n"));
}

void
LifeCycle::create_supplier_admin (void)
{
  CosNotifyChannelAdmin::AdminID adminid;
  CosNotifyChannelAdmin::InterFilterGroupOperator ifgop =
          CosNotifyChannelAdmin::OR_OP;

  supplier_admin_ = this->ec_->new_for_suppliers (ifgop,
                                                  adminid);

  if (CORBA::is_nil (supplier_admin_.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  " (%P|%t) Unable to create supplier admin\n"));
      return;
    }

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
                "created supplier admin\n"));
}

void
LifeCycle::create_consumer_admin (void)
{
  CosNotifyChannelAdmin::AdminID adminid;
  CosNotifyChannelAdmin::InterFilterGroupOperator ifgop =
    CosNotifyChannelAdmin::OR_OP;

  consumer_admin_ = ec_->new_for_consumers (ifgop, adminid);

  if (CORBA::is_nil (consumer_admin_.in()))
    {
      ACE_ERROR ((LM_ERROR,
                         " (%P|%t) Unable to find supplier admin\n"));
      return;
    }

  if (TAO_debug_level)
   ACE_DEBUG ((LM_DEBUG,
               "created consumer admin\n"));
}

void
LifeCycle::destroy_supplier_admin (void)
{
  this->supplier_admin_->destroy ();

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
                "destroyed supplier admin\n"));
}

void
LifeCycle::destroy_consumer_admin (void)
{
  this->consumer_admin_->destroy ();

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
                "destroyed consumer admin\n"));
}

void
LifeCycle::destroy_ec (void)
{
  this->ec_->destroy ();

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
                "destroyed event channel\n"));
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      LifeCycle test;

      test.parse_args (argc,
                       argv);

      test.init (argc,
                 argv);

      test.run_test ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Error: ");
      return 1;
    }

  return 0;
}
