//=============================================================================
/**
 *  @file   FilterAdmin.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef NOTIFY_FILTERADMIN_H
#define NOTIFY_FILTERADMIN_H
#include "ace/pre.h"

#include "orbsvcs/CosNotifyFilterS.h"
#include "ace/Hash_Map_Manager.h"
#include "ID_Factory.h"
#include "Event.h"
#include "notify_export.h"

/**
 * @class TAO_NS_FilterAdmin
 *
 * @brief FilterAdmin interface methods implementation.
 *
 */
class TAO_Notify_Export TAO_NS_FilterAdmin
{

 public:
  /// Constructor
  TAO_NS_FilterAdmin (void);

  /// Destructor
  virtual ~TAO_NS_FilterAdmin (void);

  // = match operation on all the filters
  /// See if any of the filters match.
  CORBA::Boolean match (const TAO_NS_Event_var &event ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosNotifyFilter::UnsupportedFilterableData
                     ));

  // = match operation on all the filters
  /// See if any of the filters match.
  CORBA::Boolean match (const TAO_NS_Event* event ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosNotifyFilter::UnsupportedFilterableData
                     ));

  virtual CosNotifyFilter::FilterID add_filter (CosNotifyFilter::Filter_ptr new_filter ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void remove_filter (CosNotifyFilter::FilterID filter ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosNotifyFilter::FilterNotFound
                     ));

  virtual CosNotifyFilter::Filter_ptr get_filter (CosNotifyFilter::FilterID filter ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosNotifyFilter::FilterNotFound
                     ));

  virtual CosNotifyFilter::FilterIDSeq * get_all_filters (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void remove_all_filters (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

 private:
  typedef ACE_Hash_Map_Manager <CosNotifyFilter::FilterID, CosNotifyFilter::Filter_var, ACE_SYNCH_NULL_MUTEX> FILTER_LIST;

  /// Mutex to serialize access to data members.
  TAO_SYNCH_MUTEX lock_;

  /// List of filters
  FILTER_LIST filter_list_;

  /// Id generator for proxy suppliers
  TAO_NS_ID_Factory filter_ids_;
};

#if defined (__ACE_INLINE__)
#include "FilterAdmin.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* NOTIFY_FILTERADMIN_H */
