// $Id$

#include "orbsvcs/PortableGroup/PG_Property_Utils.h"
#include "orbsvcs/PortableGroup/PG_Operators.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Boolean
TAO_PG::get_property_value (const PortableGroup::Name & property_name,
                            const PortableGroup::Properties & properties,
                            PortableGroup::Value & property_value)
{
  const CORBA::ULong len = properties.length ();
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      const PortableGroup::Property & property = properties[i];
      if (property.nam == property_name)
        {
          property_value = property.val;
          return 1;
        }
    }

  return 0;
}

void
TAO_PG::override_properties (
  const PortableGroup::Properties & overrides,
  PortableGroup::Properties &properties)
{
  const CORBA::ULong num_overrides = overrides.length ();
  if (num_overrides == 0)
    return;

  const CORBA::ULong old_length = properties.length ();

  const CORBA::ULong new_length =
    (num_overrides > old_length ? num_overrides : old_length);

  // Increase the length wholesale as much as possible.  The idea is
  // to keep the cost of the incremental growth that may occur below
  // to a minimum.
  properties.length (new_length);

  // @@ Slow O(n^2) operation.  Note that it may be slower than O(n^2)
  //    if the length of the property sequence must be increased
  //    on-the-fly due to the allocations and copies incurred by such
  //    an operation.
  for (CORBA::ULong i = 0; i < num_overrides; ++i)
    {
      const PortableGroup::Property &override = overrides[i];

      CORBA::ULong j = 0;
      for ( ; j < old_length; ++j)
        if (properties[j].nam == override.nam)
          {
            properties[j].val = override.val;
            break;
          }

      // No property to override.  Append the override.
      if (j == old_length)
        {
          // @@ Slow incremental growth!  In order to set the length
          //    only once, i.e. a priori, instead of multiple times a
          //    searches in the override list and the property list
          //    must be performed to determine how many additional
          //    properties from the override list must be appended to
          //    the properties list.  Depending on the size of each
          //    list, such an operation may be just as slow as this
          //    operation.
          const CORBA::ULong current_length = properties.length ();
          properties.length (current_length + 1);
          properties[current_length] = override;
        }
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
