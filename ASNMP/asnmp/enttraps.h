/* -*-C++-*- */
#ifndef ENTTRAPS_
#define ENTTRAPS_
// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//     enttraps.cpp
//
// = DESCRIPTION
//   constants for Enterprise Traps 
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
  purpose. It is provided "AS-IS without warranty of any kind,either express 
  or implied. User hereby grants a royalty-free license to any and all 
  derivatives based upon this software code base. 
=====================================================================*/ 

#include "asnmp/asn1.h"                       // ASN.1 header file

//--------------[ well known trap ids ]-----------------------------------
class ACE_Export snmpTrapsOid: public Oid 
  // = TITLE
  //     Defines the member functions for the snmpTrapsOid base class 
  //     to implement the traps defined in RFC 1215 
{
   public: 
   snmpTrapsOid (void):Oid("1.3.6.1.6.3.1.1.5"){}
};

class ACE_Export snmpTrapEnterpriseOid: public Oid 
  // = TITLE
  //     Defines the default Enterprise Oid for this software package
{
   public: 
   snmpTrapEnterpriseOid(void): Oid("1.3.6.1.6.3.1.1.4.3.0") { }
};

class ACE_Export coldStartOid: public snmpTrapsOid 
  // = TITLE
  //     Defines the RFC 1215 agent cold start generic trap  (system reboot)
{
   public: 
   coldStartOid( void){*this+=".1";}
};

class ACE_Export warmStartOid: public snmpTrapsOid 
  // = TITLE
  //     Defines the RFC 1215 agent warm start generic trap  (agent reboot)
{
   public: 
   warmStartOid( void){*this+=".2";}
};

class ACE_Export linkDownOid: public snmpTrapsOid 
  // = TITLE
  //     Defines the RFC 1215 interface (link) down trap 
{
   public: 
   linkDownOid( void){*this+=".3";}
};


// SMI LinkUp Oid
class ACE_Export linkUpOid: public snmpTrapsOid 
  // = TITLE
  //     Defines the RFC 1215 interface (link) up trap 
{
   public: 
   linkUpOid( void){*this+=".4";}
};

class ACE_Export authenticationFailureOid: public snmpTrapsOid 
  // = TITLE
  //     Defines the RFC 1215 device/agent invalid access trap
{
   public: 
   authenticationFailureOid( void){*this+=".5";}
};

class ACE_Export egpNeighborLossOid: public snmpTrapsOid 
  // = TITLE
  //     Defines the RFC 1215 Exterior Gateway Protocol neighbor loss trap 
{
   public: 
   egpNeighborLossOid( void){*this+=".6";}
};

#ifdef DEFINE_TRAP_CONSTANTS_

const coldStartOid coldStart;
const warmStartOid warmStart;
const linkDownOid linkDown;
const linkUpOid linkUp;
const authenticationFailureOid authenticationFailure;
const egpNeighborLossOid egpNeighborLoss;
const snmpTrapEnterpriseOid snmpTrapEnterprise;

#endif 

#endif // ENTTRAPS_
