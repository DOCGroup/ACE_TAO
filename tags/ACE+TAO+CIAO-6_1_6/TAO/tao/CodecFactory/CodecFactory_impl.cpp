// -*- C++ -*-
// $Id$

#include "tao/CodecFactory/CodecFactory_impl.h"
#include "tao/CodecFactory/CDR_Encaps_Codec.h"
#include "tao/SystemException.h"
#include "tao/ORB_Constants.h"
#include "tao/ORB_Core.h"
#include "tao/Codeset_Manager.h"
#include "ace/Codeset_Symbols.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CodecFactory::TAO_CodecFactory (TAO_ORB_Core * orb_core)
  : orb_core_ (orb_core)
{
}

IOP::Codec_ptr
TAO_CodecFactory::create_codec_with_codesets (const IOP::Encoding_1_2 & enc)
{
  TAO_Codeset_Translator_Base *char_trans = 0;
  TAO_Codeset_Translator_Base *wchar_trans = 0;
  CONV_FRAME::CodeSetId ncsc;
  CONV_FRAME::CodeSetId ncsw;

  TAO_Codeset_Manager *csm = this->orb_core_->codeset_manager ();

  if (csm)
    {
      char_trans = csm->get_char_trans (enc.char_codeset);
      wchar_trans = csm->get_wchar_trans (enc.wchar_codeset);
      csm->get_ncs (ncsc,ncsw); // pass by reference
    }
  else
    {
      // No codeset manager, so also raise an unsupported codeset
      throw IOP::CodecFactory::UnsupportedCodeset (enc.wchar_codeset);
    }

  if (wchar_trans == 0 &&
      enc.wchar_codeset != ACE_CODESET_ID_ISO_UTF_16 &&
      enc.wchar_codeset != ncsw)
    {
      throw IOP::CodecFactory::UnsupportedCodeset (enc.wchar_codeset);
    }

  if (char_trans == 0 &&
      enc.char_codeset != ncsc)
    {
      throw IOP::CodecFactory::UnsupportedCodeset (enc.char_codeset);
    }

  return this->create_codec_i (enc.major_version,
                               enc.minor_version,
                               enc.format,
                               char_trans,
                               wchar_trans);
}

IOP::Codec_ptr
TAO_CodecFactory::create_codec (const IOP::Encoding & enc)
{
  return this->create_codec_i (enc.major_version,
                               enc.minor_version,
                               enc.format,
                               0,
                               0);
}

IOP::Codec_ptr
TAO_CodecFactory::create_codec_i (CORBA::Octet major,
                                  CORBA::Octet minor,
                                  IOP::EncodingFormat encoding_format,
                                  TAO_Codeset_Translator_Base * char_trans,
                                  TAO_Codeset_Translator_Base * wchar_trans)
{
  // @todo: Ideally we should have some sort of CodecFactory
  //        registry to make it possible to add factories
  //        dynamically.  However, there currently isn't a need to
  //        support anything other than CDR encapsulations yet so we
  //        hardcode its Codec.  This may change once TAO starts to
  //        support messaging formats other than GIOP.

  IOP::Codec_ptr codec = IOP::Codec::_nil ();

  switch (encoding_format)
    {

      // @@ MSVC 6 gets confused and thinks that
      //    IOP::ENCODING_CDR_ENCAPS is not a constant, so its actual
      //    value (0) is used instead.
    case 0 /* IOP::ENCODING_CDR_ENCAPS */:
      if (major < 1)
        {
          // There is no such thing as a "0.x" CDR encapsulation.
          throw ::CORBA::BAD_PARAM (
            CORBA::SystemException::_tao_minor_code (
              0,
              EINVAL),
            CORBA::COMPLETED_NO);
        }

      ACE_NEW_THROW_EX (codec,
                        TAO_CDR_Encaps_Codec (major,
                                              minor,
                                              this->orb_core_,
                                              char_trans,
                                              wchar_trans),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            0,
                            ENOMEM),
                          CORBA::COMPLETED_MAYBE));
      break;

    default:
      throw IOP::CodecFactory::UnknownEncoding ();

    }

  return codec;
}

TAO_END_VERSIONED_NAMESPACE_DECL
