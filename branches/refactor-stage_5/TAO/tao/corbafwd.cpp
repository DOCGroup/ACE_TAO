// $Id$

#include "tao/corbafwd.h"

#if !defined (__ACE_INLINE__)
# include "tao/corbafwd.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao, 
           corbafwd, 
           "$Id$")

namespace CORBA
{
  // ****************************************************************

//  const CORBA::ServiceType Security = 1;

  // ****************************************************************

  CORBA::TypeCode_ptr _tc_Visibility = 0;

  const CORBA::Visibility PRIVATE_MEMBER = 0;
  const CORBA::Visibility PUBLIC_MEMBER  = 1;

  CORBA::TypeCode_ptr _tc_ValueModifier = 0;

  const CORBA::ValueModifier VM_NONE        = 0;
  const CORBA::ValueModifier VM_CUSTOM      = 1;
  const CORBA::ValueModifier VM_ABSTRACT    = 2;
  const CORBA::ValueModifier VM_TRUNCATABLE = 3;
}
