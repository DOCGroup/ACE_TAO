#include "tao/ValueFactory.h"

#if !defined (__ACE_INLINE__)
# include "tao/ValueFactory.i"
#endif /* ! __ACE_INLINE__ */


ACE_RCSID (tao,
           ValueFactory,
           "$Id$")


CORBA::ValueFactoryBase::~ValueFactoryBase (void)
{
}

/*static*/ CORBA::ValueFactoryBase *
CORBA::ValueFactoryBase::_nil (void)
{
  return 0;
}

// No-op. This should never be called, but it can't be pure virtual.
CORBA::AbstractBase *
CORBA::ValueFactoryBase::create_for_unmarshal_abstract (void)
{
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class TAO_Pseudo_Value_Var_T<CORBA::ValueFactoryBase>;
  template class TAO_Pseudo_Value_Out_T<CORBA::ValueFactoryBase, CORBA::ValueFactoryBase_var>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate TAO_Pseudo_Value_Var_T<CORBA::ValueFactoryBase>
# pragma instantiate TAO_Pseudo_Value_Out_T<CORBA::ValueFactoryBase, CORBA::ValueFactoryBase_var>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
