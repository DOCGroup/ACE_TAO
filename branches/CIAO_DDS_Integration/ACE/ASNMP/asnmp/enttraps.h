/* -*-C++-*- */
#ifndef ENTTRAPS_
#define ENTTRAPS_
//=============================================================================
/**
 *  @file     enttraps.h
 *
 *  $Id$
 *
 * constants for Enterprise Traps
 *
 *
 *  @author Peter E MellquistMichael R MacFaden  mrm@cisco.com - rework & ACE port
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

#include "asnmp/asn1.h"                       // ASN.1 header file

//--------------[ well known trap ids ]-----------------------------------
/**
 * @class snmpTrapsOid
 *
 * @brief Defines the member functions for the snmpTrapsOid base class
 * to implement the traps defined in RFC 1215
 */
class ASNMP_Export snmpTrapsOid : public Oid
{
   public:
   snmpTrapsOid (void):Oid("1.3.6.1.6.3.1.1.5"){}
   SnmpSyntax& operator=( SnmpSyntax &val){ return Oid::operator=(val); }
};

/**
 * @class snmpTrapEnterpriseOid
 *
 * @brief Defines the default Enterprise Oid for this software package
 */
class ASNMP_Export snmpTrapEnterpriseOid : public Oid
{
   public:
   snmpTrapEnterpriseOid(void): Oid("1.3.6.1.6.3.1.1.4.3.0") { }
   SnmpSyntax& operator=( SnmpSyntax &val){ return Oid::operator=(val); }
};

/**
 * @class coldStartOid
 *
 * @brief Defines the RFC 1215 agent cold start generic trap  (system reboot)
 */
class ASNMP_Export coldStartOid : public snmpTrapsOid
{
   public:
   coldStartOid( void){*this+=".1";}
   SnmpSyntax& operator=( SnmpSyntax &val){ return snmpTrapsOid::operator=(val); }
};

/**
 * @class warmStartOid
 *
 * @brief Defines the RFC 1215 agent warm start generic trap  (agent reboot)
 */
class ASNMP_Export warmStartOid : public snmpTrapsOid
{
   public:
   warmStartOid( void){*this+=".2";}
   SnmpSyntax& operator=( SnmpSyntax &val){ return snmpTrapsOid::operator=(val); }
};

/**
 * @class linkDownOid
 *
 * @brief Defines the RFC 1215 interface (link) down trap
 */
class ASNMP_Export linkDownOid : public snmpTrapsOid
{
   public:
   linkDownOid( void){*this+=".3";}
   SnmpSyntax& operator=( SnmpSyntax &val){ return snmpTrapsOid::operator=(val); }
};


// SMI LinkUp Oid
/**
 * @class linkUpOid
 *
 * @brief Defines the RFC 1215 interface (link) up trap
 */
class ASNMP_Export linkUpOid : public snmpTrapsOid
{
   public:
   linkUpOid( void){*this+=".4";}
   SnmpSyntax& operator=( SnmpSyntax &val){ return snmpTrapsOid::operator=(val); }
};

/**
 * @class authenticationFailureOid
 *
 * @brief Defines the RFC 1215 device/agent invalid access trap
 */
class ASNMP_Export authenticationFailureOid : public snmpTrapsOid
{
   public:
   authenticationFailureOid( void){*this+=".5";}
   SnmpSyntax& operator=( SnmpSyntax &val){ return snmpTrapsOid::operator=(val); }
};

/**
 * @class egpNeighborLossOid
 *
 * @brief Defines the RFC 1215 Exterior Gateway Protocol neighbor loss trap
 */
class ASNMP_Export egpNeighborLossOid : public snmpTrapsOid
{
   public:
   egpNeighborLossOid( void){*this+=".6";}
   SnmpSyntax& operator=( SnmpSyntax &val){ return snmpTrapsOid::operator=(val); }
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
