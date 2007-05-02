// -*- C++ -*-

//=============================================================================
/**
 *  @file    DA_Strategy_Base.h
 *
 *
 *
 *  The Deadlock Avoidance Strategy Base (DA_Strategy_Base) class
 *  is an abstract base class for Strategies that implement deadlock
 *  avoidance algorithms.  This class provides interfaces for passing
 *  annotations for call graph annotations, number of available threads, as well 
 *  as methods to determine whether a call is safe to make.
 *
 *
 *  @author Paul Oberlin <pauloberlin@gmail.com>
 */
//=============================================================================

#ifndef DA_STRATEGY_BASE_H
#define DA_STRATEGY_BASE_H

#include /**/ "ace/pre.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Thread_Mutex.h"
#include "ace/Atomic_Op_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Event_Handler;

template <typename AnnotationId>
class DA_Strategy_Base {

  //The annotations consist of an identifier and a resource cost value
typedef ACE_Hash_Map_Entry<ACE_Event_Handler *, int> HASH_EH_ENTRY;


typedef ACE_Hash_Map_Manager_Ex<AnnotationId,
                                int,
                                ACE_Hash<AnnotationId>,
                                ACE_Equal_To<AnnotationId>,
                                ACE_Thread_Mutex> HASH_ANNOTATIONS_MAP;

typedef ACE_Hash_Map_Iterator_Ex<AnnotationId,
                                 int,
                                 ACE_Hash<AnnotationId>,
                                 ACE_Equal_To<AnnotationId>,
                                 ACE_Thread_Mutex> HASH_ANNOTATIONS_ITER;

typedef ACE_Hash_Map_Const_Iterator_Ex<AnnotationId,
                                       int,
                                       ACE_Hash<AnnotationId>,
                                       ACE_Equal_To<AnnotationId>,
                                       ACE_Thread_Mutex> HASH_ANNOTATIONS_CONST_ITER;

typedef ACE_Hash_Map_Reverse_Iterator_Ex<AnnotationId,
                                         int,
                                         ACE_Hash<AnnotationId>,
                                         ACE_Equal_To<AnnotationId>,
                                         ACE_Thread_Mutex> HASH_ANNOTATIONS_REVERSE_ITER;

typedef HASH_ANNOTATIONS_MAP Annotations_Table;

public:
    DA_Strategy_Base(int maxThreads);
    virtual ~DA_Strategy_Base();
 
    virtual bool is_deadlock_potential(AnnotationId handle)=0;
    virtual void grant(AnnotationId handle)=0;
    virtual void release(AnnotationId upcall_handle)=0;
    int get_max_threads() { return num_avail_threads_};
    HASH_ANNOTATIONS_CONST_ITER get_annotations_iter();
    virtual int get_annotation (AnnotationId handle);
    virtual int add_annotation (AnnotationId handle, int annotation);
    virtual int remove_annotation (AnnotationId handle);
    virtual int set_annotations_table (const HASH_ANNOTATIONS_REVERSE_ITER& table);

private:
  HASH_ANNOTATIONS_MAP annotations_repo_;
  ACE_RW_Thread_Mutex lock_;
  ACE_Atomic_Op<ACE_Thread_Mutex, long> num_avail_threads_;

};

#include /**/ "ace/post.h"

#endif /* DA_STRATEGY_BASE_H */