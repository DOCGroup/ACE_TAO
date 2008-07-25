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

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename AnnotationId>
class Basic_P_Strategy : public DA_Strategy_Base<AnnotationId> {

  //The annotations consist of an identifier and a resource cost value

public:
    Basic_P_Strategy(int maxThreads);
    virtual ~Basic_P_Strategy();
    virtual int is_deadlock_potential(AnnotationId handle);
    virtual void grant(AnnotationId handle);
    virtual void release(AnnotationId upcall_handle);
private:
    int t_r;
};

ACE_END_VERSIONED_NAMESPACE_DECL

//#if defined (__ACE_INLINE__)
//#include "ace/Basic_P_Strategy.inl"
//#endif /* __ACE_INLINE__ */


template <typename AnnotationId>
ACE_INLINE 
Basic_P_Strategy<AnnotationId>::Basic_P_Strategy(int maxThreads)
:DA_Strategy_Base<AnnotationId>(maxThreads),
 t_r(maxThreads)
{
}

template <typename AnnotationId>
ACE_INLINE 
Basic_P_Strategy<AnnotationId>::~Basic_P_Strategy()
{

}

template <typename AnnotationId>
ACE_INLINE 
int Basic_P_Strategy<AnnotationId>::is_deadlock_potential(AnnotationId handle)
{
  int annotation = get_annotation(handle);
  if (annotation > t_r)
  {
     return annotation - t_r;
  }

  return 0;
}

template <typename AnnotationId>
ACE_INLINE 
void Basic_P_Strategy<AnnotationId>::grant(AnnotationId handle)
{
  --t_r;
}

template <typename AnnotationId>
ACE_INLINE 
void Basic_P_Strategy<AnnotationId>::release(AnnotationId upcall_handle)
{
  ++t_r;
}



#include /**/ "ace/post.h"

#endif /* ACE_BASIC_P_STRATEGY_H */

