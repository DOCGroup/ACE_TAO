#include "FT_Property_Validator.h"
#include "orbsvcs/PortableGroup/PG_Operators.h"
#include "orbsvcs/FT_ReplicationManagerC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::FT_Property_Validator::FT_Property_Validator ()
  : replication_style_ (1),
    membership_style_ (1),
    consistency_style_ (1),
    fault_monitoring_style_ (1),
    fault_monitoring_granularity_ (1),
    factories_ (1)
{
  this->replication_style_.length (1);
  this->replication_style_[0].id = CORBA::string_dup (::FT::FT_REPLICATION_STYLE);

  this->membership_style_.length (1);
  this->membership_style_[0].id = CORBA::string_dup (::FT::FT_MEMBERSHIP_STYLE);

  this->consistency_style_.length (1);
  this->consistency_style_[0].id = CORBA::string_dup (::FT::FT_CONSISTENCY_STYLE);

  this->fault_monitoring_style_.length (1);
  this->fault_monitoring_style_[0].id = CORBA::string_dup (::FT::FT_FAULT_MONITORING_STYLE);

  this->fault_monitoring_granularity_.length (1);
  this->fault_monitoring_granularity_[0].id = CORBA::string_dup (::FT::FT_FAULT_MONITORING_GRANULARITY);

  this->factories_.length (1);
  this->factories_[0].id = CORBA::string_dup (::FT::FT_FACTORIES);
}


TAO::FT_Property_Validator::~FT_Property_Validator () = default;


void
TAO::FT_Property_Validator::validate_property (
    PortableGroup::Properties const & props)
{
  CORBA::ULong const len = props.length ();

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      PortableGroup::Property const & property = props[i];

      if (property.nam == this->replication_style_)
        {
          FT::ReplicationStyleValue value;
          if (!(property.val >>= value)
              || (value != FT::STATELESS
                  && value != FT::COLD_PASSIVE
                  && value != FT::WARM_PASSIVE
                  && value != FT::ACTIVE
                  && value != FT::ACTIVE_WITH_VOTING
                  && value != FT::SEMI_ACTIVE))
            throw PortableGroup::InvalidProperty (property.nam, property.val);
        }
      else if (property.nam == this->membership_style_)
        {
          PortableGroup::MembershipStyleValue value;
          if (!(property.val >>= value)
              || (value != PortableGroup::MEMB_APP_CTRL
                  && value != PortableGroup::MEMB_INF_CTRL))
            throw PortableGroup::InvalidProperty (property.nam, property.val);
        }
      else if (property.nam == this->consistency_style_)
        {
          FT::ConsistencyStyleValue value;
          if (!(property.val >>= value)
              || (value != FT::CONS_APP_CTRL
                  && value != FT::CONS_INF_CTRL))
            throw PortableGroup::InvalidProperty (property.nam, property.val);
        }
      else if (property.nam == this->fault_monitoring_style_)
        {
          FT::FaultMonitoringStyleValue value;
          if (!(property.val >>= value)
              || (value != FT::PULL
                  && value != FT::PUSH
                  && value != FT::NOT_MONITORED))
            throw PortableGroup::InvalidProperty (property.nam, property.val);
        }
      else if (property.nam == this->fault_monitoring_granularity_)
        {
          FT::FaultMonitoringGranularityValue value;
          if (!(property.val >>= value)
              || (value != FT::MEMB
                  && value != FT::LOC
                  && value != FT::LOC_AND_TYPE))
            throw PortableGroup::InvalidProperty (property.nam, property.val);
        }
      else if (property.nam == this->factories_)
        {
          PortableGroup::FactoriesValue const * factories = nullptr;
          if (!(property.val >>= factories))
            throw PortableGroup::InvalidProperty (property.nam, property.val);
          else
            {
              CORBA::ULong const flen = factories->length ();

              if (flen == 0)
                throw PortableGroup::InvalidProperty (
                  property.nam,
                  property.val);

              for (CORBA::ULong j = 0; j < flen; ++j)
                {
                  PortableGroup::FactoryInfo const & factory_info =
                    (*factories)[j];

                  if (CORBA::is_nil (factory_info.the_factory.in ())
                      || factory_info.the_location.length () == 0)
                    throw PortableGroup::InvalidProperty (
                      property.nam,
                      property.val);
                }
            }
        }
    }
}

void
TAO::FT_Property_Validator::validate_criteria (
    PortableGroup::Properties const & props)
{
  CORBA::ULong const len = props.length ();
  PortableGroup::Criteria invalid_criteria;

  // Optimize for the worst case scenario where all properties are
  // invalid.
  invalid_criteria.length (len);

  /// The invalid criteria index.
  CORBA::ULong p {};

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      PortableGroup::Property const & property = props[i];
      CORBA::Long value {};

      if (!(property.val >>= value))
        invalid_criteria[p++] = property;
      else
      {
       if (property.nam == this->replication_style_)
        {
          if (  value != FT::STATELESS
             && value != FT::COLD_PASSIVE
             && value != FT::WARM_PASSIVE
             && value != FT::ACTIVE
             && value != FT::ACTIVE_WITH_VOTING
             && value != FT::SEMI_ACTIVE  )
            invalid_criteria[p++] = property;
        }
        else if (property.nam == this->membership_style_)
        {
          if (  value != PortableGroup::MEMB_APP_CTRL
             && value != PortableGroup::MEMB_INF_CTRL )
            invalid_criteria[p++] = property;
        }
        else if (property.nam == this->consistency_style_)
        {
          if (  value != FT::CONS_APP_CTRL
             && value != FT::CONS_INF_CTRL  )
            invalid_criteria[p++] = property;
        }
        else if (property.nam == this->fault_monitoring_style_)
        {
          if (  value != FT::PULL
             && value != FT::PUSH
             && value != FT::NOT_MONITORED  )
            invalid_criteria[p++] = property;
        }
        else if (property.nam == this->fault_monitoring_granularity_)
        {
          if (  value != FT::MEMB
             && value != FT::LOC
             && value != FT::LOC_AND_TYPE  )
            invalid_criteria[p++] = property;
        }
        else if (property.nam == this->factories_)
        {
          PortableGroup::FactoriesValue const * factories = nullptr;
          if (!(property.val >>= factories))
            invalid_criteria[p++] = property;
          else
            {
              CORBA::ULong const flen = factories->length ();

              if (flen == 0)
                invalid_criteria[p++] = property;
              else
                {
                  for (CORBA::ULong j = 0; j < flen; ++j)
                    {
                      PortableGroup::FactoryInfo const & factory_info =
                        (*factories)[j];

                      if (CORBA::is_nil (factory_info.the_factory.in ())
                          || factory_info.the_location.length () == 0)
                        {
                          invalid_criteria[p++] = property;
                          break;
                        }
                    }
                }
            }
        }
      }
    }

  if (p > 0)
    {
      // Reduce the length of the invalid criteria sequence in an
      // effort to optimize the copying that will occur when the below
      // exception is thrown.  Reducing the length is fast since no
      // deallocations should occur.
      invalid_criteria.length (p);

      throw PortableGroup::InvalidCriteria (invalid_criteria);
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
