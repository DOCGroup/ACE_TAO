/* -*-C++-*- */
#ifndef WPDU_H_
#define WPDU_H_
// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//    wpdu.h
//
// = DESCRIPTION
//  Adapter class. Converts a Pdu and GenTarget into a format
//  that can be stuffed out a I/O port
//
// = AUTHOR
//   Michael R. MacFaden re-worked api, use ACE API
//   Peter E Mellquist wrote original class snmpmsg
//
// ============================================================================

#include "ace/OS.h"
#include "asnmp/asn1.h"
#include "asnmp/pdu.h"
#include "asnmp/target.h"

// this is an adapter class, it peforms the following transformations
// PDU / Target -> iovec buffer
// iovec buffer -> PDU / Target

class ACE_Export wpdu 
  // = TITLE
  //      Implement an Adapter pattern between CMU SNMP and HP SNMP++ 
  // 
{
public:

  wpdu(const Pdu& pdu, const UdpTarget& target);
  // construct CMU data from HP SNMP++ objects
  wpdu(const iovec& buffer);
  // construct HP SNMP++ objects from raw buffer via CMU SNMP datatypes 
  wpdu(); // same as using iovec?

  ~wpdu();

  int valid() const;		
  // constructor completed ok? rc = 1 else 0

  const iovec& get_buffer() const; 
  // return raw data stream via CMU code

  int get_pdu(Pdu& pdu, snmp_version& version);
  // return HP SNMP++ pdu 

  const unsigned char *get_community() const;
  // return community strptr

private:
  int convert_vb_to_smival( Vb &tempvb, SmiVALUE *smival );
  int load_vbs(snmp_pdu *raw_pdu, const Pdu& pdu);
  int restore_vbs(Pdu& pdu, const snmp_pdu *raw_pdu) const;
  void free_smival_descriptor( SmiVALUE *smival );
  static void copy_iovec(iovec& dest, const iovec& src);
  int set_trap_info(snmp_pdu *raw_pdu, const Pdu& pdu) const;

  iovec iovec_;			
  // raw format

  int valid_flag_;		
  // object construction state

  snmp_version version_;	
  // snmp version 

  unsigned char community_name[MAX_COMM_STR_LEN];
  unsigned long comm_len; // = MAX_COMM_STR_LEN;
};

#endif // WPDU_H_
