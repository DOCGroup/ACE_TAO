// -*- C++ -*-
//
// $Id$

#include "CDR.h"
#include "OctetSeqC.h"
#include "Any.h"
#include "Typecode.h"
#include "Marshal.h"

#include "CDR_Encaps_Codec.h"

ACE_RCSID (TAO_CodecFactory,
           CDR_Encaps_Codec,
           "$Id$")


TAO_CDR_Encaps_Codec::TAO_CDR_Encaps_Codec (CORBA::Octet major,
                                            CORBA::Octet minor,
                                            TAO_ORB_Core * orb_core)
  : major_ (major),
    minor_ (minor),
    orb_core_ (orb_core)
{
}

TAO_CDR_Encaps_Codec::~TAO_CDR_Encaps_Codec (void)
{
}

CORBA::OctetSeq *
TAO_CDR_Encaps_Codec::encode (const CORBA::Any & data
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   IOP::Codec::InvalidTypeForEncoding))
{
  this->check_type_for_encoding (data
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // ----------------------------------------------------------------

  TAO_OutputCDR cdr ((size_t) 0,            // size
                     (int) ACE_CDR_BYTE_ORDER,
                     (ACE_Allocator *) 0,   // buffer_allocator
                     (ACE_Allocator *) 0,   // data_block_allocator
                     (ACE_Allocator *) 0,   // message_block_allocator
                     0,                     // memcpy_tradeoff
                     this->major_,
                     this->minor_);

  if ((cdr << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER))
      && (cdr << data))
    {
      CORBA::OctetSeq * octet_seq = 0;

      ACE_NEW_THROW_EX (octet_seq,
                        CORBA::OctetSeq,
                        CORBA::NO_MEMORY (
                          CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (0);

      CORBA::OctetSeq_var safe_octet_seq = octet_seq;

      octet_seq->length (cdr.total_length ());
      CORBA::Octet *buf = octet_seq->get_buffer ();

      for (const ACE_Message_Block *i = cdr.begin ();
           i != 0;
           i = i->cont ())
        {
          size_t len = i->length ();
          ACE_OS_String::memcpy (buf, i->rd_ptr (), len);
          buf += len;
        }

      return safe_octet_seq._retn ();
    }

  ACE_THROW_RETURN (CORBA::MARSHAL (), 0);
}

CORBA::Any *
TAO_CDR_Encaps_Codec::decode (const CORBA::OctetSeq & data
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   IOP::Codec::FormatMismatch))
{
  // @todo How do we check for a format mismatch so that we can throw
  //       a IOP::Codec::FormatMismatch exception?
  // @todo Is this the best way to extract the Any from the OctetSeq?

  // Notice that we need to extract the TypeCode and the value from
  // the octet sequence, and place them into the Any.  We can't just
  // insert the octet sequence into the Any.

  TAO_InputCDR cdr (ACE_reinterpret_cast (const char*,
                                          data.get_buffer ()),
                    data.length (),
                    ACE_CDR_BYTE_ORDER,
                    this->major_,
                    this->minor_,
                    this->orb_core_);

  CORBA::Boolean byte_order;
  if (cdr >> TAO_InputCDR::to_boolean (byte_order))
    {
      cdr.reset_byte_order (ACE_static_cast (int, byte_order));

      CORBA::Any * any = 0;
      ACE_NEW_THROW_EX (any,
                        CORBA::Any,
                        CORBA::NO_MEMORY (
                          CORBA_SystemException::_tao_minor_code (
                           TAO_DEFAULT_MINOR_CODE,
                           ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (0);

      CORBA::Any_var safe_any = any;

      if (cdr >> (*any))
        return safe_any._retn ();
    }

  ACE_THROW_RETURN (IOP::Codec::FormatMismatch (), 0);
}

CORBA::OctetSeq *
TAO_CDR_Encaps_Codec::encode_value (const CORBA::Any & data
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   IOP::Codec::InvalidTypeForEncoding))
{
  this->check_type_for_encoding (data
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // ----------------------------------------------------------------
  TAO_OutputCDR cdr ((size_t) 0,            // size
                     (int) ACE_CDR_BYTE_ORDER,
                     (ACE_Allocator *) 0,   // buffer_allocator
                     (ACE_Allocator *) 0,   // data_block_allocator
                     (ACE_Allocator *) 0,   // message_block_allocator
                     0,                     // memcpy_tradeoff
                     this->major_,
                     this->minor_);

  if ((cdr << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)))
    {
      CORBA::TypeCode_var tc = data.type ();

      TAO_InputCDR input (data._tao_get_cdr (),
                          data._tao_byte_order ());

      TAO_Marshal_Object::perform_append (tc.in (),
                                          &input,
                                          &cdr
                                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      // TAO extension: replace the contents of the octet sequence with
      // the CDR stream.
      CORBA::OctetSeq * octet_seq = 0;

      ACE_NEW_THROW_EX (octet_seq,
                        CORBA::OctetSeq,
                        CORBA::NO_MEMORY (
                          CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (0);

      CORBA::OctetSeq_var safe_octet_seq = octet_seq;

      octet_seq->length (cdr.total_length ());
      CORBA::Octet *buf = octet_seq->get_buffer ();

      for (const ACE_Message_Block *i = cdr.begin ();
           i != 0;
           i = i->cont ())
        {
          size_t len = i->length ();
          ACE_OS_String::memcpy (buf, i->rd_ptr (), len);
          buf += len;
        }

      return safe_octet_seq._retn ();
    }

  ACE_THROW_RETURN (CORBA::MARSHAL (), 0);
}

CORBA::Any *
TAO_CDR_Encaps_Codec::decode_value (const CORBA::OctetSeq & data,
                                    CORBA::TypeCode_ptr tc
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   IOP::Codec::FormatMismatch,
                   IOP::Codec::TypeMismatch))
{
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

  CORBA::ULong sequence_length = data.length ();
  TAO_InputCDR cdr (ACE_reinterpret_cast (const char*,
                                          data.get_buffer ()),
                    sequence_length,
                    ACE_CDR_BYTE_ORDER,
                    this->major_,
                    this->minor_,
                    this->orb_core_);

  CORBA::Boolean byte_order;
  if (cdr >> TAO_InputCDR::to_boolean (byte_order))
    {
      cdr.reset_byte_order (ACE_static_cast (int, byte_order));

      // @@ (JP) The following code depends on the fact that
      //         TAO_InputCDR does not contain chained message blocks,
      //         otherwise <begin> and <end> could be part of
      //         different buffers!

      // This will be the start of a new message block.
      char *begin = cdr.rd_ptr ();

      // Skip over the next argument.
      CORBA::TypeCode::traverse_status status =
        TAO_Marshal_Object::perform_skip (tc,
                                          &cdr
                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);  // @@ Should we throw a
                             //    IOP::Codec::TypeMismatch exception
                             //    here if this fails?

      if (status == CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          // This will be the end of the new message block.
          char *end = cdr.rd_ptr ();

          size_t size = end - begin;

          // @@ I added the following check, but I'm not sure if it is
          //    a valid check.  Can someone verify this?
          //          -Ossama

          // If the unaligned buffer size is not equal to the octet
          // sequence length (minus the "byte order byte") then the
          // TypeCode does not correspond to the data in the CDR
          // encapsulation.  However, even if they do match it is
          // still uncertain if the TypeCode corresponds to the data
          // in the octet sequence.  With this test, it is only
          // possible to determine if the TypeCode does *not* match
          // the data, not if it does match.
          if (size != sequence_length - 1)
            ACE_THROW_RETURN (IOP::Codec::TypeMismatch (), 0);

          // The ACE_CDR::mb_align() call can shift the rd_ptr by up
          // to ACE_CDR::MAX_ALIGNMENT-1 bytes. Similarly, the offset
          // adjustment can move the rd_ptr by up to the same amount.
          // We accommodate this by including
          // 2 * ACE_CDR::MAX_ALIGNMENT bytes of additional space in
          // the message block.
          ACE_Message_Block mb (size + 2 * ACE_CDR::MAX_ALIGNMENT);
          ACE_CDR::mb_align (&mb);
          ptr_arith_t offset =
            ptr_arith_t (begin) % ACE_CDR::MAX_ALIGNMENT;
          mb.rd_ptr (offset);
          mb.wr_ptr (offset + size);

          ACE_OS::memcpy (mb.rd_ptr (), begin, size);

          CORBA::Any * any = 0;
          ACE_NEW_THROW_EX (any,
                            CORBA::Any,
                            CORBA::NO_MEMORY (
                              CORBA_SystemException::_tao_minor_code (
                                TAO_DEFAULT_MINOR_CODE,
                                ENOMEM),
                              CORBA::COMPLETED_NO));
          ACE_CHECK_RETURN (0);

          CORBA::Any_var safe_any = any;

          // Stick it into the Any.  It gets duplicated there.
          any->_tao_replace (tc,
                             cdr.byte_order (),
                             &mb);

          return safe_any._retn ();
        }
      else
        ACE_THROW_RETURN (IOP::Codec::TypeMismatch (), 0);
    }

  ACE_THROW_RETURN (IOP::Codec::FormatMismatch (), 0);
}

void
TAO_CDR_Encaps_Codec::check_type_for_encoding (
  const CORBA::Any & data
  ACE_ENV_ARG_DECL)
{
  // @@ TODO: Are there any other conditions we need to check?

  CORBA::TypeCode_var typecode = data.type ();
  if (this->major_ == 1
      && this->minor_ == 0
      && typecode->equivalent (CORBA::_tc_wstring))
    ACE_THROW (IOP::Codec::InvalidTypeForEncoding ());
}
