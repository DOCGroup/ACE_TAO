// -*- C++ -*-

//=============================================================================
/**
 *  @file   Fault_Detector_T.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef FAULT_DETECTOR_T_H
#define FAULT_DETECTOR_T_H

#include "orbsvcs/FtRtEvent/EventChannel/Fault_Detector.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class ACCEPTOR, class CONNECTOR, class DETECTION_HANDLER>
class Fault_Detector_T : public Fault_Detector
{
public:
  Fault_Detector_T();
  ~Fault_Detector_T();
  int connect(const FTRT::Location& addr);
protected:
  ACCEPTOR acceptor_;
  CONNECTOR connector_;
private:
  virtual int init_acceptor();
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/FtRtEvent/EventChannel/Fault_Detector_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Fault_Detector_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif
