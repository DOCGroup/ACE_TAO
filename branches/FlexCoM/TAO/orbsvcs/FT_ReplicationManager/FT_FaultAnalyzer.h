/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_FaultAnalyzer.h
 *
 *  $Id$
 *
 *  This file is part of TAO's implementation of Fault Tolerant CORBA.
 *
 *  @author Steve Totten <totten_s@ociweb.com>
 */
//=============================================================================


#ifndef FT_FAULT_ANALYZER_H_
#define FT_FAULT_ANALYZER_H_

#include /**/ "ace/pre.h"
#include <ace/ACE.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "FT_ReplicationManagerLib_export.h"
#include "orbsvcs/CosNotifyCommC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
  * Abstract base class for application-defined fault analyzers.
  *
  */
  class TAO_ReplicationManagerLib_Export FT_FaultAnalyzer
  {

  public:
    /**
    * Default constructor.
    */
    FT_FaultAnalyzer ();

    /**
    * Destructor.
    */
    virtual ~FT_FaultAnalyzer ();

  public:

    /**
    * Validate event type to make sure it is one we can handle.
    * @param event The structured fault event, as from the Fault Notifier.
    * @return 0 if it is a valid event type, -1 otherwise.
    */
    virtual int validate_event_type (
      const CosNotification::StructuredEvent & event) = 0;

    /**
    * Analyze a fault event.
    * @param event The structured fault event, as from the Fault Notifier.
    * @return 0 on success, -1 on failure.
    */
    virtual int analyze_fault_event (
      const CosNotification::StructuredEvent & event) = 0;

    ////////////////////
    // Forbidden methods
  private:
    /// Copy constructor.
    FT_FaultAnalyzer (const FT_FaultAnalyzer & rhs);
    /// Assignment operator.
    FT_FaultAnalyzer & operator = (const FT_FaultAnalyzer & rhs);

    ///////////////
    // Data Members
  private:

  };

} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* FT_FAULT_ANALYZER_H_ */

