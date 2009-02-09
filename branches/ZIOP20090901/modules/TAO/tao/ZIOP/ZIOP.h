// -*- C++ -*-

//=============================================================================
/**
 *  @file ZIOP.h
 *
 *  $Id$
 *
 *  Dynamic loader object for the ZIOP library
 *
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ZIOP_H
#define TAO_ZIOP_H
#include /**/ "ace/pre.h"

#include "tao/ZIOP/ziop_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/ZIOP_Adapter.h"
#include "tao/Compression/Compression.h"
#include "tao/Policy_Validator.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_ZIOP_Loader
 *
 * @brief Class that loads the BiDir library.
 */

class TAO_ZIOP_Export TAO_ZIOP_Loader : public TAO_ZIOP_Adapter
{
public:

  /// Constructor
  TAO_ZIOP_Loader (void);

  /// Destructor
  virtual ~TAO_ZIOP_Loader (void);

  virtual bool decompress (TAO_InputCDR& cdr);

  // Compress the @a stream. Starting point of the compression is rd_ptr()
  virtual bool marshal_data (TAO_OutputCDR& cdr, TAO_Stub& stub);

  /// Initialize the BiDIR loader hooks.
  virtual int init (int argc, ACE_TCHAR* []);

  virtual void load_policy_validators (TAO_Policy_Validator &validator);

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);

  bool marshal_reply_data (TAO_OutputCDR& cdr, TAO_ORB_Core& orb_core);

private:

  /// Flag to indicate whether the ZIOP library has been
  /// activated.
  static bool is_activated_;

  /// Get the compression low value, returns 0 when it is not set
  CORBA::ULong compression_low_value (CORBA::Policy_ptr policy) const;

  bool get_compression_details(CORBA::Policy_ptr compression_enabling_policy,
                        CORBA::Policy_ptr compression_level_list_policy,
                        Compression::CompressorId &compressor_id, 
                        Compression::CompressionLevel &compression_level);

  bool compress_data (TAO_OutputCDR &cdr, 
                      CORBA::Object_ptr compression_manager,
                      CORBA::ULong low_value,
                      ::Compression::CompressorId compressor_id, 
                      ::Compression::CompressionLevel compression_level);

  bool compress (Compression::Compressor_ptr compressor,
                 const ::Compression::Buffer &source,
                 ::Compression::Buffer &target);

  bool check_min_ratio (CORBA::ULong original_data_length, CORBA::ULong compressed_length) const;
};

static int
TAO_Requires_ZIOP_Initializer = TAO_ZIOP_Loader::Initializer ();

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_ZIOP_Loader)
ACE_FACTORY_DECLARE (TAO_ZIOP, TAO_ZIOP_Loader)


#define TAO_ZIOP_SAFE_INCLUDE
#include "tao/ZIOP/ZIOPC.h"
#undef TAO_ZIOP_SAFE_INCLUDE

#include /**/ "ace/post.h"
#endif /* TAO_ZIOP_H */


