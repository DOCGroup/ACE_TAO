/* -*- C++ -*- */
/**
 *  @file ETCL_FilterFactory.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_ETCL_FILTERFACTORY_H
#define TAO_Notify_ETCL_FILTERFACTORY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyFilterS.h"

#include "orbsvcs/Notify/FilterFactory.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_ETCL_FilterFactory
 *
 * @brief
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_ETCL_FilterFactory :
  public virtual POA_CosNotifyFilter::FilterFactory,
  public virtual TAO_Notify_FilterFactory
{
public:
  /// Constuctor
  TAO_Notify_ETCL_FilterFactory (void);

  /// Destructor
  virtual ~TAO_Notify_ETCL_FilterFactory ();

  ///= TAO_Notify_FilterFactory methods.

  virtual CosNotifyFilter::FilterFactory_ptr create (
      PortableServer::POA_ptr filter_poa);

  ///= CosNotifyFilter::FilterFactory methods

  virtual CosNotifyFilter::Filter_ptr create_filter (
      const char * constraint_grammar);

  virtual CosNotifyFilter::MappingFilter_ptr create_mapping_filter (
      const char * constraint_grammar,
      const CORBA::Any & default_value);

protected:
  /// The POA in which to activate the Filters.
  PortableServer::POA_var filter_poa_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

ACE_FACTORY_DECLARE (TAO_Notify_Serv, TAO_Notify_ETCL_FilterFactory)

#include /**/ "ace/post.h"

#endif /* TAO_Notify_ETCL_FILTERFACTORY_H */
