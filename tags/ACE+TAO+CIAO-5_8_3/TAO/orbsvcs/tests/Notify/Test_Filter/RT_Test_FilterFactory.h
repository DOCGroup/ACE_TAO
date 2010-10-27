/* -*- C++ -*- */
/**
 *  @file RT_Test_FilterFactory.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_Tests_RT_Test_FILTERFACTORY_H
#define TAO_Notify_Tests_RT_Test_FILTERFACTORY_H
#include /**/ "ace/pre.h"

#include "rt_test_filter_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyFilterS.h"
#include "orbsvcs/Notify/FilterFactory.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_Notify_Tests_RT_Test_FilterFactory
 *
 * @brief
 *
 */
class TAO_RT_Test_Filter_Export TAO_Notify_Tests_RT_Test_FilterFactory
  : public virtual POA_CosNotifyFilter::FilterFactory
  , public virtual TAO_Notify_FilterFactory
{
public:
  /// Constructor
  TAO_Notify_Tests_RT_Test_FilterFactory (void);

  /// Destructor
  ~TAO_Notify_Tests_RT_Test_FilterFactory ();

  ///= TAO_Notify_Tests_FilterFactory methods.

  virtual CosNotifyFilter::FilterFactory_ptr create (PortableServer::POA_ptr filter_poa);
  virtual void destroy (void);

  ///= CosNotifyFilter::FilterFactory methods

  virtual CosNotifyFilter::Filter_ptr create_filter (const char * constraint_grammar);

  virtual CosNotifyFilter::MappingFilter_ptr create_mapping_filter (const char * constraint_grammar,
                                                                    const CORBA::Any & default_value);

  virtual TAO_Notify_Object::ID get_filter_id (CosNotifyFilter::Filter_ptr filter);
  virtual CosNotifyFilter::Filter_ptr get_filter (const TAO_Notify_Object::ID& id);
  virtual void save_persistent (TAO_Notify::Topology_Saver& saver);

protected:
  /// Release this object.
  virtual void release (void);

  /// The POA in which to activate the Filters.
  PortableServer::POA_var filter_poa_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

ACE_FACTORY_DECLARE (TAO_RT_Test_Filter, TAO_Notify_Tests_RT_Test_FilterFactory)

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_RT_Test_FILTERFACTORY_H */
