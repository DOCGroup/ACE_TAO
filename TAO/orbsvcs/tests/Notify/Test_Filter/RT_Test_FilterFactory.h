/* -*- C++ -*- */
/**
 *  @file RT_Test_FilterFactory.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
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
 */
class TAO_RT_Test_Filter_Export TAO_Notify_Tests_RT_Test_FilterFactory
  : public virtual POA_CosNotifyFilter::FilterFactory
  , public virtual TAO_Notify_FilterFactory
{
public:
  /// Constructor
  TAO_Notify_Tests_RT_Test_FilterFactory () = default;
  TAO_Notify_Tests_RT_Test_FilterFactory (TAO_Notify_Tests_RT_Test_FilterFactory const&) = delete;
  TAO_Notify_Tests_RT_Test_FilterFactory (TAO_Notify_Tests_RT_Test_FilterFactory&&) = delete;
  TAO_Notify_Tests_RT_Test_FilterFactory& operator= (TAO_Notify_Tests_RT_Test_FilterFactory const&) = delete;
  TAO_Notify_Tests_RT_Test_FilterFactory& operator= (TAO_Notify_Tests_RT_Test_FilterFactory&&) = delete;
  ~TAO_Notify_Tests_RT_Test_FilterFactory () override = default;

  ///= TAO_Notify_Tests_FilterFactory methods.

  CosNotifyFilter::FilterFactory_ptr create (PortableServer::POA_ptr filter_poa) override;
  void destroy () override;

  ///= CosNotifyFilter::FilterFactory methods

  CosNotifyFilter::Filter_ptr create_filter (char const* constraint_grammar) override;

  CosNotifyFilter::MappingFilter_ptr create_mapping_filter (char const* constraint_grammar,
                                                            CORBA::Any const& default_value) override;

  TAO_Notify_Object::ID get_filter_id (CosNotifyFilter::Filter_ptr filter) override;
  CosNotifyFilter::Filter_ptr get_filter (TAO_Notify_Object::ID const& id) override;
  void save_persistent (TAO_Notify::Topology_Saver& saver) override;

protected:
  /// Release this object.
  void release () override;

  /// The POA in which to activate the Filters.
  PortableServer::POA_var filter_poa_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

ACE_FACTORY_DECLARE (TAO_RT_Test_Filter, TAO_Notify_Tests_RT_Test_FilterFactory)

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_RT_Test_FILTERFACTORY_H */
