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
#include "ace/pre.h"

#include "Notify_ID_Pool_T.h"
#include "Notify_Event.h"
#include "orbsvcs/CosNotifyFilterS.h"
#include "ace/Hash_Map_Manager.h"
#include "notify_export.h"

class TAO_Notify_Export TAO_Notify_FilterAdmin_i
{
  // = TITLE
  //   TAO_Notify_FilterAdmin_i
  //
  // = DESCRIPTION
  //
  //

public:
  TAO_Notify_FilterAdmin_i (void);
  // Constructor

  virtual ~TAO_Notify_FilterAdmin_i (void);
  // Destructor

  // = match operation on all the filters
  CORBA::Boolean match (const TAO_Notify_Event &event TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosNotifyFilter::UnsupportedFilterableData
                     ));
  // see if any of the filters match.

  virtual CosNotifyFilter::FilterID add_filter (
    CosNotifyFilter::Filter_ptr new_filter
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void remove_filter (
    CosNotifyFilter::FilterID filter
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ));

  virtual CosNotifyFilter::Filter_ptr get_filter (
    CosNotifyFilter::FilterID filter
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ));

  virtual CosNotifyFilter::FilterIDSeq * get_all_filters (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void remove_all_filters (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

private:
  typedef ACE_Hash_Map_Manager <CosNotifyFilter::FilterID, CosNotifyFilter::Filter_var, TAO_SYNCH_MUTEX> FILTER_LIST;
  FILTER_LIST filter_list_;
  // List of filters

  TAO_Notify_ID_Pool_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::FilterIDSeq> filter_ids_;
  // Id generator for proxy suppliers
};

#include "ace/post.h"
#endif /* NOTIFY_FILTERADMIN_H */
