// $Id$

#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/AnyTypeCode/Marshal.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/Valuetype_Adapter.h"
#include "tao/ORB_Core.h"
#include "tao/SystemException.h"
#include "tao/CDR.h"

#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_string.h"


ACE_RCSID (tao,
           Any_Unknown_IDL_Type,
           "$Id$")


TAO::Unknown_IDL_Type::Unknown_IDL_Type (
    CORBA::TypeCode_ptr tc,
    TAO_InputCDR &cdr
  )
  : TAO::Any_Impl (0, tc, true),
    cdr_ (static_cast<ACE_Message_Block *> (0))
{
  ACE_TRY_NEW_ENV
    {
      this->_tao_decode (cdr ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::Exception, ex)
    {
    }
  ACE_ENDTRY;
}

TAO::Unknown_IDL_Type::Unknown_IDL_Type (
    CORBA::TypeCode_ptr tc
  )
  : TAO::Any_Impl (0, tc, true),
    cdr_ (static_cast<ACE_Message_Block *> (0))
{
}

TAO::Unknown_IDL_Type::~Unknown_IDL_Type (void)
{
}

CORBA::Boolean
TAO::Unknown_IDL_Type::marshal_value (TAO_OutputCDR &cdr)
{
  ACE_TRY_NEW_ENV
    {
      // We don't want the rd_ptr to move, in case we are shared by
      // another Any, so we use this to copy the state, not the buffer.
      TAO_InputCDR for_reading (this->cdr_);

      TAO::traverse_status status =
        TAO_Marshal_Object::perform_append (this->type_,
                                            &for_reading,
                                            &cdr
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (status != TAO::TRAVERSE_CONTINUE)
        {
          return 0;
        }
    }
  ACE_CATCH (CORBA::Exception, ex)
    {
      return 0;
    }
  ACE_ENDTRY;

  return 1;
}

const void *
TAO::Unknown_IDL_Type::value (void) const
{
  return this->cdr_.start ();
}

void
TAO::Unknown_IDL_Type::free_value (void)
{
  CORBA::release (this->type_);
}

TAO_InputCDR &
TAO::Unknown_IDL_Type::_tao_get_cdr (void)
{
  return this->cdr_;
}

int
TAO::Unknown_IDL_Type::_tao_byte_order (void) const
{
  return this->cdr_.byte_order ();
}
void
TAO::Unknown_IDL_Type::_tao_decode (TAO_InputCDR &cdr
                                    ACE_ENV_ARG_DECL)
{
  // @@ (JP) The following code depends on the fact that
  //         TAO_InputCDR does not contain chained message blocks,
  //         otherwise <begin> and <end> could be part of
  //         different buffers!

  // This will be the start of a new message block.
  char *begin = cdr.rd_ptr ();

  // Skip over the next argument.
  TAO::traverse_status status =
    TAO_Marshal_Object::perform_skip (this->type_,
                                      &cdr
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (status != TAO::TRAVERSE_CONTINUE)
    {
      ACE_THROW (CORBA::MARSHAL ());
    }

  // This will be the end of the new message block.
  char *end = cdr.rd_ptr ();

  // The ACE_CDR::mb_align() call can shift the rd_ptr by up to
  // ACE_CDR::MAX_ALIGNMENT - 1 bytes. Similarly, the offset adjustment
  // can move the rd_ptr by up to the same amount. We accommodate
  // this by including 2 * ACE_CDR::MAX_ALIGNMENT bytes of additional
  // space in the message block.
  size_t size = end - begin;

  ACE_Message_Block new_mb (size + 2 * ACE_CDR::MAX_ALIGNMENT);

  ACE_CDR::mb_align (&new_mb);
  ptrdiff_t offset = ptrdiff_t (begin) % ACE_CDR::MAX_ALIGNMENT;

  if (offset < 0)
    {
      offset += ACE_CDR::MAX_ALIGNMENT;
    }

  new_mb.rd_ptr (offset);
  new_mb.wr_ptr (offset + size);

  ACE_OS::memcpy (new_mb.rd_ptr (),
                  begin,
                  size);

  this->cdr_.reset (&new_mb, cdr.byte_order ());
  this->cdr_.char_translator (cdr.char_translator ());
  this->cdr_.wchar_translator (cdr.wchar_translator ());
}

CORBA::Boolean
TAO::Unknown_IDL_Type::to_object (CORBA::Object_ptr &obj) const
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ULong kind =
        this->type_->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::TypeCode_var tcvar =
        CORBA::TypeCode::_duplicate (this->type_);

      while (kind == CORBA::tk_alias)
        {
          tcvar = tcvar->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          kind = tcvar->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (kind != CORBA::tk_objref)
        {
          return 0;
        }

      return this->cdr_ >> obj;
    }
  ACE_CATCH (CORBA::Exception, ex)
    {
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
TAO::Unknown_IDL_Type::to_value (CORBA::ValueBase *&val) const
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ULong kind =
        this->type_->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::TypeCode_var tcvar =
        CORBA::TypeCode::_duplicate (this->type_);

      while (kind == CORBA::tk_alias)
        {
          tcvar = tcvar->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          kind = tcvar->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (kind != CORBA::tk_value)
        {
          return 0;
        }

      TAO_Valuetype_Adapter *adapter =
        ACE_Dynamic_Service<TAO_Valuetype_Adapter>::instance (
            TAO_ORB_Core::valuetype_adapter_name ()
          );

      if (adapter == 0)
        {
          ACE_THROW_RETURN (CORBA::INTERNAL (),
                            0);
        }

      return adapter->stream_to_value (this->cdr_, val);
    }
  ACE_CATCH (CORBA::Exception, ex)
    {
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
TAO::Unknown_IDL_Type::to_abstract_base (CORBA::AbstractBase_ptr &obj) const
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ULong kind =
        this->type_->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::TypeCode_var tcvar =
        CORBA::TypeCode::_duplicate (this->type_);

      while (kind == CORBA::tk_alias)
        {
          tcvar = tcvar->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          kind = tcvar->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (kind != CORBA::tk_value)
        {
          return 0;
        }

      TAO_Valuetype_Adapter *adapter =
        ACE_Dynamic_Service<TAO_Valuetype_Adapter>::instance (
            TAO_ORB_Core::valuetype_adapter_name ()
          );

      if (adapter == 0)
        {
          ACE_THROW_RETURN (CORBA::INTERNAL (),
                            0);
        }

      return adapter->stream_to_abstract_base (this->cdr_,
                                               obj);
    }
  ACE_CATCH (CORBA::Exception, ex)
    {
    }
  ACE_ENDTRY;

  return 0;
}
