// -*- C++ -*-

//=============================================================================
/**
 * @file CDR_Encaps_Codec.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_CDR_ENCAPS_CODEC_H
#define TAO_CDR_ENCAPS_CODEC_H

#include /**/ "ace/pre.h"

#include "tao/CodecFactory/IOP_Codec_includeC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Codeset_Translator_Base;

/**
 * @class TAO_CDR_Encaps_Codec
 *
 * @brief Implementation of a CDR encapsulation coder/decoder
 * (Codec).
 *
 * This coder/decoder (Codec) class encodes and decodes data to and
 * from a CDR encapsulation, respectively.  It is useful for creation
 * of octet sequences that contain CDR encapsulations.  Those octet
 * sequences can then be placed in a IOP::ServiceContext or an
 * IOP::TaggedComponent, for example.
 *
 * @note This Codec should not be used for operations internal to the
 * ORB core since it uses interpretive marshaling rather than compiled
 * marshaling.
 */
class TAO_CDR_Encaps_Codec
  : public virtual IOP::Codec,
    public virtual ::CORBA::LocalObject
{
public:

  /// Constructor.
  TAO_CDR_Encaps_Codec (CORBA::Octet major,
                        CORBA::Octet minor,
                        TAO_ORB_Core * orb_core,
                        TAO_Codeset_Translator_Base * char_trans,
                        TAO_Codeset_Translator_Base * wchar_trans);

  /// Encode the given data, including the TypeCode, into an octet
  /// sequence.
  virtual CORBA::OctetSeq * encode (const CORBA::Any & data);

  /// Extract the TypeCode and the value from the octet sequence and
  /// place them into an Any.
  virtual CORBA::Any * decode (const CORBA::OctetSeq & data);

  /// Encode the given data, excluding the TypeCode, into an octet
  /// sequence.
  virtual CORBA::OctetSeq * encode_value (const CORBA::Any & data);

  /// Extract the value from the octet sequence, based on the given
  /// TypeCode,  and place it into an Any.
  virtual CORBA::Any * decode_value (const CORBA::OctetSeq & data,
                                     CORBA::TypeCode_ptr tc);

protected:

  /// Destructor.
  /**
   * Only allow this class to be instantiated on the heap since it is
   * reference counted.
   */
  virtual ~TAO_CDR_Encaps_Codec (void);

  /// Verify that it is possible to encode the given data using this
  /// Codec.
  /**
   * Typical reasons for failure include attempting to encode a type
   * that isn't supported for the version of GIOP associated with this
   * Codec.
   */
  void check_type_for_encoding (const CORBA::Any & data);

private:

  /// Prevent copying through the copy constructor and the assignment
  /// operator.
  TAO_CDR_Encaps_Codec (const TAO_CDR_Encaps_Codec &);
  void operator= (const TAO_CDR_Encaps_Codec &);

private:

  /// The major GIOP version associated with this Codec.
  CORBA::Octet const major_;

  /// The minor GIOP version associated with this Codec.
  CORBA::Octet const minor_;

  /// The ORB Core to be used when decoding values from a CDR
  /// encapsulation.
  TAO_ORB_Core * const orb_core_;

  /// Char codeset translator
  TAO_Codeset_Translator_Base * char_translator_;

  /// WChar codeset translator
  TAO_Codeset_Translator_Base * wchar_translator_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_CDR_ENCAPS_CODEC_H */
