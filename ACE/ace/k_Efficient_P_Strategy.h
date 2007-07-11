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
    virtual bool is_deadlock_potential(AnnotationId handle);
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
#if defined (__ACE_INLINE__)
#include "ace/k_Efficient_P_Strategy.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* ACE_BASIC_P_STRATEGY_H */