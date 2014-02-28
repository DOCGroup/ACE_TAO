// $Id$

#ifndef TAO_ANY_SPECIAL_IMPL_T_CPP
#define TAO_ANY_SPECIAL_IMPL_T_CPP

#include "tao/AnyTypeCode/Any_Special_Impl_T.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/AnyTypeCode/Marshal.h"
#include "tao/SystemException.h"
#include "tao/AnyTypeCode/String_TypeCode_Traits.h"

#if !defined (__ACE_INLINE__)
# include "tao/AnyTypeCode/Any_Special_Impl_T.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename T, typename from_T, typename to_T>
TAO::Any_Special_Impl_T<T, from_T, to_T>::Any_Special_Impl_T (
    _tao_destructor destructor,
    CORBA::TypeCode_ptr tc,
    T * const val,
    CORBA::ULong bound
  )
  : Any_Impl (destructor,
              tc),
    value_ (val),
    bound_ (bound)
{
}

template<typename T, typename from_T, typename to_T>
TAO::Any_Special_Impl_T<T, from_T, to_T>::~Any_Special_Impl_T (void)
{
}

template<typename T, typename from_T, typename to_T>
void
TAO::Any_Special_Impl_T<T, from_T, to_T>::insert (CORBA::Any & any,
                                                  _tao_destructor destructor,
                                                  CORBA::TypeCode_ptr tc,
                                                  T * const value,
                                                  CORBA::ULong bound
  )
{
  CORBA::TypeCode_var bounded_tc;

  if (bound > 0)
    {
      CORBA::TCKind const kind = tc->kind ();

      bounded_tc =
        TAO::TypeCodeFactory::String_Traits<from_T>::create_typecode (kind,
                                                                      bound);
    }
  else
    {
      bounded_tc = CORBA::TypeCode::_duplicate (tc);
    }

  if (CORBA::is_nil (bounded_tc.in ()))
    return;

  Any_Special_Impl_T<T, from_T, to_T> * new_impl;
  ACE_NEW (new_impl,
           Any_Special_Impl_T (destructor,
                               bounded_tc.in (),
                               value,
                               bound));

  any.replace (new_impl);
}

template<typename T, typename from_T, typename to_T>
CORBA::Boolean
TAO::Any_Special_Impl_T<T, from_T, to_T>::extract (const CORBA::Any & any,
                                                   _tao_destructor destructor,
                                                   CORBA::TypeCode_ptr tc,
                                                   const T *& _tao_elem,
                                                   CORBA::ULong bound)
{
  _tao_elem = 0;

  try
    {
      CORBA::TypeCode_ptr any_type = any._tao_get_typecode ();
      CORBA::TypeCode_var unaliased_any_type =
        TAO::unaliased_typecode (any_type);

      CORBA::TCKind const any_kind =
        unaliased_any_type->kind ();

      CORBA::TCKind const try_kind = tc->kind ();

      if (any_kind != try_kind)
        {
          return false;
        }

      CORBA::ULong const length = unaliased_any_type->length ();

      if (length != bound)
        {
          return false;
        }

      TAO::Any_Impl * const impl = any.impl ();

      typedef TAO::Any_Special_Impl_T<T, from_T, to_T>
        BOUNDED_TSTRING_ANY_IMPL;

      if (impl && !impl->encoded ())
        {
          TAO::Any_Special_Impl_T<T, from_T, to_T> * const narrow_impl =
            dynamic_cast <BOUNDED_TSTRING_ANY_IMPL *> (impl);

          if (narrow_impl == 0)
            {
              return false;
            }

          _tao_elem = (T *) narrow_impl->value_;
          return true;
        }

      TAO::Any_Special_Impl_T<T, from_T, to_T> *replacement = 0;
      ACE_NEW_RETURN (replacement,
                      BOUNDED_TSTRING_ANY_IMPL (destructor,
                                                tc,
                                                0,
                                                bound),
                      false);

      auto_ptr<TAO::Any_Special_Impl_T<T, from_T, to_T> > replacement_safety (
          replacement);

      // We know this will work since the unencoded case is covered above.
      TAO::Unknown_IDL_Type * const unk =
        dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

      if (!unk)
        return false;

      // We don't want the rd_ptr of unk to move, in case it is
      // shared by another Any. This copies the state, not the buffer.
      TAO_InputCDR for_reading (unk->_tao_get_cdr ());

      CORBA::Boolean const good_decode =
        replacement->demarshal_value (for_reading);

      if (good_decode)
        {
          _tao_elem = replacement->value_;
          const_cast<CORBA::Any &> (any).replace (replacement);
          replacement_safety.release ();
          return true;
        }

      // Duplicated by Any_Impl base class constructor.
      ::CORBA::release (tc);
    }
  catch (const ::CORBA::Exception&)
    {
    }

  return false;
}

template<typename T, typename from_T, typename to_T>
CORBA::Boolean
TAO::Any_Special_Impl_T<T, from_T, to_T>::marshal_value (TAO_OutputCDR &cdr)
{
  return (cdr << from_T (this->value_, this->bound_));
}

template<typename T, typename from_T, typename to_T>
const void *
TAO::Any_Special_Impl_T<T, from_T, to_T>::value (void) const
{
  return this->value_;
}

template<typename T, typename from_T, typename to_T>
void
TAO::Any_Special_Impl_T<T, from_T, to_T>::free_value (void)
{
  if (this->value_destructor_ != 0)
    {
      (*this->value_destructor_) (this->value_);
      this->value_destructor_ = 0;
    }

  ::CORBA::release (this->type_);
  this->value_ = 0;
}

template<typename T, typename from_T, typename to_T>
void
TAO::Any_Special_Impl_T<T, from_T, to_T>::_tao_decode (TAO_InputCDR &cdr)
{
  if (this->value_destructor_ != 0)
    {
      (*this->value_destructor_) (this->value_);
      this->value_ = 0;
    }

  if (! this->demarshal_value (cdr))
    {
      throw ::CORBA::MARSHAL ();
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_ANY_T_CPP */
