// -*- C++ -*-

//=============================================================================
/**
 *  @file    Basic_P_Strategy.h
 *
 *
 *
 *
 *
 *  @author Paul Oberlin <pauloberlin@gmail.com>
 */
//=============================================================================

#ifndef ACE_BASIC_P_STRATEGY_H
#define ACE_BASIC_P_STRATEGY_H

#include /**/ "ace/pre.h"

#include "ace/DA_Strategy_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template <typename AnnotationId>
class Basic_P_Strategy : public DA_Strategy_Base<AnnotationId> {

  //The annotations consist of an identifier and a resource cost value

public:
    Basic_P_Strategy(int maxThreads);
    virtual ~Basic_P_Strategy();
    virtual bool is_deadlock_potential(AnnotationId handle);
    virtual void grant(AnnotationId handle);
    virtual void release(AnnotationId upcall_handle);
private:
    int t_r;
};
#if defined (__ACE_INLINE__)
#include "ace/Basic_P_Strategy.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* ACE_BASIC_P_STRATEGY_H */