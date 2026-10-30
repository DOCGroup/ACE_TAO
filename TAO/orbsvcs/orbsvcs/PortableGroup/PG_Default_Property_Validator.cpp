#include "orbsvcs/PortableGroup/PG_Default_Property_Validator.h"
#include "orbsvcs/PortableGroup/PG_Operators.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_PG_Default_Property_Validator::TAO_PG_Default_Property_Validator ()
  : membership_ (1),
    factories_ (1)
{
  this->membership_.length (1);
  this->membership_[0].id = CORBA::string_dup ("org.omg.PortableGroup.MembershipStyle");

  this->factories_.length (1);
  this->factories_[0].id = CORBA::string_dup ("org.omg.PortableGroup.Factories");
}

void
TAO_PG_Default_Property_Validator::validate_property (
    PortableGroup::Properties const & props)
{
  CORBA::ULong const len = props.length ();

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      PortableGroup::Property const & property = props[i];

      if (property.nam == this->membership_)
        {
          PortableGroup::MembershipStyleValue membership;
          if (!(property.val >>= membership)
              || (membership != PortableGroup::MEMB_APP_CTRL
                  && membership != PortableGroup::MEMB_INF_CTRL))
            throw PortableGroup::InvalidProperty (property.nam, property.val);
        }
      else if (property.nam == this->factories_)
        {
          PortableGroup::FactoriesValue const * factories;
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
TAO_PG_Default_Property_Validator::validate_criteria (
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

      if (property.nam == this->membership_)
        {
          PortableGroup::MembershipStyleValue membership;
          if (!(property.val >>= membership)
              || (membership != PortableGroup::MEMB_APP_CTRL
                  && membership != PortableGroup::MEMB_INF_CTRL))
            invalid_criteria[p++] = property;
        }
      else if (property.nam == this->factories_)
        {
          PortableGroup::FactoriesValue const* factories = nullptr;
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
