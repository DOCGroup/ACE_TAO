/* -*-C++-*- */
#ifndef PDU_CLS_
#define PDU_CLS_
// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//     pdu.h
//
// = DESCRIPTION
// Pdu class definition. Encapsulation of an SMI Protocol
// Data Unit (PDU) aka Packet in C++.
//
// = AUTHOR
//    Peter E Mellquist  original code
//    Michael MacFaden  mrm@cisco.com  ACE port, add iterator class for pdus
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

// TODO: this needs to be moved into the CLASS and modifyable at runtime
// TODO: define an iterator object to traverse the vbs in a pdu

#define MAX_VBS 25
#include "asnmp/vb.h"	      // include Vb class definition

class ACE_Export Pdu 
  // = TITLE
  //      Protocol Data Unit (PDU) concrete class. An abstraction of the
  //      data packet used to by two SNMP sessions to communicate. 
{
public:
  Pdu( void);
  // constructor no args

  Pdu( Vb* pvbs, const int pvb_count);
  // constructor with vbs and count

  Pdu( const Pdu &pdu);
  // constructor with another Pdu instance

  ~Pdu();
  // destructor

  Pdu& operator=( const Pdu &pdu);
  // assignment to another Pdu object overloaded

  Pdu& operator+=( Vb &vb);
  // append a vb to the pdu

  int get_vblist( Vb* pvbs, const int pvb_count);
  // extract all Vbs from Pdu

  int set_vblist( Vb* pvbs, const int pvb_count);
  // deposit all Vbs to Pdu

  int get_vb( Vb &vb, const int index = 0) const;
  // get a particular vb
  // where 0 is the first vb

  int set_vb( Vb &vb, const int index);
  // set a particular vb
  // where 0 is the first vb

  int get_vb_count() const;
  // return number of vbs

  int get_error_status() const;
  // return the error status

  char *agent_error_reason();
  // return the complete error info from this pdu 

  friend void set_error_status( Pdu *pdu, const int status);
  // set the error status

  int get_error_index() const;
  // return the error index

  friend void set_error_index( Pdu *pdu, const int index);
  // set the error index

  friend void clear_error_status( Pdu *pdu);
  // clear error status

  friend void clear_error_index( Pdu *pdu);
  // clear error index

  unsigned long get_request_id() const;
  // return the request id

  friend void set_request_id( Pdu *pdu, const unsigned long rid);
  // set the request id

  unsigned short get_type() const;
  // get the pdu type

  void set_type( unsigned short type);
  // set the pdu type

  int valid() const;
  // returns validity of Pdu instance

  int trim(const int position=1);
  // trim off the last vb, if present

  int delete_vb( const int position);
  // delete a Vb anywhere within the Pdu

  void delete_all_vbs();
  // delete_all vbs in pdu 

  void set_notify_timestamp( const TimeTicks & timestamp);
  // set notify timestamp

  void get_notify_timestamp( TimeTicks & timestamp) const;
  // get notify timestamp

  void set_notify_id( const Oid id);
  // set the notify id

  void get_notify_id( Oid &id) const;
  // get the notify id

  void set_notify_enterprise( const Oid &enterprise);
  // set the notify enterprise

  void get_notify_enterprise( Oid & enterprise) const;
  // get the notify enterprise

  char *to_string();
  // return fomatted version of this object


  protected:
    Vb *vbs_[MAX_VBS];	         
    // pointer to array of Vbs

    int vb_count_;		 
    // count of Vbs

    int error_status_;		 
    // SMI error status

    int error_index_;		 
    // SMI error index

    int validity_;		 
    // valid boolean status of object construction 
    unsigned long request_id_;	 
    // SMI request id

    unsigned short pdu_type_;	 
    // derived at run time based on request type

    TimeTicks notify_timestamp_; 
    // a timestamp associated with an infor
    // for notify Pdu objects only
    // traps & notifies

    Oid notify_id_;               
    // an id 

    Oid notify_enterprise_;
    
   private:
   char *output;                 
   // buffer for to_string()
};


class VbIter 
  // = TITLE
  //      Utility class to iterate once through a PDU varbind list 
{
   public:
   VbIter(Pdu& pdu);
   // default constructor

   int next(Vb& vb);  
   // returns 1 if ok, else 0 if none left

  private:
   VbIter(const VbIter&);
   // disallow copy constructor use

   int idx_;
   // current object in list

   Pdu *pdu_;
   // ptr to pdu being interated over 
};

#endif //PDU_CLS_

