// -*- C++ -*-

//=============================================================================
/**
 *  @file   Fault_Listener.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef TAO_FTEC_FAULT_LISTENER_H
#define TAO_FTEC_FAULT_LISTENER_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_FTEC_Fault_Listener
{
public:
  virtual ~TAO_FTEC_Fault_Listener(){};
  virtual void connection_closed()=0;
};

#endif
