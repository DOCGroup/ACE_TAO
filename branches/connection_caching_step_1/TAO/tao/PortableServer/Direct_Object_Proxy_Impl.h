// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Direct_Object_Proxy_Impl.h
//
// = DESCRIPTION
//    This files contains the definition of the direct proxy
//    for the CORBA::Object class.
//
// = AUTHOR
//     Angelo Corsaro <corsaro@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_DIRECT_OBJECT_PROXY_IMPL_H_
#define TAO_DIRECT_OBJECT_PROXY_IMPL_H_

#include "portableserver_export.h"

// -- ACE Include --
#include "ace/pre.h"

// -- TAO Include --
#include "tao/Object_Proxy_Impl.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_PortableServer_Export TAO_Direct_Object_Proxy_Impl : public virtual TAO_Object_Proxy_Impl
{
  // = TITLE
  //     TAO_Direct_Object_Proxy_Impl
  //
  // = DESCRIPTION
  //     This class implements the direct proxy for the CORBA::Object
  //     class.
public:

  virtual ~TAO_Direct_Object_Proxy_Impl (void);

  virtual CORBA::Boolean _is_a (const CORBA::Object_ptr target,
                                const CORBA::Char *logical_type_id,
                                CORBA_Environment &ACE_TRY_ENV);

#if (TAO_HAS_MINIMUM_CORBA == 0)

  virtual CORBA::Boolean _non_existent (const CORBA::Object_ptr target,
                                        CORBA_Environment &ACE_TRY_ENV);

#endif /* TAO_HAS_MINIMUM_CORBA == 0 */

};

#include "ace/post.h"

#endif /* TAO_DIRECT_OBJECT_PROXY_IMPL */
