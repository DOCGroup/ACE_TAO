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
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyFilterS.h"
#include "FilterFactory.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_Notify_ETCL_FilterFactory
 *
 * @brief
 *
 */
class TAO_Notify_Export TAO_Notify_ETCL_FilterFactory : public virtual PortableServer::RefCountServantBase
                                                  , public virtual POA_CosNotifyFilter::FilterFactory
                                                  , public TAO_Notify_FilterFactory
{
public:
  /// Constuctor
  TAO_Notify_ETCL_FilterFactory (void);

  /// Destructor
  ~TAO_Notify_ETCL_FilterFactory ();

  ///= TAO_Notify_FilterFactory methods.

  virtual CosNotifyFilter::FilterFactory_ptr create (PortableServer::POA_var& filter_poa ACE_ENV_ARG_DECL);

  ///= CosNotifyFilter::FilterFactory methods

  virtual CosNotifyFilter::Filter_ptr create_filter (const char * constraint_grammar
                                                     ACE_ENV_ARG_DECL
                                                     )
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosNotifyFilter::InvalidGrammar
                     ));

  virtual CosNotifyFilter::MappingFilter_ptr create_mapping_filter (const char * constraint_grammar,
                                                                    const CORBA::Any & default_value
                                                                    ACE_ENV_ARG_DECL
                                                                    )
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosNotifyFilter::InvalidGrammar
                     ));

protected:
  /// The POA in which to activate the Filters.
  PortableServer::POA_var filter_poa_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

ACE_FACTORY_DECLARE (TAO_Notify, TAO_Notify_ETCL_FilterFactory)

#if defined (__ACE_INLINE__)
#include "ETCL_FilterFactory.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_Notify_ETCL_FILTERFACTORY_H */
