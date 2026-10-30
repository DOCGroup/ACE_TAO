// -*- C++ -*-

//=============================================================================
/**
 *  @file   FTEC_Become_Primary_Listener.h
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef TAO_FTEC_BECOME_PRIMARY_LISTENER_H
#define TAO_FTEC_BECOME_PRIMARY_LISTENER_H

#include "orbsvcs/FtRtEvent/EventChannel/ftrtec_export.h"

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_FTRTEC_Export TAO_FTEC_Become_Primary_Listener
{
public:
  TAO_FTEC_Become_Primary_Listener() = default;
  virtual ~TAO_FTEC_Become_Primary_Listener();
  TAO_FTEC_Become_Primary_Listener(TAO_FTEC_Become_Primary_Listener const&) = delete;
  TAO_FTEC_Become_Primary_Listener(TAO_FTEC_Become_Primary_Listener&&) = delete;
  TAO_FTEC_Become_Primary_Listener& operator=(TAO_FTEC_Become_Primary_Listener const&) = delete;
  TAO_FTEC_Become_Primary_Listener& operator=(TAO_FTEC_Become_Primary_Listener&&) = delete;
  virtual void become_primary()=0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // TAO_FTEC_BECOME_PRIMARY_LISTENER_H
