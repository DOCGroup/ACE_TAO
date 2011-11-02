// -*- C++ -*-

//=============================================================================
/**
 *  @file    Direct_Collocation_Upcall_Wrapper.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen
 */
//=============================================================================

#ifndef TAO_DIRECT_COLLOCATION_UPCALL_WRAPPER_H
#define TAO_DIRECT_COLLOCATION_UPCALL_WRAPPER_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* !ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"
#include "tao/orbconf.h"
#include "tao/Collocation_Strategy.h"
#include "tao/Exception.h"
#include "tao/CORBA_methods.h"
#include "tao/Pseudo_VarOut_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Object;
  typedef Object *Object_ptr;
  typedef TAO_Pseudo_Var_T<Object> Object_var;
  typedef TAO_Pseudo_Out_T<Object> Object_out;
}

namespace TAO
{
  class Argument;

  /**
   * @class Direct_Collocation_Upcall_Wrapper
   *
   * @brief Wraps the activities direct collocation upcall
   *
   */
  class TAO_PortableServer_Export Direct_Collocation_Upcall_Wrapper
  {
  public:
    /// Perform the upcall
    void upcall (
      CORBA::Object_ptr obj,
      CORBA::Object_out forward_obj,
      bool & is_forwarded,
      TAO::Argument ** args,
      int num_args,
      const char * op,
      size_t op_len,
      TAO::Collocation_Strategy strategy);
  };

}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_DIRECT_COLLOCATION_UPCALL_WRAPPER_H */
