// $Id$

#ifndef TAO_ANY_ARRAY_IMPL_T_C
#define TAO_ANY_ARRAY_IMPL_T_C

#include "tao/Any_Array_Impl_T.h"
#include "tao/Marshal.h"
#include "tao/debug.h"
#include "ace/CORBA_macros.h"

#if !defined (__ACE_INLINE__)
# include "tao/Any_Array_Impl_T.inl"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao,
           Any_Array_Impl_T,
           "$Id$")

template<typename T_slice, typename T_forany>
TAO::Any_Array_Impl_T<T_slice, T_forany>::Any_Array_Impl_T (
    _tao_destructor destructor,
    CORBA::TypeCode_ptr tc,
    T_slice * const val
  )
  : Any_Impl (destructor,
              tc),
    value_ (val)
{
}

template<typename T_slice, typename T_forany>
TAO::Any_Array_Impl_T<T_slice, T_forany>::~Any_Array_Impl_T (void)
{
}

template<typename T_slice, typename T_forany> 
void
TAO::Any_Array_Impl_T<T_slice, T_forany>::insert (CORBA::Any & any,
                                                  _tao_destructor destructor,
                                                  CORBA::TypeCode_ptr tc,
                                                  T_slice * const value)
{
  TAO::Any_Array_Impl_T<T_slice, T_forany> *new_impl = 0;
  typedef TAO::Any_Array_Impl_T<T_slice, T_forany> ARRAY_ANY_IMPL;
  ACE_NEW (new_impl,
           ARRAY_ANY_IMPL (destructor,
                           tc,
                           value));
  any.replace (new_impl);
}

template<typename T_slice, typename T_forany> 
CORBA::Boolean
TAO::Any_Array_Impl_T<T_slice, T_forany>::extract (const CORBA::Any & any,
                                                   _tao_destructor destructor,
                                                   CORBA::TypeCode_ptr tc,
                                                   const T_slice *& _tao_elem)
{
  _tao_elem = 0;

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

      TAO::Any_Impl *impl = any.impl ();

      ACE_Message_Block *mb = impl->_tao_get_cdr ();

      if (mb == 0)
        {
          TAO::Any_Array_Impl_T<T_slice, T_forany> *narrow_impl =
            dynamic_cast <TAO::Any_Array_Impl_T<T_slice, T_forany> *> (impl);

          if (narrow_impl == 0)
            {
              return 0;
            }

          _tao_elem = reinterpret_cast<const T_slice*>(narrow_impl->value_) ;
          return 1;
        }

      TAO::Any_Array_Impl_T<T_slice, T_forany> *replacement = 0;
      typedef TAO::Any_Array_Impl_T<T_slice, T_forany> ARRAY_ANY_IMPL;
      ACE_NEW_RETURN (replacement,
                      ARRAY_ANY_IMPL (destructor,
                                      any_tc,
                                      T_forany::tao_alloc ()),
                      0);
                      
      auto_ptr<TAO::Any_Array_Impl_T<T_slice, T_forany> > replacement_safety (
          replacement
        );

      TAO_InputCDR cdr (mb->data_block (),
                        ACE_Message_Block::DONT_DELETE,
                        mb->rd_ptr () - mb->base (),
                        mb->wr_ptr () - mb->base (),
                        impl->_tao_byte_order (),
						            TAO_DEF_GIOP_MAJOR,
						            TAO_DEF_GIOP_MINOR);

      CORBA::TCKind kind = any_tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      impl->assign_translator (kind,
                               &cdr);
      CORBA::Boolean result = replacement->demarshal_value (cdr);

      if (result == 1)
        {
          _tao_elem = reinterpret_cast<const T_slice*>(replacement->value_) ;
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

template<typename T_slice, typename T_forany> 
void
TAO::Any_Array_Impl_T<T_slice, T_forany>::free_value (void)
{
  if (this->value_destructor_ != 0)
    {
      (*this->value_destructor_) (this->value_);
      this->value_destructor_ = 0;
    }

  this->value_ = 0;
}

template<typename T_slice, typename T_forany> 
void 
TAO::Any_Array_Impl_T<T_slice, T_forany>::_tao_decode (TAO_InputCDR &cdr
                                                       ACE_ENV_ARG_DECL)
{
  if (! this->demarshal_value (cdr))
    {
      ACE_THROW (CORBA::MARSHAL ());
    }
}

#endif /* TAO_ANY_ARRAY_IMPL_T_C */

