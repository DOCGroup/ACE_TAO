// $Id$

#include "Filter_Command.h"
#include "ace/Log_Msg.h"

ACE_RCSID(lib, TAO_Filter_Command, "$id$")

#include "LookupManager.h"
#include "Name.h"

TAO_NS_Filter_Command::TAO_NS_Filter_Command (void)
{
}

TAO_NS_Filter_Command::~TAO_NS_Filter_Command ()
{
}

const char*
TAO_NS_Filter_Command::get_name (void)
{
  return TAO_NS_Filter_Command::name ();
}

const char*
TAO_NS_Filter_Command::name (void)
{
  return TAO_NS_Name::filter_command;
}

void
TAO_NS_Filter_Command::init (ACE_Arg_Shifter& arg_shifter)
{
  if (arg_shifter.is_anything_left ())
    {
      if (arg_shifter.cur_arg_strncasecmp ("-CreateFactory") == 0) // -Create factory_name ec
        {
          this->command_ = CREATE_FACTORY;

          arg_shifter.consume_arg ();

          this->name_ = arg_shifter.get_current (); // FF name
          arg_shifter.consume_arg ();

          this->factory_ = arg_shifter.get_current (); //EC
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-CreateFilter") == 0) // -CreateFilter filter_name filterfactory_name
        {
          this->command_ = CREATE_FILTER;

          arg_shifter.consume_arg ();

          this->name_ = arg_shifter.get_current (); // Filter name

          arg_shifter.consume_arg ();

          this->factory_ = arg_shifter.get_current (); //FF
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-Add_Constraint") == 0) // -Add_Constraint filter_name constraint_expr
        {
          this->command_ = ADD_CONSTRAINT;

          arg_shifter.consume_arg ();

          this->name_ = arg_shifter.get_current (); // Filter name

          arg_shifter.consume_arg ();

          this->constraint_ = arg_shifter.get_current (); //Constraint
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-Add_Filter") == 0) // -Add_Filter filter_name FilterAdmin_Name
        {
          this->command_ = ADD_FILTER;

          arg_shifter.consume_arg ();

          this->name_ = arg_shifter.get_current (); // Filter name

          arg_shifter.consume_arg ();

          this->factory_ = arg_shifter.get_current (); //FilterAdmin
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-Destroy") == 0) // -Destroy filter_name
        {
          this->command_ = DESTROY;

          arg_shifter.consume_arg ();

          this->name_ = arg_shifter.get_current (); // filter

          arg_shifter.consume_arg ();
        }
    }
}

void
TAO_NS_Filter_Command::handle_create_filter_factory (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotifyChannelAdmin::EventChannel_var ec;

  LOOKUP_MANAGER->resolve (ec, this->factory_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyFilter::FilterFactory_var ff =
    ec->default_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  LOOKUP_MANAGER->_register (ff.in(), this->name_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Filter_Command::handle_create_filter (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotifyFilter::FilterFactory_var ff;

  LOOKUP_MANAGER->resolve (ff , this->factory_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyFilter::Filter_var filter =
    ff->create_filter ("ETCL" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  LOOKUP_MANAGER->_register (filter.in(), this->name_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Filter_Command::handle_add_constraint (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotifyFilter::Filter_var filter;

  LOOKUP_MANAGER->resolve (filter , this->name_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyFilter::ConstraintExpSeq constraint_list (1);
  constraint_list.length (1);

  constraint_list[0].event_types.length (0);
  constraint_list[0].constraint_expr = CORBA::string_dup (this->constraint_.c_str ());

  ACE_DEBUG ((LM_DEBUG, "Adding constraint %s\n", this->constraint_.c_str ()));
  filter->add_constraints (constraint_list ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Filter_Command::handle_add_filter (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotifyFilter::Filter_var filter;

  LOOKUP_MANAGER->resolve (filter , this->name_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyFilter::FilterAdmin_var filter_admin;

  LOOKUP_MANAGER->resolve (filter_admin , this->factory_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  filter_admin->add_filter (filter.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Filter_Command::handle_destroy_filter (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotifyFilter::Filter_var filter;

  LOOKUP_MANAGER->resolve (filter, this->name_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  filter->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Filter_Command::execute_i (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->command_ == CREATE_FACTORY)
    {
      this->handle_create_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else if (this->command_ == CREATE_FILTER)
    {
      this->handle_create_filter (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else if (this->command_ == ADD_CONSTRAINT)
    {
      this->handle_add_constraint (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else if (this->command_ == ADD_FILTER)
    {
      this->handle_add_filter (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else if (this->command_ == DESTROY)
    {
      this->handle_destroy_filter (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
}
