// -*- C++ -*-
//
// $Id$

#include "CodecFactory.h"
#include "CDR_Encaps_Codec.h"

ACE_RCSID (TAO_CodecFactory,
           CodecFactory,
           "$Id$")

TAO_CodecFactory::TAO_CodecFactory (void)
{
}

IOP::Codec_ptr
TAO_CodecFactory::create_codec (const IOP::Encoding & enc
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

  switch (enc.format)
    {

      // @@ MSVC 6 gets confused and thinks that
      //    IOP::ENCODING_CDR_ENCAPS is not a constant, so its actual
      //    value (0) is used instead.
    case 0 /* IOP::ENCODING_CDR_ENCAPS */:
      if (enc.major_version < 1)
        {
          // There is no such thing as a "0.x" CDR encapsulation.
          ACE_THROW_RETURN (CORBA::BAD_PARAM (
                              CORBA_SystemException::_tao_minor_code (
                                TAO_DEFAULT_MINOR_CODE,
                                EINVAL),
                              CORBA::COMPLETED_NO),
                            IOP::Codec::_nil ());
        }

      ACE_NEW_THROW_EX (codec,
                        TAO_CDR_Encaps_Codec (enc.major_version,
                                              enc.minor_version),
                        CORBA::NO_MEMORY (
                          CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
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
