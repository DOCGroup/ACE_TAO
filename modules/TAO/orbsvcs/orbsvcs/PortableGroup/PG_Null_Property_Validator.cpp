#include "orbsvcs/PortableGroup/PG_Null_Property_Validator.h"


ACE_RCSID (PortableGroup,
           PG_Null_Property_Validator,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_PG_Null_Property_Validator::validate_property (
    const PortableGroup::Properties &)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
}

void
TAO_PG_Null_Property_Validator::validate_criteria (
    const PortableGroup::Properties &)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidCriteria,
                   PortableGroup::CannotMeetCriteria))
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
