//$Id$

#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "tao/debug.h"
#include "Filter.h"



Filter::Filter (void)
  : event_count_ (5)
{
}

Filter::~Filter (void)
{
}

int
Filter::init (int argc, ACE_TCHAR* argv [])
{
  // Initialized the base class.
  Notify_Test_Client::init (argc,
                            argv);

  // Create all participents.
  this->create_EC ();

  CosNotifyChannelAdmin::AdminID adminid;

  this->supplier_admin_ =
    this->ec_->new_for_suppliers (this->ifgop_,
                                  adminid);

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  this->consumer_admin_ =
    this->ec_->new_for_consumers (this->ifgop_,
                                  adminid);

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));

  this->ffact_ =
    ec_->default_filter_factory ();

  return 0;
}

void
Filter::run_test (void)
{
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, " Obtaining FilterAdmin interface from ConsumerAdmin\n"));

  CosNotifyFilter::FilterAdmin_var ca_filter_admin =
    CosNotifyFilter::FilterAdmin::_narrow (consumer_admin_.in ());

  this->run_filter_test (consumer_admin_.in ());

  this->ec_->destroy ();
}

void
Filter::run_filter_test (CosNotifyFilter::FilterAdmin_ptr filter_admin)
{
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, " Calling remove_all_filters\n"));

  // Clear all filters.
  filter_admin->remove_all_filters ();

  this->verify_filter_count (filter_admin, 0);

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Adding a filter\n"));

  CosNotifyFilter::FilterID id_1 = this->add_filter (filter_admin);

  this->verify_filter_count (filter_admin, 1);

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Adding another filter\n"));

  this->add_filter (filter_admin);

  this->verify_filter_count (filter_admin, 2);

  if (TAO_debug_level)
    {
      ACE_DEBUG ((LM_DEBUG, "Calling print_filters\n"));
      this->print_filters (filter_admin);
    }

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Calling remove_filter\n"));

  // remove the filter.
  filter_admin->remove_filter (id_1);

  this->verify_filter_count (filter_admin, 1);

  if (TAO_debug_level)
    {
      ACE_DEBUG ((LM_DEBUG, "Calling print_filters\n"));
      this->print_filters (filter_admin);
    }

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Calling remove_all_filters\n"));

  filter_admin->remove_all_filters ();

  if (TAO_debug_level)
    {
      ACE_DEBUG ((LM_DEBUG, "Calling print_filters\n"));
      this->print_filters (filter_admin);
    }

  // Make sure all filters are removed -
  this->verify_filter_count (filter_admin, 0);

  ACE_DEBUG ((LM_DEBUG, "Filters test has run successfully\n"));
}

void
Filter::verify_filter_count (CosNotifyFilter::FilterAdmin_ptr filter_admin, CORBA::ULong expected_count)
{
  expected_count = expected_count; // if we don;t do this, we get a warning on linux about arg not used.
  CosNotifyFilter::FilterIDSeq_var filter_seq = filter_admin->get_all_filters ();
  ACE_ASSERT (filter_seq->length () == expected_count);
}

CosNotifyFilter::FilterID
Filter::add_filter (CosNotifyFilter::FilterAdmin_ptr filter_admin)
{
  // setup a filter at the filter admin
  CosNotifyFilter::Filter_var filter =
    this->ffact_->create_filter ("ETCL");

  ACE_ASSERT (!CORBA::is_nil (filter.in ()));

  const char* test_filter_string = "A > B";

  CosNotifyFilter::ConstraintExpSeq constraint_list (1);
  constraint_list.length (1);

  constraint_list[0].event_types.length (0);
  constraint_list[0].constraint_expr = CORBA::string_dup (test_filter_string);

  filter->add_constraints (constraint_list);

  CosNotifyFilter::FilterID id = filter_admin->add_filter (filter.in ());

  // Print the ID
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Added Filter %d\n", id));

  return id;
}

void
Filter::print_filters (CosNotifyFilter::FilterAdmin_ptr filter_admin)
{
  CosNotifyFilter::FilterIDSeq_var filter_seq = filter_admin->get_all_filters ();

  ACE_DEBUG ((LM_DEBUG, "Getting all %d filters...\n", filter_seq->length ()));

  for (CORBA::ULong i = 0; i < filter_seq->length (); ++i)
    {
        ACE_DEBUG ((LM_DEBUG, " Filter %d\n", filter_seq[i]));
    }
}


void
Filter::create_EC (void)
{
  CosNotifyChannelAdmin::ChannelID id;

  this->ec_ = notify_factory_->create_channel (this->initial_qos_,
                                               this->initial_admin_,
                                               id);

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
}

//***************************************************************************

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Filter events;

  if (events.parse_args (argc, argv) == -1)
    {
      return 1;
    }

  try
    {
      events.init (argc,
                   argv);

      events.run_test ();
    }
  catch (const CORBA::Exception& se)
    {
      se._tao_print_exception ("Error: ");
      return 1;
    }

  return 0;
}
