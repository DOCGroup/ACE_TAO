// $Id$

// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//     oid.cpp
//
// = DESCRIPTION
//   This module contains the implementation of the oid class. This
//   includes all protected and public member functions. The oid class
//   may be compiled stand alone without the use of any other library.
//
// = AUTHOR
//   Peter E Mellquist
//   Michael R MacFaden  mrm@cisco.com - rework & ACE port
// ============================================================================
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

//---------[ external C libaries used ]--------------------------------

#include "asnmp/oid.h"                  // include def for oid class
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_ctype.h"

enum Defs {SNMPBUFFSIZE=300,
           SNMPCHARSIZE=15};          // max oid value (4294967295UL)

#define NO_MEM_STR "ERROR: Oid::to_string: memory allocation failure"

//=============[Oid::get_syntax(void)]====================================
SmiUINT32 Oid::get_syntax()
{
  return sNMP_SYNTAX_OID;
}


//=============[Oid::Oid( const char *dotted_string ]=====================
// constructor using a dotted string
//
// do a string to oid using the string passed in
Oid::Oid( const char * dotted_oid_string, size_t size)
{
  // can't init enum SmiValue so just memset it clean
  set_null();

  size_t z;
  if ((z = ACE_OS::strlen(dotted_oid_string)) == 0) {
    set_invalid();
    return;
  }

  if (size == (unsigned int)-1)
    size = z;
  if (size > z)
    size = z;

  char *ptr = (char *)dotted_oid_string;;
  if (size < z) {
    // create new buffer if needed
    ACE_NEW(ptr, char [size]);

    // sz should be in StrToOid?
    ACE_OS::memcpy( (void *)ptr, dotted_oid_string, size);
  }

  size_t byte_counter;
  if (StrToOid( (char *) ptr, &smival.value.oid, byte_counter) < 0)
    set_invalid();
  if (ptr != dotted_oid_string)
    delete [] ptr;
}


//=============[Oid::Oid( const Oid &oid) ]================================
// constructor using another oid object
//
// do an oid copy using the oid object passed in
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


//=============[Oid::Oid( const unsigned long *raw_oid, int oid_len) ]====
// constructor using raw numeric form
//
// copy the integer values into the private member
Oid::Oid(const unsigned long *raw_oid, size_t oid_len)
{
  set_null();

  smival.syntax = sNMP_SYNTAX_OID;
  set_invalid();

  if (raw_oid && oid_len > 0) {
    ACE_NEW(smival.value.oid.ptr, SmiUINT32[ oid_len]);
    smival.value.oid.len = oid_len;
    for (size_t i=0; i < oid_len; i++)
      smival.value.oid.ptr[i] = raw_oid[i];
  }
}

//=============[Oid::~Oid]==============================================
// destructor
//
// free up the descriptor space
Oid::~Oid()
{
  // free up the octet deep memory
  if ( smival.value.oid.ptr ) {
    set_invalid();
  }

  // free up the output string
  if ( iv_str != 0)
    delete [] iv_str;
}


//=============[Oid::operator = const char * dotted_string ]==============
// assignment to a string operator overloaded
//
// free the existing oid
// create the new oid from the string
// return this object
void Oid::set_data( const char *dotted_oid_string)
{
  // delete the old value
  if ( smival.value.oid.ptr ) {
    set_invalid();
  }

  // assign the new value
  size_t byte_counter;
  if (StrToOid( (char *) dotted_oid_string, &smival.value.oid, byte_counter) <0)
    set_invalid();
}


//=============[Oid:: operator = const Oid &oid ]==========================
// assignment to another oid object overloaded
//
// free the existing oid
// create a new one from the object passed in
// TODO: measure perf vs memory of no realloc in case where len >= oid.len
Oid& Oid::operator=( const Oid &oid)
{
  // protect against assignment from self
  if ( this == &oid)
      return *this;

  set_invalid();

  // check for zero len on source
  if ( oid.smival.value.oid.len == 0)
     return *this;

  const SmiLPOID srcOid = (SmiLPOID) &(oid.smival.value.oid);
  init_value(srcOid, oid.smival.value.oid.len);
  return *this;
}

// assign this object the oid, set to invalid if copy fails
void Oid::init_value(const SmiLPOID srcOid, size_t len)
{
  // allocate some memory for the oid
  ACE_NEW(smival.value.oid.ptr, SmiUINT32[ len]);
  size_t byte_counter;
  OidCopy( srcOid, (SmiLPOID) &smival.value.oid, byte_counter);
}

void Oid::init_value(const unsigned long *raw_oid, size_t oid_len)
{
  if (smival.value.oid.ptr)
    delete [] smival.value.oid.ptr;

  ACE_NEW(smival.value.oid.ptr, SmiUINT32[ oid_len]);
  ACE_OS::memcpy((SmiLPBYTE) smival.value.oid.ptr,
                 (SmiLPBYTE) raw_oid,
                 (size_t) (oid_len * sizeof(SmiUINT32)));
  smival.value.oid.len = oid_len;
}

//==============[Oid:: operator += const char *a ]=========================
// append operator, appends a string
//
// allocate some space for a max oid string
// extract current string into space
// concat new string
// free up existing oid
// make a new oid from string
// delete allocated space
Oid& Oid::operator+=( const char *a)
{
  unsigned long n;

  if (!a)
    return *this;

  if ( *a=='.')
    a++;
  size_t sz = ACE_OS::strlen(a);

  if (valid()) {
    n = (smival.value.oid.len *SNMPCHARSIZE) + smival.value.oid.len + 1 + sz;
    char *ptr;
    ACE_NEW_RETURN(ptr, char[ n], *this);
    size_t byte_counter;
    if (OidToStr(&smival.value.oid, n,ptr, byte_counter) > 0) {
        delete [] ptr;
        set_invalid();
        return *this;
      }

   if (ACE_OS::strlen(ptr))
     ACE_OS::strcat(ptr,".");
   ACE_OS::strcat(ptr,a);
   if ( smival.value.oid.len !=0) {
     set_invalid();
    }

   if (StrToOid( (char *) ptr, &smival.value.oid, byte_counter) < 0) {
       set_invalid();
   }
   delete [] ptr;
  }
  else {
    size_t byte_counter;
    if (StrToOid( (char *) a, &smival.value.oid, byte_counter) < 0) {
      set_invalid();
    }
  }

  return *this;
}

//=============[ bool operator == oid,oid ]=================================
// equivlence operator overloaded
bool operator==( const Oid &lhs, const Oid &rhs)
{
  // ensure same len, then use left_comparison
  if (rhs.length() != lhs.length())
    return false;
  if( lhs.left_comparison( rhs.length(), rhs) == 0)
    return true;
  else
    return false;
}

//==============[ bool operator!=( Oid &x,Oid &y) ]=======================
//not equivlence operator overloaded
bool operator!=( const Oid &lhs,const Oid &rhs)
{
  return (!(lhs == rhs));
}

//==============[ bool operator<( Oid &x,Oid &y) ]========================
// less than < overloaded
bool operator<( const Oid &lhs,const Oid &rhs)
{
  int result;

  // call left_comparison with the current
  // Oidx, Oidy and len of Oidx
  if ((result = lhs.left_comparison( rhs.length(), rhs)) < 0)
    return true;
  else if (result > 0)
    return false;

  else{
    // if here, equivalent substrings, call the shorter one <
    if (lhs.length() < rhs.length())
      return true;
    else
      return false;
  }
}

//==============[ bool operator<=( Oid &x,Oid &y) ]=======================
// less than <= overloaded
bool operator<=( const Oid &x,const Oid &y)
{
  if ( (x < y) || (x == y) )
    return true;
  else
    return false;
}

//==============[ bool operator>( Oid &x,Oid &y) ]========================
// greater than > overloaded
bool operator>( const Oid &x,const Oid &y)
{
  // just invert existing <=
  if (!(x<=y))
    return true;
  else
    return false;
}

//==============[ bool operator>=( Oid &x,Oid &y) ]=======================
// greater than >= overloaded
bool operator>=( const Oid &x,const Oid &y)
{
  // just invert existing <
  if (!(x<y))
    return true;
  else
    return false;
}

//===============[Oid::oidval ]=============================================
// return the WinSnmp oid part
SmiLPOID Oid::oidval()
{
  return (SmiLPOID) &smival.value.oid;
}

//===============[Oid::set_data ]==---=====================================
// copy data from raw form...
void Oid::set_data( const unsigned long *raw_oid, const size_t oid_len)
{
  if (smival.value.oid.len < oid_len) {
    if ( smival.value.oid.ptr) {
      set_invalid();
    }
  }
  init_value(raw_oid, oid_len);
}


//===============[Oid::len ]================================================
// return the len of the oid
size_t Oid::length() const
{
  return smival.value.oid.len;
}

//===============[Oid::trim( unsigned int) ]============================
// trim off the n leftmost values of an oid
// Note!, does not adjust actual space for
// speed
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

//===============[Oid::set_invalid() ]====================
// make this object invalid by resetting all values
void Oid::set_invalid() {
  delete [] smival.value.oid.ptr;
  smival.value.oid.ptr = 0;
  smival.value.oid.len = 0;
  delete [] iv_str;
  iv_str = 0;
}

//===============[Oid::set_null() ]====================
void Oid::set_null() {
  smival.syntax = sNMP_SYNTAX_OID;
  smival.value.oid.ptr = 0;
  smival.value.oid.len = 0;
  iv_str = 0;
}

//===============[Oid::operator += const unsigned int) ]====================
// append operator, appends an int
//
// allocate some space for a max oid string
// extract current string into space
// concat new string
// free up existing oid
// make a new oid from string
// delete allocated space
Oid& Oid::operator+=( const unsigned long i)
{
  unsigned long n = (smival.value.oid.len * SNMPCHARSIZE)
    + ( smival.value.oid.len -1) + 1 + 4;
  char buffer[SNMPBUFFSIZE];

  // two cases: null oid, existing oid
  if (valid()) {
    // allocate some temporary space
    char *ptr;
    ACE_NEW_RETURN(ptr, char[ n], *this);
    size_t byte_counter;
    if (OidToStr(&smival.value.oid, n, ptr, byte_counter) < 0)  {
       set_invalid();
       delete [] ptr;
       return *this;
    }

    if (ACE_OS::strlen(ptr))
       ACE_OS::strcat(ptr,".");

    if (ACE_OS::sprintf( buffer,"%lu",i) != -1) {
       ACE_OS::strcat(ptr, buffer);
       if ( smival.value.oid.ptr ) {
         set_invalid();
       }
       if (StrToOid( (char *) ptr, &smival.value.oid, byte_counter) < 0) {
         set_invalid();
       }
       delete [] ptr;
    }
  }
  else {
    init_value((const unsigned long *)&i, (size_t)1);
  }

  return *this;
}

//===============[Oid::operator += const Oid) ]========================
// append operator, appends an Oid
//
// allocate some space for a max oid string
// extract current string into space
// concat new string
// free up existing oid
// make a new oid from string
// delete allocated space
Oid& Oid::operator+=( const Oid &o)
{
  SmiLPUINT32 new_oid;

  if (o.smival.value.oid.len == 0)
    return *this;

  ACE_NEW_RETURN(new_oid,
        SmiUINT32[ smival.value.oid.len + o.smival.value.oid.len], *this);
  if (smival.value.oid.ptr) {
    ACE_OS::memcpy((SmiLPBYTE) new_oid,
                   (SmiLPBYTE) smival.value.oid.ptr,
                   (size_t) (smival.value.oid.len*sizeof(SmiUINT32)));

    delete [] smival.value.oid.ptr;
  }

  // out with the old, in with the new...
  smival.value.oid.ptr = new_oid;

  ACE_OS::memcpy((SmiLPBYTE) &new_oid[smival.value.oid.len],
                 (SmiLPBYTE) o.smival.value.oid.ptr,
                 (size_t) (o.smival.value.oid.len*sizeof(SmiUINT32)));

  smival.value.oid.len += o.smival.value.oid.len;
  return *this;
}

// return string portion of the oid
//
const char * Oid::to_string()
{
  unsigned long n;
  if (!valid())
    return ""; // be consistent with other classes

  // the worst case char len of an oid can be..
  // oid.len*3 + dots in between if each oid is XXXX
  // so.. size = (len*4) + (len-1) + 1 , extra for a null

  n = (smival.value.oid.len *SNMPCHARSIZE) + ( smival.value.oid.len -1) + 1 ;
  if (n == 0)
    n = 1; // need at least 1 byte for a null string

  // adjust the len of output array in case size was adjusted
  if ( iv_str != 0)
    delete [] iv_str;
  // allocate some space for the output string
  ACE_NEW_RETURN(iv_str, char[ n], "");

  // convert to an output string
  size_t how_many;
  if ( valid() && iv_str != 0)
    if (OidToStr(&smival.value.oid,n,iv_str, how_many) < 0)
      return "ERROR: Oid::OidToStr failed";
  return iv_str;
}

//==============[Oid::suboid( unsigned int start, n) ]=============
int Oid::suboid(Oid& new_oid, size_t start, size_t how_many)
{
  if (how_many == 0)
     return 0;
  else
  if (how_many == (size_t)-1)
     how_many = length();
  else
  if (how_many > length())
      how_many = length();

  // reset new_oid
  new_oid.set_invalid();

  size_t new_size = how_many - start;
  if (new_size == 0)
     new_size++;
  new_oid.smival.value.oid.len = new_size;
  ACE_NEW_RETURN(new_oid.smival.value.oid.ptr,
        SmiUINT32 [ new_oid.smival.value.oid.len], -1);
  // copy source to destination
  ACE_OS::memcpy( (SmiLPBYTE) new_oid.smival.value.oid.ptr,
                    (SmiLPBYTE) (smival.value.oid.ptr + start),
                    new_size * sizeof(SmiLPBYTE));
  return 0;
}


//=============[Oid::StrToOid( char *string, SmiLPOID dst) ]==============
// convert a string to an oid
int Oid::StrToOid( const char *string, SmiLPOID dstOid, size_t& how_many)
{
  size_t index = 0;
  size_t number = 0;

  // make a temp buffer to copy the data into first
  SmiLPUINT32 temp;
  unsigned long nz;

  if (string && *string) {
     nz = ACE_OS::strlen( string);
    }
  else {
     dstOid->len = 0;
     dstOid->ptr = 0;
     return -1;
  }

  ACE_NEW_RETURN(temp, SmiUINT32[ nz], -1);
  while (*string!=0 && index<nz) {
    // init the number for each token
    number = 0;
    // skip over the dot
    if (*string=='.')
        string++;

            // grab a digit token and convert it to a long int
    while (ACE_OS::ace_isdigit(*string))
      number=number*10 + *(string++)-'0';

                // check for invalid chars
    if (*string!=0 && *string!='.') {
      // Error: Invalid character in string
      delete [] temp;
      return -1;
    }

    // stuff the value into the array
    temp[index] = number;
    index++;  // bump the counter
  }


  // get some space for the real oid
  ACE_NEW_RETURN(dstOid->ptr, SmiUINT32[ index], -1);
  // TODO: make tmp autoptr type delete [] temp to prevent leak

  // copy in the temp data
  ACE_OS::memcpy((SmiLPBYTE) dstOid->ptr, (SmiLPBYTE) temp,
         (size_t) (index*sizeof(SmiUINT32)));

  // set the len of the oid
  dstOid->len = index;

  // free up temp data
  delete [] temp;

  how_many = index;
  return 0;
}


//===============[Oid::OidCopy( source, destination) ]====================
// Copy an oid, return bytes copied
int Oid::OidCopy( SmiLPOID srcOid, SmiLPOID dstOid, size_t& how_many_bytes)
{
  // check source len ! zero
  if (srcOid->len == 0)
    return -1;

  // copy source to destination
  ACE_OS::memcpy((SmiLPBYTE) dstOid->ptr,
                 (SmiLPBYTE) srcOid->ptr,
                 (size_t) (srcOid->len * sizeof(SmiUINT32)));

  //set the new len
  dstOid->len = srcOid->len;
  how_many_bytes = srcOid->len;

  return 0;
}


//===============[Oid::left_comparison( n, Oid) ]=================================
// compare the n leftmost values of two oids ( left-to_right )
//
// self == Oid then return 0, they are equal
// self < Oid then return -1, <
// self > Oid then return 1,  >
int Oid::left_comparison( const unsigned long n, const Oid &o) const
{
  unsigned long z;
  unsigned long len = n;
  int reduced_len = 0;

  // 1st case they both are null
  if (( len==0)&&( this->smival.value.oid.len==0))
    return 0;  // equal

  // verify that n is valid, must be >= 0
  if ( len <=0)
    return 1;                         // ! equal

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
      return -1;                              // less than
    if ( this->smival.value.oid.ptr[z] > o.smival.value.oid.ptr[z])
      return 1;                               // greater than
    z++;
  }

  // if we truncated the len then these may not be equal
  if (reduced_len) {
    if (this->smival.value.oid.len < o.smival.value.oid.len)
      return -1;
    if (this->smival.value.oid.len > o.smival.value.oid.len)
      return 1;
  }
  return 0;                                 // equal

}


//===============[Oid::left_comparison( n, Oid) ]=================================
// compare the n rightmost bytes (right-to-left)
// returns 0, equal
// returns -1, <
// returns 1 , >
int Oid::right_comparison( const unsigned long n, const Oid &o) const
{
   // oid to compare must have at least the same number
   // of sub-ids to comparison else the argument Oid is
   // less than THIS
   if ( o.length() < n)
      return -1;

   // also can't compare argument oid for sub-ids which
   // THIS does not have
   if ( this->length() < n)
      return -1;

   int start = (int) this->length();
   int end = (int) start - (int) n;
   for ( int z=start;z< end;z--)
   {
       if ( o.smival.value.oid.ptr[z] < this->smival.value.oid.ptr[z])
          return -1;
       if ( o.smival.value.oid.ptr[z] > this->smival.value.oid.ptr[z])
          return 1;
   }

   return 0;   // they are equal

}


//================[ Oid::valid() ]========================================
// is the Oid object valid
// returns validity
int Oid::valid() const
{
   return ( smival.value.oid.ptr ? 1 : 0 );
}

//================[Oid::OidToStr ]=========================================
// convert an oid to a string
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
    return -1;

  // loop through and build up a string
  for (index=0; index < srcOid->len; index++) {

    // convert data element to a string
    if (ACE_OS::sprintf( szNumber,"%lu", srcOid->ptr[index]) == -1)
      return -1;

    // verify len is not over
    if (totLen + ACE_OS::strlen(szNumber) + 1 >= size)
      return -2;

    // if not at end, pad with a dot
    if (totLen!=0)
      string[totLen++] = '.';

    // copy the string token into the main string
    ACE_OS::strcpy(string + totLen, szNumber);

    // adjust the total len
    totLen += ACE_OS::strlen(szNumber);
  }

  how_many_bytes = totLen + 1;
  return 0;
}


//================[ general Value = operator ]========================
SnmpSyntax& Oid::operator=( SnmpSyntax &val)
{
  // protect against assignment from self
  if ( this == &val )
    return *this;

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

//================[ [] operator ]=====================================
unsigned long& Oid::operator[](size_t position)
{
  return smival.value.oid.ptr[position];
}

//================[ clone ]===========================================
SnmpSyntax *Oid::clone() const
{
  return (SnmpSyntax *) new Oid(*this);
}
