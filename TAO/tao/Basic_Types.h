// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Basic_Types.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons
 */
//=============================================================================


#ifndef TAO_BASIC_TYPES_H
#define TAO_BASIC_TYPES_H

#include "ace/pre.h"
#include "ace/CDR_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CORBA
{
  /**
   * @name CORBA Primitive Types
   *
   * Declarations of all CORBA primitive types.
   */
  //@{
  typedef ACE_CDR::Boolean Boolean;
  typedef Boolean &Boolean_out;

  typedef ACE_CDR::Octet Octet;
  typedef Octet  &Octet_out;

  typedef ACE_CDR::Short Short;
  typedef Short &Short_out;

  typedef ACE_CDR::UShort UShort;
  typedef UShort &UShort_out;

  typedef ACE_CDR::Long Long;
  typedef Long &Long_out;

  typedef ACE_CDR::ULong ULong;
  typedef ULong &ULong_out;

  typedef ACE_CDR::LongLong LongLong;
  typedef LongLong &LongLong_out;

  typedef ACE_CDR::ULongLong ULongLong;
  typedef ULongLong &ULongLong_out;

  typedef ACE_CDR::Float Float;
  typedef Float &Float_out;

  typedef ACE_CDR::Double Double;
  typedef Double &Double_out;

  typedef ACE_CDR::LongDouble LongDouble;
  typedef LongDouble &LongDouble_out;

  typedef ACE_CDR::Char Char;
  typedef Char &Char_out;

  typedef ACE_CDR::WChar WChar;
  typedef WChar &WChar_out;
  //@}

  class String_var;
  class String_out;

  class WString_var;
  class WString_out;
};

#include "ace/post.h"

#endif /* TAO_BASIC_TYPES_H */
