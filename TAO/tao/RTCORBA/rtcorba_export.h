
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef TAO_RTCORBA_EXPORT_H
#define TAO_RTCORBA_EXPORT_H

#include "ace/config-all.h"

#include "tao/Objref_VarOut_T.h"

#if defined (TAO_AS_STATIC_LIBS)
#  if !defined (TAO_RTCORBA_HAS_DLL)
#    define TAO_RTCORBA_HAS_DLL 0
#  endif /* ! TAO_RTCORBA_HAS_DLL */
#else
#  if !defined (TAO_RTCORBA_HAS_DLL)
#    define TAO_RTCORBA_HAS_DLL 1
#  endif /* ! TAO_RTCORBA_HAS_DLL */
#endif

#if defined (TAO_RTCORBA_HAS_DLL) && (TAO_RTCORBA_HAS_DLL == 1)
#  if defined (TAO_RTCORBA_BUILD_DLL)
#    define TAO_RTCORBA_Export ACE_Proper_Export_Flag
#    define TAO_RTCORBA_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TAO_RTCORBA_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TAO_RTCORBA_BUILD_DLL */
#    define TAO_RTCORBA_Export ACE_Proper_Import_Flag
#    define TAO_RTCORBA_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TAO_RTCORBA_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TAO_RTCORBA_BUILD_DLL */
#else /* TAO_RTCORBA_HAS_DLL == 1 */
#  define TAO_RTCORBA_Export
#  define TAO_RTCORBA_SINGLETON_DECLARATION(T)
#  define TAO_RTCORBA_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TAO_RTCORBA_HAS_DLL == 1 */

// Declared and exported so it can be seen by RTPortableServer.
namespace RTCORBA
{
  class PriorityModelPolicy;
  typedef PriorityModelPolicy *PriorityModelPolicy_ptr;

  class PriorityBandedConnectionPolicy;
  typedef PriorityBandedConnectionPolicy *PriorityBandedConnectionPolicy_ptr;

  class ServerProtocolPolicy;
  typedef ServerProtocolPolicy *ServerProtocolPolicy_ptr;

  class RTORB;
  typedef RTORB *RTORB_ptr;

  class ThreadpoolPolicy;
  typedef ThreadpoolPolicy *ThreadpoolPolicy_ptr;
}

namespace TAO
{
  ACE_TEMPLATE_SPECIALIZATION
  struct TAO_RTCORBA_Export Objref_Traits<RTCORBA::PriorityModelPolicy>
  {
    static RTCORBA::PriorityModelPolicy_ptr tao_duplicate (
        RTCORBA::PriorityModelPolicy_ptr
      );
    static void tao_release (RTCORBA::PriorityModelPolicy_ptr);
    static RTCORBA::PriorityModelPolicy_ptr tao_nil (void);
    static CORBA::Boolean tao_marshal (RTCORBA::PriorityModelPolicy_ptr p,
                                       TAO_OutputCDR & cdr);
  };

  ACE_TEMPLATE_SPECIALIZATION
  struct TAO_RTCORBA_Export Objref_Traits<RTCORBA::PriorityBandedConnectionPolicy>
  {
    static RTCORBA::PriorityBandedConnectionPolicy_ptr tao_duplicate (
        RTCORBA::PriorityBandedConnectionPolicy_ptr
      );
    static void tao_release (RTCORBA::PriorityBandedConnectionPolicy_ptr);
    static RTCORBA::PriorityBandedConnectionPolicy_ptr tao_nil (void);
    static CORBA::Boolean tao_marshal (RTCORBA::PriorityBandedConnectionPolicy_ptr p,
                                       TAO_OutputCDR & cdr);
  };

  ACE_TEMPLATE_SPECIALIZATION
  struct TAO_RTCORBA_Export Objref_Traits<RTCORBA::ServerProtocolPolicy>
  {
    static RTCORBA::ServerProtocolPolicy_ptr tao_duplicate (
        RTCORBA::ServerProtocolPolicy_ptr
      );
    static void tao_release (RTCORBA::ServerProtocolPolicy_ptr);
    static RTCORBA::ServerProtocolPolicy_ptr tao_nil (void);
    static CORBA::Boolean tao_marshal (RTCORBA::ServerProtocolPolicy_ptr p,
                                       TAO_OutputCDR & cdr);
  };

  ACE_TEMPLATE_SPECIALIZATION
  struct TAO_RTCORBA_Export Objref_Traits<RTCORBA::RTORB>
  {
    static RTCORBA::RTORB_ptr tao_duplicate (RTCORBA::RTORB_ptr);
    static void tao_release (RTCORBA::RTORB_ptr);
    static RTCORBA::RTORB_ptr tao_nil (void);
    static CORBA::Boolean tao_marshal (RTCORBA::RTORB_ptr p,
                                       TAO_OutputCDR & cdr);
  };

  ACE_TEMPLATE_SPECIALIZATION
  struct TAO_RTCORBA_Export Objref_Traits<RTCORBA::ThreadpoolPolicy>
  {
    static RTCORBA::ThreadpoolPolicy_ptr tao_duplicate (
        RTCORBA::ThreadpoolPolicy_ptr
      );
    static void tao_release (RTCORBA::ThreadpoolPolicy_ptr);
    static RTCORBA::ThreadpoolPolicy_ptr tao_nil (void);
    static CORBA::Boolean tao_marshal (RTCORBA::ThreadpoolPolicy_ptr p,
                                       TAO_OutputCDR & cdr);
  };
}

#endif /* TAO_RTCORBA_EXPORT_H */

// End of auto generated file.
