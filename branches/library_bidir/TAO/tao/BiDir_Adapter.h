// This may look like C, but it's really -*- C++ -*-

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
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"

class TAO_ORB_Core;

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

  /// Activate the BiDirGIOP library
  virtual int activate (CORBA::ORB_ptr,
                        int argc,
                        char *argv [],
                        CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

  virtual int parse_policy (TAO_ORB_Core *orb_core,
                            CORBA::Policy_ptr policy,
                            CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
};

#include "ace/post.h"
#endif /* TAO_BIDIR_ADAPTER_H */
