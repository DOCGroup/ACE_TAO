// $Id$

#include "tao/corbafwd.h"
#include "tao/CDR.h"

#if !defined (__ACE_INLINE__)
# include "tao/corbafwd.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao,
           corbafwd,
           "$Id$")

namespace CORBA
{
  const CORBA::Visibility PRIVATE_MEMBER = 0;
  const CORBA::Visibility PUBLIC_MEMBER  = 1;

  const CORBA::ValueModifier VM_NONE        = 0;
  const CORBA::ValueModifier VM_CUSTOM      = 1;
  const CORBA::ValueModifier VM_ABSTRACT    = 2;
  const CORBA::ValueModifier VM_TRUNCATABLE = 3;
}

CORBA::Boolean
operator<< (TAO_OutputCDR &strm, const CORBA::ParameterMode &_tao_enumval)
{
  CORBA::ULong _tao_temp = _tao_enumval;
  return strm << _tao_temp;
}

CORBA::Boolean
operator>> (TAO_InputCDR &strm, CORBA::ParameterMode &_tao_enumval)
{
  CORBA::ULong _tao_temp = 0;
  CORBA::Boolean _tao_result = strm >> _tao_temp;

  if (_tao_result == 1)
    {
      _tao_enumval = ACE_static_cast (CORBA::ParameterMode, _tao_temp);
    }

  return _tao_result;
}
