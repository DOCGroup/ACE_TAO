
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef TAO_PORTABLESERVER_EXPORT_H
#define TAO_PORTABLESERVER_EXPORT_H

#include "ace/config-all.h"

#include "tao/Objref_VarOut_T.h"

#if defined (TAO_AS_STATIC_LIBS)
#  if !defined (TAO_PORTABLESERVER_HAS_DLL)
#    define TAO_PORTABLESERVER_HAS_DLL 0
#  endif /* ! TAO_PORTABLESERVER_HAS_DLL */
#else
#  if !defined (TAO_PORTABLESERVER_HAS_DLL)
#    define TAO_PORTABLESERVER_HAS_DLL 1
#  endif /* ! TAO_PORTABLESERVER_HAS_DLL */
#endif

#if defined (TAO_PORTABLESERVER_HAS_DLL) && (TAO_PORTABLESERVER_HAS_DLL == 1)
#  if defined (TAO_PORTABLESERVER_BUILD_DLL)
#    define TAO_PortableServer_Export ACE_Proper_Export_Flag
#    define TAO_PORTABLESERVER_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TAO_PORTABLESERVER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TAO_PORTABLESERVER_BUILD_DLL */
#    define TAO_PortableServer_Export ACE_Proper_Import_Flag
#    define TAO_PORTABLESERVER_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TAO_PORTABLESERVER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TAO_PORTABLESERVER_BUILD_DLL */
#else /* TAO_PORTABLESERVER_HAS_DLL == 1 */
#  define TAO_PortableServer_Export
#  define TAO_PORTABLESERVER_SINGLETON_DECLARATION(T)
#  define TAO_PORTABLESERVER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TAO_PORTABLESERVER_HAS_DLL == 1 */

// Declared and exported so it can be seen by RTPortableServer.
namespace PortableServer
{
  class LifespanPolicy;
  typedef LifespanPolicy *LifespanPolicy_ptr;
}

namespace TAO
{
  ACE_TEMPLATE_SPECIALIZATION
  struct TAO_PortableServer_Export Objref_Traits<PortableServer::LifespanPolicy>
  {
    static PortableServer::LifespanPolicy_ptr tao_duplicate (
        PortableServer::LifespanPolicy_ptr
      );
    static void tao_release (PortableServer::LifespanPolicy_ptr);
    static PortableServer::LifespanPolicy_ptr tao_nil (void);
    static CORBA::Boolean tao_marshal (PortableServer::LifespanPolicy_ptr p,
                                       TAO_OutputCDR & cdr);
  };
}

#endif /* TAO_PORTABLESERVER_EXPORT_H */

// End of auto generated file.
