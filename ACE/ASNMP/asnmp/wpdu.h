/* -*-C++-*- */
#ifndef WPDU_H_
#define WPDU_H_
//=============================================================================
/**
 *  @file    wpdu.h
 *
 *  Adapter class. Converts a Pdu and GenTarget into a format
 *  that can be stuffed out a I/O port
 *
 *  @author Michael R. MacFaden re-worked api
 *  @author use ACE APIPeter E Mellquist wrote original class snmpmsg
 */
//=============================================================================


#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "asnmp/asn1.h"
#include "asnmp/pdu.h"
#include "asnmp/target.h"
#include <memory>

struct snmp_pdu;
/**
 * @class wpdu
 *
 * @brief Implement an Adapter pattern between CMU SNMP and HP SNMP++
 *
 * this is an adapter class, it peforms the following transformations
 * PDU / Target -> iovec buffer
 * iovec buffer -> PDU / Target
 */
class ASNMP_Export wpdu
{
public:
  /// construct CMU data from HP SNMP++ objects
  /// construct HP SNMP++ objects from raw buffer via CMU SNMP datatypes
  wpdu (Pdu const &pdu, UdpTarget const &target);
  wpdu (iovec const &buffer);
  wpdu(); // same as using iovec?
  wpdu (wpdu const &) = delete;
  wpdu &operator= (wpdu const &) = delete;
  wpdu (wpdu &&) = delete;
  wpdu &operator= (wpdu &&) = delete;

  ~wpdu();

  /// constructor completed ok? rc = 1 else 0
  int valid () const;

  /// return raw data stream via CMU code
  iovec const &get_buffer () const;

  /// return HP SNMP++ pdu
  int get_pdu (Pdu &pdu, snmp_version &version);

  /// return community strptr
  unsigned char const *get_community () const;

private:
  int convert_vb_to_smival (Vb &tempvb, SmiVALUE *smival);
  int load_vbs (snmp_pdu *raw_pdu, Pdu const &pdu);
  int restore_vbs (Pdu &pdu, snmp_pdu const *raw_pdu) const;
  void free_smival_descriptor (SmiVALUE *smival);
  static void copy_iovec (iovec &dest, iovec const &src);
  int set_trap_info (snmp_pdu *raw_pdu, Pdu const &pdu) const;

  /// raw format
  std::unique_ptr<char[]> buffer_ {};
  iovec iovec_ {};

  /// object construction state
  int valid_flag_ {};

  /// snmp version
  snmp_version version_ {version1};

  unsigned char community_name[MAX_COMM_STR_LEN] {};
  unsigned long comm_len {MAX_COMM_STR_LEN};
};

#endif // WPDU_H_
