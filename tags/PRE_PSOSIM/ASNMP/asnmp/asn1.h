/* -*-C++-*- */
#ifndef ASN1
#define ASN1
// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//    asn1.h
//
// = DESCRIPTION
// Abstract Syntax Notation One, ASN.1
// As defined in ISO/IS 8824 and ISO/IS 8825
// This implements a subset of the above International Standards that
// is sufficient to implement SNMP.
// 
// = AUTHOR
//   Steve Waldbusser/CMU ?
//
// ============================================================================

/**********************************************************************
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

#include "ace/OS.h"
#include "asnmp/snmp.h"

// CMU code to define "oid" typedef (old C style)
#ifndef EIGHTBIT_SUBIDS
typedef unsigned long	oid;
#define MAX_SUBID   0xFFFFFFFF
#else
typedef unsigned char	oid;
#define MAX_SUBID   0xFF
#endif

#define MAX_OID_LEN	    64	/* max subid's in an oid */


// asn.1 values
#define ASN_BOOLEAN	     (0x01)
#define ASN_INTEGER	     (0x02)
#define ASN_BIT_STR	     (0x03)
#define ASN_OCTET_STR	 (0x04)
#define ASN_NULL	     (0x05)
#define ASN_OBJECT_ID	 (0x06)
#define ASN_SEQUENCE	 (0x10)
#define ASN_SET		     (0x11)
#define ASN_UNIVERSAL	 (0x00)
#define ASN_APPLICATION  (0x40)
#define ASN_CONTEXT	     (0x80)
#define ASN_PRIVATE	     (0xC0)
#define ASN_PRIMITIVE	 (0x00)
#define ASN_CONSTRUCTOR	 (0x20)
#define ASN_LONG_LEN	 (0x80)
#define ASN_EXTENSION_ID (0x1F)
#define ASN_BIT8	     (0x80)

#define IS_CONSTRUCTOR(byte)	((byte) & ASN_CONSTRUCTOR)
#define IS_EXTENSION_ID(byte)	(((byte) & ASN_EXTENSION_ID) == ASN_EXTENSION_ID)

#define ASNERROR( string)
#define MAX_NAME_LEN   64 
#define SNMP_VERSION_1 0
#define SNMP_VERSION_2C 1


// defined types (from the SMI, RFC 1065) 
#define SMI_IPADDRESS   (ASN_APPLICATION | 0)
#define SMI_COUNTER	    (ASN_APPLICATION | 1)
#define SMI_GAUGE	    (ASN_APPLICATION | 2)
#define SMI_TIMETICKS   (ASN_APPLICATION | 3)
#define SMI_OPAQUE	    (ASN_APPLICATION | 4)
#define SMI_NSAP        (ASN_APPLICATION | 5)
#define SMI_COUNTER64   (ASN_APPLICATION | 6)
#define SMI_UINTEGER    (ASN_APPLICATION | 7)

#define GET_REQ_MSG	    (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x0)
#define GETNEXT_REQ_MSG	(ASN_CONTEXT | ASN_CONSTRUCTOR | 0x1)
#define GET_RSP_MSG	    (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x2)
#define SET_REQ_MSG	    (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x3)
#define TRP_REQ_MSG	    (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x4)

#define GETBULK_REQ_MSG	(ASN_CONTEXT | ASN_CONSTRUCTOR | 0x5)
#define INFORM_REQ_MSG	(ASN_CONTEXT | ASN_CONSTRUCTOR | 0x6)
#define TRP2_REQ_MSG	(ASN_CONTEXT | ASN_CONSTRUCTOR | 0x7)
#define REPORT_MSG	    (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x8)

#define SNMP_NOSUCHOBJECT    (ASN_CONTEXT | ASN_PRIMITIVE | 0x0)
#define SNMP_NOSUCHINSTANCE  (ASN_CONTEXT | ASN_PRIMITIVE | 0x1)
#define SNMP_ENDOFMIBVIEW    (ASN_CONTEXT | ASN_PRIMITIVE | 0x2)

#define SNMP_MSG_LENGTH 1500

typedef struct sockaddr_in  ipaddr;

// pdu
struct snmp_pdu {
    int	    command;	     // pdu type
    unsigned long  reqid;    // Request id 
    unsigned long  errstat;	 // Error status 
    unsigned long  errindex; // Error index 

    // Trap information 
    oid	    *enterprise;     // System OID 
    int	    enterprise_length;
    ipaddr  agent_addr;	    // address of object generating trap 
    int	    trap_type;	    // trap type 
    int	    specific_type;  // specific type 
    unsigned long  time;	// Uptime 

	// vb list
    struct variable_list *variables;
};

// vb list
struct variable_list {
    struct variable_list *next_variable;    // NULL for last variable 
    oid	    *name;                          // Object identifier of variable 
    int	    name_length;                    // number of subid's in name 
    unsigned char   type;                   // ASN type of variable 
    union {                                 // value of variable 
	long	*integer;
	unsigned char 	*string;
	oid	*objid;
	unsigned char   *bitstring;
	struct counter64 *counter64;
    } val;
    int	    val_len;
};

struct counter64 {
    unsigned long high;
    unsigned long low;
};

// CMU Interface class for ASN.1 routines
class ACE_Export asn1 
  // = TITLE
  //     Defines the member functions for the asn1 concrete interface class 
  //     CMU routines for performing ASN.1 data marshalling / representation

{
public:
static unsigned char *parse_int( unsigned char *data, 
			      int *datalength, 
			      unsigned char *type, 
			      long int *intp, 
			      int intsize);
static unsigned char *parse_unsigned_int( unsigned char *data,	
				       int *datalength,
				       unsigned char *type,
				       unsigned long *intp,
				       int	intsize);
static unsigned char *build_int( unsigned char *data,
			      int *datalength,
			      unsigned char type,
			      long *intp,
			      int intsize);

static unsigned char *build_unsigned_int( unsigned char *data,
				       int *datalength,
				       unsigned char type,
				       unsigned long *intp,
				       int intsize);

static unsigned char *parse_string( unsigned char	*data,
				 int *datalength,
				 unsigned char *type,
				 unsigned char *string,
				 int *strlength);

static unsigned char *build_string( unsigned char *data,
				 int *datalength,
				 unsigned char type,
				 unsigned char *string,
				 int strlength);

static unsigned char *parse_header( unsigned char *data,
				int *datalength,
				unsigned char *type);

static unsigned char * build_header( unsigned char *data,
				 int *datalength,
				 unsigned char type,
				 int length);

static unsigned char * build_sequence( unsigned char *data,
				   int *datalength,
				   unsigned char type,
				   int length);

static unsigned char * parse_length( unsigned char *data,
				 unsigned long  *length);

static unsigned char *build_length( unsigned char *data,
				int *datalength,
				int length);

static unsigned char *build_objid( unsigned char *data,
			       int *datalength,
			       unsigned char type,
			       oid *objid,
			       int objidlength);

static unsigned char *parse_objid( unsigned char *data,
			       int *datalength,
			       unsigned char *type,
			       oid *objid,
			       int *objidlength);

static unsigned char *parse_null(unsigned char *data, 
				int *datalength, unsigned 
				char *type);

static unsigned char *build_null( unsigned char *data,
			      int *datalength,
			      unsigned char type);

static unsigned char *parse_bitstring( unsigned char *data,
				   int *datalength,
				   unsigned char *type,
				   unsigned char *string,
				   int *strlength);

static unsigned char *build_bitstring( unsigned char *data,
				   int *datalength,
				   unsigned char type,	
				   unsigned char *string,
				   int strlength);

static unsigned char * parse_unsigned_int64(unsigned char *data,
					 int *datalength,
					 unsigned char *type,
					 struct counter64 *cp,
					 int countersize);

static unsigned char * build_unsigned_int64( unsigned char *data,
					 int *datalength,
					 unsigned char	type,
					 struct counter64 *cp,
					 int countersize);

};


// CMU Interface class for SNMPv1, SNMPv2c routines
class ACE_Export cmu_snmp 
  // = TITLE
  //    Defines the static functions for the cmu_snmp concrete class. 
  //    These routine create cmu pdu's that can be sent/received on/from agents 
{
 public:
  static struct snmp_pdu *pdu_create( int command);
  static void free_pdu( struct snmp_pdu *pdu);
  static int build( struct snmp_pdu	*pdu, 
		   unsigned char *packet, 
		   int *out_length, 
		   long version,
		   unsigned char* community,
		   int community_len);

  static void add_var(struct snmp_pdu *pdu, 
		      oid *name, 
		      int name_length,
		      SmiVALUE *smival);

  static int parse( struct snmp_pdu *pdu,
		   unsigned char  *data,
		   unsigned char *community_name,
		   unsigned long &community_len,
		   snmp_version &version,
		   int length);
  static unsigned char *auth_build( unsigned char *data,
			       int *length,
			       long int version,
			       unsigned char *community,
			       int community_len,
			       int messagelen);

   static unsigned char *build_var_op(unsigned char *data, 
				  oid * var_name, 
				  int *var_name_len, 
				  unsigned char var_val_type, 
				  int var_val_len,
				  unsigned char *var_val, 
				  int *listlength);


static unsigned char *auth_parse(unsigned char *data,
                               int *length,
                               unsigned char *sid,
			       int *slen,
                               long	*version);

/*
 @params  data, IN - pointer to the start of object 
	var_name OUT - object id of variable 
     var_name_len IN/OUT - length of variable name 
     var_val_type OUT - type of variable (int or octet string) (one byte) 
     var_val_len  OUT - length of variable
     var_val,	 OUT - pointer to ASN1 encoded value of variable
     listlength IN/OUT - number of valid bytes left in var_op_list
*/
static unsigned char *parse_var_op( unsigned char *data, 
			    oid	    *var_name,	  
			    int	    *var_name_len, 
			    unsigned char  *var_val_type,
			    int	    *var_val_len,	 
			    unsigned char  **var_val,	 
			    int	    *listlength);

};

#endif  // ASN1

