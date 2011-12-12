// $Id$

#include "Filter_Command.h"
#include "ace/Log_Msg.h"



#include "LookupManager.h"
#include "Name.h"

TAO_Notify_Tests_Filter_Command::TAO_Notify_Tests_Filter_Command (void)
{
}

TAO_Notify_Tests_Filter_Command::~TAO_Notify_Tests_Filter_Command ()
{
}

const char*
TAO_Notify_Tests_Filter_Command::get_name (void)
{
  return TAO_Notify_Tests_Filter_Command::name ();
}

const char*
TAO_Notify_Tests_Filter_Command::name (void)
{
  return TAO_Notify_Tests_Name::filter_command;
}

void
TAO_Notify_Tests_Filter_Command::init (ACE_Arg_Shifter& arg_shifter)
{
  if (arg_shifter.is_anything_left ())
    {
      if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-CreateFactory")) == 0) // -Create factory_name ec
        {
          this->command_ = CREATE_FACTORY;

          arg_shifter.consume_arg ();

          this->name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ()); // FF name
          arg_shifter.consume_arg ();

          this->factory_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ()); //EC
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-CreateFilter")) == 0) // -CreateFilter filter_name filterfactory_name
        {
          this->command_ = CREATE_FILTER;

          arg_shifter.consume_arg ();

          this->name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ()); // Filter name

          arg_shifter.consume_arg ();

          this->factory_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ()); //FF
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Add_Constraint")) == 0) // -Add_Constraint filter_name constraint_expr
        {
          this->command_ = ADD_CONSTRAINT;

          arg_shifter.consume_arg ();

          this->name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ()); // Filter name

          arg_shifter.consume_arg ();

          this->constraint_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ()); //Constraint
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Add_Filter")) == 0) // -Add_Filter filter_name FilterAdmin_Name
        {
          this->command_ = ADD_FILTER;

          arg_shifter.consume_arg ();

          this->name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ()); // Filter name

          arg_shifter.consume_arg ();

          this->factory_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ()); //FilterAdmin
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Destroy")) == 0) // -Destroy filter_name
        {
          this->command_ = DESTROY;

          arg_shifter.consume_arg ();

          this->name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ()); // filter

          arg_shifter.consume_arg ();
        }
    }
}

void
TAO_Notify_Tests_Filter_Command::handle_create_filter_factory (void)
{
  CosNotifyChannelAdmin::EventChannel_var ec;

  LOOKUP_MANAGER->resolve (ec, this->factory_.c_str ());

  CosNotifyFilter::FilterFactory_var ff =
    ec->default_filter_factory ();

  LOOKUP_MANAGER->_register (ff.in(), this->name_.c_str ());
}

void
TAO_Notify_Tests_Filter_Command::handle_create_filter (void)
{
  CosNotifyFilter::FilterFactory_var ff;

  LOOKUP_MANAGER->resolve (ff , this->factory_.c_str ());

  CosNotifyFilter::Filter_var filter =
    ff->create_filter ("ETCL");

  LOOKUP_MANAGER->_register (filter.in(), this->name_.c_str ());
}

void
TAO_Notify_Tests_Filter_Command::handle_add_constraint (void)
{
  CosNotifyFilter::Filter_var filter;

  LOOKUP_MANAGER->resolve (filter , this->name_.c_str ());

  CosNotifyFilter::ConstraintExpSeq constraint_list (1);
  constraint_list.length (1);

  constraint_list[0].event_types.length (0);
  constraint_list[0].constraint_expr = CORBA::string_dup (this->constraint_.c_str ());

  ACE_DEBUG ((LM_DEBUG, "Adding constraint %s\n", this->constraint_.c_str ()));
  filter->add_constraints (constraint_list);
}

void
TAO_Notify_Tests_Filter_Command::handle_add_filter (void)
{
  CosNotifyFilter::Filter_var filter;

  LOOKUP_MANAGER->resolve (filter , this->name_.c_str ());

  CosNotifyFilter::FilterAdmin_var filter_admin;

  LOOKUP_MANAGER->resolve (filter_admin , this->factory_.c_str ());

  filter_admin->add_filter (filter.in ());
}

void
TAO_Notify_Tests_Filter_Command::handle_destroy_filter (void)
{
  CosNotifyFilter::Filter_var filter;

  LOOKUP_MANAGER->resolve (filter, this->name_.c_str ());

  filter->destroy ();
}

void
TAO_Notify_Tests_Filter_Command::execute_i (void)
{
  if (this->command_ == CREATE_FACTORY)
    {
      this->handle_create_filter_factory ();
    }
  else if (this->command_ == CREATE_FILTER)
    {
      this->handle_create_filter ();
    }
  else if (this->command_ == ADD_CONSTRAINT)
    {
      this->handle_add_constraint ();
    }
  else if (this->command_ == ADD_FILTER)
    {
      this->handle_add_filter ();
    }
  else if (this->command_ == DESTROY)
    {
      this->handle_destroy_filter ();
    }
}
