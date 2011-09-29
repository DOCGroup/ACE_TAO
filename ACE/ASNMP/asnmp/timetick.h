/* -*-C++-*- */
#ifndef TIMETICKS_
#define TIMETICKS_
//=============================================================================
/**
 *  @file     timetick.h
 *
 *  $Id$
 *
 * Class definition for SMI Timeticks class.
 *
 *
 *  @author Michael R. MacFaden <mrm@cisco.com>
 */
//=============================================================================

/*===================================================================
  Copyright (c) 1996
  Hewlett-Packard Company

  ATTENTION: USE OF THIS SOFTWARE IS SUBJECT TO THE FOLLOWING TERMS.
  Permission to use, copy, modify, distribute and/or sell this software
  and/or its documentation is hereby granted without fee. User agrees
  to display the above copyright notice and this license notice in all
  copies of the software and any documentation of the software. User
  agrees to assume all liability for the use of the software; Hewlett-Packard
  makes no representations about the suitability of this software for any
  purpose. It is provided "AS-IS without warranty of any kind,either express
  or implied. User hereby grants a royalty-free license to any and all
  derivatives based upon this software code base.
=====================================================================*/

#include "asnmp/integer.h"

#define TICKOUTBUF 30 // max formatted time string

//------------[ TimeTicks Class ]-----------------------------------
// The timeticks class allows all the functionality of unsigned
// integers but is recognized as a distinct SMI type. TimeTicks
// objects may be get or set into Vb objects.
//
/**
 * @class TimeTicks
 *
 * @brief Define RFC1155 TimeTicks Data object
 */
class ASNMP_Export TimeTicks : public SnmpUInt32
{
public:
  TimeTicks( const unsigned long i = 0);

  /// copy constructor
  TimeTicks( const TimeTicks &t);

  /// destructor
  ~TimeTicks();

  /// syntax type
  SmiUINT32 get_syntax();

  /// get a printable ASCII value
  const char *to_string();

  /// create a new instance of this Value
  SnmpSyntax *clone() const;

  /// copy an instance of this Value
  SnmpSyntax& operator=(SnmpSyntax &val);

  /// overloaded assignment
  TimeTicks& operator=( const TimeTicks &uli);

  /// overloaded assignment
  TimeTicks& operator=( const unsigned long int i);

  /// otherwise, behave like an unsigned long
  operator unsigned long();

protected:
  /// for storing printed form
  char output_buffer[TICKOUTBUF];
};
#endif // TIMETICKS_
