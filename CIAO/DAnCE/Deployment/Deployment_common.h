// $Id$

/**
 * @file Deployment_common.h
 *
 * This file collects common type definitions used across different
 * objects of DAnCE toolchain.
 *
 * @author Gan Deng <dengg@dre.vanderbilt.edu>
 */

#ifndef DANCE_COMMON_H
#define DANCE_COMMON_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"
#include "ace/Hash_Map_Manager_T.h"
#include "tao/AnyTypeCode/Any.h"

#if !defined ACE_LACKS_PRAGMA_ONCE
#pragma once
#endif /* ! ACE_LACKS_PRAGMA_ONCE */

/// A struct which captures the binding information about a component

namespace DAnCE
{
  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  CORBA::Any,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex> PROPERTY_MAP;
}

#include /**/ "ace/post.h"
#endif /* DANCE_COMMON_H */
