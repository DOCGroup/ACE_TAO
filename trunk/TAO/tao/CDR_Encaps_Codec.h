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

#include "ace/pre.h"

#include "TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "IOPC.h"
#include "LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */


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
class TAO_Export TAO_CDR_Encaps_Codec
  : public virtual IOP::Codec,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  TAO_CDR_Encaps_Codec (CORBA::Octet major, CORBA::Octet minor);

  /// Encode the given data, including the TypeCode, into an octet
  /// sequence.
  virtual CORBA::OctetSeq * encode (const CORBA::Any & data
                                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     IOP::Codec::InvalidTypeForEncoding));

  /// Extract the TypeCode and the value from the octet sequence and
  /// place them into an Any.
  virtual CORBA::Any * decode (const CORBA::OctetSeq & data
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     IOP::Codec::FormatMismatch));

  /// Encode the given data, excluding the TypeCode, into an octet
  /// sequence.
  virtual CORBA::OctetSeq * encode_value (const CORBA::Any & data
                                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     IOP::Codec::InvalidTypeForEncoding));

  /// Extract the value from the octet sequence, based on the given
  /// TypeCode,  and place it into an Any.
  virtual CORBA::Any * decode_value (const CORBA::OctetSeq & data,
                                     CORBA::TypeCode_ptr tc
                                     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     IOP::Codec::FormatMismatch,
                     IOP::Codec::TypeMismatch));

protected:

  /// Destructor.
  /**
   * Only allow this class to be instantiated on the heap since it is
   * reference counted.
   */
  ~TAO_CDR_Encaps_Codec (void);

  /// Verify that it is possible to encode the given data using this
  /// Codec.
  /**
   * Typical reasons for failure include attempting to encode a type
   * that isn't supported for the version of GIOP associated with this
   * Codec.
   */
  void check_type_for_encoding (const CORBA::Any & data
                                ACE_ENV_ARG_DECL);

private:

  /// Prevent copying through the copy constructor and the assignment
  /// operator.
  ACE_UNIMPLEMENTED_FUNC (
    TAO_CDR_Encaps_Codec (const TAO_CDR_Encaps_Codec &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_CDR_Encaps_Codec &))

private:

  /// The major GIOP version associated with this Codec.
  CORBA::Octet major_;

  /// The minor GIOP version associated with this Codec.
  CORBA::Octet minor_;

};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"

#endif  /* TAO_CDR_ENCAPS_CODEC_H */

