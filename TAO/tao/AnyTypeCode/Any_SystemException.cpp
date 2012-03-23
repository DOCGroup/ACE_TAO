// -*- C++ -*-
// $Id$

#include "tao/AnyTypeCode/Any_SystemException.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/AnyTypeCode/Marshal.h"
#include "tao/AnyTypeCode/TypeCode.h"

#include "tao/CDR.h"
#include "tao/Exception.h"
#include "tao/CORBA_String.h"
#include "tao/SystemException.h"

#include "ace/Auto_Ptr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::Any_SystemException::Any_SystemException (_tao_destructor destructor,
                                               CORBA::TypeCode_ptr tc,
                                               CORBA::SystemException * const val)
  : Any_Impl (destructor,
              tc),
    value_ (val)
{
}

TAO::Any_SystemException::Any_SystemException (_tao_destructor destructor,
                                               CORBA::TypeCode_ptr tc,
                                               const CORBA::SystemException & val)
  : Any_Impl (destructor,
              tc)
{
 this->value_ =
   dynamic_cast <CORBA::SystemException *> (val._tao_duplicate ());
}

TAO::Any_SystemException::Any_SystemException (CORBA::TypeCode_ptr tc)
  : Any_Impl (0,
              tc),
    value_ (0)
{
}

TAO::Any_SystemException::~Any_SystemException (void)
{
}

void
TAO::Any_SystemException::insert (CORBA::Any & any,
                                  _tao_destructor destructor,
                                  CORBA::TypeCode_ptr tc,
                                  CORBA::SystemException * const value)
{
  Any_SystemException *new_impl = 0;
  ACE_NEW (new_impl,
           Any_SystemException (destructor,
                                tc,
                                value));
  any.replace (new_impl);
}

void
TAO::Any_SystemException::insert_copy (CORBA::Any & any,
                                       _tao_destructor destructor,
                                       CORBA::TypeCode_ptr tc,
                                       const CORBA::SystemException & value)
{
  Any_SystemException *new_impl = 0;
  ACE_NEW (new_impl,
           Any_SystemException (destructor,
                                tc,
                                value));
  any.replace (new_impl);
}

CORBA::Boolean
TAO::Any_SystemException::extract (const CORBA::Any & any,
                                   _tao_destructor destructor,
                                   CORBA::TypeCode_ptr tc,
                                   const CORBA::SystemException *& _tao_elem,
                                   TAO::excp_factory f)
{
  _tao_elem = 0;

  try
    {
      CORBA::TypeCode_ptr any_tc = any._tao_get_typecode ();
      CORBA::Boolean const _tao_equiv = any_tc->equivalent (tc);

      if (_tao_equiv == false)
        {
          return false;
        }

      TAO::Any_Impl *impl = any.impl ();

      if (!impl->encoded ())
        {
          TAO::Any_SystemException *narrow_impl =
            dynamic_cast <TAO::Any_SystemException *> (impl);

          if (narrow_impl == 0)
            {
              return false;
            }

          _tao_elem = narrow_impl->value_;
          return true;
        }

      CORBA::SystemException *empty_value =  (*f) ();

      TAO::Any_SystemException *replacement = 0;
      ACE_NEW_RETURN (replacement,
                      TAO::Any_SystemException (destructor,
                                                any_tc,
                                                empty_value),
                      false);

      auto_ptr<TAO::Any_SystemException > replacement_safety (replacement);

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
    }
  catch (const ::CORBA::Exception&)
    {
    }

  return false;
}

void
TAO::Any_SystemException::free_value (void)
{
  if (this->value_destructor_ != 0)
    {
      (*this->value_destructor_) (this->value_);
      this->value_destructor_ = 0;
    }

  ::CORBA::release (this->type_);
  this->value_ = 0;
}

const void *
TAO::Any_SystemException::value (void) const
{
  return this->value_;
}

CORBA::Boolean
TAO::Any_SystemException::marshal_value (TAO_OutputCDR &cdr)
{
  try
    {
      this->value_->_tao_encode (cdr);
      return true;
    }
  catch (const ::CORBA::Exception&)
    {
    }
  return false;
}

CORBA::Boolean
TAO::Any_SystemException::demarshal_value (TAO_InputCDR &cdr)
{
  try
    {
      this->value_->_tao_decode (cdr);
      return true;
    }
  catch (const ::CORBA::Exception&)
    {
    }
  return false;
}

TAO_END_VERSIONED_NAMESPACE_DECL
