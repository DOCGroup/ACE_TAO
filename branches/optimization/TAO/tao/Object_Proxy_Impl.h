
//=============================================================================
/**
 *  @file    Object_Proxy_Impl.h
 *
 *  $Id$
 *
 *  This files contains the proxy definition of the interface that
 *  all the Object proxy have to imlements.
 *
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_OBJECT_PROXY_IMPL_H_
#define TAO_OBJECT_PROXY_IMPL_H_

#include "ace/pre.h"
#include "tao/corbafwd.h"

/**
 * @class TAO_Object_Proxy_Impl
 *
 * @brief TAO_Object_Proxy_Impl
 *
 * This class defines the interface that the proxy for the class
 * CORBA::Object have to implement.
 */
class TAO_Export TAO_Object_Proxy_Impl
{
public:

  virtual ~TAO_Object_Proxy_Impl (void);


  virtual CORBA::Boolean _is_a (const CORBA::Object_ptr target,
                                const CORBA::Char *logical_type_id,
                                CORBA_Environment &ACE_TRY_ENV) = 0;

#if (TAO_HAS_MINIMUM_CORBA == 0)

  virtual CORBA::Boolean _non_existent (const CORBA::Object_ptr target,
                                        CORBA_Environment &ACE_TRY_ENV) = 0;

#endif /* TAO_HAS_MINIMUM_CORBA == 0 */

protected:
  TAO_Object_Proxy_Impl (void);

};

#include "ace/post.h"

#endif /* TAO_OBJECT_PROXY_IMPL_H_ */
