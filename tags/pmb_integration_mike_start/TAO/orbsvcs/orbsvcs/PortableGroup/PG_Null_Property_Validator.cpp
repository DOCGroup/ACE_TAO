#include "PG_Null_Property_Validator.h"


ACE_RCSID (PortableGroup,
           PG_Null_Property_Validator,
           "$Id$")


void
TAO_PG_Null_Property_Validator::validate_property (
    const PortableGroup::Properties &
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
}

void
TAO_PG_Null_Property_Validator::validate_criteria (
    const PortableGroup::Properties &
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidCriteria,
                   PortableGroup::CannotMeetCriteria))
{
}
