// -*- C++ -*-
#include "tao/AnyTypeCode/ExceptionA.h"
#include "tao/AnyTypeCode/Any_Dual_Impl_T.h"
#include "tao/SystemException.h"
#include "tao/ORB_Constants.h"
#include "tao/CORBA_String.h"
#include "tao/CDR.h"
#include "tao/debug.h"

#include "ace/Malloc.h"
#include "ace/SString.h"
#include "ace/OS_NS_string.h"

#include "ace/OS_NS_stdio.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Specializations for CORBA::Exception Any operators.
namespace TAO
{
  template<>
  void
  Any_Dual_Impl_T<CORBA::Exception>::value (CORBA::Exception const &val)
  {
    this->value_ = val._tao_duplicate ();
  }

  template<>
  CORBA::Boolean
  Any_Dual_Impl_T<CORBA::Exception>::marshal_value (TAO_OutputCDR &cdr)
  {
    try
      {
        this->value_->_tao_encode (cdr);

        return true;
      }
    catch (::CORBA::Exception const &)
      {
      }

    return false;
  }

  template<>
  CORBA::Boolean
  Any_Dual_Impl_T<CORBA::Exception>::demarshal_value (TAO_InputCDR &cdr)
  {
    try
      {
        this->value_->_tao_decode (cdr);

        return true;
      }
    catch (::CORBA::Exception const &)
      {
      }

    return false;
  }

  // This should never get called since we don't have extraction operators
  // for CORBA::Exception, but it is here to sidestep the constructor call
  // in the unspecialized version that causes a problem with compilers that
  // require explicit instantiation
  template<>
  CORBA::Boolean
  Any_Dual_Impl_T<CORBA::Exception>::extract (
      CORBA::Any const &,
      _tao_destructor,
      CORBA::TypeCode_ptr,
      CORBA::Exception const *&)
  {
    return false;
  }
}

namespace CORBA
{
/// Inserts a copy of a CORBA::Exception into an Any.
void
operator<<= (CORBA::Any &any, CORBA::Exception const &exception)
{
  TAO::Any_Dual_Impl_T<CORBA::Exception>::insert_copy (
      any,
      CORBA::Exception::_tao_any_destructor,
      exception._tao_type (),
      exception);
}

/// Insertion of CORBA::Exception - non-copying.
void
operator<<= (CORBA::Any &any, CORBA::Exception *exception)
{
  TAO::Any_Dual_Impl_T<CORBA::Exception>::insert (
      any,
      CORBA::Exception::_tao_any_destructor,
      exception->_tao_type (),
      exception);
}

}

TAO_END_VERSIONED_NAMESPACE_DECL
