#include "PG_Default_Property_Validator.h"
#include "PG_Operators.h"


ACE_RCSID (PortableGroup,
           PG_Default_Property_Validator,
           "$Id$")


TAO_PG_Default_Property_Validator::TAO_PG_Default_Property_Validator (void)
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
    const PortableGroup::Properties & props
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
  const CORBA::ULong len = props.length ();

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      const PortableGroup::Property & property = props[i];

      if (property.nam == this->membership_)
        {
          PortableGroup::MembershipStyleValue membership;
          if (!(property.val >>= membership)
              || (membership != PortableGroup::MEMB_APP_CTRL
                  && membership != PortableGroup::MEMB_INF_CTRL))
            ACE_THROW (PortableGroup::InvalidProperty (property.nam,
                                                       property.val));
        }
      else if (property.nam == this->factories_)
        {
          PortableGroup::FactoriesValue * factories;
          if (!(property.val >>= factories))
            ACE_THROW (PortableGroup::InvalidProperty (property.nam,
                                                       property.val));
          else
            {
              const CORBA::ULong flen = factories->length ();

              if (flen == 0)
                ACE_THROW (PortableGroup::InvalidProperty (property.nam,
                                                           property.val));

              for (CORBA::ULong j = 0; j < flen; ++j)
                {
                  const PortableGroup::FactoryInfo & factory_info =
                    (*factories)[j];

                  if (CORBA::is_nil (factory_info.the_factory.in ())
                      || factory_info.the_location.length () == 0)
                    ACE_THROW (PortableGroup::InvalidProperty (property.nam,
                                                               property.val));
                }
            }
        }
    }
}

void
TAO_PG_Default_Property_Validator::validate_criteria (
    const PortableGroup::Properties & props
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidCriteria,
                   PortableGroup::CannotMeetCriteria))
{
  const CORBA::ULong len = props.length ();
  PortableGroup::Criteria invalid_criteria;

  // Optimize for the worst case scenario where all properties are
  // invalid.
  invalid_criteria.length (len);

  /// The invalid criteria index.
  CORBA::ULong p = 0;

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      const PortableGroup::Property & property = props[i];

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
          PortableGroup::FactoriesValue * factories;
          if (!(property.val >>= factories))
            invalid_criteria[p++] = property;
          else
            {
              const CORBA::ULong flen = factories->length ();

              if (flen == 0)
                invalid_criteria[p++] = property;
              else
                {
                  for (CORBA::ULong j = 0; j < flen; ++j)
                    {
                      const PortableGroup::FactoryInfo & factory_info =
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
    ACE_THROW (PortableGroup::InvalidCriteria (invalid_criteria));
}
