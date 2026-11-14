/* -*-C++-*- */
#ifndef TIMETICKS_
#define TIMETICKS_
//=============================================================================
/**
 *  @file     timetick.h
 *
 * Class definition for SMI Timeticks class.
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

constexpr size_t TICKOUTBUF = 3 * sizeof (unsigned long) + 12;

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
  explicit TimeTicks (unsigned long const i = 0);

  TimeTicks (TimeTicks const &t);
  TimeTicks (TimeTicks &&) = delete;

  ~TimeTicks () override = default;

  TimeTicks &operator= (TimeTicks &&) = delete;

  /// syntax type
  SmiUINT32 get_syntax() override;

  /// get a printable ASCII value
  char const *to_string () override;

  /// create a new instance of this Value
  SnmpSyntax *clone() const override;

  /// copy an instance of this Value
  SnmpSyntax& operator=(SnmpSyntax &val) override;

  /// overloaded assignment
  TimeTicks &operator= (TimeTicks const &uli);

  /// overloaded assignment
  TimeTicks &operator= (unsigned long const i);

  /// otherwise, behave like an unsigned long
  operator unsigned long() const;

protected:
  /// for storing printed form
  char output_buffer[TICKOUTBUF] {};
};
#endif // TIMETICKS_
