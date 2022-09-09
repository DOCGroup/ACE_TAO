/* -*- C++ -*- */
/**
 *  @file ETCL_FilterFactory.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_ETCL_FILTERFACTORY_H
#define TAO_Notify_ETCL_FILTERFACTORY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyFilterExtS.h"

#include "orbsvcs/Notify/FilterFactory.h"
#include "orbsvcs/Notify/ID_Factory.h"
#include "orbsvcs/Notify/ETCL_Filter.h"
#include "orbsvcs/Notify/Topology_Saver.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_ETCL_FilterFactory
 *
 * @brief
 */
class TAO_Notify_Serv_Export TAO_Notify_ETCL_FilterFactory :
  public virtual POA_CosNotifyFilterExt::FilterFactory,
  public virtual TAO_Notify_FilterFactory
{
public:
  /// Constructor
  TAO_Notify_ETCL_FilterFactory (void);

  /// Destructor
  ~TAO_Notify_ETCL_FilterFactory () override;

  ///= TAO_Notify_FilterFactory methods.

  CosNotifyFilter::FilterFactory_ptr create (
      PortableServer::POA_ptr filter_poa) override;

  void destroy (void) override;

  ///= CosNotifyFilter::FilterFactory methods

  CosNotifyFilter::Filter_ptr create_filter (
      const char * constraint_grammar) override;

  CosNotifyFilter::MappingFilter_ptr create_mapping_filter (
      const char * constraint_grammar,
      const CORBA::Any & default_value) override;

  void remove_filter (
      CosNotifyFilter::Filter_ptr filter) override;


  void save_persistent (TAO_Notify::Topology_Saver& saver) override;

  TAO_Notify::Topology_Object* load_child (
          const ACE_CString &type,
          CORBA::Long id,
          const TAO_Notify::NVPList& attrs) override;

  TAO_Notify_Object::ID get_filter_id (CosNotifyFilter::Filter_ptr filter) override;
  CosNotifyFilter::Filter_ptr get_filter (const TAO_Notify_Object::ID& id) override;

  CosNotifyFilter::FilterID get_filterid (CosNotifyFilter::Filter_ptr filter) override;
  CosNotifyFilter::Filter_ptr get_filter (CosNotifyFilter::FilterID id) override;


protected:
  CosNotifyFilter::Filter_ptr
    create_filter (const char *constraint_grammar,
                   const TAO_Notify_Object::ID& id,
                   TAO_Notify_ETCL_Filter*& filter);

  TAO_Notify_Object::ID find_filter_id (CosNotifyFilter::Filter_ptr filter);
  CosNotifyFilter::Filter_ptr find_filter (const TAO_Notify_Object::ID& id);

  /// Release this object.
  void release (void) override;

  /// The POA in which to activate the Filters.
  PortableServer::POA_var filter_poa_;

  /// Id generator for proxy suppliers
  TAO_Notify_ID_Factory filter_ids_;

  typedef ACE_Hash_Map_Manager <TAO_Notify_Object::ID,
                                TAO_Notify_ETCL_Filter*,
                                TAO_SYNCH_MUTEX>  FILTERMAP;

  FILTERMAP filters_;
  TAO_SYNCH_MUTEX mtx_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

ACE_FACTORY_DECLARE (TAO_Notify_Serv, TAO_Notify_ETCL_FilterFactory)

#include /**/ "ace/post.h"

#endif /* TAO_Notify_ETCL_FILTERFACTORY_H */
