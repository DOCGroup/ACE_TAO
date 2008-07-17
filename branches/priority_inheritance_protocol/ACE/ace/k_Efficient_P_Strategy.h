// -*- C++ -*-

//=============================================================================
/**
 *  @file    k_Efficient_P_Strategy.h
 *
 *
 *
 *
 *
 *  @author Paul Oberlin <pauloberlin@gmail.com>
 */
//=============================================================================

#ifndef ACE_K_EFFICIENT_P_STRATEGY_H
#define ACE_K_EFFICIENT_P_STRATEGY_H

#include /**/ "ace/pre.h"

#include "ace/DA_Strategy_Base.h"
#include "ace/Mutex.h"
#include <vector>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template <typename AnnotationId>
class k_Efficient_P_Strategy : public DA_Strategy_Base<AnnotationId> {

  //The annotations consist of an identifier and a resource cost value

public:
    //note: k must be less than maxThreads
    k_Efficient_P_Strategy(int maxThreads, int k);
    virtual ~k_Efficient_P_Strategy();
    virtual int is_deadlock_potential(AnnotationId handle);
  virtual void grant(AnnotationId handle);
    virtual void release(AnnotationId upcall_handle);
private:
    int compute_min_illegal();
    int get_min_illegal();
    int min_illegal_;
    ACE_Mutex computation_mutex_;
    int k_;
    bool min_illegal_is_computed_ ;
    std::vector<int> a;
    std::vector<int> A;
};
//#if defined (__ACE_INLINE__)
//#include "ace/k_Efficient_P_Strategy.inl"
//#endif /* __ACE_INLINE__ */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename AnnotationId>
ACE_INLINE 
k_Efficient_P_Strategy<AnnotationId>::k_Efficient_P_Strategy(int maxThreads, int k)
:DA_Strategy_Base<AnnotationId>(maxThreads),
 k_(k)
  {
    a.resize(k_ + 1);
    A.resize(k_ + 1);
    for (int i=0; i<k_; ++i) {
      a[i] = 0;
      A[i] = 0;
    }
    min_illegal_ = maxThreads;
    min_illegal_is_computed_ = true;
}

template <typename AnnotationId>
ACE_INLINE 
k_Efficient_P_Strategy<AnnotationId>::~k_Efficient_P_Strategy()
{

}

template <typename AnnotationId>
ACE_INLINE 
int k_Efficient_P_Strategy<AnnotationId>::is_deadlock_potential(AnnotationId handle)
{
  int annotation = DA_Strategy_Base<AnnotationId>::get_annotation(handle);
  
  int min_illegal = get_min_illegal();
  if (annotation >= min_illegal)
  {
     return annotation - min_illegal + 1;
  }

  return 0;
}

template <typename AnnotationId>
ACE_INLINE
int
k_Efficient_P_Strategy<AnnotationId>::compute_min_illegal() 
{
  int T = this->get_max_threads();
  for (int i=0; i<k_; ++i) {
    if (!(A[i] < (T - i))) {
      return i;
    }
  }
  if (A[k_]>0) {
    return (T - A[k_]);
  } 
  return  T;
}

template <typename AnnotationId>
ACE_INLINE
int
k_Efficient_P_Strategy<AnnotationId>::get_min_illegal() 
{
  computation_mutex_.acquire();
  if (!min_illegal_is_computed_) {
	  min_illegal_ = compute_min_illegal();
	  min_illegal_is_computed_ = true;
  }
  computation_mutex_.release();
  return min_illegal_;
}

template <typename AnnotationId>
ACE_INLINE 
void k_Efficient_P_Strategy<AnnotationId>::grant(AnnotationId handle)
{
    int annotation = get_annotation(handle);
    computation_mutex_.acquire();
    if (annotation < k_) 
    {
      a[annotation] ++;
      for (int i=0; i<=annotation; ++i) 
      {
	      A[i]++;
      }
    } 
    else
    {
      a[k_] ++;
      for (int i=0; i<=k_ ; ++i)
      {
	      A[i]++;
      }
    }
    min_illegal_is_computed_ = false;
    computation_mutex_.release();
}

template <typename AnnotationId>
ACE_INLINE 
void k_Efficient_P_Strategy<AnnotationId>::release(AnnotationId handle)
{
    int annotation = get_annotation(handle);
    computation_mutex_.acquire();
/*    if (annotation < k ) {
      assert(a[annotation]>0);
    } else {
      assert(a[k] >0);
    }
*/
    if (annotation < k_)
    {
      a[annotation] --;
      for (int i=0; i<=annotation; ++i) 
      {
	      A[i]--;
      }
    } 
    else 
    {
      a[k_] --;
      for (int i=0; i<=k_ ; ++i) 
      {
	      A[i]--;
      }
    }
    min_illegal_is_computed_ = false;
    computation_mutex_.release();
}


ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* ACE_BASIC_P_STRATEGY_H */
