// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_Strategy_Repository.h
 *
 *  $Id$
 *
 *  @author  Yan Dai <dai_y@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_STRATEGY_FACTORY_H
#define TAO_CSD_STRATEGY_FACTORY_H

#include /**/ "ace/pre.h"

#include "tao/CSD_Framework/CSD_FW_Export.h"
#include "tao/CSD_Framework/CSD_FrameworkC.h"
#include "ace/Service_Object.h"
#include "ace/Service_Config.h"
#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
  * @class TAO_CSD_Strategy_Factory
  *
  * @brief An ACE_Service_Object capable of creating TP_Strategy objects.
  *
  * TBD - Explain in more detail.
  *
  */
class TAO_CSD_FW_Export TAO_CSD_Strategy_Repository : public ACE_Service_Object
{
public:

  /// Constructor.
  TAO_CSD_Strategy_Repository();

  /// Virtual Destructor.
  virtual ~TAO_CSD_Strategy_Repository();

  int init (int argc, ACE_TCHAR ** argv);

  /// Factory method used to create a CSD_Strategy object.
  CSD_Framework::Strategy_ptr find (const ACE_CString& poa_name);

  int add_strategy (const ACE_CString& poa_name,
                    CSD_Framework::Strategy_ptr strategy);

private:
  struct Strategy_Node {
    Strategy_Node(const ACE_CString& poa_name,
                  CSD_Framework::Strategy_ptr strategy);
    ~Strategy_Node();
    void add_node (Strategy_Node *);
    Strategy_Node *find(const ACE_CString& name);

    ACE_CString poa_name_;
    CSD_Framework::Strategy_var strategy_;
    Strategy_Node * next_;
  };
  Strategy_Node * strategy_list_head_;
};


ACE_STATIC_SVC_DECLARE_EXPORT (TAO_CSD_FW, TAO_CSD_Strategy_Repository)
ACE_FACTORY_DECLARE (TAO_CSD_FW, TAO_CSD_Strategy_Repository)

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_CSD_STRATEGY_FACTORY_H */
