/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_FilterAdmin_i.h
//
// = DESCRIPTION
//
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_FILTERADMIN_H
#define NOTIFY_FILTERADMIN_H

#include "ace/Hash_Map_Manager.h"
#include "orbsvcs/orbsvcs/CosNotifyFilterS.h"
#include "orbsvcs/orbsvcs/Notify/ID_Pool_T.h"

class  TAO_Notify_FilterAdmin_i :
public virtual POA_CosNotifyFilter::FilterAdmin
{
  // = TITLE
  //   TAO_Notify_FilterAdmin_i
  // = DESCRIPTION
  //
  //

public:
  TAO_Notify_FilterAdmin_i (void);
  // Constructor

  virtual ~TAO_Notify_FilterAdmin_i (void);
  // Destructor

  // = match operation on all the filters
  CORBA::Boolean match (
    const CORBA::Any & filterable_data,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::UnsupportedFilterableData
  ));

  CORBA::Boolean match_structured (
    const CosNotification::StructuredEvent & filterable_data,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::UnsupportedFilterableData
  ));
  // see if any of the filters match.

  virtual CosNotifyFilter::FilterID add_filter (
    CosNotifyFilter::Filter_ptr new_filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void remove_filter (
    CosNotifyFilter::FilterID filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ));

  virtual CosNotifyFilter::Filter_ptr get_filter (
    CosNotifyFilter::FilterID filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ));

  virtual CosNotifyFilter::FilterIDSeq * get_all_filters (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void remove_all_filters (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

private:
  ACE_Hash_Map_Manager <CosNotifyFilter::FilterID,
                                         CosNotifyFilter::Filter_var,
                                                          ACE_SYNCH_MUTEX>
  filter_list_;
  // List of filters

  typedef
  ACE_Hash_Map_Iterator <CosNotifyFilter::FilterID,
                                          CosNotifyFilter::Filter_var,
                                                           ACE_SYNCH_MUTEX>
  FILTER_LIST_ITER;

  typedef ACE_Hash_Map_Entry<CosNotifyFilter::FilterID,
                                              CosNotifyFilter::Filter_var>
  FILTER_ENTRY;

  ID_Pool<CosNotifyFilter::FilterID> filter_ids_;
  // Id generator for proxy suppliers
};

#endif /* NOTIFY_FILTERADMIN_H */
