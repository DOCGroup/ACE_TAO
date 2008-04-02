// $Id$

/**
 * @file Server_init.h
 *
 * Initializing CIAO Server side ORB, if they need to.
 * I'm also puting some utilities functions here.
 *
 * @author Nanbor Wang <nanbor@cs.wustl.edu>
 * @author Gan Deng <dengg@dre.vanderbilt.edu>
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 */

#ifndef CIAO_SERVER_INIT_H
#define CIAO_SERVER_INIT_H

#include /**/ "ace/pre.h"

#include "CIAO_Server_Export.h"

#if !defined ACE_LACKS_PRAGMA_ONCE
#pragma once
#endif /* ! ACE_LACKS_PRAGMA_ONCE */

#include <ace/SString.h>
#include <orbsvcs/CosNamingC.h>
#include <ccm/CCM_StandardConfiguratorC.h>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
namespace CORBA
{
  class ORB;
  typedef ORB *ORB_ptr;
}
TAO_END_VERSIONED_NAMESPACE_DECL

namespace CIAO
{
  /**
   * The initialize routine for any server that need to access
   * component.  This routine now simply registers various valuetype
   * factories defined in CIAO_Component.pidl.  Currently, this method
   * should be call right after ORB initialization but we should try
   * to register these stuff automatically.
   */
  CIAO_SERVER_Export int Server_init (CORBA::ORB_ptr o);

  namespace Utility
  {
    /// Write a string (usually a stringified IOR) to a file
    /// designated by the @c pathname.  The file named will always get
    /// overwritten.
    CIAO_SERVER_Export int write_IOR (const char *pathname,
                                      const char *IOR);
    
    class CIAO_SERVER_Export NameUtility
    {
      /**
       * A utility class to bind naming context. This class is contributed by
       * Dipa Suri <dipa.suri@lmco.com>.
       *
       * The concept/code are derived from
       * http://www.informit.com/articles/article.asp?p=23266&seqNum=6
       */
    
    public:
      static bool bind_name (const char *namestr, 
                             CORBA::Object_ptr obj,
                             CosNaming::NamingContextExt_var &root);
    
      static void create_name (const char *namestr, CosNaming::Name &);

      /// For each 'NameComponent' in 'name', create a corresponding 'NamingContext'.
      static  void create_context_path (const CosNaming::NamingContextExt_ptr,
                                        const CosNaming::Name &);

      /// For the first [0, length-2] NameComponents of 'name', create a
      /// corresponding 'NamingContext'.
      /// For the length-1 NameComponent of 'name', bind it to the object
      /// reference 'obj'.
      static  bool bind_object_path (const CosNaming::NamingContextExt_ptr,
                                     const CosNaming::Name&,
                                     const CORBA::Object_ptr);

      /// Get a list of all bindings under the given context, up to
      /// max_list_size.
      static CosNaming::BindingList * list_bindings (const CosNaming::NamingContext_ptr,
                                                     const CosNaming::Name&,
                                                     CORBA::ULong);

      /// Recursively unbind all objects and contexts below the given name
      /// context, given the initial context nc.
      /// Assumes a tree shape to service (not cyclic)
      static void recursive_unbind (const CosNaming::NamingContext_ptr,
                                    const CosNaming::Name&);
      
    };
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_SERVER_INIT_H */
