// -*- C++ -*-

//=============================================================================
/**
 *  @file   rtcorbafwd.h
 *
 *  $Id$
 *
 * Forward declare the basic types used in the RT-CORBA namespace.
 * As with the CORBA namespace they are too many to just lump them
 * in the same file, and having a place where they are only forward
 * declared breaks cyclic dependencies.
 *
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_RTCORBAFWD_H
#define TAO_RTCORBAFWD_H
#include "ace/pre.h"

#include "tao/corbafwd.h"
#include "rtcorba_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Priority_Mapping;

class TAO_RT_Current;
class TAO_RT_Current_var;
class TAO_RT_Current_out;

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO TAO_RTCORBA_Export

#if defined (TAO_EXPORT_NESTED_CLASSES)
#  if defined (TAO_EXPORT_NESTED_MACRO)
#    undef TAO_EXPORT_NESTED_MACRO
#  endif /* defined (TAO_EXPORT_NESTED_MACRO) */
#  define TAO_EXPORT_NESTED_MACRO TAO_RTCORBA_Export
#endif /* TAO_EXPORT_NESTED_CLASSES */

TAO_NAMESPACE RTCORBA
{
  // = TITLE
  //   Provides the RTCORBA namespace.
  //
  // = DESCRIPTION
  //   The contents of the RTCORBA module are defined in
  //   orbos/99-02-12, the complete listing is in section 4.16
  //   Notice that the name of the namespace was changed from RT_CORBA
  //   to RTCORBA by the corrigendum in:
  //   orbos/99-03-29

  typedef CORBA::Short NativePriority;

  typedef CORBA::Short Priority;
  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Short minPriority;
  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Short maxPriority;

  typedef TAO_Priority_Mapping PriorityMapping;
}
TAO_NAMESPACE_CLOSE  // end of class (namespace) RTCORBA

#include "ace/post.h"
#endif /* TAO_RTCORBAFWD_H */
