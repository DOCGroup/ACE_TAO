// -*- C++ -*-
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

#include /**/ "ace/pre.h"

#include "orbsvcs/CosNotifyFilterS.h"

#include "ace/Hash_Map_Manager.h"

#include "orbsvcs/Notify/ID_Factory.h"
#include "orbsvcs/Notify/Event.h"
#include "orbsvcs/Notify/notify_serv_export.h"
#include "orbsvcs/Notify/Topology_Object.h"
#include "orbsvcs/Notify/EventChannel.h"

class TAO_Notify_EventChannel;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_FilterAdmin
 *
 * @brief FilterAdmin interface methods implementation.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_FilterAdmin
  : public TAO_Notify::Topology_Object
{

 public:
  /// Constructor
  TAO_Notify_FilterAdmin (void);

  /// Destructor
  virtual ~TAO_Notify_FilterAdmin (void);

  // = match operation on all the filters
  /// See if any of the filters match.
  CORBA::Boolean match (const TAO_Notify_Event::Ptr &event);

  // = match operation on all the filters
  /// See if any of the filters match.
  CORBA::Boolean match (const TAO_Notify_Event* event);

  virtual CosNotifyFilter::FilterID add_filter (CosNotifyFilter::Filter_ptr new_filter);

  virtual void remove_filter (CosNotifyFilter::FilterID filter);

  virtual CosNotifyFilter::Filter_ptr get_filter (CosNotifyFilter::FilterID filter);

  virtual CosNotifyFilter::FilterIDSeq * get_all_filters (void);

  virtual void remove_all_filters (void);


  // TAO_Notify::Topology_Object

  virtual void save_persistent (TAO_Notify::Topology_Saver& saver);
  virtual TAO_Notify::Topology_Object* load_child (const ACE_CString &type, CORBA::Long id,
    const TAO_Notify::NVPList& attrs);

  void event_channel (TAO_Notify_EventChannel* ec);

 private:
  typedef ACE_Hash_Map_Manager <CosNotifyFilter::FilterID, CosNotifyFilter::Filter_var, ACE_SYNCH_NULL_MUTEX> FILTER_LIST;

  virtual void release (void);

  /// Mutex to serialize access to data members.
  TAO_SYNCH_MUTEX lock_;

  /// List of filters
  FILTER_LIST filter_list_;

  /// Id generator for proxy suppliers
  TAO_Notify_ID_Factory filter_ids_;

  TAO_Notify_EventChannel::Ptr ec_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/FilterAdmin.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* NOTIFY_FILTERADMIN_H */
