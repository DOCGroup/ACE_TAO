// $Id$

#include "tao/ParameterMode.h"
#include "tao/CDR.h"

ACE_RCSID (tao,
           ParameterMode,
           "$Id$")

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
