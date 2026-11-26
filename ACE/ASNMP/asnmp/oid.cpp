/**
 *  @file     oid.cpp
 *
 * This module contains the implementation of the oid class. This
 * includes all protected and public member functions. The oid class
 * may be compiled stand alone without the use of any other library.
 *
 *  @author Peter E MellquistMichael R MacFaden  mrm@cisco.com - rework & ACE port
 */

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
  purpose. It is provided "AS-IS" without warranty of any kind,either express
  or implied. User hereby grants a royalty-free license to any and all
  derivatives based upon this software code base.
=====================================================================*/

#include <cstring>
#include <memory>

#include "asnmp/oid.h"                  // include def for oid class
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_ctype.h"
#include <cerrno>
#include <limits>
#include <new>
#include <utility>

enum Defs {SNMPBUFFSIZE=300,
           SNMPCHARSIZE=15};          // max oid value (4294967295UL)

#define NO_MEM_STR "ERROR: Oid::to_string: memory allocation failure"

SmiUINT32 Oid::get_syntax()
{
  return sNMP_SYNTAX_OID;
}

/// Construct an OID from its dotted string representation.
Oid::Oid( const char * dotted_oid_string, size_t size)
{
  // can't init enum SmiValue so just memset it clean
  set_null();

  size_t const z = std::strlen (dotted_oid_string);
  if (z == 0)
  {
    set_invalid();
    return;
  }

  if (size == (unsigned int)-1)
  {
    size = z;
  }
  if (size > z)
  {
    size = z;
  }

  char const *parse_string = dotted_oid_string;
  std::unique_ptr<char[]> truncated_string;
  if (size < z) {
    truncated_string.reset (new (std::nothrow) char[size + 1]);
    if (!truncated_string)
      {
        set_invalid ();
        return;
      }

    std::memcpy (truncated_string.get (), dotted_oid_string, size);
    truncated_string[size] = '\0';
    parse_string = truncated_string.get ();
  }

  size_t byte_counter {};
  if (StrToOid (parse_string, &smival.value.oid, byte_counter) < 0)
  {
    set_invalid();
  }
}

/// Copy an OID.
Oid::Oid ( const Oid &oid)
  : SnmpSyntax (oid)
{
  set_null();

  // allocate some memory for the oid
  // in this case the size to allocate is the same
  // size as the source oid
  if (oid.smival.value.oid.len) {
    ACE_NEW(smival.value.oid.ptr, SmiUINT32[ oid.smival.value.oid.len]);
    size_t byte_counter;
    OidCopy( (SmiLPOID) &(oid.smival.value.oid),(SmiLPOID)
              &smival.value.oid, byte_counter);
  }
}

Oid::Oid (Oid &&oid) noexcept
  : SnmpSyntax (oid),
    iv_str (std::move (oid.iv_str))
{
  oid.smival.value.oid.ptr = nullptr;
  oid.smival.value.oid.len = 0;
}

/// Construct an OID from raw integer values.
Oid::Oid(const unsigned long *raw_oid, size_t oid_len)
{
  set_null();

  smival.syntax = sNMP_SYNTAX_OID;
  set_invalid();

  if (raw_oid && oid_len > 0) {
    ACE_NEW(smival.value.oid.ptr, SmiUINT32[ oid_len]);
    smival.value.oid.len = oid_len;
    for (size_t i=0; i < oid_len; ++i)
      {
        smival.value.oid.ptr[i] = raw_oid[i];
      }
  }
}

/// Release the descriptor storage.
Oid::~Oid()
{
  set_invalid ();
}

void Oid::set_data( const char *dotted_oid_string)
{
  // delete the old value
  if ( smival.value.oid.ptr ) {
    set_invalid();
  }

  // assign the new value
  size_t byte_counter;
  if (StrToOid (dotted_oid_string, &smival.value.oid, byte_counter) < 0)
  {
    set_invalid();
  }
}

/// Assign from another OID.
// TODO: measure perf vs memory of no realloc in case where len >= oid.len
Oid& Oid::operator=( const Oid &oid)
{
  // protect against assignment from self
  if (this == std::addressof (oid))
  {
    return *this;
  }

  set_invalid();

  // check for zero len on source
  if ( oid.smival.value.oid.len == 0)
  {
    return *this;
  }

  const SmiLPOID srcOid = (SmiLPOID) &(oid.smival.value.oid);
  init_value(srcOid, oid.smival.value.oid.len);
  return *this;
}

Oid &Oid::operator= (Oid &&oid) noexcept
{
  if (this != std::addressof (oid))
    {
      this->set_invalid ();
      this->smival = oid.smival;
      this->iv_str = std::move (oid.iv_str);
      oid.smival.value.oid.ptr = nullptr;
      oid.smival.value.oid.len = 0;
    }
  return *this;
}

Oid& Oid::operator= (char const *dotted_oid_string)
{
  return *this = Oid (dotted_oid_string);
}

/// Initialize this OID from another raw OID value.
void Oid::init_value(const SmiLPOID srcOid, size_t len)
{
  // allocate some memory for the oid
  ACE_NEW(smival.value.oid.ptr, SmiUINT32[ len]);
  size_t byte_counter;
  OidCopy (srcOid, std::addressof (smival.value.oid), byte_counter);
}

void Oid::init_value(const unsigned long *raw_oid, size_t oid_len)
{
  if (smival.value.oid.ptr)
  {
    delete [] smival.value.oid.ptr;
  }

  ACE_NEW(smival.value.oid.ptr, SmiUINT32[ oid_len]);
  std::memcpy (smival.value.oid.ptr, raw_oid, oid_len * sizeof (SmiUINT32));
  smival.value.oid.len = oid_len;
}

/// Append a dotted string to this OID.
Oid& Oid::operator+=( const char *a)
{
  if (!a)
  {
    return *this;
  }

  if ( *a=='.')
  {
    ++a;
  }
  const size_t sz = std::strlen (a);

  if (valid()) {
    unsigned long const n = (smival.value.oid.len *SNMPCHARSIZE) + smival.value.oid.len + 1 + sz;
    std::unique_ptr<char[]> const ptr (new (std::nothrow) char[n]);
    if (!ptr)
      {
        errno = ENOMEM;
        return *this;
      }
    size_t byte_counter;
    if (OidToStr (&smival.value.oid, n, ptr.get (), byte_counter) > 0) {
        set_invalid();
        return *this;
      }

   if (std::strlen (ptr.get ()))
   {
     std::strcat (ptr.get (), ".");
   }
   std::strcat (ptr.get (), a);
   if ( smival.value.oid.len !=0) {
     set_invalid();
    }

   if (StrToOid (ptr.get (), &smival.value.oid, byte_counter) < 0) {
       set_invalid();
   }
  }
  else {
    size_t byte_counter;
    if (StrToOid (a, &smival.value.oid, byte_counter) < 0) {
      set_invalid();
    }
  }

  return *this;
}

/// Compare two OIDs for equality.
bool operator==( const Oid &lhs, const Oid &rhs)
{
  // ensure same len, then use left_comparison
  if (rhs.length() != lhs.length())
  {
    return false;
  }
  if( lhs.left_comparison( rhs.length(), rhs) == 0)
    {
      return true;
    }
  else
  {
    return false;
  }
}

/// Compare two OIDs for inequality.
bool operator!=( const Oid &lhs,const Oid &rhs)
{
  return (!(lhs == rhs));
}

/// Compare two OIDs for ordering.
bool operator<( const Oid &lhs,const Oid &rhs)
{
  int result;

  // call left_comparison with the current
  // Oidx, Oidy and len of Oidx
  if ((result = lhs.left_comparison( rhs.length(), rhs)) < 0)
    {
      return true;
    }
  else if (result > 0)
    {
      return false;
    }

  else{
    // if here, equivalent substrings, call the shorter one <
    if (lhs.length() < rhs.length())
      {
        return true;
      }
    else
    {
      return false;
    }
  }
}

/// Compare two OIDs for less-than-or-equal ordering.
bool operator<=( const Oid &x,const Oid &y)
{
  if ( (x < y) || (x == y) )
    {
      return true;
    }
  else
  {
    return false;
  }
}

/// Compare two OIDs for greater-than ordering.
bool operator>( const Oid &x,const Oid &y)
{
  // just invert existing <=
  if (!(x<=y))
    {
      return true;
    }
  else
  {
    return false;
  }
}

/// Compare two OIDs for greater-than-or-equal ordering.
bool operator>=( const Oid &x,const Oid &y)
{
  // just invert existing <
  if (!(x<y))
    {
      return true;
    }
  else
  {
    return false;
  }
}

/// Return the WinSNMP OID representation.
SmiLPOID Oid::oidval()
{
  return std::addressof (smival.value.oid);
}

/// Set this OID from raw integer values.
void Oid::set_data( const unsigned long *raw_oid, const size_t oid_len)
{
  if (smival.value.oid.len < oid_len) {
    if ( smival.value.oid.ptr) {
      set_invalid();
    }
  }
  init_value(raw_oid, oid_len);
}

/// Return the number of sub-identifiers.
size_t Oid::length() const
{
  return smival.value.oid.len;
}

/// Trim the requested number of leftmost sub-identifiers.
/// The allocated storage is retained for speed.
void Oid::trim( const size_t n)
{
  // verify that n is legal
  if ((n<=smival.value.oid.len)&&(n>0)) {
    smival.value.oid.len -= n;

    if (smival.value.oid.len == 0) {
      set_invalid();
    }
  }
}

/// Invalidate this OID and release its storage.
void Oid::set_invalid() {
  delete [] smival.value.oid.ptr;
  smival.value.oid.ptr = nullptr;
  smival.value.oid.len = 0;
  iv_str.reset ();
}

void Oid::set_null() {
  smival.syntax = sNMP_SYNTAX_OID;
  smival.value.oid.ptr = nullptr;
  smival.value.oid.len = 0;
  iv_str.reset ();
}

/// Append an integer sub-identifier to this OID.
Oid& Oid::operator+=( const unsigned long i)
{
  size_t const max_component_size =
    std::numeric_limits<unsigned long>::digits10 + 1 > SNMPCHARSIZE
      ? std::numeric_limits<unsigned long>::digits10 + 1
      : SNMPCHARSIZE;
  size_t const n = smival.value.oid.len * max_component_size
    + smival.value.oid.len + std::numeric_limits<unsigned long>::digits10 + 2;

  // two cases: null oid, existing oid
  if (valid()) {
    char buffer[SNMPBUFFSIZE];
    // allocate some temporary space
    char *ptr;
    ACE_NEW_RETURN(ptr, char[ n], *this);
    size_t byte_counter;
    if (OidToStr(&smival.value.oid, n, ptr, byte_counter) < 0)  {
       set_invalid();
       delete [] ptr;
       return *this;
    }

    if (std::strlen (ptr))
    {
      std::strcat (ptr, ".");
    }

    if (ACE_OS::sprintf( buffer,"%lu",i) != -1) {
       std::strcat (ptr, buffer);
       if ( smival.value.oid.ptr ) {
         set_invalid();
       }
       if (StrToOid (ptr, &smival.value.oid, byte_counter) < 0) {
         set_invalid();
       }
       delete [] ptr;
    }
  }
  else {
    init_value (std::addressof (i), 1);
  }

  return *this;
}

/// Append another OID's sub-identifiers.
Oid& Oid::operator+=( const Oid &o)
{
  if (o.smival.value.oid.len == 0)
  {
    return *this;
  }

  size_t const old_length = smival.value.oid.len;
  size_t const new_length = old_length + o.smival.value.oid.len;
  std::unique_ptr<SmiUINT32[]> new_oid (new (std::nothrow) SmiUINT32[new_length]);
  if (!new_oid)
    {
      errno = ENOMEM;
      return *this;
    }

  if (old_length != 0)
    {
      std::memcpy (new_oid.get (), smival.value.oid.ptr, old_length * sizeof (SmiUINT32));
    }

  std::memcpy (new_oid.get () + old_length, o.smival.value.oid.ptr, o.smival.value.oid.len * sizeof (SmiUINT32));

  delete [] smival.value.oid.ptr;
  smival.value.oid.ptr = new_oid.release ();
  smival.value.oid.len = new_length;
  return *this;
}

/// Return this OID's dotted string representation.
const char * Oid::to_string()
{
  unsigned long n;
  if (!valid())
  {
    return ""; // be consistent with other classes
  }

  // the worst case char len of an oid can be..
  // oid.len*3 + dots in between if each oid is XXXX
  // so.. size = (len*4) + (len-1) + 1 , extra for a null

  n = (smival.value.oid.len *SNMPCHARSIZE) + ( smival.value.oid.len -1) + 1 ;
  if (n == 0)
  {
    n = 1; // need at least 1 byte for a null string
  }

  // adjust the len of output array in case size was adjusted
  std::unique_ptr<char[]> new_string {new (std::nothrow) char[n]};
  if (!new_string)
    {
      errno = ENOMEM;
      return "";
    }

  // convert to an output string
  if (valid ())
    {
      size_t how_many {};
      if (OidToStr (&smival.value.oid, n, new_string.get (), how_many) < 0)
        {
          return "ERROR: Oid::OidToStr failed";
        }
    }
  iv_str = std::move (new_string);
  return iv_str.get ();
}

int Oid::suboid(Oid& new_oid, size_t start, size_t how_many)
{
  if (how_many == 0)
    {
      return 0;
    }
  else
  if (how_many == (size_t)-1)
    {
      how_many = length();
    }
  else
  if (how_many > length())
    {
      how_many = length();
    }

  // reset new_oid
  new_oid.set_invalid();

  size_t new_size = how_many - start;
  if (new_size == 0)
  {
    ++new_size;
  }
  new_oid.smival.value.oid.len = new_size;
  ACE_NEW_RETURN(new_oid.smival.value.oid.ptr,
        SmiUINT32 [ new_oid.smival.value.oid.len], -1);
  // copy source to destination
  std::memcpy (new_oid.smival.value.oid.ptr, smival.value.oid.ptr + start, new_size * sizeof (SmiUINT32));
  return 0;
}

/// Convert a dotted string to an OID value.
int Oid::StrToOid( const char *string, SmiLPOID dstOid, size_t& how_many)
{
  size_t index = 0;

  // make a temp buffer to copy the data into first
  size_t nz;

  if (string && *string) {
     nz = std::strlen (string);
    }
  else {
     dstOid->len = 0;
     dstOid->ptr = nullptr;
     return -1;
  }

  std::unique_ptr<SmiUINT32[]> const temp (new (std::nothrow) SmiUINT32[nz]);
  if (!temp)
    {
      errno = ENOMEM;
      return -1;
    }
  while (*string!=0 && index<nz) {
    // init the number for each token
    size_t number = 0;
    // skip over the dot
    if (*string=='.')
    {
      ++string;
    }

            // grab a digit token and convert it to a long int
    while (ACE_OS::ace_isdigit(*string))
      {
        number=number*10 + *(string++)-'0';
      }

                // check for invalid chars
    if (*string!=0 && *string!='.') {
      // Error: Invalid character in string
      return -1;
    }

    // stuff the value into the array
    temp[index] = static_cast<SmiUINT32> (number);
    ++index;  // bump the counter
  }

  // get some space for the real oid
  ACE_NEW_RETURN(dstOid->ptr, SmiUINT32[ index], -1);
  // copy in the temp data
  std::memcpy (dstOid->ptr, temp.get (), index * sizeof (SmiUINT32));

  // set the len of the oid
  dstOid->len = index;

  how_many = index;
  return 0;
}

/// Copy an OID value and report the number of copied sub-identifiers.
int Oid::OidCopy( SmiLPOID srcOid, SmiLPOID dstOid, size_t& how_many_bytes)
{
  // check source len ! zero
  if (srcOid->len == 0)
  {
    return -1;
  }

  // copy source to destination
  std::memcpy (dstOid->ptr, srcOid->ptr, srcOid->len * sizeof (SmiUINT32));

  //set the new len
  dstOid->len = srcOid->len;
  how_many_bytes = srcOid->len;

  return 0;
}

/// Compare the requested number of leftmost sub-identifiers.
/// Returns 0 when equal, -1 when this OID is less, and 1 when greater.
int Oid::left_comparison( const unsigned long n, const Oid &o) const
{
  unsigned long z;
  unsigned long len = n;
  int reduced_len = 0;

  // 1st case they both are null
  if (( len==0)&&( this->smival.value.oid.len==0))
  {
    return 0;  // equal
  }

  if (len == 0)
  {
    return 1;                         // ! equal
  }

  // only compare for the minimal length
  if (len > this->smival.value.oid.len) {
    len = this->smival.value.oid.len;
    reduced_len = 1;
  }
  if (len > o.smival.value.oid.len) {
    len = o.smival.value.oid.len;
    reduced_len = 1;
  }

  z = 0;
  while(z < len) {
    if ( this->smival.value.oid.ptr[z] < o.smival.value.oid.ptr[z])
    {
      return -1;                              // less than
    }
    if ( this->smival.value.oid.ptr[z] > o.smival.value.oid.ptr[z])
    {
      return 1;                               // greater than
    }
    ++z;
  }

  // if we truncated the len then these may not be equal
  if (reduced_len) {
    if (this->smival.value.oid.len < o.smival.value.oid.len)
    {
      return -1;
    }
    if (this->smival.value.oid.len > o.smival.value.oid.len)
    {
      return 1;
    }
  }
  return 0;                                 // equal

}

/// Compare the requested number of rightmost sub-identifiers.
/// Returns 0 when equal, -1 when this OID is less, and 1 when greater.
int Oid::right_comparison( const unsigned long n, const Oid &o) const
{
   // oid to compare must have at least the same number
   // of sub-ids to comparison else the argument Oid is
   // less than THIS
   if ( o.length() < n)
   {
     return -1;
   }

   // also can't compare argument oid for sub-ids which
   // THIS does not have
   if ( this->length() < n)
   {
     return -1;
   }

   size_t const this_length = this->length ();
   size_t const other_length = o.length ();
   for (size_t offset = 0; offset < n; ++offset)
   {
       size_t const this_index = this_length - offset - 1;
       size_t const other_index = other_length - offset - 1;
       if (this->smival.value.oid.ptr[this_index] < o.smival.value.oid.ptr[other_index])
       {
         return -1;
       }
       if (this->smival.value.oid.ptr[this_index] > o.smival.value.oid.ptr[other_index])
       {
         return 1;
       }
   }

   return 0;   // they are equal

}

/// Return whether this OID has valid storage.
int Oid::valid() const
{
   return ( smival.value.oid.ptr ? 1 : 0 );
}

/// Convert an OID value to its dotted string representation.
int Oid::OidToStr( SmiLPOID srcOid, unsigned long size,
                  char *string, size_t& how_many_bytes)
{
  unsigned long index = 0;
  unsigned totLen = 0;
  char szNumber[SNMPBUFFSIZE];

  // init the string
  string[totLen] = 0;

  // verify there is something to copy
  if (srcOid->len == 0)
  {
    return -1;
  }

  // loop through and build up a string
  for (index=0; index < srcOid->len; ++index) {
    // convert data element to a string
    if (ACE_OS::sprintf( szNumber,"%lu", srcOid->ptr[index]) == -1)
    {
      return -1;
    }

    // verify len is not over
    if (totLen + std::strlen (szNumber) + 1 >= size)
    {
      return -2;
    }

    // if not at end, pad with a dot
    if (totLen!=0)
    {
      string[totLen++] = '.';
    }

    // copy the string token into the main string
    std::strcpy (string + totLen, szNumber);

    // adjust the total len
    totLen += std::strlen (szNumber);
  }

  how_many_bytes = totLen + 1;
  return 0;
}

SnmpSyntax& Oid::operator=( SnmpSyntax &val)
{
  // protect against assignment from self
  if (this == std::addressof (val))
  {
    return *this;
  }

  // blow away old value
  smival.value.oid.len = 0;
  if (smival.value.oid.ptr) {
    set_invalid();
  }

  // assign new value
  if (val.valid()) {
    switch (val.get_syntax()) {
    case sNMP_SYNTAX_OID:
      set_data( ((Oid &)val).smival.value.oid.ptr,
               (unsigned int)((Oid &)val).smival.value.oid.len);
      break;
    }
  }
  return *this;
}

unsigned long& Oid::operator[](size_t position)
{
  return smival.value.oid.ptr[position];
}

SnmpSyntax *Oid::clone() const
{
  return new Oid(*this);
}
