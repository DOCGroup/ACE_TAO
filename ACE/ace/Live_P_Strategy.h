// -*- C++ -*-

//=============================================================================
/**
 *  @file    Live_P_Strategy.h
 *
 *
 *
 *
 *
 *  @author Paul Oberlin <pauloberlin@gmail.com>
 */
//=============================================================================

#ifndef ACE_LIVE_P_STRATEGY_H
#define ACE_LIVE_P_STRATEGY_H

#include /**/ "ace/pre.h"

#include "ace/DA_Strategy_Base.h"
#include "ace/RB_Tree.h"
#include "ace/Mutex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//forward decl
class Live_P_Tree;

template <typename AnnotationId>
class Live_P_Strategy : public DA_Strategy_Base<AnnotationId> {

  //The annotations consist of an identifier and a resource cost value

public:
    Live_P_Strategy(int maxThreads);
    virtual ~Live_P_Strategy();
  virtual int is_deadlock_potential(AnnotationId handle);
    virtual void grant(AnnotationId handle);
    virtual void release(AnnotationId upcall_handle);
private:
   Live_P_Tree* tree_pimpl_; 
   bool min_illegal_is_computed_;
   int min_illegal_;
   ACE_Mutex computation_mutex_;
  
};
#if defined (__ACE_INLINE__)
#include "ace/Live_P_Strategy.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* ACE_LIVE_P_STRATEGY_H */
