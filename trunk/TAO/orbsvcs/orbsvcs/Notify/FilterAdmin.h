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

#include "ID_Factory.h"
#include "Event.h"
#include "notify_serv_export.h"
#include "Topology_Object.h"

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
  CORBA::Boolean match (const TAO_Notify_Event_var &event ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosNotifyFilter::UnsupportedFilterableData
                     ));

  // = match operation on all the filters
  /// See if any of the filters match.
  CORBA::Boolean match (const TAO_Notify_Event* event ACE_ENV_ARG_DECL)
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


  // TAO_Notify::Topology_Object

  virtual void save_persistent (TAO_Notify::Topology_Saver& saver ACE_ENV_ARG_DECL);
  virtual TAO_Notify::Topology_Object* load_child (const ACE_CString &type, CORBA::Long id,
    const TAO_Notify::NVPList& attrs ACE_ENV_ARG_DECL);
  virtual void release (void);
 private:
  typedef ACE_Hash_Map_Manager <CosNotifyFilter::FilterID, CosNotifyFilter::Filter_var, ACE_SYNCH_NULL_MUTEX> FILTER_LIST;

  /// Mutex to serialize access to data members.
  TAO_SYNCH_MUTEX lock_;

  /// List of filters
  FILTER_LIST filter_list_;

  /// Id generator for proxy suppliers
  TAO_Notify_ID_Factory filter_ids_;
};

#if defined (__ACE_INLINE__)
#include "FilterAdmin.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* NOTIFY_FILTERADMIN_H */
