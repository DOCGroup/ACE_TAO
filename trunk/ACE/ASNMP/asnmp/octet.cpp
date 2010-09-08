// $Id$

// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//    octet.cpp
//
// = DESCRIPTION
//    Implements the SMI Octet datatype (RFC 1155)
//    This class is fully contained and does not rely on or any other
//    SNMP libraries. Ported to ACE by Michael MacFaden mrm@cisco.com
//
// = AUTHOR
//   Peter E Mellquist
//
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
  purpose. It is provided "AS-IS without warranty of any kind,either express
  or implied. User hereby grants a royalty-free license to any and all
  derivatives based upon this software code base.
=====================================================================*/

#include "asnmp/octet.h"    // include definition for octet class
#include "ace/OS_Memory.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_ctype.h"
#include "ace/os_include/os_ctype.h"

//============[ syntax type ]=========================================
SmiUINT32 OctetStr::get_syntax()
{
  return sNMP_SYNTAX_OCTETS;
}

inline
void init_octet_smi(SmiVALUE& smi)
{
  smi.syntax = sNMP_SYNTAX_OCTETS;
  smi.value.string.ptr = 0;
  smi.value.string.len = 0;
}

inline
void reset_octet_smi(SmiVALUE& smi)
{
  delete [] smi.value.string.ptr;
  smi.value.string.ptr = 0;
  smi.value.string.len = 0;
}

inline
int copy_octet_smi(SmiVALUE& smi, int size, const char *src, int& valid_flag)
{
  valid_flag = 0;

  if (smi.value.string.ptr)
    delete [] smi.value.string.ptr;

  ACE_NEW_RETURN(smi.value.string.ptr, SmiBYTE[size], 1);
  ACE_OS::memcpy( smi.value.string.ptr, src, size);
  smi.value.string.len = size;
  valid_flag = 1;
  return 0;
}

//============[ default constructor ]=========================
OctetStr::OctetStr( const char  * string, long size):
  output_buffer(0), validity(0)
{

  init_octet_smi(smival);

  // check for null string
  if ( !string)
    return;

  if (size == -1) // calc if no length given - assume c style string
    size = ACE_OS::strlen( string);

  copy_octet_smi(smival, size, string, validity);
}


//============[ set the data on an already constructed Octet ]============
// TODO: should return status since it can fail in so many places
void OctetStr::set_data( const SmiBYTE* string, long size)
{
  // invalid args, set octetStr to not valid
  if ( !string || !size) {
    validity = 0;
    return;
  }

  // assume non-zero terminated string
  if (size == -1) // calc if no length given - assume c style string
    size = ACE_OS::strlen( (char *)string);

  // free up already used space
  if ( smival.value.string.ptr ) {
    reset_octet_smi(smival);
  }
  smival.value.string.len = 0;


  copy_octet_smi(smival, size, (const char *)string, validity);
}

//============[ constructor using another octet object ]==============
OctetStr::OctetStr ( const OctetStr &octet):
  SnmpSyntax (octet), output_buffer(0), validity(1)
{
  init_octet_smi(smival);
   // check for zero len case
   if ( octet.smival.value.string.len == 0) {
      return;
   }

   // must be a valid object
   if ( octet.validity == 0) {
      validity = 0;
      return;
   }

   // get the mem needed
  copy_octet_smi(smival, octet.smival.value.string.len,
                  (const char *)octet.smival.value.string.ptr, validity);
}

//=============[ destructor ]=========================================
OctetStr::~OctetStr()
{
 reset_octet_smi(smival);
 delete [] output_buffer;
}


//=============[ assignment to a string operator overloaded ]=========
OctetStr& OctetStr::operator=( const char  *string)
{
   // get the string size
   size_t nz;

   // free up previous memory if needed
   if ( smival.value.string.ptr ) {
     reset_octet_smi(smival);
   }

   // if empty then we are done
   if (!string || !(nz = ACE_OS::strlen( string))) {
     validity = 1;
     return *this;
   }

   // get memory needed
   copy_octet_smi(smival, nz, string, validity);
   return *this;
}

//=============[ assignment to another oid object overloaded ]========
OctetStr& OctetStr::operator=( const OctetStr &octet)
{
   // protect against assignment from self
   if ( this == &octet )
       return *this;

   // don't assign from invalid objs
   if (!octet.validity) {
     return *this;
   }

   // free up previous memory if needed
   if ( smival.value.string.len ) {
     reset_octet_smi(smival);
   }

   if (!octet.smival.value.string.len) {
     validity = 1;
     return *this;
   }

   // get some memory
   copy_octet_smi(smival, octet.smival.value.string.len,
                 (const char*) octet.smival.value.string.ptr, validity);
   return *this;                       // return self reference
}

//==============[ equivlence operator overloaded ]====================
bool operator==( const OctetStr &lhs, const OctetStr &rhs)
{
   if( lhs.left_comparison( rhs.smival.value.string.len, rhs)==0)
       return true;
   else
       return false;
}

//==============[ not equivlence operator overloaded ]================
bool operator!=( const OctetStr &lhs, const OctetStr &rhs)
{
   if( lhs.left_comparison( rhs.smival.value.string.len, rhs)!=0)
       return true;
   else
       return false;
}

//==============[ less than < overloaded ]============================
bool operator<( const OctetStr &lhs, const OctetStr &rhs)
{
   if( lhs.left_comparison( rhs.smival.value.string.len, rhs)<0)
      return true;
   else
      return false;
}

//==============[ less than <= overloaded ]===========================
bool operator<=( const OctetStr &lhs, const OctetStr &rhs)
{
   if(( lhs.left_comparison( rhs.smival.value.string.len, rhs)<0) ||
      ( lhs.left_comparison( rhs.smival.value.string.len, rhs)==0))
      return true;
   else
      return false;
}

//===============[ greater than > overloaded ]========================
bool operator>( const OctetStr &lhs, const OctetStr &rhs)
{
  if( lhs.left_comparison( rhs.smival.value.string.len, rhs)>0)
      return true;
  else
      return false;
}

//===============[ greater than >= overloaded ]=======================
bool operator>=( const OctetStr &lhs, const OctetStr &rhs)
{
  if(( lhs.left_comparison( rhs.smival.value.string.len, rhs)>0) ||
     ( lhs.left_comparison( rhs.smival.value.string.len, rhs)==0))
     return true;
  else
     return false;
}

//===============[ equivlence operator overloaded ]===================
bool operator==( const OctetStr &lhs,const char  *rhs)
{
   OctetStr to( rhs);
   if( lhs.left_comparison( to.smival.value.string.len,to)==0)
       return true;
   else
       return false;
}

//===============[ not equivlence operator overloaded ]===============
bool operator!=( const OctetStr &lhs,const char  *rhs)
{
   OctetStr to( rhs);
   if ( lhs.left_comparison( to.smival.value.string.len,to)!=0)
       return true;
   else
       return false;
}

//===============[ less than < operator overloaded ]==================
bool operator<( const OctetStr &lhs,const char  *rhs)
{
   OctetStr to( rhs);
   if ( lhs.left_comparison( to.smival.value.string.len,to)<0)
       return true;
   else
       return false;
}

//===============[ less than <= operator overloaded ]=================
bool operator<=( const OctetStr &lhs,char  *rhs)
{
   OctetStr to( rhs);
   if (( lhs.left_comparison( to.smival.value.string.len,to)<0) ||
       ( lhs.left_comparison( to.smival.value.string.len,to)==0))
      return true;
   else
      return false;
}

//===============[ greater than > operator overloaded ]===============
bool operator>( const OctetStr &lhs,const char  *rhs)
{
   OctetStr to( rhs);
   if ( lhs.left_comparison( to.smival.value.string.len,to)>0)
       return true;
   else
       return false;
}

//===============[ greater than >= operator overloaded ]==============
bool operator>=( const OctetStr &lhs,const char  *rhs)
{
   OctetStr to( rhs);
   if (( lhs.left_comparison( to.smival.value.string.len,to)>0) ||
       ( lhs.left_comparison( to.smival.value.string.len,to)==0))
      return true;
   else
      return false;
}

//===============[ append operator, appends a string ]================
OctetStr& OctetStr::operator+=( const char  *a)
{
  SmiBYTE  *tmp;         // temp pointer
  size_t slen,nlen;

  // get len of string
  if ( !a || ((slen = ACE_OS::strlen( a)) == 0))
    return *this;

  // total len of octet
  nlen =  slen + (size_t) smival.value.string.len;
  ACE_NEW_RETURN(tmp, SmiBYTE [ nlen], *this);
  ACE_OS::memcpy ( tmp, smival.value.string.ptr,
                   (size_t) smival.value.string.len);
  ACE_OS::memcpy( tmp + smival.value.string.len, a, (size_t) slen);
  // delete the original
  if ( smival.value.string.ptr )
     reset_octet_smi(smival);
  smival.value.string.ptr = tmp;
  smival.value.string.len = nlen;
  return *this;
}

//================[ append one OctetStr to another ]==================
OctetStr& OctetStr::operator+=( const OctetStr& octetstr)
{
  SmiBYTE  *tmp;         // temp pointer
  size_t slen,nlen;

  if (!octetstr.validity ||
      !(slen = (size_t)octetstr.length()))
    return *this;

  // total len of octet
  nlen =  slen + (size_t) smival.value.string.len;
  // get mem needed
  ACE_NEW_RETURN(tmp, SmiBYTE[ nlen], *this);
  ACE_OS::memcpy ( tmp, smival.value.string.ptr,
                   (size_t) smival.value.string.len);
  ACE_OS::memcpy( tmp + smival.value.string.len, octetstr.data(),
                  (size_t) slen);
  if ( smival.value.string.ptr )
      reset_octet_smi(smival);
  smival.value.string.ptr = tmp;
  smival.value.string.len = nlen;
  return *this;
}

//================[ appends an int ]==================================
OctetStr& OctetStr::operator+=( const char c)
{
    SmiBYTE  *tmp;
    // get the memory needed plus one extra byte
    ACE_NEW_RETURN(tmp, SmiBYTE[ smival.value.string.len + 1], *this);
    ACE_OS::memcpy ( tmp, smival.value.string.ptr,
                (size_t) smival.value.string.len); // len of original
    tmp[ smival.value.string.len ] = c;         // assign in byte
    if ( smival.value.string.ptr )      // delete the original
      reset_octet_smi(smival);

    smival.value.string.ptr = tmp;      // point to one
    smival.value.string.len++;          // up the len
    return *this;
}


//================[ compare n elements of an Octet ]==================
int OctetStr::left_comparison( const long n, const OctetStr &o) const
{
   long z, w;

   // both are empty, they are equal
   if (( smival.value.string.len == 0) &&
           ( o.smival.value.string.len == 0))
           return 0;  // equal

   // self is empty and param has something
   if (( smival.value.string.len == 0) &&
           ( o.smival.value.string.len >0) &&
           (n>0))
           return -1;

   // self has something and param has nothing
   if (( smival.value.string.len > 0) &&
           ( o.smival.value.string.len ==0) &&
           (n>0))
           return 1;

   // special case
   if (( smival.value.string.len == 0) &&
           ( o.smival.value.string.len > 0) &&
           ( n == 0))
           return 0;

   // pick the Min of n, this and the param len
   // this is the maximum # to iterate a search
   w = smival.value.string.len < o.smival.value.string.len
           ? smival.value.string.len : o.smival.value.string.len;
   if (n<w) w=n;

   z = 0;
   while( z < w) {
          if ( smival.value.string.ptr[z] < o.smival.value.string.ptr[z])
            return -1;                          // less than
      if ( smival.value.string.ptr[z] > o.smival.value.string.ptr[z])
            return 1;                           // greater than
      z++;
   }

   if (( z == 0) &&
           ( smival.value.string.len == 0) &&
           ( o.smival.value.string.len > 0))
           return -1;

   if (( z == 0) &&
           ( o.smival.value.string.len == 0) &&
           ( smival.value.string.len > 0))
           return 1;

   return 0;
}

//================[ return the length of the oid ]=======================
size_t OctetStr::length() const
{
   return (size_t) smival.value.string.len;
}

//================[ operator[]: access as if array ]==================
SmiBYTE& OctetStr::operator[]( int position)
{
    return  smival.value.string.ptr[position];
}

//===============[ reuturns pointer to internal data ]===============
SmiBYTE  * OctetStr::data() const
{
   return smival.value.string.ptr;
}

//================[ returns validity ]================================
int OctetStr::valid() const
{
   return validity;
}

//================[ clone() ]=========================================
SnmpSyntax * OctetStr::clone() const
{
  OctetStr *tmp = new OctetStr(*this);
  return ( SnmpSyntax *) tmp;
}

//================[ ASCII format return ]=============================
const char  * OctetStr::to_string()
{
  for ( unsigned long i=0; i < smival.value.string.len; i++) {
    if (( smival.value.string.ptr[i] != '\r')&&
                ( smival.value.string.ptr[i] != '\n')&&
                (ACE_OS::ace_isprint((int) (smival.value.string.ptr[i]))==0))
         return(to_string_hex());
  }

  if ( output_buffer != 0)
          delete [] output_buffer;

  ACE_NEW_RETURN(output_buffer, char[smival.value.string.len + 1], "");
  if (smival.value.string.len)
    ACE_OS::memcpy(output_buffer, smival.value.string.ptr,
                   (int) smival.value.string.len);
  output_buffer[smival.value.string.len] = '\0';
  return output_buffer;
}


//================[ general Value = operator ]========================
SnmpSyntax& OctetStr::operator=( SnmpSyntax &val)
{
   // protect against assignment from self
   if ( this == &val )
       return *this;

  // blow away the old value
  validity=0;
  if (smival.value.string.ptr) {
      reset_octet_smi(smival);
  }
  smival.value.string.len = 0;
  if (val.valid()){
    switch (val.get_syntax()) {
      case sNMP_SYNTAX_OCTETS:
      case sNMP_SYNTAX_IPADDR:
        set_data( ((OctetStr &)val).smival.value.string.ptr,
                  ((OctetStr &)val).smival.value.string.len);
        break;
    }
  }
  return *this;
}

//================[ format the output into hex ]========================
const char *OctetStr::to_string_hex()
{
  int cnt;
  char char_buf[80];              // holds ASCII representation of data
  char *buf_ptr;                  // pointer into ASCII listing
  char *line_ptr;                 // pointer into Hex listing
  int  storageNeeded;             // how much space do we need ?
  int  local_len = (int) smival.value.string.len;
  SmiBYTE *bytes = smival.value.string.ptr;


  storageNeeded = (int) ((smival.value.string.len/16)+1) * 72 + 1;

  if ( output_buffer != 0)
          delete [] output_buffer;

  ACE_NEW_RETURN(output_buffer, char[storageNeeded], "");

  line_ptr = output_buffer;

    /*----------------------------------------*/
    /* processing loop for entire data buffer */
    /*----------------------------------------*/
  while (local_len > 0) {
    cnt      = 16;        /* print 16 bytes per line */
    buf_ptr  = char_buf;
    ACE_OS::sprintf(line_ptr, "  ");
    line_ptr += 2;  /* indent */

      /*-----------------------*/
      /* process a single line */
      /*-----------------------*/
    while (cnt-- > 0 && local_len-- > 0) {
      ACE_OS::sprintf(line_ptr, "%2.2X ", *bytes);

      line_ptr +=3;   /* the display of a byte always 3 chars long */
      if (ACE_OS::ace_isprint(*bytes))
        ACE_OS::sprintf(buf_ptr, "%c", *bytes);
      else
        ACE_OS::sprintf(buf_ptr, ".");
      bytes++;
      buf_ptr++;
    }
    cnt++;

       /*----------------------------------------------------------*/
       /* this is to make sure that the ASCII displays line up for */
       /* incomplete lines of hex                                  */
       /*----------------------------------------------------------*/
    while (cnt-- > 0) {
      ACE_OS::sprintf(line_ptr,"   ");
      line_ptr += 3;
    }

      /*------------------------------------------*/
      /* append the ASCII display to the Hex line */
      /*------------------------------------------*/

// TODO: verify ACE_OS:: on NT works like this or not

#if defined(_WIN32)
    const char *fmt = "   %s\r\n";
#else
    const char *fmt = "   %s\n";
#endif // _WIN32

    ACE_OS::sprintf(line_ptr, fmt, char_buf);
    line_ptr += 3 + ACE_OS::strlen(char_buf);
  }

  return output_buffer;
}
