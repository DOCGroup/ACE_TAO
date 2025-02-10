// -*- C++ -*-

//=============================================================================
/**
 *  @file    Servant_Location.h
 *
 *  @author Irfan Pyarali
 */
//=============================================================================

#ifndef TAO_Servant_Location_H
#define TAO_Servant_Location_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#include /**/ "tao/Versioned_Namespace.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

enum class TAO_Servant_Location
{
  Found,
  Default_Servant,
  Servant_Manager,
  Not_Found
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_Servant_Location_H */
