// -*- C++ -*-

//=============================================================================
/**
 *  @file   Fault_Detector_Loader.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef FAULT_DETECTOR_LOADER_H
#define FAULT_DETECTOR_LOADER_H

#include  "ace/pre.h"

#include "tao/orbconf.h"

#include "ace/Service_Object.h"
#include "ace/Service_Config.h"
#include "ftrtec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class Fault_Detector;

namespace FTRTEC {
  class TAO_FTRTEC_Export Fault_Detector_Loader : public ACE_Service_Object
  {
  public:
    /// Constructor.
    Fault_Detector_Loader (void);

    /// Destructor.
    virtual ~Fault_Detector_Loader (void);

    virtual int init (int argc,
      ACE_TCHAR* []);
    Fault_Detector* detector();
  private:
    auto_ptr<Fault_Detector> detector_;
  };


  ACE_STATIC_SVC_DECLARE_EXPORT (TAO_FTRTEC, Fault_Detector_Loader)
    ACE_STATIC_SVC_REQUIRE(Fault_Detector_Loader);
  ACE_FACTORY_DECLARE (TAO_FTRTEC, Fault_Detector_Loader)
}

#include  "ace/post.h"
#endif //FAULT_DETECTOR_LOADER_H
