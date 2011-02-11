// -*- C++ -*-
// $Id$

#include "tao/CodecFactory/CDR_Encaps_Codec.h"

#include "tao/CDR.h"
#include "tao/OctetSeqC.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/AnyTypeCode/Any_Impl.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/Marshal.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include "tao/SystemException.h"
#include "tao/ORB_Constants.h"
#include "tao/Codeset_Translator_Base.h"

#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_string.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CDR_Encaps_Codec::TAO_CDR_Encaps_Codec (
  CORBA::Octet major,
  CORBA::Octet minor,
  TAO_ORB_Core * orb_core,
  TAO_Codeset_Translator_Base * char_trans,
  TAO_Codeset_Translator_Base * wchar_trans)
  : major_ (major),
    minor_ (minor),
    orb_core_ (orb_core),
    char_translator_ (char_trans),
    wchar_translator_ (wchar_trans)
{
}

TAO_CDR_Encaps_Codec::~TAO_CDR_Encaps_Codec (void)
{
}

CORBA::OctetSeq *
TAO_CDR_Encaps_Codec::encode (const CORBA::Any & data)
{
  this->check_type_for_encoding (data);

  // ----------------------------------------------------------------

  TAO_OutputCDR cdr ((size_t) 0,            // size
                     (int) TAO_ENCAP_BYTE_ORDER,
                     (ACE_Allocator *) 0,   // buffer_allocator
                     (ACE_Allocator *) 0,   // data_block_allocator
                     (ACE_Allocator *) 0,   // message_block_allocator
                     0,                     // memcpy_tradeoff
                     this->major_,
                     this->minor_);

  if (this->char_translator_)
    {
      this->char_translator_->assign (&cdr);
    }
  if (this->wchar_translator_)
    {
      this->wchar_translator_->assign (&cdr);
    }

  if ((cdr << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER))
      && (cdr << data))
    {
      CORBA::OctetSeq * octet_seq = 0;

      ACE_NEW_THROW_EX (octet_seq,
                        CORBA::OctetSeq,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            0,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      CORBA::OctetSeq_var safe_octet_seq = octet_seq;

      octet_seq->length (static_cast<CORBA::ULong> (cdr.total_length ()));
      CORBA::Octet *buf = octet_seq->get_buffer ();

      for (const ACE_Message_Block *i = cdr.begin ();
           i != 0;
           i = i->cont ())
        {
          size_t const len = i->length ();
          ACE_OS::memcpy (buf, i->rd_ptr (), len);
          buf += len;
        }

      return safe_octet_seq._retn ();
    }

  throw ::CORBA::MARSHAL ();
}

CORBA::Any *
TAO_CDR_Encaps_Codec::decode (const CORBA::OctetSeq & data)
{
  // @todo How do we check for a format mismatch so that we can throw
  //       a IOP::Codec::FormatMismatch exception?
  // @todo Is this the best way to extract the Any from the OctetSeq?

  // Notice that we need to extract the TypeCode and the value from
  // the octet sequence, and place them into the Any.  We can't just
  // insert the octet sequence into the Any.

  ACE_Message_Block mb (data.length () + 2 * ACE_CDR::MAX_ALIGNMENT);
  ACE_CDR::mb_align (&mb);

  ACE_OS::memcpy (mb.rd_ptr (), data.get_buffer (), data.length ());

  size_t rd_pos = mb.rd_ptr () - mb.base ();
  size_t wr_pos = mb.wr_ptr () - mb.base () + data.length ();

  TAO_InputCDR cdr (mb.data_block (),
                    ACE_Message_Block::DONT_DELETE,
                    rd_pos,
                    wr_pos,
                    ACE_CDR_BYTE_ORDER,
                    this->major_,
                    this->minor_,
                    this->orb_core_);

  if (this->char_translator_)
    {
      this->char_translator_->assign (&cdr);
    }
  if (this->wchar_translator_)
    {
      this->wchar_translator_->assign (&cdr);
    }

  CORBA::Boolean byte_order;
  if (cdr >> TAO_InputCDR::to_boolean (byte_order))
    {
      cdr.reset_byte_order (static_cast<int> (byte_order));

      CORBA::Any * any = 0;
      ACE_NEW_THROW_EX (any,
                        CORBA::Any,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            0,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      CORBA::Any_var safe_any = any;

      if (cdr >> (*any))
        return safe_any._retn ();
    }

  throw IOP::Codec::FormatMismatch ();
}

CORBA::OctetSeq *
TAO_CDR_Encaps_Codec::encode_value (const CORBA::Any & data)
{
  this->check_type_for_encoding (data);

  // ----------------------------------------------------------------
  TAO_OutputCDR cdr ((size_t) 0,            // size
                     (int) TAO_ENCAP_BYTE_ORDER,
                     (ACE_Allocator *) 0,   // buffer_allocator
                     (ACE_Allocator *) 0,   // data_block_allocator
                     (ACE_Allocator *) 0,   // message_block_allocator
                     0,                     // memcpy_tradeoff
                     this->major_,
                     this->minor_);

  if (this->char_translator_)
    {
      this->char_translator_->assign (&cdr);
    }
  if (this->wchar_translator_)
    {
      this->wchar_translator_->assign (&cdr);
    }

  if ((cdr << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)))
    {
      TAO::Any_Impl *impl = data.impl ();

      if (impl->encoded ())
        {
          TAO::Unknown_IDL_Type * const unk =
            dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

          if (!unk)
            throw ::CORBA::INTERNAL ();

          // We don't want unk's rd_ptr to move, in case we are shared by
          // another Any, so we use this to copy the state, not the buffer.
          TAO_InputCDR for_reading (unk->_tao_get_cdr ());

          TAO_Marshal_Object::perform_append (data._tao_get_typecode (),
                                              &for_reading,
                                              &cdr);
        }
      else
        {
          impl->marshal_value (cdr);
        }

      // TAO extension: replace the contents of the octet sequence with
      // the CDR stream.
      CORBA::OctetSeq * octet_seq = 0;

      ACE_NEW_THROW_EX (octet_seq,
                        CORBA::OctetSeq,
                        CORBA::NO_MEMORY (
                            CORBA::SystemException::_tao_minor_code (
                                0,
                                ENOMEM
                              ),
                            CORBA::COMPLETED_NO
                          ));

      CORBA::OctetSeq_var safe_octet_seq = octet_seq;

      octet_seq->length (static_cast<CORBA::ULong> (cdr.total_length ()));
      CORBA::Octet *buf = octet_seq->get_buffer ();

      for (const ACE_Message_Block *i = cdr.begin ();
           i != 0;
           i = i->cont ())
        {
          size_t len = i->length ();
          ACE_OS::memcpy (buf,
                          i->rd_ptr (),
                          len);
          buf += len;
        }

      return safe_octet_seq._retn ();
    }

  throw ::CORBA::MARSHAL ();
}

CORBA::Any *
TAO_CDR_Encaps_Codec::decode_value (const CORBA::OctetSeq & data,
                                    CORBA::TypeCode_ptr tc)
{
  // The ACE_CDR::mb_align() call can shift the rd_ptr by up
  // to ACE_CDR::MAX_ALIGNMENT-1 bytes. Similarly, the offset
  // adjustment can move the rd_ptr by up to the same amount.
  // We accommodate this by including
  // 2 * ACE_CDR::MAX_ALIGNMENT bytes of additional space in
  // the message block.
  ACE_Message_Block mb (data.length () + 2 * ACE_CDR::MAX_ALIGNMENT);
  ACE_CDR::mb_align (&mb);

  ACE_OS::memcpy (mb.rd_ptr (),
                  data.get_buffer (),
                  data.length ());

  // @todo How do we check for a type mismatch so that we can
  //       throw a IOP::Codec::TypeMismatch exception?
  //       @@ I added a check below.  See the comment.  I'm not sure
  //          if it is a valid check.
  //             -Ossama

  // @todo Most of this code was copied from
  //         operator>> (TAO_InputCDR &cdr, CORBA::Any &x)
  //       in Any.cpp.  Rather than copy the code, the code should be
  //       refactored to make it possible to use the given TypeCode
  //       rather than attempt to extract it from the CDR
  //       encapsulation.

  size_t rd_pos = mb.rd_ptr () - mb.base ();
  size_t wr_pos = mb.wr_ptr () - mb.base () + data.length ();

  TAO_InputCDR cdr (mb.data_block (),
                    ACE_Message_Block::DONT_DELETE,
                    rd_pos,
                    wr_pos,
                    ACE_CDR_BYTE_ORDER,
                    this->major_,
                    this->minor_,
                    this->orb_core_);

  if (this->char_translator_)
    {
      this->char_translator_->assign (&cdr);
    }
  if (this->wchar_translator_)
    {
      this->wchar_translator_->assign (&cdr);
    }

  CORBA::Boolean byte_order;

  if (cdr >> TAO_InputCDR::to_boolean (byte_order))
    {
      cdr.reset_byte_order (static_cast<int> (byte_order));

      CORBA::Any * any = 0;
      ACE_NEW_THROW_EX (any,
                        CORBA::Any,
                        CORBA::NO_MEMORY (
                            CORBA::SystemException::_tao_minor_code (
                                0,
                                ENOMEM
                              ),
                            CORBA::COMPLETED_NO
                          ));

      CORBA::Any_var safe_any = any;

      // Stick it into the Any.
      TAO::Unknown_IDL_Type *unk = 0;
      ACE_NEW_RETURN (unk,
                      TAO::Unknown_IDL_Type (tc, cdr),
                      0);
      any->replace (unk);
      return safe_any._retn ();
    }

  throw IOP::Codec::FormatMismatch ();
}

void
TAO_CDR_Encaps_Codec::check_type_for_encoding (const CORBA::Any & data)
{
  // @@ TODO: Are there any other conditions we need to check?

  CORBA::TypeCode_var typecode = data.type ();
  if (this->major_ == 1
      && this->minor_ == 0
      && typecode->equivalent (CORBA::_tc_wstring))
    throw IOP::Codec::InvalidTypeForEncoding ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
