// ============================================================================
// $Id$
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Trader_Base.cpp
//
// = AUTHOR
//    Irfan Pyarali <irfan@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// ============================================================================

#include "Trader.h"
#include "Offer_Iterators_T.h"

// The following #include is needed only for the instantiation pragmas.
#include "Trader_Interfaces.h"

ACE_RCSID(Trader, Trader, "$Id$")

  // *************************************************************
  // TAO_Trader_Base
  // *************************************************************

TAO_Trader_Base::TAO_Trader_Base (void)
  : trading_components_ (*this),
    import_attributes_ (*this),
    support_attributes_ (*this),
    link_attributes_ (*this)
{
}

TAO_Trader_Base::~TAO_Trader_Base (void)
{
}

TAO_Import_Attributes_i &
TAO_Trader_Base::import_attributes (void)
{
  return this->import_attributes_;
}

const TAO_Import_Attributes_i &
TAO_Trader_Base::import_attributes (void) const
{
  return this->import_attributes_;
}

TAO_Support_Attributes_i &
TAO_Trader_Base::support_attributes (void)
{
  return this->support_attributes_;
}

const TAO_Support_Attributes_i &
TAO_Trader_Base::support_attributes (void) const
{
  return this->support_attributes_;
}

TAO_Link_Attributes_i &
TAO_Trader_Base::link_attributes (void)
{
  return this->link_attributes_;
}

const TAO_Link_Attributes_i &
TAO_Trader_Base::link_attributes (void) const
{
  return this->link_attributes_;
}

TAO_Trading_Components_i &
TAO_Trader_Base::trading_components (void)
{
  return this->trading_components_;
}

const TAO_Trading_Components_i &
TAO_Trader_Base::trading_components (void) const
{
  return this->trading_components_;
}

CORBA::Boolean
TAO_Trader_Base::is_valid_identifier_name (const char* ident)
{
  int return_value = 0;

  if (ident == 0)
    return return_value;

  int length = ACE_OS::strlen (ident);
  if (length >= 1 && isalpha (ident[0]))
    {
      return_value = 1;
      for (int i = 0; i < length; i++)
        {
          if (! (isalnum (ident[i]) || ident[i] == '_'))
            {
              return_value = 0;
              break;
            }
        }
    }

  return return_value;
}

  // *************************************************************
  // Attribute Classes
  // *************************************************************

TAO_Support_Attributes_i::
TAO_Support_Attributes_i (TAO_Lockable &locker)
  : locker_ (locker),
    supports_modifiable_properties_ (1),
    supports_dynamic_properties_ (1),
    supports_proxy_offers_ (0),
    type_repos_ (CosTrading::TypeRepository::_nil ()),
    service_type_repos_ (CosTradingRepos::ServiceTypeRepository::_nil ())
{
}

TAO_Support_Attributes_i::~TAO_Support_Attributes_i (void)
{
}

CORBA::Boolean
TAO_Support_Attributes_i::supports_modifiable_properties (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (),
                         0);
  return this->supports_modifiable_properties_;
}

void
TAO_Support_Attributes_i::
supports_modifiable_properties (CORBA::Boolean new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());
  this->supports_modifiable_properties_ = new_value;
}

CORBA::Boolean
TAO_Support_Attributes_i::supports_dynamic_properties (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (),
                         0);

  return this->supports_dynamic_properties_;
}

void
TAO_Support_Attributes_i::
supports_dynamic_properties (CORBA::Boolean new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());
  this->supports_dynamic_properties_ = new_value;
}

CORBA::Boolean
TAO_Support_Attributes_i::supports_proxy_offers (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (),
                         0);

  return this->supports_proxy_offers_;
}

void
TAO_Support_Attributes_i::
supports_proxy_offers (CORBA::Boolean new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());
  this->supports_proxy_offers_ = new_value;
}

CosTrading::TypeRepository_ptr
TAO_Support_Attributes_i::type_repos (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (),
                         CosTrading::TypeRepository::_nil ());
  return this->type_repos_.ptr ();
}

void
TAO_Support_Attributes_i::
type_repos (CosTrading::TypeRepository_ptr new_value)
{
  CORBA::Environment TAO_IN_ENV;
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());

  this->type_repos_ = new_value;
  this->service_type_repos_ =
    CosTradingRepos::ServiceTypeRepository::_narrow (new_value, TAO_IN_ENV);
}

CosTradingRepos::ServiceTypeRepository_ptr
TAO_Support_Attributes_i::service_type_repos (void) const
{
  return this->service_type_repos_.ptr ();
}


TAO_Import_Attributes_i::
TAO_Import_Attributes_i (TAO_Lockable &locker)
  : locker_ (locker),
    def_search_card_ (10),
    max_search_card_ (100),
    def_match_card_ (10),
    max_match_card_ (100),
    def_return_card_ (10),
    max_return_card_ (100),
    max_list_ (0),
    def_hop_count_ (5),
    max_hop_count_ (5),
    def_follow_policy_ (CosTrading::if_no_local),
    max_follow_policy_ (CosTrading::always)
{
}


TAO_Import_Attributes_i::~TAO_Import_Attributes_i (void)
{
}

CORBA::ULong
TAO_Import_Attributes_i::def_search_card (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), 10);
  return this->def_search_card_;
}

void
TAO_Import_Attributes_i::def_search_card (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());

  if (new_value > this->max_search_card_)
    this->def_search_card_ = this->max_search_card_;
  else
    this->def_search_card_ = new_value;
}

CORBA::ULong
TAO_Import_Attributes_i::max_search_card (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), 10);
  return this->max_search_card_;
}

void
TAO_Import_Attributes_i::max_search_card (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());

  this->max_search_card_ = new_value;

  if (this->def_search_card_ > this->max_search_card_)
    this->def_search_card_ = this->max_search_card_;
}

CORBA::ULong
TAO_Import_Attributes_i::def_match_card (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), 10);
  return this->def_match_card_;
}

void
TAO_Import_Attributes_i::def_match_card (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());

  if (new_value > this->max_match_card_)
    this->def_match_card_ = this->max_match_card_;
  else
    this->def_match_card_ = new_value;
}

CORBA::ULong
TAO_Import_Attributes_i::max_match_card (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), 10);
  return this->max_match_card_;
}

void
TAO_Import_Attributes_i::max_match_card (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());
  this->max_match_card_ = new_value;

  if (this->def_match_card_ > this->max_match_card_)
    this->def_match_card_ = this->max_match_card_;
}

CORBA::ULong
TAO_Import_Attributes_i::def_return_card (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), 10);
  return this->def_return_card_;
}

void
TAO_Import_Attributes_i::def_return_card (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());

  if (new_value > this->max_return_card_)
    this->def_return_card_ = this->max_return_card_;
  else
    this->def_return_card_ = new_value;
}

CORBA::ULong
TAO_Import_Attributes_i::max_return_card (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), 10);
  return this->max_return_card_;
}

void
TAO_Import_Attributes_i::max_return_card (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());
  this->max_return_card_ = new_value;

  if (this->def_return_card_ > this->max_return_card_)
    this->def_return_card_ = this->max_return_card_;
}

CORBA::ULong
TAO_Import_Attributes_i::max_list (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), 0);
  return this->max_list_;
}

void
TAO_Import_Attributes_i::max_list (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());
  this->max_list_ = new_value;
}

CORBA::ULong
TAO_Import_Attributes_i::def_hop_count (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), 0);
  return this->def_hop_count_;
}

void
TAO_Import_Attributes_i::def_hop_count (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());

  if (new_value > this->max_hop_count_)
    this->def_hop_count_ = this->max_hop_count_;
  else
    this->def_hop_count_ = new_value;
}

CORBA::ULong
TAO_Import_Attributes_i::max_hop_count (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), 0);
  return this->max_hop_count_;
}

void
TAO_Import_Attributes_i::max_hop_count (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());
  this->max_hop_count_ = new_value;

  if (this->def_hop_count_ > this->max_hop_count_)
    this->def_hop_count_ = this->max_hop_count_;
}

CosTrading::FollowOption
TAO_Import_Attributes_i::def_follow_policy (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), CosTrading::local_only);
  return this->def_follow_policy_;
}

void
TAO_Import_Attributes_i::def_follow_policy (CosTrading::FollowOption new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());

  if (new_value > this->max_follow_policy_)
    this->def_follow_policy_ = this->max_follow_policy_;
  else
    this->def_follow_policy_ = new_value;
}

CosTrading::FollowOption
TAO_Import_Attributes_i::max_follow_policy (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), CosTrading::local_only);
  return this->max_follow_policy_;
}

void
TAO_Import_Attributes_i::max_follow_policy (CosTrading::FollowOption new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());
  this->max_follow_policy_ = new_value;

  if (this->def_follow_policy_ > this->max_follow_policy_)
    this->def_follow_policy_ = this->max_follow_policy_;
}


TAO_Trading_Components_i::TAO_Trading_Components_i (TAO_Lockable &locker)
  : locker_ (locker),
    lookup_ (CosTrading::Lookup::_nil ()),
    register_ (CosTrading::Register::_nil ()),
    link_ (CosTrading::Link::_nil ()),
    proxy_ (CosTrading::Proxy::_nil ()),
    admin_ (CosTrading::Admin::_nil ())
{
}

TAO_Trading_Components_i::~TAO_Trading_Components_i (void)
{
}

CosTrading::Lookup_ptr
TAO_Trading_Components_i::lookup_if (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (),
                         CosTrading::Lookup::_nil ());
  return this->lookup_.ptr ();
}

void
TAO_Trading_Components_i::lookup_if (CosTrading::Lookup_ptr new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());
  this->lookup_ = new_value;
}

CosTrading::Register_ptr
TAO_Trading_Components_i::register_if (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (),
                         CosTrading::Register::_nil ());
  return this->register_.ptr ();
}

void
TAO_Trading_Components_i::register_if (CosTrading::Register_ptr new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());
  this->register_ = new_value;
}

CosTrading::Link_ptr
TAO_Trading_Components_i::link_if (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (),
                         CosTrading::Link::_nil ());
  return this->link_.ptr ();
}

void
TAO_Trading_Components_i::link_if (CosTrading::Link_ptr new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());
  this->link_ = new_value;
}

CosTrading::Proxy_ptr
TAO_Trading_Components_i::proxy_if (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (),
                         CosTrading::Proxy::_nil ());
  return this->proxy_.ptr ();
}

void
TAO_Trading_Components_i::proxy_if (CosTrading::Proxy_ptr new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());
  this->proxy_ = new_value;
}

CosTrading::Admin_ptr
TAO_Trading_Components_i::admin_if (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (),
                         CosTrading::Admin::_nil ());
  return this->admin_.ptr ();
}

void
TAO_Trading_Components_i::admin_if (CosTrading::Admin_ptr new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());
  this->admin_ = new_value;
}


TAO_Link_Attributes_i::TAO_Link_Attributes_i (TAO_Lockable &locker)
  :locker_ (locker),
   max_link_follow_policy_ (CosTrading::local_only)
{
}

TAO_Link_Attributes_i::~TAO_Link_Attributes_i (void)
{
}

CosTrading::FollowOption
TAO_Link_Attributes_i::max_link_follow_policy (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (),
                         CosTrading::local_only);
  return this->max_link_follow_policy_;
}

void
TAO_Link_Attributes_i::max_link_follow_policy (CosTrading::FollowOption new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());
  this->max_link_follow_policy_ = new_value;
}


// = Methods to deal with ACE_Hash_Map_Manager.

TAO_String_Hash_Key::TAO_String_Hash_Key (void)
{
}

TAO_String_Hash_Key::TAO_String_Hash_Key (char * name)
  : CORBA_String_var (name)
{
}

TAO_String_Hash_Key::TAO_String_Hash_Key (const char * name)
  : CORBA_String_var (name)
{
}

TAO_String_Hash_Key::TAO_String_Hash_Key (const CORBA::String_var &src)
  : CORBA_String_var (src)
{
}

int
TAO_String_Hash_Key::operator == (const TAO_String_Hash_Key &hash_key) const
{
  return ACE_OS::strcmp (this->in (), hash_key.in ()) == 0;
}

int
operator < (const TAO_String_Hash_Key &left,
            const TAO_String_Hash_Key &right)
{
  return ACE_OS::strcmp (left.in (), right.in ()) < 0;
}


u_long
TAO_String_Hash_Key::hash (void) const
{
  u_long ret = ACE::hash_pjw (this->in ());
  return ret;
}

TAO_String_Hash_Key::~TAO_String_Hash_Key (void)
{
}

int
operator< (const CosTradingRepos::ServiceTypeRepository::IncarnationNumber &l,
           const CosTradingRepos::ServiceTypeRepository::IncarnationNumber &r)
{
  if (l.high < r.high)
    return 1;
  else if (l.high == r.high)
    return (l.low < r.low);
  else
    return 0;
}

int
operator> (const CosTradingRepos::ServiceTypeRepository::IncarnationNumber &l,
           const CosTradingRepos::ServiceTypeRepository::IncarnationNumber &r)
{
  return (r < l);
}

int
operator== (const CosTrading::Admin::OctetSeq& left,
            const CosTrading::Admin::OctetSeq& right)
{
  int return_value = 0;
  CORBA::ULong left_length = left.length (),
    right_length = right.length ();

  if (left_length == right_length)
    {
      return_value = 1;
      for (CORBA::ULong i = 0; i < left_length; i++)
        {
          if (left[i] != right[i])
            {
              return_value = 0;
              break;
            }
        }
    }

  return return_value;
}

  // *************************************************************
  // TAO_Trader_Factory
  // *************************************************************

#include "ace/Arg_Shifter.h"
#include "Trader_T.h"

TAO_Trader_Factory::TAO_TRADER*
TAO_Trader_Factory::create_trader (int& argc, char** argv)
{
  TAO_Trader_Factory trader_factory (argc, argv);
  return trader_factory.manufacture_trader ();
}

TAO_Trader_Factory::TAO_Trader_Factory (int& argc, char** argv)
  : conformance_ (TAO_TRADER_LINKED),
    threadsafe_ (0),
    supports_dynamic_properties_ (1),
    supports_modifiable_properties_ (1),
    def_search_card_ (20),
    max_search_card_ (50),
    def_match_card_ (20),
    max_match_card_ (50),
    def_return_card_ (20),
    max_return_card_ (50),
    def_hop_count_ (5),
    max_hop_count_ (10),
    def_follow_policy_ (CosTrading::if_no_local),
    max_follow_policy_ (CosTrading::always)
{
  this->parse_args (argc, argv);
}

TAO_Trader_Factory::TAO_TRADER*
TAO_Trader_Factory::manufacture_trader (void)
{
  typedef TAO_Trader<ACE_Null_Mutex, ACE_Null_Mutex> TRADER;

#if defined ACE_HAS_THREADS
  typedef TAO_Trader<ACE_Thread_Mutex, ACE_RW_Thread_Mutex>  MT_TRADER;
#else
  typedef TAO_Trader<ACE_Null_Mutex, ACE_Null_Mutex>  MT_TRADER;
#endif /* ACE_HAS_THREADS */

  TAO_TRADER* return_value = 0;
  int components = ACE_static_cast (int, TAO_Trader_Base::LOOKUP);

  if (this->conformance_ >= TAO_TRADER_SIMPLE)
    components |= ACE_static_cast (int, TAO_Trader_Base::REGISTER);

  if (this->conformance_ >= TAO_TRADER_STANDALONE)
    components |= ACE_static_cast (int, TAO_Trader_Base::ADMIN);

  if (this->conformance_ >= TAO_TRADER_LINKED)
    components |= ACE_static_cast (int, TAO_Trader_Base::LINK);

  if (this->threadsafe_)
    {
      ACE_NEW_RETURN (return_value,
                      MT_TRADER (ACE_static_cast (TAO_Trader_Base::Trader_Components,
                                                  components)),
                      0);
    }
  else
    {
      ACE_NEW_RETURN (return_value,
                      TRADER (ACE_static_cast (TAO_Trader_Base::Trader_Components,
                                               components)),
                      0);
    }

  TAO_Import_Attributes_i import_attributes =
    return_value->import_attributes ();
  TAO_Support_Attributes_i support_attributes =
    return_value->support_attributes ();

  import_attributes.def_search_card (this->def_search_card_);
  import_attributes.max_search_card (this->max_search_card_);
  import_attributes.def_match_card (this->def_match_card_);
  import_attributes.max_match_card (this->max_match_card_);
  import_attributes.def_return_card (this->def_return_card_);
  import_attributes.max_return_card (this->max_return_card_);
  import_attributes.def_hop_count (this->def_hop_count_);
  import_attributes.max_hop_count (this->max_hop_count_);
  import_attributes.def_follow_policy (this->def_follow_policy_);
  import_attributes.max_follow_policy (this->max_follow_policy_);
  support_attributes.supports_modifiable_properties (this->supports_modifiable_properties_);
  support_attributes.supports_dynamic_properties (this->supports_dynamic_properties_);

  return return_value;
}

void
TAO_Trader_Factory::parse_args (int& argc, char** argv)
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      char *current_arg = arg_shifter.get_current ();

      if (ACE_OS::strcmp (current_arg, "-TSthreadsafe") == 0)
        {
          arg_shifter.consume_arg ();
          this->threadsafe_ = 1;
        }
      else if (ACE_OS::strcmp (current_arg, "-TSconformance") == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              char* conformance_str = arg_shifter.get_current ();

              if (ACE_OS::strcasecmp (conformance_str, "Linked") == 0)
                this->conformance_ = TAO_TRADER_LINKED;
              else if (ACE_OS::strcasecmp (conformance_str, "Query") == 0)
                this->conformance_ = TAO_TRADER_QUERY;
              else if (ACE_OS::strcasecmp (conformance_str, "Simple") == 0)
                this->conformance_ = TAO_TRADER_SIMPLE;
              else if (ACE_OS::strcasecmp (conformance_str, "Standalone") == 0)
                this->conformance_ = TAO_TRADER_STANDALONE;

              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-TSsupports_dynamic_properties") == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              char* arg_str = arg_shifter.get_current ();

              if (ACE_OS::strcasecmp (arg_str, "true") == 0)
                this->supports_dynamic_properties_ = 1;
              else if (ACE_OS::strcasecmp (arg_str, "false") == 0)
                this->supports_dynamic_properties_ = 0;

              arg_shifter.consume_arg ();
            }

        }
      else if (ACE_OS::strcmp (current_arg, "-TSsupports_modifiable_properties") == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              char* arg_str = arg_shifter.get_current ();

              if (ACE_OS::strcasecmp (arg_str, "true") == 0)
                this->supports_modifiable_properties_ = 1;
              else if (ACE_OS::strcasecmp (arg_str, "false") == 0)
                this->supports_modifiable_properties_ = 0;

              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-TSdef_search_card") == 0 ||
               ACE_OS::strcmp (current_arg, "-TSmax_search_card") == 0 ||
               ACE_OS::strcmp (current_arg, "-TSdef_match_card") == 0 ||
               ACE_OS::strcmp (current_arg, "-TSmax_match_card") == 0 ||
               ACE_OS::strcmp (current_arg, "-TSdef_return_card") == 0 ||
               ACE_OS::strcmp (current_arg, "-TSmax_return_card") == 0 ||
               ACE_OS::strcmp (current_arg, "-TSdef_hop_count") == 0 ||
               ACE_OS::strcmp (current_arg, "-TSmax_hop_count") == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              CORBA::ULong value =
                ACE_static_cast (CORBA::ULong,
                                 ACE_OS::atoi (arg_shifter.get_current ()));
              arg_shifter.consume_arg ();

              if (ACE_OS::strstr (current_arg, "card"))
                {
                  if (ACE_OS::strstr (current_arg, "max"))
                    {
                      if (ACE_OS::strstr (current_arg, "search"))
                        this->max_search_card_ = value;
                      else if (ACE_OS::strstr (current_arg, "match"))
                        this->max_match_card_ = value;
                      else
                        this->max_return_card_ = value;
                    }
                  else
                    {
                      if (ACE_OS::strstr (current_arg, "search"))
                        this->def_search_card_ = value;
                      else if (ACE_OS::strstr (current_arg, "match"))
                        this->def_match_card_ = value;
                      else
                        this->def_return_card_ = value;
                    }
                }
              else
                {
                  if (ACE_OS::strstr (current_arg, "max"))
                    this->max_hop_count_ = value;
                  else
                    this->def_hop_count_ = value;
                }
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-TSdef_follow_policy") == 0 ||
               ACE_OS::strcmp (current_arg, "-TSmax_follow_policy") == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              char* arg_str = arg_shifter.get_current ();
              CosTrading::FollowOption follow_option;

              if (ACE_OS::strcasecmp (arg_str, "always") == 0)
                follow_option = CosTrading::always;
              else if (ACE_OS::strcasecmp (arg_str, "if_no_local") == 0)
                follow_option = CosTrading::if_no_local;
              else if (ACE_OS::strcasecmp (arg_str, "local_only") == 0)
                follow_option = CosTrading::local_only;
              else if (ACE_OS::strstr (current_arg, "def"))
                follow_option = this->def_follow_policy_;
              else
                follow_option = this->max_follow_policy_;

              if (ACE_OS::strstr (current_arg, "def"))
                this->def_follow_policy_ = follow_option;
              else
                this->max_follow_policy_ = follow_option;

              arg_shifter.consume_arg ();
            }
        }
      else
        arg_shifter.ignore_arg ();
    }
}

  // *************************************************************
  // TAO_Sequence_Extracter_Base
  // *************************************************************

CORBA::TCKind
TAO_Sequence_Extracter_Base::
sequence_type (CORBA::TypeCode* type_code,
               CORBA::Environment& env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::TCKind return_value = CORBA::tk_void,
    type_kind = type_code->kind (env);

  if (type_kind == CORBA::tk_alias || type_kind == CORBA::tk_sequence)
    {
      CORBA::TypeCode_ptr base = type_code;

      while (base->kind (env) == CORBA::tk_alias)
        base = base->content_type (env);

      if (base->kind (env) == CORBA::tk_sequence)
        {
          base = base->content_type (env);
          TAO_CHECK_ENV_RETURN (env, return_value);

          return_value = base->kind (env);
        }
    }

  return return_value;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
template class TAO_Offer_Database<ACE_RW_Thread_Mutex>;
template class TAO_Service_Offer_Iterator<ACE_RW_Thread_Mutex>;
template class TAO_Trader<ACE_Thread_Mutex, ACE_RW_Thread_Mutex>;
template class TAO_Lookup<ACE_Thread_Mutex, ACE_RW_Thread_Mutex>;
template class TAO_Register<ACE_Thread_Mutex, ACE_RW_Thread_Mutex>;
template class TAO_Admin<ACE_Thread_Mutex, ACE_RW_Thread_Mutex>;
template class TAO_Link<ACE_Thread_Mutex, ACE_RW_Thread_Mutex>;
template class TAO_Proxy<ACE_Thread_Mutex, ACE_RW_Thread_Mutex>;
template class TAO_Register_Offer_Iterator<ACE_RW_Thread_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, CosTrading::Link::LinkInfo, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_RW_Thread_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, CosTrading::Link::LinkInfo, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_RW_Thread_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, CosTrading::Link::LinkInfo, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_RW_Thread_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, CosTrading::Link::LinkInfo, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_RW_Thread_Mutex>;
template class ACE_Hash_Map_Entry<TAO_String_Hash_Key,TAO_Offer_Database<ACE_RW_Thread_Mutex>::Offer_Map_Entry*>;
template class ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, TAO_Offer_Database<ACE_RW_Thread_Mutex>::Offer_Map_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, TAO_Offer_Database<ACE_RW_Thread_Mutex>::Offer_Map_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, TAO_Offer_Database<ACE_RW_Thread_Mutex>::Offer_Map_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, TAO_Offer_Database<ACE_RW_Thread_Mutex>::Offer_Map_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
#endif /* ACE_MT_SAFE */
template class TAO_Offer_Database<ACE_Null_Mutex>;
template class TAO_Service_Offer_Iterator<ACE_Null_Mutex>;
template class TAO_Register_Offer_Iterator<ACE_Null_Mutex>;
template class TAO_Trader_Components<POA_CosTrading::Lookup>;
template class TAO_Support_Attributes<POA_CosTrading::Lookup>;
template class TAO_Import_Attributes<POA_CosTrading::Lookup>;
template class TAO_Trader_Components<POA_CosTrading::Register>;
template class TAO_Support_Attributes<POA_CosTrading::Register>;
template class TAO_Trader_Components<POA_CosTrading::Admin>;
template class TAO_Support_Attributes<POA_CosTrading::Admin>;
template class TAO_Import_Attributes<POA_CosTrading::Admin>;
template class TAO_Link_Attributes<POA_CosTrading::Admin>;
template class TAO_Trader_Components<POA_CosTrading::Link>;
template class TAO_Support_Attributes<POA_CosTrading::Link>;
template class TAO_Link_Attributes<POA_CosTrading::Link>;
template class TAO_Trader_Components<POA_CosTrading::Proxy>;
template class TAO_Support_Attributes<POA_CosTrading::Proxy>;
template class TAO_Trader<ACE_Null_Mutex, ACE_Null_Mutex>;
template class TAO_Lookup<ACE_Null_Mutex, ACE_Null_Mutex>;
template class TAO_Register<ACE_Null_Mutex, ACE_Null_Mutex>;
template class TAO_Admin<ACE_Null_Mutex, ACE_Null_Mutex>;
template class TAO_Link<ACE_Null_Mutex, ACE_Null_Mutex>;
template class TAO_Proxy<ACE_Null_Mutex, ACE_Null_Mutex>;
# if defined (ACE_HAS_MOSTLY_UNICODE_APIS)
    // These aren't needed unless ACE_HAS_MOSTLY_UNICODE_APIS.
    // Otherwise, they're in ace/ARGV.cpp.
    template class ACE_Node<char*>;
    template class ACE_Unbounded_Queue<char*>;
    template class ACE_Unbounded_Queue_Iterator<char*>;
#endif /* ACE_HAS_MOSTLY_UNICODE_APIS */
template class ACE_Node<CosTrading::Admin::OctetSeq *>;
template class ACE_Unbounded_Queue<CosTrading::Admin::OctetSeq *>;
template class ACE_Unbounded_Queue_Iterator<CosTrading::Admin::OctetSeq *>;
template class ACE_Node<TAO_String_Hash_Key>;
template class ACE_Unbounded_Set<TAO_String_Hash_Key>;
template class ACE_Unbounded_Set_Iterator<TAO_String_Hash_Key>;
template class ACE_Hash<TAO_String_Hash_Key>;
template class ACE_Equal_To<TAO_String_Hash_Key>;
template class ACE_Hash_Map_Entry<TAO_String_Hash_Key, int>;
template class ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, int, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, int, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, int, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, int, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<TAO_String_Hash_Key, CORBA::TypeCode_ptr>;
template class ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, CORBA::TypeCode_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, CORBA::TypeCode_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, CORBA::TypeCode_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, CORBA::TypeCode_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<CORBA::ULong, CosTrading::Offer*>;
template class ACE_Hash_Map_Manager_Ex<CORBA::ULong, CosTrading::Offer*, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<CORBA::ULong, CosTrading::Offer*, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<CORBA::ULong, CosTrading::Offer*, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<CORBA::ULong, CosTrading::Offer*, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<TAO_String_Hash_Key, CosTrading::Link::LinkInfo>;
template class ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, CosTrading::Link::LinkInfo, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, CosTrading::Link::LinkInfo, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, CosTrading::Link::LinkInfo, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, CosTrading::Link::LinkInfo, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<TAO_String_Hash_Key,TAO_Offer_Database<ACE_Null_Mutex>::Offer_Map_Entry*>;
template class ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, TAO_Offer_Database<ACE_Null_Mutex>::Offer_Map_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, TAO_Offer_Database<ACE_Null_Mutex>::Offer_Map_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, TAO_Offer_Database<ACE_Null_Mutex>::Offer_Map_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, TAO_Offer_Database<ACE_Null_Mutex>::Offer_Map_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
#pragma instantiate TAO_Register_Offer_Iterator<ACE_RW_Thread_Mutex>
#pragma instantiate TAO_Offer_Database<ACE_RW_Thread_Mutex>
#pragma instantiate TAO_Service_Offer_Iterator<ACE_RW_Thread_Mutex>
#pragma instantiate TAO_Trader<ACE_Thread_Mutex, ACE_RW_Thread_Mutex>
#pragma instantiate TAO_Lookup<ACE_Thread_Mutex, ACE_RW_Thread_Mutex>
#pragma instantiate TAO_Register<ACE_Thread_Mutex, ACE_RW_Thread_Mutex>
#pragma instantiate TAO_Admin<ACE_Thread_Mutex, ACE_RW_Thread_Mutex>
#pragma instantiate TAO_Link<ACE_Thread_Mutex, ACE_RW_Thread_Mutex>
#pragma instantiate TAO_Proxy<ACE_Thread_Mutex, ACE_RW_Thread_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, CosTrading::Link::LinkInfo, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_RW_Thread_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, CosTrading::Link::LinkInfo, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_RW_Thread_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, CosTrading::Link::LinkInfo, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_RW_Thread_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, CosTrading::Link::LinkInfo, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_RW_Thread_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<TAO_String_Hash_Key,TAO_Offer_Database<ACE_RW_Thread_Mutex>::Offer_Map_Entry*>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, TAO_Offer_Database<ACE_RW_Thread_Mutex>::Offer_Map_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, TAO_Offer_Database<ACE_RW_Thread_Mutex>::Offer_Map_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, TAO_Offer_Database<ACE_RW_Thread_Mutex>::Offer_Map_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, TAO_Offer_Database<ACE_RW_Thread_Mutex>::Offer_Map_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#endif /* ACE_MT_SAFE */
#pragma instantiate TAO_Offer_Database<ACE_Null_Mutex>
#pragma instantiate TAO_Service_Offer_Iterator<ACE_Null_Mutex>
#pragma instantiate TAO_Register_Offer_Iterator<ACE_Null_Mutex>
#pragma instantiate TAO_Trader_Components<POA_CosTrading::Lookup>
#pragma instantiate TAO_Support_Attributes<POA_CosTrading::Lookup>
#pragma instantiate TAO_Import_Attributes<POA_CosTrading::Lookup>
#pragma instantiate TAO_Trader_Components<POA_CosTrading::Register>
#pragma instantiate TAO_Support_Attributes<POA_CosTrading::Register>
#pragma instantiate TAO_Trader_Components<POA_CosTrading::Admin>
#pragma instantiate TAO_Support_Attributes<POA_CosTrading::Admin>
#pragma instantiate TAO_Import_Attributes<POA_CosTrading::Admin>
#pragma instantiate TAO_Link_Attributes<POA_CosTrading::Admin>
#pragma instantiate TAO_Trader_Components<POA_CosTrading::Link>
#pragma instantiate TAO_Support_Attributes<POA_CosTrading::Link>
#pragma instantiate TAO_Link_Attributes<POA_CosTrading::Link>
#pragma instantiate TAO_Trader_Components<POA_CosTrading::Proxy>
#pragma instantiate TAO_Support_Attributes<POA_CosTrading::Proxy>
#pragma instantiate TAO_Trader<ACE_Null_Mutex, ACE_Null_Mutex>
#pragma instantiate TAO_Lookup<ACE_Null_Mutex, ACE_Null_Mutex>
#pragma instantiate TAO_Register<ACE_Null_Mutex, ACE_Null_Mutex>
#pragma instantiate TAO_Admin<ACE_Null_Mutex, ACE_Null_Mutex>
#pragma instantiate TAO_Link<ACE_Null_Mutex, ACE_Null_Mutex>
#pragma instantiate TAO_Proxy<ACE_Null_Mutex, ACE_Null_Mutex>
# if defined (ACE_HAS_MOSTLY_UNICODE_APIS)
    // These aren't needed unless ACE_HAS_MOSTLY_UNICODE_APIS.
    // Otherwise, they're in ace/ARGV.cpp.
#   pragma instantiate ACE_Node<char*>
#   pragma instantiate ACE_Unbounded_Queue<char*>
#   pragma instantiate ACE_Unbounded_Queue_Iterator<char*>
# endif /* ACE_HAS_MOSTLY_UNICODE_APIS */
#pragma instantiate ACE_Node<CosTrading::Admin::OctetSeq *>
#pragma instantiate ACE_Unbounded_Queue<CosTrading::Admin::OctetSeq *>
#pragma instantiate ACE_Unbounded_Queue_Iterator<CosTrading::Admin::OctetSeq *>
#pragma instantiate ACE_Node<TAO_String_Hash_Key>
#pragma instantiate ACE_Unbounded_Set<TAO_String_Hash_Key>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_String_Hash_Key>
#pragma instantiate ACE_Hash<TAO_String_Hash_Key>
#pragma instantiate ACE_Equal_To<TAO_String_Hash_Key>
#pragma instantiate ACE_Hash_Map_Entry<TAO_String_Hash_Key, int>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, int, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, int, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, int, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, int, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<TAO_String_Hash_Key, CORBA::TypeCode_ptr>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, CORBA::TypeCode_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, CORBA::TypeCode_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, CORBA::TypeCode_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, CORBA::TypeCode_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<CORBA::ULong, CosTrading::Offer*>
#pragma instantiate ACE_Hash_Map_Manager_Ex<CORBA::ULong, CosTrading::Offer*, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<CORBA::ULong, CosTrading::Offer*, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<CORBA::ULong, CosTrading::Offer*, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<CORBA::ULong, CosTrading::Offer*, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<TAO_String_Hash_Key, CosTrading::Link::LinkInfo>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, CosTrading::Link::LinkInfo, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, CosTrading::Link::LinkInfo, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, CosTrading::Link::LinkInfo, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, CosTrading::Link::LinkInfo, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<TAO_String_Hash_Key,TAO_Offer_Database<ACE_Null_Mutex>::Offer_Map_Entry*>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, TAO_Offer_Database<ACE_Null_Mutex>::Offer_Map_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, TAO_Offer_Database<ACE_Null_Mutex>::Offer_Map_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, TAO_Offer_Database<ACE_Null_Mutex>::Offer_Map_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, TAO_Offer_Database<ACE_Null_Mutex>::Offer_Map_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
