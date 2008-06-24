// -*- C++ -*-

//=============================================================================
/**
 *  @file ZIOP.h
 *
 *  $Id$
 *
 *  Dynamic loader object for BiDir GIOP
 *
 *
 *  @author  Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ZIOP_GIOP_H
#define TAO_ZIOP_GIOP_H
#include /**/ "ace/pre.h"

#include "tao/ZIOP/ziop_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/ZIOP_Adapter.h"
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

  virtual bool decompress (TAO_ServerRequest& server_request);

  // Compress the @a stream. Starting point of the compression is rd_ptr()
  virtual bool marshal_data (TAO_Operation_Details &details, TAO_OutputCDR &stream, TAO::Profile_Transport_Resolver &resolver);

  virtual void generate_service_context (TAO_Service_Context &service_cntx, TAO_Transport & );

  /// Initialize the BiDIR loader hooks.
  virtual int init (int argc, ACE_TCHAR* []);

  virtual void load_policy_validators (TAO_Policy_Validator &validator);

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);

private:

  /// Flag to indicate whether the ZIOP library has been
  /// activated.
  static bool is_activated_;

  /// Get the compression low value, returns 0 when it is not set
  CORBA::ULong compression_low_value (TAO::Profile_Transport_Resolver &resolver) const;

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
#endif /* TAO_ZIOP_GIOP_H */


