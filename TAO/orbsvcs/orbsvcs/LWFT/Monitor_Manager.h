// -*- C++ -*-

//=============================================================================
/**
 * @file Monitor_Manager.h
 *
 * $Id$
 *
 * @brief An auto ptr type class for Monitor_Base and its
 *        subclasses, which are all reference counted
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MONITOR_MANAGER_H
#define MONITOR_MANAGER_H

#include /**/ "ace/pre.h"

#include "ace/Monitor_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    class Monitor_Manager
    {
    public:
      Monitor_Manager (void);
      Monitor_Manager (Monitor_Base *);
      Monitor_Manager (const Monitor_Manager &);
      ~Monitor_Manager (void);
      
      Monitor_Manager & operator= (Monitor_Base *);
      Monitor_Manager & operator= (const Monitor_Manager &);
      Monitor_Base * operator-> (void) const;
      
      Monitor_Base * get (void) const;
      Monitor_Base * release (void);
      
    private:
      void free (void);
      void reset (Monitor_Base *);
      
    private:
      Monitor_Base * ptr_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "Monitor_Manager.inl"
#endif /* __ACE_INLINE__ */

#endif // ACE_HAS_MONITOR_FRAMEWORK == 1

#include /**/ "ace/post.h"

#endif // MONITOR_MANAGER_H
