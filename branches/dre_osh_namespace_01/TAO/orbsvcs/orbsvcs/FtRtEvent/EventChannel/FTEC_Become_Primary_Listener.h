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

#include "ftrtec_export.h"

class TAO_FTRTEC_Export TAO_FTEC_Become_Primary_Listener
{
public:
  virtual ~TAO_FTEC_Become_Primary_Listener();
  virtual void become_primary()=0;
};

#endif // TAO_FTEC_BECOME_PRIMARY_LISTENER_H
