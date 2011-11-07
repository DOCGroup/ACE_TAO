/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    FlowSpec_Dbase.h
 *
 *  $Id$
 *
 *  @author Vishal Kachroo <vishal@cs.wustl.edu>
 */
//=============================================================================


#ifndef FLOWSPEC_DBASE_H
#define FLOWSPEC_DBASE_H

// This file contains the different FlowSpecs that the QoS enabled
// application uses. Its a good idea to list them all here so the
// application code is clean.

ACE_Flow_Spec notraffic (ACE_QOS_NOT_SPECIFIED,
                         ACE_QOS_NOT_SPECIFIED,
                         ACE_QOS_NOT_SPECIFIED,
                         ACE_QOS_NOT_SPECIFIED,
                         ACE_QOS_NOT_SPECIFIED,
                         ACE_SERVICETYPE_NOTRAFFIC,
                         ACE_QOS_NOT_SPECIFIED,
                         ACE_QOS_NOT_SPECIFIED,
                         25,
                         1);

ACE_Flow_Spec g711 (9200,
                    708,
                    18400,
                    0,
                    0,
                    ACE_SERVICETYPE_CONTROLLEDLOAD,
                    368,
                    368,
                    25,
                    1);

// The default session address is macarena.cs.wustl.edu. I am using macarena
// as my receiver for testing.
#define DEFAULT_QOS_SESSION_MACHINE "128.252.165.127"
#define DEFAULT_QOS_SESSION_PORT 8001

#endif /* FLOWSPEC_DBASE_H */

