/* -*- C++ -*- */
/**
 *  @file Priority_Mapping.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_PRIORITY_MAPPING_H
#define TAO_NS_PRIORITY_MAPPING_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"

/**
 * @class TAO_NS_Priority_Mapping
 *
 * @brief A default Priority mapping. simply returns the same number.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_NS_Priority_Mapping
{
public:
  /// The destructor
  virtual ~TAO_NS_Priority_Mapping (void);

  virtual CORBA::Boolean
      to_native (CORBA::Short corba_priority,
                 CORBA::Short &native_priority);
  virtual CORBA::Boolean
      to_CORBA (CORBA::Short native_priority,
                CORBA::Short &corba_priority);
};

#include "ace/post.h"
#endif /* TAO_NS_PRIORITY_MAPPING_H */
