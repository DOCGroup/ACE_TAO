// $Id$

#ifndef TAO_ANY_SPECIAL_BASIC_IMPL_T_C
#define TAO_ANY_SPECIAL_BASIC_IMPL_T_C

#include "tao/Any_Special_Basic_Impl_T.h"
#include "tao/Marshal.h"
#include "tao/debug.h"
#include "ace/CORBA_macros.h"

#if !defined (__ACE_INLINE__)
# include "tao/Any_Special_Basic_Impl_T.inl"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao,
           Any_Special_Basic_Impl_T,
           "$Id$")

template<typename T, typename from_T, typename to_T>
TAO::Any_Special_Basic_Impl_T<T, from_T, to_T>::Any_Special_Basic_Impl_T (
    CORBA::TypeCode_ptr tc,
    const T & val
  )
  : Any_Impl (0,
              tc),
    value_ (val)
{
}

template<typename T, typename from_T, typename to_T>
TAO::Any_Special_Basic_Impl_T<T, from_T, to_T>::~Any_Special_Basic_Impl_T (
    void
  )
{
}

template<typename T, typename from_T, typename to_T> void
TAO::Any_Special_Basic_Impl_T<T, from_T, to_T>::insert (
    CORBA::Any & any,
    CORBA::TypeCode_ptr tc,
    const T & value
  )
{
  Any_Special_Basic_Impl_T<T, from_T, to_T> *new_impl = 0;
  ACE_NEW (new_impl,
           Any_Special_Basic_Impl_T (tc,
                                     value));
  any.replace (new_impl);
}

template<typename T, typename from_T, typename to_T> 
CORBA::Boolean
TAO::Any_Special_Basic_Impl_T<T, from_T, to_T>::extract (
    const CORBA::Any & any,
    CORBA::TypeCode_ptr tc,
    T & _tao_elem
  )
{
  ACE_TRY_NEW_ENV
    {
      CORBA::TypeCode_ptr any_tc = any._tao_get_typecode ();

      CORBA::Boolean _tao_equiv = any_tc->equivalent (tc
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (_tao_equiv == 0)
        {
          return 0;
        }

      typedef TAO::Any_Special_Basic_Impl_T<T, from_T, to_T> 
        UNSIGNED_CHAR_ANY_IMPL;

      TAO::Any_Impl *impl = any.impl ();

      ACE_Message_Block *mb = impl->_tao_get_cdr ();

      if (mb == 0)
        {
          TAO::Any_Special_Basic_Impl_T<T, from_T, to_T> *narrow_impl =
            dynamic_cast <UNSIGNED_CHAR_ANY_IMPL *> (impl);

          if (narrow_impl == 0)
            {
              return 0;
            }

          _tao_elem = narrow_impl->value_;
          return 1;
        }

      TAO::Any_Special_Basic_Impl_T<T, from_T, to_T> *replacement = 0;
      ACE_NEW_RETURN (replacement,
                      UNSIGNED_CHAR_ANY_IMPL (any_tc,
                                              0),
                      0);
                      
      auto_ptr<UNSIGNED_CHAR_ANY_IMPL> replacement_safety (replacement);

      TAO_InputCDR cdr (mb->data_block (),
                        ACE_Message_Block::DONT_DELETE,
                        mb->rd_ptr () - mb->base (),
                        mb->wr_ptr () - mb->base (),
                        impl->_tao_byte_order (),
						            TAO_DEF_GIOP_MAJOR,
						            TAO_DEF_GIOP_MINOR);

      CORBA::TCKind kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      impl->assign_translator (kind,
                               &cdr);
      CORBA::Boolean result = replacement->demarshal_value (cdr);

      if (result == 1)
        {
          _tao_elem = replacement->value_;
          ACE_const_cast (CORBA::Any &, any).replace (replacement);
          replacement_safety.release ();
          return result;
        }
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
  
  return 0;
}

template<typename T, typename from_T, typename to_T> 
void 
TAO::Any_Special_Basic_Impl_T<T, from_T, to_T>::_tao_decode (TAO_InputCDR &cdr
                                                             ACE_ENV_ARG_DECL)
{
  if (! this->demarshal_value (cdr))
    {
      ACE_THROW (CORBA::MARSHAL ());
    }
}

#endif /* TAO_ANY_T_C */

