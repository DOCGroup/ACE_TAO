// -*- C++ -*-

//=============================================================================
/**
 * @file CodecFactory_impl.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_CODEC_FACTORY_IMPL_H
#define TAO_CODEC_FACTORY_IMPL_H

#include /**/ "ace/pre.h"

#include "tao/CodecFactory/IOP_Codec_includeC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Codeset_Translator_Base;

/**
 * @class TAO_CodecFactory
 *
 * @brief Implementation of the IOP::CodecFactory interface.
 *
 * This class can be used to create Codec (coder/decoder) of a given
 * type, such as a CDR encapsulation Codec.
 */
class TAO_CodecFactory
  : public virtual IOP::CodecFactory,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor
  TAO_CodecFactory (TAO_ORB_Core * orb_core);

  /// Create a Coder/Decoder for the given type of encoding.
  virtual IOP::Codec_ptr create_codec (const IOP::Encoding & enc);

  /// Create a Coder/Decoder for the given type of encoding and codesets.
  virtual IOP::Codec_ptr create_codec_with_codesets (const IOP::Encoding_1_2 & enc);

private:
  /// Prevent copying through the copy constructor and the assignment
  /// operator.
  TAO_CodecFactory (const TAO_CodecFactory &);
  void operator= (const TAO_CodecFactory &);

  IOP::Codec_ptr create_codec_i (CORBA::Octet major,
                                 CORBA::Octet minor,
                                 IOP::EncodingFormat encoding_format,
                                 TAO_Codeset_Translator_Base * char_trans,
                                 TAO_Codeset_Translator_Base * wchar_trans);
private:

  /// Pointer to the ORB Core.
  /**
   * Some Codec implementations may need access to the ORB Core with
   * which they are associated.
   */
  TAO_ORB_Core * const orb_core_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif  /* TAO_CODEC_FACTORY_IMPL_H */
