/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_DefaultFaultAnalyzer.h
 *
 *  $Id$
 *
 *  This file is part of TAO's implementation of Fault Tolerant CORBA.
 *  This is the default implementation of a fault analyzer that
 *  implements the interface of the abstract base class
 *  TAO::FT_FaultAnalyzer.
 *
 *  @author Steve Totten <totten_s@ociweb.com>
 */
//=============================================================================


#ifndef FT_DEFAULT_FAULT_ANALYZER_H_
#define FT_DEFAULT_FAULT_ANALYZER_H_

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/FT_ReplicationManager/FT_FaultAnalyzer.h"

// #include "orbsvcs/CosNotifyCommS.h"
// #include "orbsvcs/FT_NotifierC.h"
// #include "orbsvcs/FT_ReplicationManagerC.h"
// #include "orbsvcs/FT_ReplicationManager/FT_FaultEventDescriptor.h"

namespace TAO
{

  /**
  * Default fault analyzer.
  *
  */
  class FT_DefaultFaultAnalyzer
    : public TAO::FT_FaultAnalyzer
  {

  public:
    /**
    * Default constructor.
    */
    FT_DefaultFaultAnalyzer ();

    /**
    * Destructor.
    */
    virtual ~FT_DefaultFaultAnalyzer ();

  public:

    /**
    * Validate event type to make sure it is one we can handle.
    * @param event The structured fault event, as from the Fault Notifier.
    * @return 0 if it is a valid event type, -1 otherwise.
    */
    virtual int validate_event_type (
      const CosNotification::StructuredEvent & event);

    /**
    * Analyze a fault event.
    * @param event The structured fault event, as from the Fault Notifier.
    * @return 0 on success, -1 on failure.
    */
    virtual int analyze_fault_event (
      const CosNotification::StructuredEvent & event);

    ////////////////////
    // Forbidden methods
  private:
    /// Copy constructor.
    FT_DefaultFaultAnalyzer (const FT_DefaultFaultAnalyzer & rhs);
    /// Assignment operator.
    FT_DefaultFaultAnalyzer & operator = (const FT_DefaultFaultAnalyzer & rhs);

  };

} // namespace TAO

#include /**/ "ace/post.h"

#endif  /* FT_DEFAULT_FAULT_ANALYZER_H_ */

