//=============================================================================
/**
 *  @file   Notify_FilterAdmin_i.h
 *
 *  $Id$
 *
 *
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef NOTIFY_FILTERADMIN_H
#define NOTIFY_FILTERADMIN_H
#include "ace/pre.h"

#include "Notify_ID_Pool_T.h"
#include "Notify_Event.h"
#include "orbsvcs/CosNotifyFilterS.h"
#include "ace/Hash_Map_Manager.h"
#include "notify_export.h"

/**
 * @class TAO_Notify_FilterAdmin_i
 *
 * @brief TAO_Notify_FilterAdmin_i
 *
 */
class TAO_Notify_Export TAO_Notify_FilterAdmin_i
{

public:
  /// Constructor
  TAO_Notify_FilterAdmin_i (void);

  /// Destructor
  virtual ~TAO_Notify_FilterAdmin_i (void);

  // = match operation on all the filters
  /// See if any of the filters match.
  CORBA::Boolean match (TAO_Notify_Event &event ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosNotifyFilter::UnsupportedFilterableData
                     ));

  virtual CosNotifyFilter::FilterID add_filter (
    CosNotifyFilter::Filter_ptr new_filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void remove_filter (
    CosNotifyFilter::FilterID filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ));

  virtual CosNotifyFilter::Filter_ptr get_filter (
    CosNotifyFilter::FilterID filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ));

  virtual CosNotifyFilter::FilterIDSeq * get_all_filters (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void remove_all_filters (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

private:
  /// List of filters
  typedef ACE_Hash_Map_Manager <CosNotifyFilter::FilterID, CosNotifyFilter::Filter_var, TAO_SYNCH_MUTEX> FILTER_LIST;
  FILTER_LIST filter_list_;

  /// Id generator for proxy suppliers
  TAO_Notify_ID_Pool_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::FilterIDSeq> filter_ids_;
};

#include "ace/post.h"
#endif /* NOTIFY_FILTERADMIN_H */
