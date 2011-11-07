/* -*-C++-*- */
#ifndef PDU_CLS_
#define PDU_CLS_
//=============================================================================
/**
 *  @file     pdu.h
 *
 *  $Id$
 *
 * Pdu class definition. Encapsulation of an SMI Protocol
 * Data Unit (PDU) aka Packet in C++.
 *
 *
 *  @author Peter E Mellquist  original code Michael MacFaden  mrm@cisco.com  ACE port
 *  @author add iterator class for pdus
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

// TODO: this needs to be moved into the CLASS and modifyable at runtime
// TODO: define an iterator object to traverse the vbs in a pdu

#define MAX_VBS 25
#include "asnmp/vb.h"         // include Vb class definition

/**
 * @class Pdu
 *
 * @brief Protocol Data Unit (PDU) concrete class. An abstraction of the
 * data packet used to by two SNMP sessions to communicate.
 */
class ASNMP_Export Pdu
{
public:
  /// constructor no args
  Pdu( void);

  /// constructor with vbs and count
  Pdu( Vb* pvbs, const int pvb_count);

  /// constructor with another Pdu instance
  Pdu( const Pdu &pdu);

  /// destructor
  ~Pdu();

  /// assignment to another Pdu object overloaded
  Pdu& operator=( const Pdu &pdu);

  /// append a vb to the pdu
  Pdu& operator+=( Vb &vb);

  // TODO: add Pdu& operator-=(const Vb &vb);

  /// extract all Vbs from Pdu
  int get_vblist( Vb* pvbs, const int pvb_count);

  /// deposit all Vbs to Pdu
  int set_vblist( Vb* pvbs, const int pvb_count);

  /// get a particular vb
  /// where 0 is the first vb
  int get_vb( Vb &vb, const int index = 0) const;

  /// set a particular vb
  /// where 0 is the first vb
  int set_vb( Vb &vb, const int index);

  /// return number of vbs
  int get_vb_count() const;

  /// return the error status
  int get_error_status() const;

  /// return the complete error info from this pdu
  const char *agent_error_reason();

  /// set the error status
  friend ASNMP_Export void set_error_status( Pdu *pdu, const int status);

  /// return the error index
  int get_error_index() const;

  /// set the error index
  friend ASNMP_Export void set_error_index( Pdu *pdu, const int index);

  /// clear error status
  friend ASNMP_Export void clear_error_status( Pdu *pdu);

  /// clear error index
  friend ASNMP_Export void clear_error_index( Pdu *pdu);

  /// return the request id
  unsigned long get_request_id() const;

  /// set the request id
  friend void set_request_id( Pdu *pdu, const unsigned long rid);

  /// get the pdu type
  unsigned short get_type() const;

  /// set the pdu type
  void set_type( unsigned short type);

  /// returns validity of Pdu instance
  int valid() const;

  /// trim off count vbs from the end of the vb list
  int trim(const int count=1);

  /// delete a Vb anywhere within the Pdu
  int delete_vb( const int position);

  /// delete_all vbs in pdu
  void delete_all_vbs();

  /// set notify timestamp
  void set_notify_timestamp( const TimeTicks & timestamp);

  /// get notify timestamp
  void get_notify_timestamp( TimeTicks & timestamp) const;

  /// set the notify id
  void set_notify_id( const Oid id);

  /// get the notify id
  void get_notify_id( Oid &id) const;

  /// set the notify enterprise
  void set_notify_enterprise( const Oid &enterprise);

  /// get the notify enterprise
  void get_notify_enterprise( Oid & enterprise) const;

  /// return fomatted version of this object
  const char *to_string();

protected:
  /// pointer to array of Vbs
  Vb *vbs_[MAX_VBS];

  /// count of Vbs
  int vb_count_;

  /// SMI error status
  int error_status_;

  /// SMI error index
  int error_index_;

  /// valid boolean status of object construction
  /// SMI request id
  int validity_;
  unsigned long request_id_;

  /// derived at run time based on request type
  unsigned short pdu_type_;

  /**
    * a timestamp associated with an infor
    * for notify Pdu objects only
    * traps & notifies
    */
  TimeTicks notify_timestamp_;

  /// an id
  Oid notify_id_;

  Oid notify_enterprise_;

private:
  /// buffer for to_string()
  char *output_;
};


/**
 * @class VbIter
 *
 * @brief Utility class to iterate once through a PDU varbind list
 */
class ASNMP_Export VbIter
{
public:
  /// default constructor
  VbIter(Pdu& pdu);

  /// returns 1 if ok, else 0 if none left
  int next(Vb& vb);

private:
  /// disallow copy constructor use
  VbIter(const VbIter&);

  /// current object in list
  int idx_;

  /// ptr to pdu being interated over
  Pdu *pdu_;
};

#endif //PDU_CLS_

