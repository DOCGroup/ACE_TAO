// -*- C++ -*-

//=============================================================================
/**
 *  @file    BiDir_Adapter.h
 *
 *  $Id$
 *
 *  @author  Balachandran Natarajan <parsons@cs.wustl.edu>
 */
//=============================================================================
#ifndef TAO_BIDIR_ADAPTER_H
#define TAO_BIDIR_ADAPTER_H

#include /**/ "ace/pre.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/SystemException.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Policy_Validator;

/**
 * @class TAO_BiDir_Adapter
 *
 * @brief TAO_BiDir_Adapter.
 *
 * Class that offers an interface to the ORB to load and manipulate
 * BiDirGIOP library.
 */
class TAO_Export TAO_BiDir_Adapter : public ACE_Service_Object
{
public:

  /// The virtual destructor
  virtual ~TAO_BiDir_Adapter (void);

  virtual void load_policy_validators (TAO_Policy_Validator &validator ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_BIDIR_ADAPTER_H */
