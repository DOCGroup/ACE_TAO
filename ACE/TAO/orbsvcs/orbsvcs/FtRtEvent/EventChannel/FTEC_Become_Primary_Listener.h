// -*- C++ -*-

//=============================================================================
/**
 *  @file   FTEC_Become_Primary_Listener.h
 *
 *  $Id$
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
  virtual ~TAO_FTEC_Become_Primary_Listener();
  virtual void become_primary()=0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // TAO_FTEC_BECOME_PRIMARY_LISTENER_H
