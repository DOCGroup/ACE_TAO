// $Id$

#ifndef TAO_ANY_IMPL_T_C
#define TAO_ANY_IMPL_T_C

#include "tao/Any_Impl_T.h"
#include "tao/Any.h"
#include "tao/Marshal.h"
#include "tao/CDR.h"
#include "tao/Environment.h"
#include "tao/Exception.h"

#include "ace/CORBA_macros.h"
#include "ace/Auto_Ptr.h"
#include "ace/OS_Memory.h"

#if !defined (__ACE_INLINE__)
# include "tao/Any_Impl_T.inl"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao,
           Any_Impl_T,
           "$Id$")

template<typename T>
TAO::Any_Impl_T<T>::Any_Impl_T (_tao_destructor destructor,
                                CORBA::TypeCode_ptr tc,
                                T * const val)
  : Any_Impl (destructor,
              tc),
    value_ (val)
{
}

template<typename T>
TAO::Any_Impl_T<T>::~Any_Impl_T (void)
{
}

template<typename T>
void
TAO::Any_Impl_T<T>::insert (CORBA::Any & any,
                            _tao_destructor destructor,
                            CORBA::TypeCode_ptr tc,
                            T * const value)
{
  TAO::Any_Impl_T<T> *new_impl = 0;
  ACE_NEW (new_impl,
           TAO::Any_Impl_T<T> (destructor,
                               tc,
                               value));
  any.replace (new_impl);
}

template<typename T>
CORBA::Boolean
TAO::Any_Impl_T<T>::extract (const CORBA::Any & any,
                             _tao_destructor destructor,
                             CORBA::TypeCode_ptr tc,
                             T *& _tao_elem)
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
          TAO::Any_Impl_T<T> *narrow_impl =
            dynamic_cast <TAO::Any_Impl_T<T> *> (impl);

          if (narrow_impl == 0)
            {
              return 0;
            }

          _tao_elem = (T *) narrow_impl->value_;
          return 1;
        }

      TAO::Any_Impl_T<T> *replacement = 0;
      ACE_NEW_RETURN (replacement,
                      TAO::Any_Impl_T<T> (destructor,
                                          any_tc,
                                          0),
                      0);

      auto_ptr<TAO::Any_Impl_T<T> > replacement_safety (replacement);

      TAO_InputCDR cdr (mb->data_block (),
                        ACE_Message_Block::DONT_DELETE,
                        mb->rd_ptr () - mb->base (),
                        mb->wr_ptr () - mb->base (),
                        impl->_tao_byte_order (),
                        TAO_DEF_GIOP_MAJOR,
                        TAO_DEF_GIOP_MINOR);

      impl->assign_translator (any_tc,
                               &cdr
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Boolean result = replacement->demarshal_value (cdr);

      if (result == 1)
        {
          _tao_elem = ACE_const_cast (T *, replacement->value_);
          ACE_const_cast (CORBA::Any &, any).replace (replacement);
          replacement_safety.release ();
          return 1;
        }

      // Duplicated by Any_Impl base class constructor.
      CORBA::release (any_tc);
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

  return 0;
}

template<typename T>
void
TAO::Any_Impl_T<T>::free_value (void)
{
  if (this->value_destructor_ != 0)
    {
      (*this->value_destructor_) (this->value_);
      this->value_destructor_ = 0;
    }

  CORBA::release (this->type_);
  this->value_ = 0;
}

template<typename T>
void
TAO::Any_Impl_T<T>::_tao_decode (TAO_InputCDR &cdr
                                 ACE_ENV_ARG_DECL)
{
  if (! this->demarshal_value (cdr))
    {
      ACE_THROW (CORBA::MARSHAL ());
    }
}

#endif /* TAO_ANY_IMPL_T_C */
