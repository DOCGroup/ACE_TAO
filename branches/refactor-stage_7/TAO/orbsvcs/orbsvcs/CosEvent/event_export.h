
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef TAO_EVENT_EXPORT_H
#define TAO_EVENT_EXPORT_H

#include "ace/config-all.h"

#if defined (TAO_AS_STATIC_LIBS)
#  if !defined (TAO_EVENT_HAS_DLL)
#    define TAO_EVENT_HAS_DLL 0
#  endif /* ! TAO_EVENT_HAS_DLL */
#else
#  if !defined (TAO_EVENT_HAS_DLL)
#    define TAO_EVENT_HAS_DLL 1
#  endif /* ! TAO_EVENT_HAS_DLL */
#endif

#if defined (TAO_EVENT_HAS_DLL) && (TAO_EVENT_HAS_DLL == 1)
#  if defined (TAO_EVENT_BUILD_DLL)
#    define TAO_Event_Export ACE_Proper_Export_Flag
#    define TAO_EVENT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TAO_EVENT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TAO_EVENT_BUILD_DLL */
#    define TAO_Event_Export ACE_Proper_Import_Flag
#    define TAO_EVENT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TAO_EVENT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TAO_EVENT_BUILD_DLL */
#else /* TAO_EVENT_HAS_DLL == 1 */
#  define TAO_Event_Export
#  define TAO_EVENT_SINGLETON_DECLARATION(T)
#  define TAO_EVENT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TAO_EVENT_HAS_DLL == 1 */

// Export decl of Objref_Traits<> specializations needed by other libs.
namespace CosEventChannelAdmin
{
  class ProxyPushSupplier;
  typedef ProxyPushSupplier *ProxyPushSupplier_ptr;
  class ProxyPushConsumer;
  typedef ProxyPushConsumer *ProxyPushConsumer_ptr;
}

namespace CosEventComm
{
  class PushSupplier;
  typedef PushSupplier *PushSupplier_ptr;
  class PushConsumer;
  typedef PushConsumer *PushConsumer_ptr;
  class PullSupplier;
  typedef PullSupplier *PullSupplier_ptr;
  class PullConsumer;
  typedef PullConsumer *PullConsumer_ptr;
}

namespace TAO
{
  ACE_TEMPLATE_SPECIALIZATION
  struct TAO_Event_Export Objref_Traits<CosEventChannelAdmin::ProxyPushSupplier>
  {
    static CosEventChannelAdmin::ProxyPushSupplier_ptr tao_duplicate (
        CosEventChannelAdmin::ProxyPushSupplier_ptr
      );
    static void tao_release (CosEventChannelAdmin::ProxyPushSupplier_ptr);
    static CosEventChannelAdmin::ProxyPushSupplier_ptr tao_nil (void);
    static CORBA::Boolean tao_marshal (
        CosEventChannelAdmin::ProxyPushSupplier_ptr p,
        TAO_OutputCDR & cdr
      );
  };

  ACE_TEMPLATE_SPECIALIZATION
  struct TAO_Event_Export Objref_Traits<CosEventChannelAdmin::ProxyPushConsumer>
  {
    static CosEventChannelAdmin::ProxyPushConsumer_ptr tao_duplicate (
        CosEventChannelAdmin::ProxyPushConsumer_ptr
      );
    static void tao_release (CosEventChannelAdmin::ProxyPushConsumer_ptr);
    static CosEventChannelAdmin::ProxyPushConsumer_ptr tao_nil (void);
    static CORBA::Boolean tao_marshal (
        CosEventChannelAdmin::ProxyPushConsumer_ptr p,
        TAO_OutputCDR & cdr
      );
  };

  ACE_TEMPLATE_SPECIALIZATION
  struct TAO_Event_Export Objref_Traits<CosEventComm::PushSupplier>
  {
    static CosEventComm::PushSupplier_ptr tao_duplicate (
        CosEventComm::PushSupplier_ptr
      );
    static void tao_release (CosEventComm::PushSupplier_ptr);
    static CosEventComm::PushSupplier_ptr tao_nil (void);
    static CORBA::Boolean tao_marshal (
        CosEventComm::PushSupplier_ptr p,
        TAO_OutputCDR & cdr
      );
  };

  ACE_TEMPLATE_SPECIALIZATION
  struct TAO_Event_Export Objref_Traits<CosEventComm::PushConsumer>
  {
    static CosEventComm::PushConsumer_ptr tao_duplicate (
        CosEventComm::PushConsumer_ptr
      );
    static void tao_release (CosEventComm::PushConsumer_ptr);
    static CosEventComm::PushConsumer_ptr tao_nil (void);
    static CORBA::Boolean tao_marshal (
        CosEventComm::PushConsumer_ptr p,
        TAO_OutputCDR & cdr
      );
  };

  ACE_TEMPLATE_SPECIALIZATION
  struct TAO_Event_Export Objref_Traits<CosEventComm::PullSupplier>
  {
    static CosEventComm::PullSupplier_ptr tao_duplicate (
        CosEventComm::PullSupplier_ptr
      );
    static void tao_release (CosEventComm::PullSupplier_ptr);
    static CosEventComm::PullSupplier_ptr tao_nil (void);
    static CORBA::Boolean tao_marshal (
        CosEventComm::PullSupplier_ptr p,
        TAO_OutputCDR & cdr
      );
  };

  ACE_TEMPLATE_SPECIALIZATION
  struct TAO_Event_Export Objref_Traits<CosEventComm::PullConsumer>
  {
    static CosEventComm::PullConsumer_ptr tao_duplicate (
        CosEventComm::PullConsumer_ptr
      );
    static void tao_release (CosEventComm::PullConsumer_ptr);
    static CosEventComm::PullConsumer_ptr tao_nil (void);
    static CORBA::Boolean tao_marshal (
        CosEventComm::PullConsumer_ptr p,
        TAO_OutputCDR & cdr
      );
  };
}

#endif /* TAO_EVENT_EXPORT_H */

// End of auto generated file.
