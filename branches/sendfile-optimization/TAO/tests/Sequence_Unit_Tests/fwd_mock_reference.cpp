/**
 * @file
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#include "fwd_mock_reference.hpp"

fwd_mock_reference *
TAO::Objref_Traits<fwd_mock_reference>::duplicate (
    fwd_mock_reference_ptr
  )
{
  return 0;
}

void
TAO::Objref_Traits<fwd_mock_reference>::release (
    fwd_mock_reference_ptr
  )
{
}

fwd_mock_reference_ptr
TAO::Objref_Traits<fwd_mock_reference>::nil (void)
{
  return 0;
}

CORBA::Boolean
TAO::Objref_Traits<fwd_mock_reference>::marshal (
    fwd_mock_reference_ptr,
    TAO_OutputCDR &
  )
{
  return true;
}
