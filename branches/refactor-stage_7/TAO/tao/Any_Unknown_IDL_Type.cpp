// $Id$

#include "tao/Any_Unknown_IDL_Type.h"
#include "tao/Valuetype_Adapter.h"
#include "tao/ORB_Core.h"

#include "ace/Dynamic_Service.h"

ACE_RCSID (tao,
           Any_Unknown_IDL_Type,
           "$Id$")

TAO::Unknown_IDL_Type::Unknown_IDL_Type (
    CORBA::TypeCode_ptr tc,
    const ACE_Message_Block *mb,
    int byte_order,
    ACE_Char_Codeset_Translator *ctrans,
    ACE_WChar_Codeset_Translator *wtrans
  )
  : TAO::Any_Impl (0, tc),
    cdr_ (ACE_Message_Block::duplicate (mb)),
    byte_order_ (byte_order),
    char_translator_ (ctrans),
    wchar_translator_ (wtrans)
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
      TAO_InputCDR input (this->cdr_,
                          this->byte_order_);

      CORBA::TypeCode::traverse_status status =
        TAO_Marshal_Object::perform_append (this->type_,
                                            &input,
                                            &cdr
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (status != CORBA::TypeCode::TRAVERSE_CONTINUE)
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
  return this->cdr_;
}

void
TAO::Unknown_IDL_Type::free_value (void)
{
  CORBA::release (this->type_);
  ACE_Message_Block::release (this->cdr_);
}

ACE_Message_Block *
TAO::Unknown_IDL_Type::_tao_get_cdr (void) const
{
  return this->cdr_;
}

int
TAO::Unknown_IDL_Type::_tao_byte_order (void) const
{
  return this->byte_order_;
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
  CORBA::TypeCode::traverse_status status =
    TAO_Marshal_Object::perform_skip (this->type_,
                                      &cdr
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (status != CORBA::TypeCode::TRAVERSE_CONTINUE)
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

  ACE_Message_Block::release (this->cdr_);
  ACE_NEW (this->cdr_,
           ACE_Message_Block (size + 2 * ACE_CDR::MAX_ALIGNMENT));

  ACE_CDR::mb_align (this->cdr_);
  ptrdiff_t offset = ptrdiff_t (begin) % ACE_CDR::MAX_ALIGNMENT;

  if (offset < 0)
    offset += ACE_CDR::MAX_ALIGNMENT;

  this->cdr_->rd_ptr (offset);
  this->cdr_->wr_ptr (offset + size);

  ACE_OS::memcpy (this->cdr_->rd_ptr (),
                  begin,
                  size);

  // Get character translators.
  this->char_translator_ = cdr.char_translator();
  this->wchar_translator_ = cdr.wchar_translator();
}

void
TAO::Unknown_IDL_Type::assign_translator (CORBA::TypeCode_ptr tc,
                                          TAO_InputCDR *cdr
                                          ACE_ENV_ARG_DECL)
{
  CORBA::TCKind kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  switch (kind)
    {
      case CORBA::tk_string:
      case CORBA::tk_char:
        cdr->char_translator (this->char_translator_);
        break;
      case CORBA::tk_wstring:
      case CORBA::tk_wchar:
        cdr->wchar_translator(this->wchar_translator_);
        break;
      default:
        break;
    }
}

CORBA::Boolean
TAO::Unknown_IDL_Type::to_object (CORBA::Object_ptr &obj) const
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ULong kind =
        this->type_->kind (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::TypeCode_var tcvar =
        CORBA::TypeCode::_duplicate (this->type_);

      while (kind == CORBA::tk_alias)
        {
          tcvar = tcvar->content_type (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          kind = tcvar->kind (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (kind != CORBA::tk_objref)
        {
          return 0;
        }

      TAO_InputCDR stream (this->cdr_,
                           this->byte_order_,
                           TAO_DEF_GIOP_MAJOR,
                           TAO_DEF_GIOP_MINOR,
                           0);

      return stream >> obj;
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
        this->type_->kind (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::TypeCode_var tcvar =
        CORBA::TypeCode::_duplicate (this->type_);

      while (kind == CORBA::tk_alias)
        {
          tcvar = tcvar->content_type (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          kind = tcvar->kind (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (kind != CORBA::tk_value)
        {
          return 0;
        }

      TAO_InputCDR stream (this->cdr_,
                           this->byte_order_,
                           TAO_DEF_GIOP_MAJOR,
                           TAO_DEF_GIOP_MINOR,
                           0);

      TAO_Valuetype_Adapter *adapter =
        ACE_Dynamic_Service<TAO_Valuetype_Adapter>::instance (
            TAO_ORB_Core::valuetype_adapter_name ()
          );

      if (adapter == 0)
        {
          ACE_THROW_RETURN (CORBA::INTERNAL (),
                            0);
        }

      return adapter->stream_to_value (stream,
                                       val);
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
        this->type_->kind (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::TypeCode_var tcvar =
        CORBA::TypeCode::_duplicate (this->type_);

      while (kind == CORBA::tk_alias)
        {
          tcvar = tcvar->content_type (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          kind = tcvar->kind (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (kind != CORBA::tk_value)
        {
          return 0;
        }

      TAO_InputCDR stream (this->cdr_,
                           this->byte_order_,
                           TAO_DEF_GIOP_MAJOR,
                           TAO_DEF_GIOP_MINOR,
                           TAO_ORB_Core_instance ());

      TAO_Valuetype_Adapter *adapter =
        ACE_Dynamic_Service<TAO_Valuetype_Adapter>::instance (
            TAO_ORB_Core::valuetype_adapter_name ()
          );

      if (adapter == 0)
        {
          ACE_THROW_RETURN (CORBA::INTERNAL (),
                            0);
        }

      return adapter->stream_to_abstract_base (stream,
                                               obj);
    }
  ACE_CATCH (CORBA::Exception, ex)
    {
    }
  ACE_ENDTRY;

  return 0;
}

