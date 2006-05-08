// -*- C++ -*-
//
// $Id$

#include "tao/CodecFactory/CodecFactory_impl.h"
#include "tao/CodecFactory/CDR_Encaps_Codec.h"
#include "tao/SystemException.h"
#include "tao/ORB_Constants.h"
#include "tao/ORB_Core.h"
#include "tao/Codeset_Manager.h"
#include "ace/Codeset_Symbols.h"

ACE_RCSID (CodecFactory_impl,
           CodecFactory,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CodecFactory::TAO_CodecFactory (TAO_ORB_Core * orb_core)
  : orb_core_ (orb_core)
{
}

IOP::Codec_ptr
TAO_CodecFactory::create_codec_with_codesets (const IOP::Encoding_1_2 & enc
                                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     IOP::CodecFactory::UnknownEncoding,
                     IOP::CodecFactory::UnsupportedCodeset))
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
      ACE_THROW_RETURN (IOP::CodecFactory::UnsupportedCodeset (
                          enc.wchar_codeset),
                        IOP::Codec::_nil ());
    }

  if (wchar_trans == 0 &&
      enc.wchar_codeset != ACE_CODESET_ID_ISO_UTF_16 &&
      enc.wchar_codeset != ncsw)
    {
      ACE_THROW_RETURN (IOP::CodecFactory::UnsupportedCodeset (
                          enc.wchar_codeset),
                        IOP::Codec::_nil ());
    }

  if (char_trans == 0 &&
      enc.char_codeset != ncsc)
    {
      ACE_THROW_RETURN (IOP::CodecFactory::UnsupportedCodeset (
                          enc.char_codeset),
                        IOP::Codec::_nil ());
    }

  return this->create_codec_i (enc.major_version,
                               enc.minor_version,
                               enc.format,
                               char_trans,
                               wchar_trans
                               ACE_ENV_ARG_PARAMETER);
}

IOP::Codec_ptr
TAO_CodecFactory::create_codec (const IOP::Encoding & enc
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   IOP::CodecFactory::UnknownEncoding))
{
  return this->create_codec_i (enc.major_version,
                               enc.minor_version,
                               enc.format,
                               0,
                               0
                               ACE_ENV_ARG_PARAMETER);
}

IOP::Codec_ptr
TAO_CodecFactory::create_codec_i (CORBA::Octet major,
                                  CORBA::Octet minor,
                                  IOP::EncodingFormat encoding_format,
                                  TAO_Codeset_Translator_Base * char_trans,
                                  TAO_Codeset_Translator_Base * wchar_trans
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   IOP::CodecFactory::UnknownEncoding))
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
          ACE_THROW_RETURN (CORBA::BAD_PARAM (
                              CORBA::SystemException::_tao_minor_code (
                                0,
                                EINVAL),
                              CORBA::COMPLETED_NO),
                            IOP::Codec::_nil ());
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
      ACE_CHECK_RETURN (IOP::Codec::_nil ());
      break;

    default:
      ACE_THROW_RETURN (IOP::CodecFactory::UnknownEncoding (),
                        IOP::Codec::_nil ());

    }

  return codec;
}

TAO_END_VERSIONED_NAMESPACE_DECL
