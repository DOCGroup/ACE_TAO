// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_Strategy_Repository.h
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
#include "ace/SString.h"
#include "ace/Synch.h"

#include <string>
#include <unordered_map>

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
  TAO_CSD_Strategy_Repository() = default;

  /// Virtual Destructor.
  ~TAO_CSD_Strategy_Repository() override = default;

  int init (int argc, ACE_TCHAR ** argv) override;

  /// Factory method used to create a CSD_Strategy object.
  CSD_Framework::Strategy_ptr find (ACE_CString const &poa_name);

  int add_strategy (ACE_CString const &poa_name,
                    CSD_Framework::Strategy_ptr strategy);

private:
  TAO_CSD_Strategy_Repository (TAO_CSD_Strategy_Repository const &) = delete;
  TAO_CSD_Strategy_Repository &operator= (TAO_CSD_Strategy_Repository const &) = delete;
  TAO_CSD_Strategy_Repository (TAO_CSD_Strategy_Repository &&) = delete;
  TAO_CSD_Strategy_Repository &operator= (TAO_CSD_Strategy_Repository &&) = delete;

  using Strategy_Map =
    std::unordered_map<std::string, CSD_Framework::Strategy_var>;

  Strategy_Map strategy_map_;
};


ACE_STATIC_SVC_DECLARE_EXPORT (TAO_CSD_FW, TAO_CSD_Strategy_Repository)
ACE_FACTORY_DECLARE (TAO_CSD_FW, TAO_CSD_Strategy_Repository)

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_CSD_STRATEGY_FACTORY_H */
