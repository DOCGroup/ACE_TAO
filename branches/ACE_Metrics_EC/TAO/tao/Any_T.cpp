// $Id$

#ifndef TAO_ANY_T_C
#define TAO_ANY_T_C

#include "tao/Any_T.h"
#include "tao/Marshal.h"
#include "tao/debug.h"
#include "ace/CORBA_macros.h"

#if !defined (__ACE_INLINE__)
# include "tao/Any_T.inl"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao,
           Any_T,
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

      CORBA::TCKind kind = any_tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      impl->assign_translator (kind,
                               &cdr);
      CORBA::Boolean result = replacement->demarshal_value (cdr);

      if (result == 1)
        {
          _tao_elem = ACE_const_cast (T *, replacement->value_);
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

template<typename T>
void
TAO::Any_Impl_T<T>::free_value (void)
{
  if (this->value_destructor_ != 0)
    {
      (*this->value_destructor_) (this->value_);
      this->value_destructor_ = 0;
    }

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

// =======================================================================

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

// =======================================================================

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
  CORBA::TypeCode_ptr bounded_tc = CORBA::TypeCode::_nil ();

  if (bound > 0)
    {
      CORBA::TCKind kind = ACE_static_cast (CORBA::TCKind,
                                            tc->kind_);
      static CORBA::Long _oc_buffer [] =
        {
          TAO_ENCAP_BYTE_ORDER,
          ACE_static_cast (CORBA::Long, bound)
        };

      ACE_NEW (bounded_tc,
               CORBA::TypeCode (kind,
                                sizeof _oc_buffer,
                                (char *) &_oc_buffer,
                                1,
                                0));
    }
  else
    {
      bounded_tc = CORBA::TypeCode::_duplicate (tc);
    }

  Any_Special_Impl_T<T, from_T, to_T> *new_impl = 0;
  ACE_NEW (new_impl,
           Any_Special_Impl_T (destructor,
                               bounded_tc,
                               value,
                               bound));
  any.replace (new_impl);
}

template<typename T, typename from_T, typename to_T>
CORBA::Boolean
TAO::Any_Special_Impl_T<T, from_T, to_T>::extract(const CORBA::Any & any,
                                                   _tao_destructor destructor,
                                                   CORBA::TypeCode_ptr tc,
                                                   const T *& _tao_elem,
                                                   CORBA::ULong bound
  )
{
  _tao_elem = 0;

  ACE_TRY_NEW_ENV
    {
      CORBA::TypeCode_ptr any_type = any._tao_get_typecode ();
      CORBA::TypeCode_var unaliased_any_type =
        any_type->unalias (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::TCKind any_kind =
        unaliased_any_type->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::TCKind try_kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (any_kind != try_kind)
        {
          return 0;
        }

      CORBA::ULong length =
        unaliased_any_type->length (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (length != bound)
        {
          return 0;
        }

      TAO::Any_Impl *impl = any.impl ();

      typedef TAO::Any_Special_Impl_T<T, from_T, to_T>
        BOUNDED_TSTRING_ANY_IMPL;

      TAO::Any_Special_Impl_T<T, from_T, to_T> *narrow_impl =
        dynamic_cast <BOUNDED_TSTRING_ANY_IMPL *> (impl);

      if (narrow_impl != 0)
        {
          _tao_elem = narrow_impl->value_;
          return 1;
        }

      ACE_Message_Block *mb = impl->_tao_get_cdr ();

      if (mb == 0)
        {
          return 0;
        }

      TAO::Any_Special_Impl_T<T, from_T, to_T> *replacement = 0;
      ACE_NEW_RETURN (replacement,
                      BOUNDED_TSTRING_ANY_IMPL (destructor,
                                                CORBA::TypeCode::_duplicate (tc),
                                                0,
                                                bound),
                      0);

      auto_ptr<TAO::Any_Special_Impl_T<T, from_T, to_T> > replacement_safety (
          replacement
        );

      TAO_InputCDR cdr (mb->data_block (),
                        ACE_Message_Block::DONT_DELETE,
                        mb->rd_ptr () - mb->base (),
                        mb->wr_ptr () - mb->base (),
                        impl->_tao_byte_order (),
                                                            TAO_DEF_GIOP_MAJOR,
                                                            TAO_DEF_GIOP_MINOR);

      CORBA::TCKind kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      impl->assign_translator (kind,
                               &cdr);
      CORBA::Boolean result = replacement->demarshal_value (cdr);

      if (result == 1)
        {
          _tao_elem = replacement->value_;
          ACE_const_cast (CORBA::Any &, any).replace (replacement);
          replacement_safety.release ();
          return 1;
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
TAO::Any_Special_Impl_T<T, from_T, to_T>::free_value (void)
{
  if (this->value_destructor_ != 0)
    {
      (*this->value_destructor_) (this->value_);
      this->value_destructor_ = 0;
    }

  CORBA::release (this->type_);
  this->type_ = CORBA::TypeCode::_nil ();
  this->value_ = 0;
}

template<typename T, typename from_T, typename to_T>
void
TAO::Any_Special_Impl_T<T, from_T, to_T>::_tao_decode (
    TAO_InputCDR &cdr
    ACE_ENV_ARG_DECL
  )
{
  if (! this->demarshal_value (cdr))
    {
      ACE_THROW (CORBA::MARSHAL ());
    }
}

// =======================================================================

template<typename T>
TAO::Any_Basic_Impl_T<T>::Any_Basic_Impl_T (CORBA::TypeCode_ptr tc,
                                            const T & val)
  : Any_Impl (0,
              tc),
    value_ (val)
{
}

template<typename T>
TAO::Any_Basic_Impl_T<T>::~Any_Basic_Impl_T (void)
{
}

template<typename T>
void
TAO::Any_Basic_Impl_T<T>::insert (CORBA::Any & any,
                                  CORBA::TypeCode_ptr tc,
                                  const T & value)
{
  Any_Basic_Impl_T<T> *new_impl = 0;
  ACE_NEW (new_impl,
           Any_Basic_Impl_T (tc,
                             value));
  any.replace (new_impl);
}

template<typename T>
CORBA::Boolean
TAO::Any_Basic_Impl_T<T>::extract (const CORBA::Any & any,
                                   CORBA::TypeCode_ptr tc,
                                   T & _tao_elem)
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

      TAO::Any_Impl *impl = any.impl ();

      ACE_Message_Block *mb = impl->_tao_get_cdr ();

      if (mb == 0)
        {
          TAO::Any_Basic_Impl_T<T> *narrow_impl =
            dynamic_cast <TAO::Any_Basic_Impl_T<T> *> (impl);

          if (narrow_impl == 0)
            {
              return 0;
            }

          _tao_elem = narrow_impl->value_;
          return 1;
        }

      TAO::Any_Basic_Impl_T<T> *replacement =
        TAO::Any_Basic_Impl_T<T>::create_empty (any_tc);

      auto_ptr<TAO::Any_Basic_Impl_T<T> > replacement_safety (replacement);

      TAO_InputCDR cdr (mb->data_block (),
                        ACE_Message_Block::DONT_DELETE,
                        mb->rd_ptr () - mb->base (),
                        mb->wr_ptr () - mb->base (),
                        impl->_tao_byte_order (),
                                                            TAO_DEF_GIOP_MAJOR,
                                                            TAO_DEF_GIOP_MINOR);

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

template<typename T>
TAO::Any_Basic_Impl_T<T> *
TAO::Any_Basic_Impl_T<T>::create_empty (CORBA::TypeCode_ptr tc)
{
  TAO::Any_Basic_Impl_T<T> * retval = 0;
  ACE_NEW_RETURN (retval,
                  TAO::Any_Basic_Impl_T<T> (tc,
                                            ACE_static_cast (T, 0)),
                  0);
  return retval;
}

template<typename T>
void
TAO::Any_Basic_Impl_T<T>::_tao_decode (TAO_InputCDR &cdr
                                       ACE_ENV_ARG_DECL)
{
  if (! this->demarshal_value (cdr))
    {
      ACE_THROW (CORBA::MARSHAL ());
    }
}

// =======================================================================

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

// =======================================================================

template<typename T>
TAO::Any_Dual_Impl_T<T>::Any_Dual_Impl_T (_tao_destructor destructor,
                                          CORBA::TypeCode_ptr tc,
                                          T * const val)
  : Any_Impl (destructor,
              tc),
    value_ (val)
{
}

template<typename T>
TAO::Any_Dual_Impl_T<T>::Any_Dual_Impl_T (_tao_destructor destructor,
                                          CORBA::TypeCode_ptr tc,
                                          const T & val)
  : Any_Impl (destructor,
              tc)
{
  ACE_NEW (this->value_,
           T (val));
}

template<typename T>
TAO::Any_Dual_Impl_T<T>::Any_Dual_Impl_T (CORBA::TypeCode_ptr tc)
  : Any_Impl (0,
              tc)
{
}

template<typename T>
TAO::Any_Dual_Impl_T<T>::~Any_Dual_Impl_T (void)
{
}

template<typename T>
void
TAO::Any_Dual_Impl_T<T>::insert (CORBA::Any & any,
                                 _tao_destructor destructor,
                                 CORBA::TypeCode_ptr tc,
                                 T * const value)
{
  Any_Dual_Impl_T<T> *new_impl = 0;
  ACE_NEW (new_impl,
           Any_Dual_Impl_T (destructor,
                            tc,
                            value));
  any.replace (new_impl);
}

template<typename T>
void
TAO::Any_Dual_Impl_T<T>::insert_copy (CORBA::Any & any,
                                      _tao_destructor destructor,
                                      CORBA::TypeCode_ptr tc,
                                      const T & value)
{
  Any_Dual_Impl_T<T> *new_impl = 0;
  ACE_NEW (new_impl,
           Any_Dual_Impl_T (destructor,
                            tc,
                            value));
  any.replace (new_impl);
}

template<typename T>
CORBA::Boolean
TAO::Any_Dual_Impl_T<T>::extract (const CORBA::Any & any,
                                  _tao_destructor destructor,
                                  CORBA::TypeCode_ptr tc,
                                  const T *& _tao_elem)
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
          TAO::Any_Dual_Impl_T<T> *narrow_impl =
            dynamic_cast <TAO::Any_Dual_Impl_T<T> *> (impl);

          if (narrow_impl == 0)
            {
              return 0;
            }

          _tao_elem = narrow_impl->value_;
          return 1;
        }

      T *empty_value = 0;
      ACE_NEW_RETURN (empty_value,
                      T,
                      0);
      TAO::Any_Dual_Impl_T<T> *replacement = 0;
      ACE_NEW_RETURN (replacement,
                      TAO::Any_Dual_Impl_T<T> (destructor,
                                               any_tc,
                                               empty_value),
                      0);

      auto_ptr<TAO::Any_Dual_Impl_T<T> > replacement_safety (replacement);

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

template<typename T>
void
TAO::Any_Dual_Impl_T<T>::free_value (void)
{
  if (this->value_destructor_ != 0)
    {
      (*this->value_destructor_) (this->value_);
      this->value_destructor_ = 0;
    }

  this->value_ = 0;
}

template<typename T>
void
TAO::Any_Dual_Impl_T<T>::_tao_decode (TAO_InputCDR &cdr
                                      ACE_ENV_ARG_DECL)
{
  if (! this->demarshal_value (cdr))
    {
      ACE_THROW (CORBA::MARSHAL ());
    }
}

#endif /* TAO_ANY_T_C */
