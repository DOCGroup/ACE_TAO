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
#include "ace/Hash_Map_Manager.h"
#include "ace/Thread_Mutex.h"
#include "ace/Atomic_Op_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Event_Handler;

template <typename AnnotationId>
class DA_Strategy_Base {

   public:

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


    DA_Strategy_Base(int maxThreads);
    virtual ~DA_Strategy_Base();

    virtual int is_deadlock_potential(AnnotationId handle)=0;
    virtual void grant(AnnotationId handle)=0;
    virtual void release(AnnotationId upcall_handle)=0;
    int get_max_threads() { return num_avail_threads_.value();}
    HASH_ANNOTATIONS_CONST_ITER get_annotations_iter() const;
    virtual int get_annotation (AnnotationId handle) const;
    virtual int add_annotation (AnnotationId handle, int annotation);
    virtual int remove_annotation (AnnotationId handle);
    virtual int set_annotations_table (const HASH_ANNOTATIONS_REVERSE_ITER& table);

private:
  HASH_ANNOTATIONS_MAP annotations_repo_;
  ACE_RW_Thread_Mutex lock_;
  ACE_Atomic_Op<ACE_Thread_Mutex, int> num_avail_threads_;

};

//#if defined (__ACE_INLINE__)
//#include "ace/DA_Strategy_Base.inl"
template <typename AnnotationId>
ACE_INLINE 
DA_Strategy_Base<AnnotationId>::DA_Strategy_Base (int maxThreads)
  :num_avail_threads_ (maxThreads)
{
}

template <typename AnnotationId>
ACE_INLINE
DA_Strategy_Base<AnnotationId>::~DA_Strategy_Base()
{
}

template <typename AnnotationId>
ACE_INLINE int 
DA_Strategy_Base<AnnotationId>::get_annotation (AnnotationId id) const
{
  int annotation;
  if (annotations_repo_.find (id, annotation) == -1)
    return -1;
  else return annotation;
}

template <typename AnnotationId>
ACE_INLINE int 
DA_Strategy_Base<AnnotationId>::set_annotations_table (
        const HASH_ANNOTATIONS_REVERSE_ITER& table)
{
  HASH_ANNOTATIONS_REVERSE_ITER iter(table);
  int rc=0;
  
  for (;!(iter.done()); iter++)
    {
      rc = annotations_repo_.bind((*iter).ext_id_, (*iter).int_id_);
      if (rc != 0) break;
    }

  return rc;
}

template <typename AnnotationId>
ACE_INLINE int 
DA_Strategy_Base<AnnotationId>::add_annotation (AnnotationId id, int annotation)
{
  int rc;
  if (annotation > num_avail_threads_.value()) {
    rc = -1;
    ACE_ERROR ((LM_ERROR,
               ACE_TEXT ("%p.\n"),
               ACE_TEXT ("DA_Strategy_Base annotation may not exceed number of threads")));
  } else {
    rc = annotations_repo_.bind (id, annotation);
  }
  /*  
    ACE_DEBUG ((LM_DEBUG, "In add_annotation\n"));
  HASH_ANNOTATIONS_CONST_ITER iter(annotations_repo_);
  for (;!(iter.done()); iter++)
    {
      ACE_DEBUG ((LM_DEBUG, "%d-%d\n", (*iter).ext_id_, (*iter).int_id_));
    }
  */
  return rc;
}

template <typename AnnotationId>
ACE_INLINE ACE_Hash_Map_Const_Iterator_Ex<AnnotationId,
                                       int,
                                       ACE_Hash<AnnotationId>,
                                       ACE_Equal_To<AnnotationId>,
                                       ACE_Thread_Mutex>
DA_Strategy_Base<AnnotationId>::get_annotations_iter() const
{
  
  return annotations_repo_.begin();
}

template <typename AnnotationId>
ACE_INLINE int  
DA_Strategy_Base<AnnotationId>::remove_annotation (AnnotationId id)
{
  return annotations_repo_.unbind (id);
}


//#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* DA_STRATEGY_BASE_H */

