// $Id$

#include "orbsvcs/Trader/Trader.h"
#include "orbsvcs/Trader/Offer_Iterators_T.h"
#include "ace/Thread_Mutex.h"
#include "ace/RW_Thread_Mutex.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_ctype.h"

// The following #include is needed only for the instantiation pragmas.
#include "orbsvcs/Trader/Trader_Interfaces.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
TAO_Trader_Base::is_valid_property_name (const char* ident)
{
  bool return_value = false;

  if (ident == 0)
    return return_value;

  size_t length = ACE_OS::strlen (ident);
  if (length >= 1 && ACE_OS::ace_isalpha (ident[0]))
    {
      return_value = true;
      for (size_t i = 0; i < length; i++)
        {
          if (! (ACE_OS::ace_isalnum (ident[i]) || ident[i] == '_'))
            {
              return_value = false;
              break;
            }
        }
    }

  return return_value;
}

CORBA::Boolean
TAO_Trader_Base::is_valid_identifier_name (const char* ident)
{
  static char const * const double_colon = "::";

  if (ident == 0)
    return 0;

  int return_value = 1;

  // Allow scoped identifiers
  CORBA::Boolean done = 0;
  char const * pos =
    ACE_OS::strstr (ident,
                    double_colon);

  do
  {
    if ('_' == ident[0])
      {
        // Treat escaped identifiers the same way as IDL
        ++ident;
      }

    size_t length =
      pos ? pos - ident : ACE_OS::strlen (ident);

    if (length >= 1 && ACE_OS::ace_isalpha (ident[0]))
      {
        // First character must be alpha
        for (size_t i = 0; i < length; ++i)
          {
            if (! (ACE_OS::ace_isalnum (ident[i])
                   || ident[i] == '_'))
              {
                // Subsequent characters is not alpha, numeric, or '_'
                return_value = 0;
                break;
              }
          }
      }
    else
      return_value = 0;

    if (pos)
      {
        // More identifiers
        ident = pos + 2;
        pos = ACE_OS::strstr (ident, double_colon);
      }
    else
      {
        // Last or only identifier
        done = 1;
      }
  }
  while (!done);

  return return_value;
}

TAO_Lockable::~TAO_Lockable (void)
{
}

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
  // @@ Seth, There is no way to propagate the exception out.
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());

  this->type_repos_ = new_value;
  // @@ What can we do even if we catch this?
  this->service_type_repos_ =
    CosTradingRepos::ServiceTypeRepository::_narrow (new_value);
}

CosTradingRepos::ServiceTypeRepository_ptr
TAO_Support_Attributes_i::service_type_repos (void) const
{
  return this->service_type_repos_.ptr ();
}


TAO_Import_Attributes_i::
TAO_Import_Attributes_i (TAO_Lockable &locker)
  : locker_ (locker),
    def_search_card_ (200),
    max_search_card_ (500),
    def_match_card_ (200),
    max_match_card_ (500),
    def_return_card_ (200),
    max_return_card_ (500),
    max_list_ (0),
    def_hop_count_ (5),
    max_hop_count_ (10),
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

bool
operator< (const CosTradingRepos::ServiceTypeRepository::IncarnationNumber &l,
           const CosTradingRepos::ServiceTypeRepository::IncarnationNumber &r)
{
  if (l.high < r.high)
    return true;
  else if (l.high == r.high)
    return (l.low < r.low);
  else
    return false;
}

bool
operator> (const CosTradingRepos::ServiceTypeRepository::IncarnationNumber &l,
           const CosTradingRepos::ServiceTypeRepository::IncarnationNumber &r)
{
  return (r < l);
}

bool
operator== (const CosTrading::Admin::OctetSeq& left,
            const CosTrading::Admin::OctetSeq& right)
{
  bool return_value = false;

  const CORBA::ULong left_length = left.length ();
  const CORBA::ULong right_length = right.length ();

  if (left_length == right_length)
    {
      return_value = true;

      for (CORBA::ULong i = 0; i < left_length; ++i)
        {
          if (left[i] != right[i])
            {
              return_value = false;
              break;
            }
        }
    }

  return return_value;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include "ace/Arg_Shifter.h"
#include "orbsvcs/Trader/Trader_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Trader_Factory::TAO_TRADER*
TAO_Trader_Factory::create_trader (int& argc, ACE_TCHAR** argv)
{
  TAO_Trader_Factory trader_factory (argc, argv);
  return trader_factory.manufacture_trader ();
}

TAO_Trader_Factory::TAO_Trader_Factory (int& argc, ACE_TCHAR** argv)
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

TAO_Trader_Factory::TAO_TRADER *
TAO_Trader_Factory::manufacture_trader (void)
{
  typedef TAO_Trader<ACE_Null_Mutex, ACE_Null_Mutex> TRADER;

#if defined ACE_HAS_THREADS
  typedef TAO_Trader<TAO_SYNCH_MUTEX, TAO_SYNCH_RW_MUTEX>  MT_TRADER;
#else
  typedef TAO_Trader<ACE_Null_Mutex, ACE_Null_Mutex>  MT_TRADER;
#endif /* ACE_HAS_THREADS */

  TAO_TRADER* return_value = 0;
  int components = static_cast<int> (TAO_Trader_Base::LOOKUP);

  if (this->conformance_ >= TAO_TRADER_SIMPLE)
    components |= static_cast<int> (TAO_Trader_Base::REGISTER);

  if (this->conformance_ >= TAO_TRADER_STANDALONE)
    components |= static_cast<int> (TAO_Trader_Base::ADMIN);

  if (this->conformance_ >= TAO_TRADER_LINKED)
    components |= static_cast<int> (TAO_Trader_Base::LINK);

  if (this->threadsafe_)
    ACE_NEW_RETURN (return_value,
                    MT_TRADER (static_cast<TAO_Trader_Base::Trader_Components> (components)),
                    0);
  else
    ACE_NEW_RETURN (return_value,
                    TRADER (static_cast<TAO_Trader_Base::Trader_Components> (components)),
                    0);

  TAO_Import_Attributes_i &import_attributes =
    return_value->import_attributes ();
  TAO_Support_Attributes_i &support_attributes =
    return_value->support_attributes ();

  import_attributes.max_search_card (this->max_search_card_);
  import_attributes.def_search_card (this->def_search_card_);
  import_attributes.max_match_card (this->max_match_card_);
  import_attributes.def_match_card (this->def_match_card_);
  import_attributes.max_return_card (this->max_return_card_);
  import_attributes.def_return_card (this->def_return_card_);
  import_attributes.max_hop_count (this->max_hop_count_);
  import_attributes.def_hop_count (this->def_hop_count_);
  import_attributes.max_follow_policy (this->max_follow_policy_);
  import_attributes.def_follow_policy (this->def_follow_policy_);
  support_attributes.supports_modifiable_properties (this->supports_modifiable_properties_);
  support_attributes.supports_dynamic_properties (this->supports_dynamic_properties_);

  return return_value;
}

void
TAO_Trader_Factory::parse_args (int& argc, ACE_TCHAR** argv)
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR* current_arg = arg_shifter.get_current ();

      if (ACE_OS::strcmp (current_arg, ACE_TEXT("-TSthreadsafe")) == 0)
        {
          arg_shifter.consume_arg ();
          this->threadsafe_ = 1;
        }
      else if (ACE_OS::strcmp (current_arg, ACE_TEXT("-TSconformance")) == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* conformance_str = arg_shifter.get_current ();

              if (ACE_OS::strcasecmp (conformance_str, ACE_TEXT("Linked")) == 0)
                this->conformance_ = TAO_TRADER_LINKED;
              else if (ACE_OS::strcasecmp (conformance_str, ACE_TEXT("Query")) == 0)
                this->conformance_ = TAO_TRADER_QUERY;
              else if (ACE_OS::strcasecmp (conformance_str, ACE_TEXT("Simple")) == 0)
                this->conformance_ = TAO_TRADER_SIMPLE;
              else if (ACE_OS::strcasecmp (conformance_str, ACE_TEXT("Standalone")) == 0)
                this->conformance_ = TAO_TRADER_STANDALONE;

              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcmp (current_arg, ACE_TEXT("-TSsupports_dynamic_properties")) == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* arg_str = arg_shifter.get_current ();

              if (ACE_OS::strcasecmp (arg_str, ACE_TEXT("true")) == 0)
                this->supports_dynamic_properties_ = 1;
              else if (ACE_OS::strcasecmp (arg_str, ACE_TEXT("false")) == 0)
                this->supports_dynamic_properties_ = 0;

              arg_shifter.consume_arg ();
            }

        }
      else if (ACE_OS::strcmp (current_arg, ACE_TEXT("-TSsupports_modifiable_properties")) == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* arg_str = arg_shifter.get_current ();

              if (ACE_OS::strcasecmp (arg_str, ACE_TEXT("true")) == 0)
                this->supports_modifiable_properties_ = 1;
              else if (ACE_OS::strcasecmp (arg_str, ACE_TEXT("false")) == 0)
                this->supports_modifiable_properties_ = 0;

              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcmp (current_arg, ACE_TEXT("-TSdef_search_card")) == 0 ||
               ACE_OS::strcmp (current_arg, ACE_TEXT("-TSmax_search_card")) == 0 ||
               ACE_OS::strcmp (current_arg, ACE_TEXT("-TSdef_match_card")) == 0 ||
               ACE_OS::strcmp (current_arg, ACE_TEXT("-TSmax_match_card")) == 0 ||
               ACE_OS::strcmp (current_arg, ACE_TEXT("-TSdef_return_card")) == 0 ||
               ACE_OS::strcmp (current_arg, ACE_TEXT("-TSmax_return_card")) == 0 ||
               ACE_OS::strcmp (current_arg, ACE_TEXT("-TSdef_hop_count")) == 0 ||
               ACE_OS::strcmp (current_arg, ACE_TEXT("-TSmax_hop_count")) == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              CORBA::ULong value =
                static_cast<CORBA::ULong> (ACE_OS::atoi (arg_shifter.get_current ()));
              arg_shifter.consume_arg ();

              if (ACE_OS::strstr (current_arg, ACE_TEXT("card")))
                {
                  if (ACE_OS::strstr (current_arg, ACE_TEXT("max")))
                    {
                      if (ACE_OS::strstr (current_arg, ACE_TEXT("search")))
                        this->max_search_card_ = value;
                      else if (ACE_OS::strstr (current_arg, ACE_TEXT("match")))
                        this->max_match_card_ = value;
                      else
                        this->max_return_card_ = value;
                    }
                  else
                    {
                      if (ACE_OS::strstr (current_arg, ACE_TEXT("search")))
                        this->def_search_card_ = value;
                      else if (ACE_OS::strstr (current_arg, ACE_TEXT("match")))
                        this->def_match_card_ = value;
                      else
                        this->def_return_card_ = value;
                    }
                }
              else
                {
                  if (ACE_OS::strstr (current_arg, ACE_TEXT("max")))
                    this->max_hop_count_ = value;
                  else
                    this->def_hop_count_ = value;
                }
            }
        }
      else if (ACE_OS::strcmp (current_arg, ACE_TEXT("-TSdef_follow_policy")) == 0
               || ACE_OS::strcmp (current_arg, ACE_TEXT("-TSmax_follow_policy")) == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* arg_str = arg_shifter.get_current ();
              CosTrading::FollowOption follow_option;

              if (ACE_OS::strcasecmp (arg_str, ACE_TEXT("always")) == 0)
                follow_option = CosTrading::always;
              else if (ACE_OS::strcasecmp (arg_str, ACE_TEXT("if_no_local")) == 0)
                follow_option = CosTrading::if_no_local;
              else if (ACE_OS::strcasecmp (arg_str, ACE_TEXT("local_only")) == 0)
                follow_option = CosTrading::local_only;
              else if (ACE_OS::strstr (current_arg, ACE_TEXT("def")))
                follow_option = this->def_follow_policy_;
              else
                follow_option = this->max_follow_policy_;

              if (ACE_OS::strstr (current_arg, ACE_TEXT("def")))
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

TAO_END_VERSIONED_NAMESPACE_DECL
