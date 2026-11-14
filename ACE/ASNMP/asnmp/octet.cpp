
//=============================================================================
/**
 *  @file    octet.cpp
 *
 *  Implements the SMI Octet datatype (RFC 1155)
 *  This class is fully contained and does not rely on or any other
 *  SNMP libraries. Ported to ACE by Michael MacFaden mrm@cisco.com
 *
 *  @author Peter E Mellquist
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

#include "asnmp/octet.h"    // include definition for octet class
#include "ace/OS_NS_ctype.h"
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <memory>
#include <new>
#include <utility>

//============[ syntax type ]=========================================
SmiUINT32 OctetStr::get_syntax()
{
  return sNMP_SYNTAX_OCTETS;
}

inline
void init_octet_smi(SmiVALUE& smi)
{
  smi.syntax = sNMP_SYNTAX_OCTETS;
  smi.value.string.ptr = nullptr;
  smi.value.string.len = 0;
}

inline
void reset_octet_smi(SmiVALUE& smi)
{
  delete [] smi.value.string.ptr;
  smi.value.string.ptr = nullptr;
  smi.value.string.len = 0;
}

inline
int copy_octet_smi (SmiVALUE &smi, SmiUINT32 size, char const *src, int &valid_flag)
{
  std::unique_ptr<SmiBYTE[]> new_data {new (std::nothrow) SmiBYTE[size]};
  if (!new_data)
    {
      valid_flag = 0;
      errno = ENOMEM;
      return 1;
    }

  std::memcpy (new_data.get (), src, size);
  delete [] smi.value.string.ptr;
  smi.value.string.ptr = new_data.release ();
  smi.value.string.len = size;
  valid_flag = 1;
  return 0;
}

//============[ default constructor ]=========================
OctetStr::OctetStr (char const *string, long size)
{
  init_octet_smi (smival);

  // check for null string
  if (!string)
    return;

  if (size == -1) // calc if no length given - assume c style string
    size = static_cast<long> (std::strlen (string));

  if (size < 0)
    return;

  copy_octet_smi (smival, static_cast<SmiUINT32> (size), string, validity);
}

//============[ set the data on an already constructed Octet ]============
// TODO: should return status since it can fail in so many places
void OctetStr::set_data (SmiBYTE const *string, long size)
{
  // invalid args, set octetStr to not valid
  if (!string || !size)
    {
      validity = 0;
      return;
    }

  // assume non-zero terminated string
  if (size == -1) // calc if no length given - assume c style string
    size = static_cast<long> (std::strlen (reinterpret_cast<char const *> (string)));

  if (size < 0)
    {
      validity = 0;
      return;
    }

  // free up already used space
  if (smival.value.string.ptr)
    reset_octet_smi (smival);
  smival.value.string.len = 0;

  copy_octet_smi (smival, static_cast<SmiUINT32> (size),
                  reinterpret_cast<char const *> (string), validity);
}

//============[ constructor using another octet object ]==============
OctetStr::OctetStr (OctetStr const &octet)
  : SnmpSyntax (octet), validity (1)
{
  init_octet_smi (smival);
  // check for zero len case
  if (octet.smival.value.string.len == 0)
    return;

  // must be a valid object
  if (octet.validity == 0)
    {
      validity = 0;
      return;
    }

  // get the mem needed
  copy_octet_smi (smival, octet.smival.value.string.len,
                  reinterpret_cast<char const *> (octet.smival.value.string.ptr), validity);
}

//=============[ destructor ]=========================================
OctetStr::~OctetStr ()
{
  reset_octet_smi (smival);
}

//=============[ assignment to a string operator overloaded ]=========
OctetStr &OctetStr::operator= (char const *string)
{
   // get the string size
   size_t nz {};

   // free up previous memory if needed
   if (smival.value.string.ptr)
     reset_octet_smi (smival);

   // if empty then we are done
   if (!string || !(nz = std::strlen (string))) {
     validity = 1;
     return *this;
   }

   // get memory needed
   copy_octet_smi (smival, static_cast<SmiUINT32> (nz), string, validity);
   return *this;
}

//=============[ assignment to another oid object overloaded ]========
OctetStr &OctetStr::operator= (OctetStr const &octet)
{
   // protect against assignment from self
   if (this == std::addressof (octet))
       return *this;

   // don't assign from invalid objs
   if (!octet.validity) {
     return *this;
   }

   // free up previous memory if needed
   if (smival.value.string.ptr)
     reset_octet_smi(smival);

   if (!octet.smival.value.string.len) {
     validity = 1;
     return *this;
   }

   // get some memory
   copy_octet_smi (smival, octet.smival.value.string.len,
                   reinterpret_cast<char const *> (octet.smival.value.string.ptr), validity);
   return *this;                       // return self reference
}

//==============[ equivlence operator overloaded ]====================
bool operator== (OctetStr const &lhs, OctetStr const &rhs)
{
   if( lhs.left_comparison( rhs.smival.value.string.len, rhs)==0)
       return true;
   else
       return false;
}

//==============[ not equivlence operator overloaded ]================
bool operator!= (OctetStr const &lhs, OctetStr const &rhs)
{
   if( lhs.left_comparison( rhs.smival.value.string.len, rhs)!=0)
       return true;
   else
       return false;
}

//==============[ less than < overloaded ]============================
bool operator< (OctetStr const &lhs, OctetStr const &rhs)
{
   if( lhs.left_comparison( rhs.smival.value.string.len, rhs)<0)
      return true;
   else
      return false;
}

//==============[ less than <= overloaded ]===========================
bool operator<= (OctetStr const &lhs, OctetStr const &rhs)
{
   if(( lhs.left_comparison( rhs.smival.value.string.len, rhs)<0) ||
      ( lhs.left_comparison( rhs.smival.value.string.len, rhs)==0))
      return true;
   else
      return false;
}

//===============[ greater than > overloaded ]========================
bool operator> (OctetStr const &lhs, OctetStr const &rhs)
{
  if( lhs.left_comparison( rhs.smival.value.string.len, rhs)>0)
      return true;
  else
      return false;
}

//===============[ greater than >= overloaded ]=======================
bool operator>= (OctetStr const &lhs, OctetStr const &rhs)
{
  if(( lhs.left_comparison( rhs.smival.value.string.len, rhs)>0) ||
     ( lhs.left_comparison( rhs.smival.value.string.len, rhs)==0))
     return true;
  else
     return false;
}

//===============[ equivlence operator overloaded ]===================
bool operator== (OctetStr const &lhs, char const *rhs)
{
   OctetStr to( rhs);
   if( lhs.left_comparison( to.smival.value.string.len,to)==0)
       return true;
   else
       return false;
}

//===============[ not equivlence operator overloaded ]===============
bool operator!= (OctetStr const &lhs, char const *rhs)
{
   OctetStr to( rhs);
   if ( lhs.left_comparison( to.smival.value.string.len,to)!=0)
       return true;
   else
       return false;
}

//===============[ less than < operator overloaded ]==================
bool operator< (OctetStr const &lhs, char const *rhs)
{
   OctetStr to( rhs);
   if ( lhs.left_comparison( to.smival.value.string.len,to)<0)
       return true;
   else
       return false;
}

//===============[ less than <= operator overloaded ]=================
bool operator<= (OctetStr const &lhs, char *rhs)
{
   OctetStr to( rhs);
   if (( lhs.left_comparison( to.smival.value.string.len,to)<0) ||
       ( lhs.left_comparison( to.smival.value.string.len,to)==0))
      return true;
   else
      return false;
}

//===============[ greater than > operator overloaded ]===============
bool operator> (OctetStr const &lhs, char const *rhs)
{
   OctetStr to( rhs);
   if ( lhs.left_comparison( to.smival.value.string.len,to)>0)
       return true;
   else
       return false;
}

//===============[ greater than >= operator overloaded ]==============
bool operator>= (OctetStr const &lhs, char const *rhs)
{
   OctetStr to( rhs);
   if (( lhs.left_comparison( to.smival.value.string.len,to)>0) ||
       ( lhs.left_comparison( to.smival.value.string.len,to)==0))
      return true;
   else
      return false;
}

//===============[ append operator, appends a string ]================
OctetStr &OctetStr::operator+= (char const *a)
{
  size_t slen {};
  size_t nlen {};

  // get len of string
  if (!a || ((slen = std::strlen (a)) == 0))
    return *this;

  // total len of octet
  nlen =  slen + (size_t) smival.value.string.len;
  std::unique_ptr<SmiBYTE[]> tmp {new (std::nothrow) SmiBYTE[nlen]};
  if (!tmp)
    {
      errno = ENOMEM;
      return *this;
    }
  if (smival.value.string.len)
    std::memcpy (tmp.get (), smival.value.string.ptr, smival.value.string.len);
  std::memcpy (tmp.get () + smival.value.string.len, a, slen);
  // delete the original
  if ( smival.value.string.ptr )
     reset_octet_smi(smival);
  smival.value.string.ptr = tmp.release ();
  smival.value.string.len = nlen;
  return *this;
}

//================[ append one OctetStr to another ]==================
OctetStr &OctetStr::operator+= (OctetStr const &octetstr)
{
  size_t slen {};
  size_t nlen {};

  if (!octetstr.validity ||
      !(slen = (size_t)octetstr.length()))
    return *this;

  // total len of octet
  nlen =  slen + (size_t) smival.value.string.len;
  // get mem needed
  std::unique_ptr<SmiBYTE[]> tmp {new (std::nothrow) SmiBYTE[nlen]};
  if (!tmp)
    {
      errno = ENOMEM;
      return *this;
    }
  if (smival.value.string.len)
    std::memcpy (tmp.get (), smival.value.string.ptr, smival.value.string.len);
  std::memcpy (tmp.get () + smival.value.string.len, octetstr.data (), slen);
  if ( smival.value.string.ptr )
      reset_octet_smi(smival);
  smival.value.string.ptr = tmp.release ();
  smival.value.string.len = nlen;
  return *this;
}

//================[ appends an int ]==================================
OctetStr &OctetStr::operator+= (char c)
{
    size_t const new_size = smival.value.string.len + 1;
    // get the memory needed plus one extra byte
    std::unique_ptr<SmiBYTE[]> tmp {new (std::nothrow) SmiBYTE[new_size]};
    if (!tmp)
      {
        errno = ENOMEM;
        return *this;
      }
    if (smival.value.string.len)
      std::memcpy (tmp.get (), smival.value.string.ptr, smival.value.string.len);
    tmp[smival.value.string.len] = static_cast<SmiBYTE> (c);
    if ( smival.value.string.ptr )      // delete the original
      reset_octet_smi(smival);

    smival.value.string.ptr = tmp.release ();
    smival.value.string.len++;          // up the len
    return *this;
}

//================[ compare n elements of an Octet ]==================
int OctetStr::left_comparison (long n, OctetStr const &o) const
{
   long z {};
   long w {};

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
      ++z;
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
size_t OctetStr::length () const
{
   return (size_t) smival.value.string.len;
}

//================[ operator[]: access as if array ]==================
SmiBYTE& OctetStr::operator[]( int position)
{
    return  smival.value.string.ptr[position];
}

//===============[ reuturns pointer to internal data ]===============
SmiBYTE *OctetStr::data () const
{
   return smival.value.string.ptr;
}

//================[ returns validity ]================================
int OctetStr::valid() const
{
   return validity;
}

//================[ clone() ]=========================================
SnmpSyntax *OctetStr::clone () const
{
  return new OctetStr (*this);
}

//================[ ASCII format return ]=============================
char const *OctetStr::to_string ()
{
  for ( unsigned long i=0; i < smival.value.string.len; i++) {
    if (( smival.value.string.ptr[i] != '\r')&&
                ( smival.value.string.ptr[i] != '\n')&&
                (ACE_OS::ace_isprint((int) (smival.value.string.ptr[i]))==0))
         return(to_string_hex());
  }

  std::unique_ptr<char[]> new_buffer {new (std::nothrow) char[smival.value.string.len + 1]};
  if (!new_buffer)
    {
      errno = ENOMEM;
      return "";
    }
  if (smival.value.string.len)
    std::memcpy (new_buffer.get (), smival.value.string.ptr, smival.value.string.len);
  new_buffer[smival.value.string.len] = '\0';
  output_buffer = std::move (new_buffer);
  return output_buffer.get ();
}

//================[ general Value = operator ]========================
SnmpSyntax &OctetStr::operator= (SnmpSyntax &val)
{
   // protect against assignment from self
   if (this == std::addressof (val))
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
        set_data (static_cast<OctetStr &> (val).smival.value.string.ptr,
                  static_cast<OctetStr &> (val).smival.value.string.len);
        break;
    }
  }
  return *this;
}

//================[ format the output into hex ]========================
char const *OctetStr::to_string_hex ()
{
  int cnt {};
  char char_buf[80] {};           // holds ASCII representation of data
  char *buf_ptr {};
  char *line_ptr {};
  int storageNeeded {};
  int local_len = static_cast<int> (smival.value.string.len);
  SmiBYTE *bytes = smival.value.string.ptr;

  storageNeeded = (int) ((smival.value.string.len/16)+1) * 72 + 1;

  std::unique_ptr<char[]> new_buffer {new (std::nothrow) char[storageNeeded]};
  if (!new_buffer)
    {
      errno = ENOMEM;
      return "";
    }

  line_ptr = new_buffer.get ();
  line_ptr[0] = '\0';

    /*----------------------------------------*/
    /* processing loop for entire data buffer */
    /*----------------------------------------*/
  while (local_len > 0) {
    cnt      = 16;        /* print 16 bytes per line */
    buf_ptr  = char_buf;
    std::sprintf (line_ptr, "  ");
    line_ptr += 2;  /* indent */

      /*-----------------------*/
      /* process a single line */
      /*-----------------------*/
    while (cnt-- > 0 && local_len-- > 0) {
      std::sprintf (line_ptr, "%2.2X ", static_cast<unsigned int> (*bytes));

      line_ptr +=3;   /* the display of a byte always 3 chars long */
      if (ACE_OS::ace_isprint(*bytes))
        std::sprintf (buf_ptr, "%c", *bytes);
      else
        std::sprintf (buf_ptr, ".");
      ++bytes;
      ++buf_ptr;
    }
    ++cnt;

       /*----------------------------------------------------------*/
       /* this is to make sure that the ASCII displays line up for */
       /* incomplete lines of hex                                  */
       /*----------------------------------------------------------*/
    while (cnt-- > 0) {
      std::sprintf (line_ptr, "   ");
      line_ptr += 3;
    }

      /*------------------------------------------*/
      /* append the ASCII display to the Hex line */
      /*------------------------------------------*/

// TODO: verify ACE_OS:: on NT works like this or not

#if defined(ACE_WIN32)
  char const *fmt = "   %s\r\n";
#else
    char const *fmt = "   %s\n";
#endif // ACE_WIN32

    std::sprintf (line_ptr, fmt, char_buf);
    line_ptr += 3 + std::strlen (char_buf);
  }

  output_buffer = std::move (new_buffer);
  return output_buffer.get ();
}
