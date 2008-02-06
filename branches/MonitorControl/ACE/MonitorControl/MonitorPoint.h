// -*- C++ -*-

//=============================================================================
/**
 * @file MonitorPoint.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MONITORPOINT_H
#define MONITORPOINT_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "MonitorControl/MonitorPoint_T.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    /// This specialization is instantiated when the monitor point is to
    /// enabled. ACE logging may be integrated with some of the operation
    /// implementatoins, but there isn't any need to provide an additional
    /// or wrapped interface to control it, for example to redirect the
    /// output, since the existing interface in ACE is already as simple
    /// as possible. However, if we want to be able to control logging
    /// options for individual monitor points, some interface will have to
    /// be provided. Addition of such fine-grained control is doubtful,
    /// since it would make the monitor points much more heavyweight.
    template<>
    class MONITORCONTROL_Export MonitorPoint<true> : public Monitor_Base
    {
      /// Meaningful implementations of these methods in the enabled
      /// version of the monitor point. Note that the virtual method
      /// update() isn't implemented here, but in the class that
      /// inherits from this one.
      
    public:
      /// Meaningful implementation of member function.
      virtual void group_parent (const Monitor_Base* parent);
      
      /// Meaningful implementation of member function.
      virtual void add_group_child (const Monitor_Base* child);
      
      /// Meaningful implementation of member function.
      virtual void constraint (const ETCL_Constraint* constraint);
      
      /// Meaningful implementation of member function.
      virtual void receive (const double data);
      
      /// Meaningful implementation of member function.
      virtual void clear (void);
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // MONITORPOINT_H
