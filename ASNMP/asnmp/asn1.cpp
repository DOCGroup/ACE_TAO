
//=============================================================================
/**
 *  @file     asn1.cpp
 *
 *  $Id$
 *
 *  The Vb class is an encapsulation of the snmp variable binding.
 * This module contains the class definition for the variable binding (VB)
 * class. The VB class is an encapsulation of a SNMP VB. A VB object is
 * composed of one SNMP++ Oid and one SMI value. The Vb class utilizes Oid
 * objects and thus requires the Oid class. To use this class,
 * set oid, value then call valid() to be sure object was constructed correctly.
 *
 *
 *  @author S. Waldbusser (assumed)Michael R MacFaden  mrm@cisco.com - rework & ACE port
 */
//=============================================================================

/**********************************************************************
// *
   * Abstract Syntax Notation One, ASN.1
   * As defined in ISO/IS 8824 and ISO/IS 8825
   * This implements a subset of the above International Standards that
   * is sufficient to implement SNMP.
   *
   * Encodes abstract data types into a machine independent stream of bytes.
   *
   Copyright 1988, 1989, 1991, 1992 by Carnegie Mellon University

   All Rights Reserved

   Permission to use, copy, modify, and distribute this software and its
   documentation for any purpose and without fee is hereby granted,
   provided that the above copyright notice appear in all copies and that
   both that copyright notice and this permission notice appear in
   supporting documentation, and that the name of CMU not be
   used in advertising or publicity pertaining to distribution of the
   software without specific, written prior permission.

   CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
   ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
   CMU BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
   ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
   WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
   ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
   SOFTWARE.
   ******************************************************************/

#include "asnmp/asn1.h"
#include "asnmp/snmp.h"
#include "ace/OS_NS_string.h"

/*
 * parse_int - pulls a long out of an ASN int type.
 *  On entry, datalength is input as the number of valid bytes following
 *   "data".  On exit, it is returned as the number of valid bytes
 *   following the end of this object.
 *
 *  Returns a pointer to the first byte past the end
 *   of this object (i.e. the start of the next object).
 *  Returns 0 on any error.
 */
u_char * asn1::parse_int( u_char *data,
                              int *datalength,
                              u_char *type,
                              long int *intp,
                              int intsize)
{
  ACE_TRACE("asn1::parse_int");
  /*
   * ASN.1 integer ::= 0x02 asnlength byte {byte}*
   */
  u_char *bufp = data;
  u_long            asn_length;
  long   value = 0;

  if (intsize != sizeof (long)){
    ASNERROR("not long");
    return 0;
  }
  *type = *bufp++;
  bufp =asn1::parse_length(bufp, &asn_length);
  if (bufp == 0){
    ASNERROR("bad length");
    return 0;
  }
  if ((int)(asn_length + (bufp - data)) > *datalength){
    ASNERROR("overflow of message");
    return 0;
  }
  if ((int)asn_length > intsize){
    ASNERROR("I don't support such large integers");
    return 0;
  }
  *datalength -= (int)asn_length + (bufp - data);
  if (*bufp & 0x80)
    value = -1; /* integer is negative */
  while(asn_length--)
    value = (value << 8) | *bufp++;
  *intp = value;
  return bufp;
}


/*
 * parse_unsigned_int - pulls an u_long out of an ASN int type.
 *  On entry, datalength is input as the number of valid bytes following
 *   "data".  On exit, it is returned as the number of valid bytes
 *   following the end of this object.
 *
 *  Returns a pointer to the first byte past the end
 *   of this object (i.e. the start of the next object).
 *  Returns 0 on any error.
 */
u_char * asn1::parse_unsigned_int( u_char *data,
                                       int *datalength,
                                       u_char *type,
                                       u_long *intp,
                                       int      intsize)
{
  ACE_TRACE("asn1::parse_unsigned_int");
  /*
   * ASN.1 integer ::= 0x02 asnlength byte {byte}*
   */
  u_char *bufp = data;
  u_long            asn_length;
  u_long value = 0;

  if (intsize != sizeof (long)){
    ASNERROR("not long");
    return 0;
  }
  *type = *bufp++;
  bufp = asn1::parse_length(bufp, &asn_length);
  if (bufp == 0){
    ASNERROR("bad length");
    return 0;
  }
  if ((int)(asn_length + (bufp - data)) > *datalength){
    ASNERROR("overflow of message");
    return 0;
  }
  if (((int)asn_length > (intsize + 1)) ||
      (((int)asn_length == intsize + 1) && *bufp != 0x00)){
    ASNERROR("I don't support such large integers");
    return 0;
  }
  *datalength -= (int)asn_length + (bufp - data);
  if (*bufp & 0x80)
    value = (u_long) -1;
  while(asn_length--)
    value = (value << 8) | *bufp++;
  *intp = value;
  return bufp;
}


/*
 * build_int - builds an ASN object containing an integer.
 *  On entry, datalength is input as the number of valid bytes following
 *   "data".  On exit, it is returned as the number of valid bytes
 *   following the end of this object.
 *
 *  Returns a pointer to the first byte past the end
 *   of this object (i.e. the start of the next object).
 *  Returns 0 on any error.
 */
u_char * asn1::build_int( u_char *data,
                              int *datalength,
                              u_char type,
                              long *intp,
                              int intsize)
{
  ACE_TRACE("asn1::build_int");
  /*
   * ASN.1 integer ::= 0x02 asnlength byte {byte}*
   */

  long integer;
  u_long mask;

  if (intsize != sizeof (long))
    return 0;
  integer = *intp;
  /*
   * Truncate "unnecessary" bytes off of the most significant end of this
   * 2's complement integer.  There should be no sequence of 9
   * consecutive 1's or 0's at the most significant end of the
   * integer.
   */
  mask = u_long (0x1FF) << ((8 * (sizeof(u_long) - 1)) - 1);
  /* mask is 0xFF800000 on a big-endian machine */
  while((((integer & mask) == 0) || ((integer & mask) == mask))
        && intsize > 1){
    intsize--;
    integer <<= 8;
  }
  data = asn1::build_header(data, datalength, type, intsize);
  if (data == 0)
    return 0;
  if (*datalength < intsize)
    return 0;
  *datalength -= intsize;
  mask = u_long (0xFF) << (8 * (sizeof(u_long) - 1));
  /* mask is 0xFF000000 on a big-endian machine */
  while(intsize--){
    *data++ = (u_char)((integer & mask) >> (8 * (sizeof(long) - 1)));
    integer <<= 8;
  }
  return data;
}


/*
 * build_unsigned_int - builds an ASN object containing an integer.
 *  On entry, datalength is input as the number of valid bytes following
 *   "data".  On exit, it is returned as the number of valid bytes
 *   following the end of this object.
 *
 *  Returns a pointer to the first byte past the end
 *   of this object (i.e. the start of the next object).
 *  Returns 0 on any error.
 */
u_char * asn1::build_unsigned_int( u_char *data,
                                       int *datalength,
                                       u_char type,
                                       u_long *intp,
                                       int intsize)
{
  ACE_TRACE("asn1::build_unsigned_int");
  /*
   * ASN.1 integer ::= 0x02 asnlength byte {byte}*
   */

  u_long integer;
  u_long mask;
  int add_null_byte = 0;

  if (intsize != sizeof (long))
    return 0;
  integer = *intp;
  mask = u_long (0xFF) << (8 * (sizeof(u_long) - 1));
  /* mask is 0xFF000000 on a big-endian machine */
  if ((u_char)((integer & mask) >> (8 * (sizeof(long) - 1))) & 0x80){
    /* if MSB is set */
    add_null_byte = 1;
    intsize++;
  }
  /*
   * Truncate "unnecessary" bytes off of the most significant end of this 2's complement integer.
   * There should be no sequence of 9 consecutive 1's or 0's at the most significant end of the
   * integer.
   */
  mask = u_long (0x1FF) << ((8 * (sizeof(u_long) - 1)) - 1);
  /* mask is 0xFF800000 on a big-endian machine */
  while((((integer & mask) == 0) || ((integer & mask) == mask)) && intsize > 1){
    intsize--;
    integer <<= 8;
  }
  data = asn1::build_header(data, datalength, type, intsize);
  if (data == 0)
    return 0;
  if (*datalength < intsize)
    return 0;
  *datalength -= intsize;
  if (add_null_byte == 1){
    *data++ = '\0';
    intsize--;
  }
  mask = u_long (0xFF) << (8 * (sizeof(u_long) - 1));
  /* mask is 0xFF000000 on a big-endian machine */
  while(intsize--){
    *data++ = (u_char)((integer & mask) >> (8 * (sizeof(long) - 1)));
    integer <<= 8;
  }
  return data;
}


/*
 * parse_string - pulls an octet string out of an ASN octet string type.
 *  On entry, datalength is input as the number of valid bytes following
 *   "data".  On exit, it is returned as the number of valid bytes
 *   following the beginning of the next object.
 *
 *  "string" is filled with the octet string.
 *
 *  Returns a pointer to the first byte past the end
 *   of this object (i.e. the start of the next object).
 *  Returns 0 on any error.
 */
u_char * asn1::parse_string( u_char     *data,
                                 int *datalength,
                                 u_char *type,
                                 u_char *string,
                                 int *strlength)
{
  ACE_TRACE("asn1::parse_string");
  /*
   * ASN.1 octet string ::= primstring | cmpdstring
   * primstring ::= 0x04 asnlength byte {byte}*
   * cmpdstring ::= 0x24 asnlength string {string}*
   */
  u_char *bufp = data;
  u_long            asn_length;

  *type = *bufp++;
  bufp = asn1::parse_length(bufp, &asn_length);
  if (bufp == 0)
    return 0;
  if ((int)(asn_length + (bufp - data)) > *datalength){
    ASNERROR("overflow of message");
    return 0;
  }
  if ((int)asn_length > *strlength){
    ASNERROR("I don't support such long strings");
    return 0;
  }
  // fixed
  ACE_OS::memcpy((char *)string, (char *)bufp,  (int)asn_length);
  *strlength = (int)asn_length;
  *datalength -= (int)asn_length + (bufp - data);
  return bufp + asn_length;
}


/*
 * build_string - Builds an ASN octet string object containing the input string.
 *  On entry, datalength is input as the number of valid bytes following
 *   "data".  On exit, it is returned as the number of valid bytes
 *   following the beginning of the next object.
 *
 *  Returns a pointer to the first byte past the end
 *   of this object (i.e. the start of the next object).
 *  Returns 0 on any error.
 */
u_char * asn1::build_string( u_char *data,
                                 int *datalength,
                                 u_char type,
                                 u_char *string,
                                 int strlength)
{
  ACE_TRACE("asn1::build_string");
  /*
   * ASN.1 octet string ::= primstring | cmpdstring
   * primstring ::= 0x04 asnlength byte {byte}*
   * cmpdstring ::= 0x24 asnlength string {string}*
   * This code will never send a compound string.
   */
  data = asn1::build_header(data, datalength, type, strlength);
  if (data == 0)
    return 0;
  if (*datalength < strlength)
    return 0;
  // fixed
  ACE_OS::memcpy((u_char *)data,(u_char *)string, strlength);
  *datalength -= strlength;
  return data + strlength;
}


/*
 * parse_header - interprets the ID and length of the current object.
 *  On entry, datalength is input as the number of valid bytes following
 *   "data".  On exit, it is returned as the number of valid bytes
 *   in this object following the id and length.
 *
 *  Returns a pointer to the first byte of the contents of this object.
 *  Returns 0 on any error.
 */
u_char *asn1::parse_header( u_char *data,
                                int *datalength,
                                u_char *type)
{
  ACE_TRACE("asn1::parse_header");
  u_char *bufp = data;
  register int header_len;
  u_long            asn_length;

  /* this only works on data types < 30, i.e. no extension octets */
  if (IS_EXTENSION_ID(*bufp)){
    ASNERROR("can't process ID >= 30");
    return 0;
  }
  *type = *bufp;
  bufp = asn1::parse_length(bufp + 1, &asn_length);
  if (bufp == 0)
    return 0;
  header_len = bufp - data;
  if ((int)(header_len + asn_length) > *datalength){
    ASNERROR("asn length too long");
    return 0;
  }
  *datalength = (int)asn_length;
  return bufp;
}

/*
 * asn1::build_header - builds an ASN header for an object with the ID and
 * length specified.
 *  On entry, datalength is input as the number of valid bytes following
 *   "data".  On exit, it is returned as the number of valid bytes
 *   in this object following the id and length.
 *
 *  This only works on data types < 30, i.e. no extension octets.
 *  The maximum length is 0xFFFF;
 *
 *  Returns a pointer to the first byte of the contents of this object.
 *  Returns 0 on any error.
 */
u_char * asn1::build_header( u_char *data,
                                 int *datalength,
                                 u_char type,
                                 int length)
{
  ACE_TRACE("asn1::build_header");
  if (*datalength < 1)
    return 0;
  *data++ = type;
  (*datalength)--;
  return asn1::build_length(data, datalength, length);

}

/*
 * asn_build_sequence - builds an ASN header for a sequence with the ID and
 * length specified.
 *  On entry, datalength is input as the number of valid bytes following
 *   "data".  On exit, it is returned as the number of valid bytes
 *   in this object following the id and length.
 *
 *  This only works on data types < 30, i.e. no extension octets.
 *  The maximum length is 0xFFFF;
 *
 *  Returns a pointer to the first byte of the contents of this object.
 *  Returns 0 on any error.
 */
u_char * asn1::build_sequence( u_char *data,
                                   int *datalength,
                                   u_char type,
                                   int length)
{
  ACE_TRACE("asn1::build_sequence");
  *datalength -= 4;
  if (*datalength < 0){
    *datalength += 4;   /* fix up before punting */
    return 0;
  }
  *data++ = type;
  *data++ = (u_char)(0x02 | ASN_LONG_LEN);
  *data++ = (u_char)((length >> 8) & 0xFF);
  *data++ = (u_char)(length & 0xFF);
  return data;
}

/*
 * parse_length - interprets the length of the current object.
 *  On exit, length contains the value of this length field.
 *
 *  Returns a pointer to the first byte after this length
 *  field (aka: the start of the data field).
 *  Returns 0 on any error.
 */
u_char * asn1::parse_length( u_char *data,
                                 u_long  *length)
{
  ACE_TRACE("asn1::parse_length");
  u_char lengthbyte = *data;

  if (lengthbyte & ASN_LONG_LEN){
    lengthbyte &= ~ASN_LONG_LEN;        /* turn MSb off */
    if (lengthbyte == 0){
      ASNERROR("We don't support indefinite lengths");
      return 0;
    }
    if (lengthbyte > sizeof(long)){
      ASNERROR("we can't support data lengths that long");
      return 0;
    }
    // fixed
    ACE_OS::memcpy((char *)length, (char *)data + 1, (int)lengthbyte);
    *length = ACE_NTOHL(*length);
    *length >>= (8 * ((sizeof *length) - lengthbyte));
    return data + lengthbyte + 1;
  } else { /* short asnlength */
    *length = (long)lengthbyte;
    return data + 1;
  }
}

u_char *asn1::build_length( u_char *data,
                                int *datalength,
                                int length)
{
  ACE_TRACE("asn1::build_length");
  u_char    *start_data = data;

  /* no indefinite lengths sent */
  if (length < 0x80){
    if (*datalength < 1){
      ASNERROR("build_length");
      return 0;
    }
    *data++ = (u_char)length;
  } else if (length <= 0xFF){
    if (*datalength < 2){
      ASNERROR("build_length");
      return 0;
    }
    *data++ = (u_char)(0x01 | ASN_LONG_LEN);
    *data++ = (u_char)length;
  } else { /* 0xFF < length <= 0xFFFF */
    if (*datalength < 3){
      ASNERROR("build_length");
      return 0;
    }
    *data++ = (u_char)(0x02 | ASN_LONG_LEN);
    *data++ = (u_char)((length >> 8) & 0xFF);
    *data++ = (u_char)(length & 0xFF);
  }
  *datalength -= (data - start_data);
  return data;

}

/*
 * parse_objid - pulls an object indentifier out of an ASN object identifier type.
 *  On entry, datalength is input as the number of valid bytes following
 *   "data".  On exit, it is returned as the number of valid bytes
 *   following the beginning of the next object.
 *
 *  "objid" is filled with the object identifier.
 *
 *  Returns a pointer to the first byte past the end
 *   of this object (i.e. the start of the next object).
 *  Returns 0 on any error.
 */
u_char *asn1::parse_objid( u_char *data,
                               int *datalength,
                               u_char *type,
                               oid *objid,
                               int *objidlength)
{
  ACE_TRACE("asn1::parse_objid");
  /*
   * ASN.1 objid ::= 0x06 asnlength subidentifier {subidentifier}*
   * subidentifier ::= {leadingbyte}* lastbyte
   * leadingbyte ::= 1 7bitvalue
   * lastbyte ::= 0 7bitvalue
   */
  u_char *bufp = data;
  oid *oidp = objid + 1;
  u_long subidentifier;
  long   length;
  u_long            asn_length;

  *type = *bufp++;
  bufp = asn1::parse_length(bufp, &asn_length);
  if (bufp == 0)
    return 0;
  if ((int)asn_length + (bufp - data) > *datalength){
    ASNERROR("overflow of message");
    return 0;
  }
  *datalength -= (int)asn_length + (bufp - data);

  /* Handle invalid object identifier encodings of the form 06 00 robustly */
  if (asn_length == 0)
    objid[0] = objid[1] = 0;

  length = asn_length;
  (*objidlength)--;     /* account for expansion of first byte */
  while (length > 0 && (*objidlength)-- > 0){
    subidentifier = 0;
    do {        /* shift and add in low order 7 bits */
      subidentifier = (subidentifier << 7) + (*(u_char *)bufp & ~ASN_BIT8);
      length--;
    } while (*(u_char *)bufp++ & ASN_BIT8);     /* last byte has high bit clear */
    if (subidentifier > (u_long)MAX_SUBID){
      ASNERROR("subidentifier too long");
      return 0;
    }
    *oidp++ = (oid)subidentifier;
  }

  /*
   * The first two subidentifiers are encoded into the first component
   * with the value (X * 40) + Y, where:
   *    X is the value of the first subidentifier.
   *  Y is the value of the second subidentifier.
   */
  subidentifier = (u_long)objid[1];
  if (subidentifier == 0x2B){
    objid[0] = 1;
    objid[1] = 3;
  } else {
    objid[1] = (u_char)(subidentifier % 40);
    objid[0] = (u_char)((subidentifier - objid[1]) / 40);
  }

  *objidlength = (int)(oidp - objid);
  return bufp;
}

/*
 * build_objid - Builds an ASN object identifier object containing the
 * input string.
 *  On entry, datalength is input as the number of valid bytes following
 *   "data".  On exit, it is returned as the number of valid bytes
 *   following the beginning of the next object.
 *
 *  Returns a pointer to the first byte past the end
 *   of this object (i.e. the start of the next object).
 *  Returns 0 on any error.
 */
u_char *asn1::build_objid( u_char *data,
                               int *datalength,
                               u_char type,
                               oid *objid,
                               int objidlength)
{
  ACE_TRACE("asn1::build_objid");
  /*
   * ASN.1 objid ::= 0x06 asnlength subidentifier {subidentifier}*
   * subidentifier ::= {leadingbyte}* lastbyte
   * leadingbyte ::= 1 7bitvalue
   * lastbyte ::= 0 7bitvalue
   */
  u_char buf[MAX_OID_LEN];
  u_char *bp = buf;
  oid *op = objid;
  int    asnlength;
  u_long subid, mask, testmask;
  int bits, testbits;

  if (objidlength < 2){
    *bp++ = 0;
    objidlength = 0;
  } else {
    *bp++ = (u_char) (op[1] + (op[0] * 40));
    objidlength -= 2;
    op += 2;
  }

  while(objidlength-- > 0){
    subid = *op++;
    if (subid < 127){ /* off by one? */
      *bp++ = (u_char )subid;
    } else {
      mask = 0x7F; /* handle subid == 0 case */
      bits = 0;
      /* testmask *MUST* !!!! be of an u_type */
      for(testmask = 0x7F, testbits = 0; testmask != 0;
          testmask <<= 7, testbits += 7){
        if (subid & testmask){  /* if any bits set */
          mask = testmask;
          bits = testbits;
        }
      }
      /* mask can't be zero here */
      for(;mask != 0x7F; mask >>= 7, bits -= 7){
        /* fix a mask that got truncated above */
        if (mask == 0x1E00000)
          mask = 0xFE00000;
        *bp++ = (u_char)(((subid & mask) >> bits) | ASN_BIT8);
      }
      *bp++ = (u_char)(subid & mask);
    }
  }
  asnlength = bp - buf;
  data = asn1::build_header(data, datalength, type, asnlength);
  if (data == 0)
    return 0;
  if (*datalength < asnlength)
    return 0;
  // fixed
  ACE_OS::memcpy((char *)data, (char *)buf,  asnlength);
  *datalength -= asnlength;
  return data + asnlength;
}

/*
 * parse_null - Interprets an ASN null type.
 *  On entry, datalength is input as the number of valid bytes following
 *   "data".  On exit, it is returned as the number of valid bytes
 *   following the beginning of the next object.
 *
 *  Returns a pointer to the first byte past the end
 *   of this object (i.e. the start of the next object).
 *  Returns 0 on any error.
 */
u_char *asn1::parse_null(u_char *data,
                                int *datalength,
                                u_char *type)
{
  ACE_TRACE("asn1::parse_null");
  /*
   * ASN.1 null ::= 0x05 0x00
   */
  u_char   *bufp = data;
  u_long            asn_length;

  *type = *bufp++;
  bufp = asn1::parse_length(bufp, &asn_length);
  if (bufp == 0)
    return 0;
  if (asn_length != 0){
    ASNERROR("Malformed 0");
    return 0;
  }
  *datalength -= (bufp - data);
  return bufp + asn_length;
}


/*
 * build_null - Builds an ASN null object.
 *  On entry, datalength is input as the number of valid bytes following
 *   "data".  On exit, it is returned as the number of valid bytes
 *   following the beginning of the next object.
 *
 *  Returns a pointer to the first byte past the end
 *   of this object (i.e. the start of the next object).
 *  Returns 0 on any error.
 */
u_char *asn1::build_null( u_char *data,
                              int *datalength,
                              u_char type)
{
  ACE_TRACE("asn1::build_null");
  /*
   * ASN.1 null ::= 0x05 0x00
   */
  return asn1::build_header(data, datalength, type, 0);
}

/*
 * parse_bitstring - pulls a bitstring out of an ASN bitstring type.
 *  On entry, datalength is input as the number of valid bytes following
 *   "data".  On exit, it is returned as the number of valid bytes
 *   following the beginning of the next object.
 *
 *  "string" is filled with the bit string.
 *
 *  Returns a pointer to the first byte past the end
 *   of this object (i.e. the start of the next object).
 *  Returns 0 on any error.
 */
u_char *asn1::parse_bitstring( u_char *data,
                                   int *datalength,
                                   u_char *type,
                                   u_char *string,
                                   int *strlength)
{
  ACE_TRACE("asn1::parse_bitstring");
  /*
   * bitstring ::= 0x03 asnlength unused {byte}*
   */
  u_char *bufp = data;
  u_long            asn_length;

  *type = *bufp++;
  bufp = asn1::parse_length(bufp, &asn_length);
  if (bufp == 0)
    return 0;
  if ((int)(asn_length + (bufp - data)) > *datalength){
    ASNERROR("overflow of message");
    return 0;
  }
  if ((int) asn_length > *strlength){
    ASNERROR("I don't support such long bitstrings");
    return 0;
  }
  if (asn_length < 1){
    ASNERROR("Invalid bitstring");
    return 0;
  }
  if (*bufp > 7){
    ASNERROR("Invalid bitstring");
    return 0;
  }
  // fixed
  ACE_OS::memcpy((char *)string,(char *)bufp,  (int)asn_length);
  *strlength = (int)asn_length;
  *datalength -= (int)asn_length + (bufp - data);
  return bufp + asn_length;
}


/*
 * build_bitstring - Builds an ASN bit string object containing the
 * input string.
 *  On entry, datalength is input as the number of valid bytes following
 *   "data".  On exit, it is returned as the number of valid bytes
 *   following the beginning of the next object.
 *
 *  Returns a pointer to the first byte past the end
 *   of this object (i.e. the start of the next object).
 *  Returns 0 on any error.
 */
u_char *asn1::build_bitstring( u_char *data,
                                   int *datalength,
                                   u_char type,
                                   u_char *string,
                                   int strlength)
{
  ACE_TRACE("asn1::build_bitstring");
  /*
   * ASN.1 bit string ::= 0x03 asnlength unused {byte}*
   */
  if (strlength < 1 || *string || *string > 7){
    ASNERROR("Building invalid bitstring");
    return 0;
  }
  data = asn1::build_header(data, datalength, type, strlength);
  if (data == 0)
    return 0;
  if (*datalength < strlength)
    return 0;
  // fixed
  ACE_OS::memcpy((char *)data,(char *)string, strlength);
  *datalength -= strlength;
  return data + strlength;
}


/*
 * parse_unsigned_int64 - pulls a 64 bit u_long out of an ASN int
 * type.
 *  On entry, datalength is input as the number of valid bytes following
 *   "data".  On exit, it is returned as the number of valid bytes
 *   following the end of this object.
 *
 *  Returns a pointer to the first byte past the end
 *   of this object (i.e. the start of the next object).
 *  Returns 0 on any error.
 */
u_char * asn1::parse_unsigned_int64(u_char *data,
                                         int *datalength,
                                         u_char *type,
                                         struct counter64 *cp,
                                         int countersize)
{
  ACE_TRACE("asn1::parse_unsigned_int64");
  /*
   * ASN.1 integer ::= 0x02 asnlength byte {byte}*
   */
  u_char *bufp = data;
  u_long            asn_length;
  u_long low = 0, high = 0;
  int intsize = 4;

  if (countersize != sizeof(struct counter64)){
    ASNERROR("not right size");
    return 0;
  }
  *type = *bufp++;
  bufp = asn1::parse_length(bufp, &asn_length);
  if (bufp == 0){
    ASNERROR("bad length");
    return 0;
  }
  if ((int)(asn_length + (bufp - data)) > *datalength){
    ASNERROR("overflow of message");
    return 0;
  }
  if (((int)asn_length > (intsize * 2 + 1)) ||
      (((int)asn_length == (intsize * 2) + 1) && *bufp != 0x00)){
    ASNERROR("I don't support such large integers");
    return 0;
  }
  *datalength -= (int)asn_length + (bufp - data);
  if (*bufp & 0x80){
    low = (u_long) -1; // integer is negative
    high = (u_long) -1;
  }
  while(asn_length--){
    high = (high << 8) | ((low & 0xFF000000) >> 24);
    low = (low << 8) | *bufp++;
  }
  cp->low = low;
  cp->high = high;
  return bufp;
}


/*
 * build_unsigned_int64 - builds an ASN object containing a 64 bit integer.
 *  On entry, datalength is input as the number of valid bytes following
 *   "data".  On exit, it is returned as the number of valid bytes
 *   following the end of this object.
 *
 *  Returns a pointer to the first byte past the end
 *   of this object (i.e. the start of the next object).
 *  Returns 0 on any error.
 */
u_char * asn1::build_unsigned_int64( u_char *data,
                                         int *datalength,
                                         u_char type,
                                         struct counter64 *cp,
                                         int countersize)
{
  ACE_TRACE("asn1::build_unsigned_int64");
  /*
   * ASN.1 integer ::= 0x02 asnlength byte {byte}*
   */

  u_long low, high;
  u_long mask, mask2;
  int add_null_byte = 0;
  int intsize;

  if (countersize != sizeof (struct counter64))
    return 0;
  intsize = 8;
  low = cp->low;
  high = cp->high;
  mask = u_long (0xFF) << (8 * (sizeof(u_long) - 1));
  /* mask is 0xFF000000 on a big-endian machine */
  if ((u_char)((high & mask) >> (8 * (sizeof(long) - 1))) & 0x80){
    /* if MSB is set */
    add_null_byte = 1;
    intsize++;
  }
  /*
   * Truncate "unnecessary" bytes off of the most significant end of this 2's
   * complement integer.
   * There should be no sequence of 9 consecutive 1's or 0's at the most
   * significant end of the integer.
   */
  mask2 = u_long (0x1FF) << ((8 * (sizeof(u_long) - 1)) - 1);
  /* mask2 is 0xFF800000 on a big-endian machine */
  while((((high & mask2) == 0) || ((high & mask2) == mask2))
        && intsize > 1){
    intsize--;
    high = (high << 8)
      | ((low & mask) >> (8 * (sizeof(long) - 1)));
    low <<= 8;
  }
  data = asn1::build_header(data, datalength, type, intsize);
  if (data == 0)
    return 0;
  if (*datalength < intsize)
    return 0;
  *datalength -= intsize;
  if (add_null_byte == 1){
    *data++ = '\0';
    intsize--;
  }
  while(intsize--){
    *data++ = (u_char)((high & mask) >> (8 * (sizeof(long) - 1)));
    high = (high << 8)
      | ((low & mask) >> (8 * (sizeof(long) - 1)));
    low <<= 8;

  }
  return data;
}


// create a pdu
struct snmp_pdu * cmu_snmp::pdu_create( int command)
{
  ACE_TRACE("cmu_snmp::snmp_pdu_create");
  struct snmp_pdu *pdu;

  ACE_NEW_RETURN(pdu, snmp_pdu, 0);
  ACE_OS::memset((char *)pdu, 0,sizeof(struct snmp_pdu));
  pdu->command = command;
  pdu->errstat = 0;
  pdu->errindex = 0;
  pdu->enterprise = 0;
  pdu->enterprise_length = 0;
  pdu->variables = 0;
  return pdu;
}

// release a pdu from memory
void cmu_snmp::free_pdu( struct snmp_pdu *pdu)
{
  ACE_TRACE("cmu_snmp::free_pdu");
  struct variable_list *vp, *ovp;

  vp = pdu->variables;
  while(vp){
    // release the oid part
    if (vp->name)
      delete [] vp->name;
    // if deep data, then release as well
    if (vp->val.string)
      delete [] vp->val.string;
    ovp = vp;
    // go to the next one
    vp = vp->next_variable;
    // release up vb itself
    delete ovp;
  }
  // if enterprise release it up
  if (pdu->enterprise)
    delete [] pdu->enterprise;
  // release up pdu itself
  delete pdu;
}


// add a null var to a pdu
void cmu_snmp::add_var(struct snmp_pdu *pdu,
                  oid *name,
                  int name_length,
                  SmiVALUE *smival)
{
  ACE_TRACE("cmu_snmp::add_var");

  struct variable_list *vars = 0;

  // if we don't have a vb list ,create one
  if (pdu->variables == 0)
    {
      ACE_NEW(pdu->variables, variable_list);
      vars = pdu->variables;
    }
  else
    { // we have one, find the end
      for (vars = pdu->variables; vars->next_variable; vars = vars->next_variable)
        {
          // Do nothing.
        }

      // create one
      ACE_NEW(vars->next_variable, variable_list);
      // bump ptr
      vars = vars->next_variable;
    }

  // add the oid with no data
  vars->next_variable = 0;

  // hook in the Oid portion
  ACE_NEW(vars->name, oid[(name_length)]);

  // fixed
  ACE_OS::memcpy((char *)vars->name,(char *)name, name_length * sizeof(oid));
  vars->name_length = name_length;

  // hook in the SMI value
  switch( smival->syntax)
    {
      // null , do nothing
    case sNMP_SYNTAX_NULL:
    case sNMP_SYNTAX_NOSUCHOBJECT:
    case sNMP_SYNTAX_NOSUCHINSTANCE:
    case sNMP_SYNTAX_ENDOFMIBVIEW:
      {
        vars->type = (u_char) smival->syntax;
        vars->val.string = 0;
        vars->val_len = 0;
      }
      break;

      // octects
    case sNMP_SYNTAX_OCTETS:
    case sNMP_SYNTAX_OPAQUE:
    case sNMP_SYNTAX_IPADDR:
      {
        vars->type = (u_char) smival->syntax;
        ACE_NEW(vars->val.string,
                u_char[(unsigned)smival->value.string.len]);
        vars->val_len = (int) smival->value.string.len;
        ACE_OS::memcpy( (u_char *) vars->val.string,
                       (u_char *) smival->value.string.ptr,
                       (unsigned) smival->value.string.len);
      }
      break;

      // oid
    case sNMP_SYNTAX_OID:
      {
        vars->type = (u_char) smival->syntax;
        vars->val_len = (int) smival->value.oid.len * sizeof(oid);
        ACE_NEW(vars->val.objid, oid[(unsigned)vars->val_len]);
        ACE_OS::memcpy((u_long *)vars->val.objid,
                       (u_long *)smival->value.oid.ptr,
                       (unsigned) vars->val_len);
      }
      break;



    case sNMP_SYNTAX_TIMETICKS:
    case sNMP_SYNTAX_CNTR32:
    case sNMP_SYNTAX_GAUGE32:
    case sNMP_SYNTAX_UINT32:
      {
        long templong;
        vars->type = (u_char) smival->syntax;
        ACE_NEW(vars->val.integer, long);
        vars->val_len = sizeof(long);
        templong = (long) smival->value.uNumber;
        ACE_OS::memcpy( (long*) vars->val.integer,
                       (long*) &templong,
                       sizeof(long));
      }
      break;

    case sNMP_SYNTAX_INT32:
      {
        long templong;
        vars->type = (u_char) smival->syntax;
        ACE_NEW(vars->val.integer, long);
        vars->val_len = sizeof(long);
        templong = (long) smival->value.sNumber;
        ACE_OS::memcpy( (long*) vars->val.integer,
                       (long*) &templong,
                       sizeof(long));
      }
      break;

      // 64 bit counter
    case sNMP_SYNTAX_CNTR64:
      {
        vars->type = ( u_char) smival->syntax;
        ACE_NEW(vars->val.counter64, counter64);
        vars->val_len = sizeof(struct counter64);
        ACE_OS::memcpy( (struct counter64*) vars->val.counter64,
                       (SmiLPCNTR64) &(smival->value.hNumber),
                       sizeof( SmiCNTR64));
      }
      break;

    } // end switch

}

// build the authentication
// works for v1 or v2c
u_char *cmu_snmp::auth_build( u_char *data,
                               int *length,
                               long int version,
                               u_char *community,
                               int community_len,
                               int messagelen)
{
  ACE_TRACE("cmu_snmp::auth_build");
  u_char *params;
  int     plen;

  params = community;
  plen = community_len;

  data = asn1::build_sequence(data,
                            length,
                            (u_char)(ASN_SEQUENCE | ASN_CONSTRUCTOR),
                            messagelen + plen + 5);
  if (data == 0){
    ASNERROR("buildheader");
    return 0;
  }
  data = asn1::build_int(data,
                       length,
                       (u_char)(ASN_UNIVERSAL | ASN_PRIMITIVE | ASN_INTEGER),
                       (long *)&version,
                       sizeof(version));
  if (data == 0){
    ASNERROR("buildint");
    return 0;
  }

  data = asn1::build_string(data,
                          length,
                          (u_char)(ASN_UNIVERSAL | ASN_PRIMITIVE | ASN_OCTET_STR),
                          params,
                          plen );
  if (data == 0){
    ASNERROR("buildstring");
    return 0;
  }

  return (u_char *)data;
}


// build a variable binding
u_char * cmu_snmp::build_var_op(u_char *data, oid * var_name,
                                  int *var_name_len,
                                  u_char var_val_type,
                                  int var_val_len, u_char *var_val,
                                  int *listlength)

{
  ACE_TRACE("cmu_snmp::build_var_op");
  int dummyLen, headerLen;
  u_char *dataPtr;

  dummyLen = *listlength;
  dataPtr = data;

  data += 4;
  dummyLen -=4;
  if (dummyLen < 0)
    return 0;

  headerLen = data - dataPtr;
  *listlength -= headerLen;
  data = asn1::build_objid( data, listlength,
                 (u_char)(ASN_UNIVERSAL | ASN_PRIMITIVE | ASN_OBJECT_ID),
                         var_name, *var_name_len);
  if (data == 0) {
    ASNERROR("");
    return 0;
  }

  // based on the type...
  switch(var_val_type) {
  case ASN_INTEGER:
    data = asn1::build_int( data, listlength, var_val_type, (long *)var_val,
                         var_val_len);
    break;

  case SMI_GAUGE:
  case SMI_COUNTER:
  case SMI_TIMETICKS:
  case SMI_UINTEGER:
    data = asn1::build_unsigned_int( data,
                                  listlength,
                                  var_val_type,
                                  (u_long *)var_val,
                                  var_val_len);
    break;

  case SMI_COUNTER64:
    data = asn1::build_unsigned_int64(data,
                                    listlength,
                                    var_val_type,
                                    (struct counter64 *)var_val,
                                    var_val_len);
    break;

  case ASN_OCTET_STR:
  case SMI_IPADDRESS:
  case SMI_OPAQUE:
  case SMI_NSAP:
    data = asn1::build_string(data, listlength, var_val_type,
                            var_val, var_val_len);
    break;

  case ASN_OBJECT_ID:
    data = asn1::build_objid(data, listlength, var_val_type,
                           (oid *)var_val, var_val_len / sizeof(oid));
    break;

  case ASN_NULL:
    data = asn1::build_null(data, listlength, var_val_type);
    break;

  case ASN_BIT_STR:
    data = asn1::build_bitstring(data, listlength, var_val_type,
                               var_val, var_val_len);
    break;

  case SNMP_NOSUCHOBJECT:
  case SNMP_NOSUCHINSTANCE:
  case SNMP_ENDOFMIBVIEW:
    data = asn1::build_null(data, listlength, var_val_type);
    break;

  default:
    ASNERROR("wrong type");
    return 0;
  }
  if (data == 0) {
    ASNERROR("");
    return 0;
  }
  dummyLen = (data - dataPtr) - headerLen;

  asn1::build_sequence(dataPtr, &dummyLen,
                     (u_char)(ASN_SEQUENCE | ASN_CONSTRUCTOR),
                     dummyLen);
  return data;
}


// serialize the pdu
int cmu_snmp::build( struct snmp_pdu *pdu, u_char *packet,
               int *out_length, long version,
               u_char* community, int community_len)
{
  ACE_TRACE("cmu_snmp::build");
  u_char buf[SNMP_MSG_LENGTH];
  u_char  *cp;
  struct variable_list *vp;
  int length;
  int totallength;

  length = *out_length;
  cp = packet;
  for(vp = pdu->variables; vp; vp = vp->next_variable) {
    cp = cmu_snmp::build_var_op( cp, vp->name,
                           &vp->name_length, vp->type,
                           vp->val_len, (u_char *)vp->val.string,
                           &length);
    if (cp == 0)
      return -1;
  }
  totallength = cp - packet;

  length = SNMP_MSG_LENGTH;

  // encode the total len
  cp = asn1::build_header( buf, &length,
                        (u_char)(ASN_SEQUENCE | ASN_CONSTRUCTOR),
                        totallength);
  if (cp == 0)
    return -1;
  ACE_OS::memcpy( (char *)cp, (char *)packet,totallength);
  totallength += cp - buf;

  length = *out_length;
  if (pdu->command != TRP_REQ_MSG) {

    // request id
    cp = asn1::build_int( packet,
                       &length,
               (u_char )(ASN_UNIVERSAL | ASN_PRIMITIVE | ASN_INTEGER),
                       (long *)&pdu->reqid,
                       sizeof(pdu->reqid));
    if (cp == 0)
      return -1;

    // error status
    cp = asn1::build_int(cp,
                       &length,
               (u_char)(ASN_UNIVERSAL | ASN_PRIMITIVE | ASN_INTEGER),
                       (long *)&pdu->errstat, sizeof(pdu->errstat));
    if (cp == 0)
      return -1;

    // error index
    cp = asn1::build_int(cp,
                       &length,
               (u_char)(ASN_UNIVERSAL | ASN_PRIMITIVE | ASN_INTEGER),
                       (long *)&pdu->errindex, sizeof(pdu->errindex));
    if (cp == 0)
      return -1;
  }
  else {        // this is a trap message

    // enterprise
    cp = asn1::build_objid( packet,
                         &length,
                 (u_char)(ASN_UNIVERSAL | ASN_PRIMITIVE | ASN_OBJECT_ID),
                         (oid *)pdu->enterprise,
                         pdu->enterprise_length);
    if (cp == 0)
      return -1;

    // agent-addr
    cp = asn1::build_string(cp,
                            &length,
                            // HDN Fixed to use correct tag
                            (u_char)SMI_IPADDRESS,
                            //(u_char)(ASN_UNIVERSAL | ASN_PRIMITIVE | ASN_OCTET_STR),
                          (u_char *)&pdu->agent_addr.sin_addr.s_addr,
                          sizeof(pdu->agent_addr.sin_addr.s_addr));
    if (cp == 0)
      return -1;

    long tmp (static_cast <long> (pdu->trap_type));
    // generic trap
    cp = asn1::build_int(cp,
                         &length,
                         (u_char)(ASN_UNIVERSAL | ASN_PRIMITIVE | ASN_INTEGER),
                         &tmp,
                         sizeof(pdu->trap_type));
    if (cp == 0)
      return -1;

    tmp = static_cast <long> (pdu->specific_type);
    // specific trap
    cp = asn1::build_int( cp,
                          &length,
                          (u_char)(ASN_UNIVERSAL | ASN_PRIMITIVE | ASN_INTEGER),
                          &tmp,
                          sizeof(pdu->specific_type));
    if (cp == 0)
      return -1;

    // timestamp
    cp = asn1::build_int(cp,
                       &length,
                         // HDN Fixed to use correct tag
                         (u_char)SMI_TIMETICKS,
                         //(u_char )(ASN_UNIVERSAL | ASN_PRIMITIVE | ASN_INTEGER),
                       (long *)&pdu->time,
                       sizeof(pdu->time));
    if (cp == 0)
      return -1;
  }

  if (length < totallength)
    return -1;
  // fixed
  ACE_OS::memcpy((char *)cp, (char *)buf, totallength);
  totallength += cp - packet;

  length = SNMP_MSG_LENGTH;
  cp = asn1::build_header(buf,
                        &length,
                        (u_char)pdu->command,
                        totallength);
  if (cp == 0)
    return -1;
  if (length < totallength)
    return -1;
  // fixed
  ACE_OS::memcpy((char *)cp, (char *)packet, totallength);
  totallength += cp - buf;

  length = *out_length;

  cp = cmu_snmp::auth_build( packet,
                       &length,
                       version,
                       community,
                       community_len,
                       totallength );
  if (cp == 0)
    return -1;
  if ((*out_length - (cp - packet)) < totallength)
    return -1;
  // fixed
  ACE_OS::memcpy((char *)cp, (char *)buf, totallength);
  totallength += cp - packet;
  *out_length = totallength;

  return 0;
}

// parse the authentication header
u_char *cmu_snmp::auth_parse(u_char *data,
                               int *length, u_char *sid,
                               int *slen, long  *version)
{
  ACE_TRACE("cmu_snmp::auth_parse");
  u_char type;

  // get the type
  data = asn1::parse_header( data, length, &type);
  if (data == 0){
    ASNERROR("bad header");
    return 0;
  }

  if (type != (ASN_SEQUENCE | ASN_CONSTRUCTOR)) {
    ASNERROR("wrong auth header type");
    return 0;
  }

  // get the version
  data = asn1::parse_int(data, length, &type, version, sizeof(*version));
  if (data == 0) {
    ASNERROR("bad parse of version");
    return 0;
  }

  // get the community name
  data = asn1::parse_string(data, length, &type, sid, slen);
  if (data == 0) {
    ASNERROR("bad parse of community");
    return 0;
  }

  return (u_char *)data;
}

/* u_char *data,  // IN - pointer to the start of object
  oid       *var_name,      // OUT - object id of variable
  int       *var_name_len,  // IN/OUT - length of variable name
  u_char  *var_val_type, // OUT - type of variable
                                 (int or octet string) (one byte)
  int       *var_val_len,     // OUT - length of variable
  u_char  **var_val,  // OUT - pointer to ASN1 encoded value of variable
*/

u_char *
cmu_snmp::parse_var_op( u_char *data, oid *var_name,
                       int *var_name_len,  u_char  *var_val_type,
                       int *var_val_len, u_char  **var_val,
                       int  *listlength)
{
  ACE_TRACE("cmu_snmp::parse_var_op");
  u_char var_op_type;
  int   var_op_len = *listlength;
  u_char *var_op_start = data;

  data = asn1::parse_header(data, &var_op_len, &var_op_type);
  if (data == 0){
    ASNERROR("");
    return 0;
  }
  if (var_op_type != (u_char)(ASN_SEQUENCE | ASN_CONSTRUCTOR))
    return 0;
  data = asn1::parse_objid(data, &var_op_len, &var_op_type, var_name, var_name_len);
  if (data == 0) {
    ASNERROR("");
    return 0;
  }
  if (var_op_type != (u_char)
                     (ASN_UNIVERSAL | ASN_PRIMITIVE | ASN_OBJECT_ID))
    return 0;
  *var_val = data;      /* save pointer to this object */
  /* find out what type of object this is */
  data = asn1::parse_header(data, &var_op_len, var_val_type);
  if (data == 0) {
    ASNERROR("");
    return 0;
  }
  *var_val_len = var_op_len;
  data += var_op_len;
  *listlength -= (int)(data - var_op_start);
  return data;
}



// build a pdu from a data and length
int cmu_snmp::parse( struct snmp_pdu *pdu,
               u_char  *data,
               u_char *community_name,
               u_long &community_len,
               snmp_version &spp_version,
               int length)
{
  ACE_TRACE("cmu_snmp::parse");
  u_char  msg_type;
  u_char  type;
  u_char  *var_val;
  long    version;
  int       len, four;
  u_char community[256];
  int community_length = 256;
  struct variable_list *vp = 0;
  oid       objid[MAX_NAME_LEN], *op;

  // authenticates message and returns length if valid
  data = cmu_snmp::auth_parse(data,
                         &length,
                         community,
                         &community_length,
                         &version);
  if (data == 0)
    return -1;

  // copy the returned community name
  ACE_OS::memcpy( (u_char *) community_name,
                 (u_char *) community,
                 community_length);
  community_len = (long) community_length;

  if( version != SNMP_VERSION_1 && version != SNMP_VERSION_2C ) {
    ASNERROR("Wrong version");
    return -1;
  }

  spp_version = (snmp_version) version;

  data = asn1::parse_header(data,
                          &length,
                          &msg_type);
  if (data == 0)
    return -1;
  pdu->command = msg_type;

  if (pdu->command != TRP_REQ_MSG){
    // get the rid
    data = asn1::parse_int(data,
                         &length, &type,
                         (long *)&pdu->reqid,
                         sizeof(pdu->reqid));
    if (data == 0)
      return -1;
    // get the error status
    data = asn1::parse_int(data,
                         &length,
                         &type,
                         (long *)&pdu->errstat,
                         sizeof(pdu->errstat));
    if (data == 0)
      return -1;
    // get the error index
    data = asn1::parse_int(data,
                         &length,
                         &type,
                         (long *)&pdu->errindex,
                         sizeof(pdu->errindex));
    if (data == 0)
      return -1;
  }
  else {  // is a trap

    // get the enterprise
    pdu->enterprise_length = MAX_NAME_LEN;
    data = asn1::parse_objid(data,
                           &length,
                           &type,
                           objid,
                           &pdu->enterprise_length);
    if (data == 0)
      return -1;

    ACE_NEW_RETURN(pdu->enterprise,
                   oid[pdu->enterprise_length*sizeof(oid)],-1);

    // fixed
    ACE_OS::memcpy((char *)pdu->enterprise,(char *)objid,
                   pdu->enterprise_length * sizeof(oid));

    // get source address
    four = 4;
    data = asn1::parse_string(data, &length, &type,
                            (u_char *)&pdu->agent_addr.sin_addr.s_addr,
                            &four);
    if (data == 0)
      return -1;

    long tmp (static_cast <long> (pdu->trap_type));

    // get trap type
    data = asn1::parse_int(data, &length, &type, &tmp,
                         sizeof(pdu->trap_type));
    if (data == 0)
      return -1;

    // trap type
    tmp = static_cast <long> (pdu->specific_type);
    data = asn1::parse_int(data, &length, &type, &tmp,
                         sizeof(pdu->specific_type));
    if (data == 0)
      return -1;

    // timestamp
    data = asn1::parse_int(data, &length, &type, (long *)&pdu->time,
                            sizeof(pdu->time));
    if (data == 0)
      return -1;
  }

  // get the vb list
  data = asn1::parse_header(data, &length, &type);
  if (data == 0)
    return -1;

  if (type != (u_char)(ASN_SEQUENCE | ASN_CONSTRUCTOR))
    return -1;

  while((int)length > 0) {
    if (pdu->variables == 0) {
      ACE_NEW_RETURN(pdu->variables, variable_list, -1);
      vp = pdu->variables;
    } else {
      ACE_NEW_RETURN(vp->next_variable, variable_list, -1);
      vp = vp->next_variable;
    }
    vp->next_variable = 0;
    vp->val.string = 0;
    vp->name = 0;
    vp->name_length = MAX_NAME_LEN;
    data = cmu_snmp::parse_var_op( data, objid,
                             &vp->name_length, &vp->type,
                             &vp->val_len, &var_val,
                             (int *)&length);
     if (data == 0)
      return -1;

    ACE_NEW_RETURN(op, oid[(unsigned)vp->name_length * sizeof(oid)], -1);

    // fixed
    ACE_OS::memcpy((char *)op, (char *)objid, vp->name_length * sizeof(oid));
    vp->name = op;

    len = SNMP_MSG_LENGTH;
    switch((short)vp->type) {
    case ASN_INTEGER:
    case SMI_COUNTER:
    case SMI_GAUGE:
    case SMI_TIMETICKS:
    case SMI_UINTEGER:
      ACE_NEW_RETURN(vp->val.integer,long, -1);
      vp->val_len = sizeof(long);
      asn1::parse_int(var_val, &len, &vp->type, (long *)vp->val.integer, sizeof(vp->val.integer));
      break;
    case SMI_COUNTER64:
      ACE_NEW_RETURN(vp->val.counter64, counter64, -1);
      vp->val_len = sizeof(struct counter64);
      asn1::parse_unsigned_int64(var_val, &len, &vp->type,
                               (struct counter64 *)vp->val.counter64,
                               sizeof(*vp->val.counter64));
      break;

    case ASN_OCTET_STR:
    case SMI_IPADDRESS:
    case SMI_OPAQUE:
    case SMI_NSAP:
      ACE_NEW_RETURN(vp->val.string, u_char[(unsigned)vp->val_len + 1], -1);
      asn1::parse_string(var_val, &len, &vp->type, vp->val.string,
                         &vp->val_len);
      break;

    case ASN_OBJECT_ID:
      vp->val_len = MAX_NAME_LEN;
      asn1::parse_objid(var_val, &len, &vp->type, objid, &vp->val_len);
      //vp->val_len *= sizeof(oid);

      ACE_NEW_RETURN(vp->val.objid, oid[(unsigned)vp->val_len*sizeof(oid)], -1);

      // fixed
      ACE_OS::memcpy((char *)vp->val.objid,
                     (char *)objid,
                     vp->val_len * sizeof(oid));
      break;

    case SNMP_NOSUCHOBJECT:
    case SNMP_NOSUCHINSTANCE:
    case SNMP_ENDOFMIBVIEW:
    case ASN_NULL:
      break;
    default:
      ASNERROR("bad type returned ");
      break;
    }
  }
  return 0;
}
