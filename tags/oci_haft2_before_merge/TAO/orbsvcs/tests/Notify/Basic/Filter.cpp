//$Id$

#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "tao/debug.h"
#include "Filter.h"

ACE_RCSID (Notify_Tests, Filter, "$Id$")

Filter::Filter (void)
  : event_count_ (5)
{
}

Filter::~Filter (void)
{
}

int
Filter::init (int argc, char* argv []
                   ACE_ENV_ARG_DECL)
{
  // Initialized the base class.
  Notify_Test_Client::init (argc,
                            argv
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Create all participents.
  this->create_EC (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CosNotifyChannelAdmin::AdminID adminid;

  this->supplier_admin_ =
    this->ec_->new_for_suppliers (this->ifgop_,
                                  adminid
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  this->consumer_admin_ =
    this->ec_->new_for_consumers (this->ifgop_,
                                  adminid
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));

  this->ffact_ =
    ec_->default_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

void
Filter::run_test (ACE_ENV_SINGLE_ARG_DECL)
{
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, " Obtaining FilterAdmin interface from ConsumerAdmin\n"));

  CosNotifyFilter::FilterAdmin_var ca_filter_admin =
    CosNotifyFilter::FilterAdmin::_narrow (consumer_admin_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->run_filter_test (consumer_admin_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->ec_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Filter::run_filter_test (CosNotifyFilter::FilterAdmin_ptr filter_admin ACE_ENV_ARG_DECL)
{
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, " Calling remove_all_filters\n"));

  // Clear all filters.
  filter_admin->remove_all_filters (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->verify_filter_count (filter_admin, 0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Adding a filter \n"));

  CosNotifyFilter::FilterID id_1 = this->add_filter (filter_admin ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->verify_filter_count (filter_admin, 1 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Adding another filter \n"));

  this->add_filter (filter_admin ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->verify_filter_count (filter_admin, 2 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (TAO_debug_level)
    {
      ACE_DEBUG ((LM_DEBUG, "Calling print_filters \n"));
      this->print_filters (filter_admin ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Calling remove_filter\n"));

  // remove the filter.
  filter_admin->remove_filter (id_1 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->verify_filter_count (filter_admin, 1 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (TAO_debug_level)
    {
      ACE_DEBUG ((LM_DEBUG, "Calling print_filters \n"));
      this->print_filters (filter_admin ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Calling remove_all_filters \n"));

  filter_admin->remove_all_filters (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (TAO_debug_level)
    {
      ACE_DEBUG ((LM_DEBUG, "Calling print_filters \n"));
      this->print_filters (filter_admin ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  // Make sure all filters are removed -
  this->verify_filter_count (filter_admin, 0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG, "Filters test has run successfully\n"));
}

void
Filter::verify_filter_count (CosNotifyFilter::FilterAdmin_ptr filter_admin, CORBA::ULong expected_count ACE_ENV_ARG_DECL)
{
  expected_count = expected_count; // if we don;t do this, we get a warning on linux about arg not used.
  CosNotifyFilter::FilterIDSeq_var filter_seq = filter_admin->get_all_filters (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_ASSERT (filter_seq->length () == expected_count);
}

CosNotifyFilter::FilterID
Filter::add_filter (CosNotifyFilter::FilterAdmin_ptr filter_admin ACE_ENV_ARG_DECL)
{
  // setup a filter at the filter admin
  CosNotifyFilter::Filter_var filter =
    this->ffact_->create_filter ("ETCL" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_ASSERT (!CORBA::is_nil (filter.in ()));

  const char* test_filter_string = "A > B";

  CosNotifyFilter::ConstraintExpSeq constraint_list (1);
  constraint_list.length (1);

  constraint_list[0].event_types.length (0);
  constraint_list[0].constraint_expr = CORBA::string_dup (test_filter_string);

  filter->add_constraints (constraint_list ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CosNotifyFilter::FilterID id = filter_admin->add_filter (filter.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Print the ID
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Added Filter %d\n", id));

  return id;
}

void
Filter::print_filters (CosNotifyFilter::FilterAdmin_ptr filter_admin ACE_ENV_ARG_DECL)
{
  CosNotifyFilter::FilterIDSeq_var filter_seq = filter_admin->get_all_filters (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG, "Getting all %d filters...\n ", filter_seq->length ()));

  for (CORBA::ULong i = 0; i < filter_seq->length (); ++i)
    {
        ACE_DEBUG ((LM_DEBUG, " Filter %d\n", filter_seq[i]));
    }
}


void
Filter::create_EC (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotifyChannelAdmin::ChannelID id;

  this->ec_ = notify_factory_->create_channel (this->initial_qos_,
                                               this->initial_admin_,
                                               id
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
}

//***************************************************************************

int
main (int argc, char* argv[])
{
  Filter events;

  if (events.parse_args (argc, argv) == -1)
    {
      return 1;
    }

  ACE_TRY_NEW_ENV
    {
      events.init (argc,
                   argv
                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      events.run_test (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::UserException, ue)
    {
      ACE_PRINT_EXCEPTION (ue,
                           "Events user error: ");
      return 1;
    }
  ACE_CATCH (CORBA::SystemException, se)
    {
      ACE_PRINT_EXCEPTION (se,
                           "Events system error: ");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)


#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)


#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
