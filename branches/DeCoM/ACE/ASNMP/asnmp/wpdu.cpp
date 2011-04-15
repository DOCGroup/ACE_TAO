
//=============================================================================
/**
 *  @file    wpdu.cpp
 *
 *  $Id$
 *
 *  Adapter class. Converts between raw wire format and Pdu objects
 *  that can be stuffed out a I/O port or reconstructed
 *
 *
 *  @author Michael R. MacFaden rework the class api and impl using ACEPeter E Mellquist  implementation/code from snmp++ snmpmsg class
 */
//=============================================================================


#include "asnmp/wpdu.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"

#define DEFINE_TRAP_CONSTANTS_
#include "asnmp/enttraps.h"

#define MAX_COMM_STR_LEN 255
#define V1_COLD_START 0
#define V1_WARM_START 1
#define V1_LINK_DOWN 2
#define V1_LINK_UP 3
#define V1_AUTH_FAILURE 4
#define V1_EGP_NEIGHBOR_LOSS 5
#define V1_ENT_SPECIFIC 6

inline
void reset_iov(iovec& iov)
{
  iov.iov_base = 0;
  iov.iov_len = 0;
}

wpdu::wpdu(const Pdu& pdu, const UdpTarget& target):
   valid_flag_(SNMP_CLASS_INVALID ), comm_len(MAX_COMM_STR_LEN)
{
   reset_iov(iovec_);
   version_ = target.get_version();
   int status;
   OctetStr comm_str;

   community_name[0] = 0;

   snmp_pdu *raw_pdu;   // create a raw pdu
   raw_pdu = cmu_snmp::pdu_create( (int) pdu.get_type());
   if (!raw_pdu) {
     valid_flag_ = SNMP_CLASS_RESOURCE_UNAVAIL;
     return;
   }

   raw_pdu->reqid = pdu.get_request_id();
   raw_pdu->errstat= (unsigned long) pdu.get_error_status();
   raw_pdu->errindex= (unsigned long) pdu.get_error_index();

   switch (raw_pdu->command) {
     case sNMP_PDU_GET:
     case sNMP_PDU_GETNEXT:
       target.get_read_community(comm_str);
       break;

     case sNMP_PDU_SET:
       target.get_write_community(comm_str);
       break;

     case sNMP_PDU_V1TRAP:
       target.get_read_community(comm_str);
       if (set_trap_info(raw_pdu, pdu)) // will free raw_pdu
          return;
       break;
     case sNMP_PDU_RESPONSE:
       break;

   default:
     ACE_ASSERT(0);
     return;
   }

   if (load_vbs(raw_pdu, pdu)) {
     cmu_snmp::free_pdu( raw_pdu);
     valid_flag_ = SNMP_CLASS_RESOURCE_UNAVAIL;
     return;
   }

   // TODO: determine how big raw_pdu serializes out to
   iovec_.iov_len = target.get_max_pdu_size();
   ACE_NEW(iovec_.iov_base, char [iovec_.iov_len]);

   // create raw byte stream
   // The intermediate integer is to avoid type-punned pointer
   // dereferencing.
   int out_length = iovec_.iov_len;
   status = cmu_snmp::build( raw_pdu,
                            (unsigned char *)iovec_.iov_base,
                            &out_length,
                            target.get_version(),
                            comm_str.data(),
                            comm_str.length());
   iovec_.iov_len = out_length;

   if ( status != 0) {
     valid_flag_ = SNMP_ERROR_WRONG_ENCODING;
     cmu_snmp::free_pdu( raw_pdu);
     return;
   }

   cmu_snmp::free_pdu( raw_pdu);
   valid_flag_ = SNMP_CLASS_SUCCESS;
}

int wpdu::set_trap_info(snmp_pdu *raw_pdu, const Pdu& pdu) const
{
  Oid enterprise;
  Oid trapid; // validate caller has set this correctly
  pdu.get_notify_id( trapid);
  if ( !trapid.valid() || trapid.length() < 2 ) {
     cmu_snmp::free_pdu( raw_pdu);
     return SNMP_CLASS_INVALID_NOTIFYID;
  }


  raw_pdu->specific_type=0;

  // TODO: object should emit numeric instead of this kind of mess...
  if ( trapid == coldStart)
    raw_pdu->trap_type = V1_COLD_START;  // cold start
  else if ( trapid == warmStart)
    raw_pdu->trap_type = V1_WARM_START;  // warm start
  else if( trapid == linkDown)
    raw_pdu->trap_type = V1_LINK_DOWN;  // link down
  else if ( trapid == linkUp)
    raw_pdu->trap_type = V1_LINK_UP;  // link up
  else if ( trapid == authenticationFailure )
    raw_pdu->trap_type = V1_AUTH_FAILURE;  // authentication failure
  else if ( trapid == egpNeighborLoss)
    raw_pdu->trap_type = V1_EGP_NEIGHBOR_LOSS;  // egp neighbor loss
  else {
    raw_pdu->trap_type = V1_ENT_SPECIFIC;     // enterprise specific
                               // last oid subid is the specific value
                               // if 2nd to last subid is "0", remove it
                               // enterprise is always the notify oid prefix
   raw_pdu->specific_type = (int) trapid[(int) (trapid.length() - 1)];
   trapid.trim(1);
   if ( trapid[(int)(trapid.length() - 1)] == 0 )
     trapid.trim(1);
   enterprise = trapid;
  }

  if ( raw_pdu->trap_type != V1_ENT_SPECIFIC)
    pdu.get_notify_enterprise( enterprise);
  if ( enterprise.length() > 0) {
    // note!!  To the contrary, enterprise OID val is
    // copied here and raw_pdu->enterprise is freed in free_pdu
    // as it should be (HDN)
    // these are hooks into an SNMP++ oid
    // and therefor the raw_pdu enterprise
    // should not free them. null them out!!
    SmiLPOID rawOid;
    rawOid = enterprise.oidval();
    // HDN - enterprise is a local object, cannot simply assign pointer
    //raw_pdu->enterprise = rawOid->ptr;
    raw_pdu->enterprise_length = (int) rawOid->len;
    ACE_NEW_RETURN(raw_pdu->enterprise,
                   oid[raw_pdu->enterprise_length],-1);
    ACE_OS::memcpy((char *)raw_pdu->enterprise,(char *)rawOid->ptr,
                   raw_pdu->enterprise_length * sizeof(oid));
  }

  TimeTicks timestamp;
  pdu.get_notify_timestamp( timestamp);
  raw_pdu->time = ( unsigned long) timestamp;

  // HDN - set agent addr using the local hostname if possible
  char localHostName[MAXHOSTNAMELEN];
  Snmp::get_host_name(localHostName, MAXHOSTNAMELEN);
  if (ACE_OS::strlen(localHostName) > 0) {
    GenAddress addr(localHostName);
    OctetStr octet;
    addr.to_octet(octet);
    ACE_OS::memcpy(&(raw_pdu->agent_addr.sin_addr),
                   octet.data(),
                   octet.length());
  }

  return 0;
}

wpdu::wpdu(const iovec& iov): valid_flag_(0),comm_len(MAX_COMM_STR_LEN)
{
  community_name[0] = 0;
   reset_iov(iovec_);
   version_ = version1;         // TODO: figure where this should come from
   ACE_NEW(iovec_.iov_base, char[iov.iov_len]);
   if (!iovec_.iov_base) {
     valid_flag_ = SNMP_CLASS_RESOURCE_UNAVAIL;
     return;
   }

   copy_iovec(iovec_, iov);
   valid_flag_ = SNMP_CLASS_SUCCESS;
}

wpdu::wpdu(): valid_flag_(0), comm_len(MAX_COMM_STR_LEN)
{
  community_name[0] = 0;
   reset_iov(iovec_);
   version_ = version1;         // TODO: figure where this should come from
}

int wpdu::valid() const
{
  return (valid_flag_ == SNMP_CLASS_SUCCESS);
}

int wpdu::load_vbs(snmp_pdu *raw_pdu, const Pdu& pdu)
{
  int status = 0;

   // load up the payload
   // for all Vbs in list, add them to the pdu
   int vb_count;
   Vb tempvb;
   Oid tempoid;
   SmiLPOID smioid;
   SmiVALUE smival;

   vb_count = pdu.get_vb_count();

   for (int z = 0; z < vb_count; z++) {
      pdu.get_vb( tempvb, z);
      tempvb.get_oid( tempoid);
      smioid = tempoid.oidval();
      // what are we trying to convert here (vb oid part or value part)
      status = convert_vb_to_smival( tempvb, &smival );
      if ( status != SNMP_CLASS_SUCCESS)
         return status;

      // add the var to the raw pdu
      cmu_snmp::add_var(raw_pdu, smioid->ptr, (int) smioid->len, &smival);
      free_smival_descriptor( &smival);
    }

  return status;
}

// supports overlapped copies
// static
void wpdu::copy_iovec(iovec& dest, const iovec& src)
{
  if (&dest == &src)
    return;

   ACE_OS::memmove( dest.iov_base, src.iov_base, src.iov_len);
   dest.iov_len = src.iov_len;
}

int wpdu::convert_vb_to_smival( Vb &tempvb, SmiVALUE *smival )
{
  smival->syntax = tempvb.get_syntax();

  switch ( smival->syntax ) {

  case sNMP_SYNTAX_NULL:
    break;

    // case sNMP_SYNTAX_INT32:
  case sNMP_SYNTAX_INT:
    {
      SnmpInt32 tmp;
      tempvb.get_value(tmp);
      smival->value.sNumber = tmp;
    }
    break;

    //    case sNMP_SYNTAX_UINT32:
  case sNMP_SYNTAX_GAUGE32:
  case sNMP_SYNTAX_CNTR32:
  case sNMP_SYNTAX_TIMETICKS:
    {
      SnmpUInt32 tmp;
      tempvb.get_value(tmp);
      smival->value.uNumber = tmp;
    }
    break;

    // case Counter64
  case sNMP_SYNTAX_CNTR64:
    {
      Counter64 c64;
      tempvb.get_value(c64);
      smival->value.hNumber.hipart = c64.high();
      smival->value.hNumber.lopart = c64.low();
    }
    break;

   // OID syntax
  case sNMP_SYNTAX_OID:
    {
      Oid tmpoid;
      tmpoid.oidval();
      tempvb.get_value(tmpoid);
      SmiLPOID smi = tmpoid.oidval();
      smival->value.oid.len = tmpoid.length();
      ACE_NEW_RETURN(smival->value.oid.ptr,
                SmiUINT32 [smival->value.oid.len], 1);
      ACE_OS::memcpy(smival->value.oid.ptr, smi->ptr,
                     smival->value.oid.len *sizeof(SmiUINT32));
    }
    break;

  case sNMP_SYNTAX_BITS:
  case sNMP_SYNTAX_OCTETS:
  case sNMP_SYNTAX_IPADDR:
    {
      OctetStr os;
      tempvb.get_value(os);
      smival->value.string.ptr = 0;
      smival->value.string.len = os.length();
      if ( smival->value.string.len > 0 ) {
        ACE_NEW_RETURN(smival->value.string.ptr,
                SmiBYTE [smival->value.string.len], 1);
        if ( smival->value.string.ptr )  {
          for (int i=0; i<(int) smival->value.string.len ; i++)
            smival->value.string.ptr[i] = os[i];
        }
        else   {
          smival->syntax = sNMP_SYNTAX_NULL;  // invalidate the smival
          return SNMP_CLASS_RESOURCE_UNAVAIL;
        }
      }
    }
    break;

    default:
     ACE_DEBUG((LM_DEBUG, "wpdu::convert_vb_to_smival did not convert vb\n"));
     // ACE_ASSERT(0);
  } // switch

  return 0;
}

// free a SMI value
void wpdu::free_smival_descriptor( SmiVALUE *smival )
{
  switch ( smival->syntax ) {
  case sNMP_SYNTAX_OCTETS:
  case sNMP_SYNTAX_OPAQUE:
  case sNMP_SYNTAX_IPADDR:
  case sNMP_SYNTAX_BITS:                 // obsoleted in SNMPv2 Draft Std
    delete [] smival->value.string.ptr;
    break;

  case sNMP_SYNTAX_OID:
    delete [] smival->value.oid.ptr;
    break;
  }
  smival->syntax = sNMP_SYNTAX_NULL;
}


wpdu::~wpdu()
{
  delete [] (char*) iovec_.iov_base;
}

const iovec& wpdu::get_buffer() const
{
  return iovec_;
}

// return a pdu from a buffer
int wpdu::get_pdu(Pdu& pdu, snmp_version& version)
{
  if (iovec_.iov_len == 0)
    return -1; // NO DATA

  snmp_pdu *raw_pdu;
  raw_pdu = cmu_snmp::pdu_create(0);
  if (!raw_pdu) {
    return SNMP_CLASS_RESOURCE_UNAVAIL;
  }

  // max value a client can send us - TODO: replace this with an
  // api to get actual string length
  int status = cmu_snmp::parse( raw_pdu, (unsigned char *)iovec_.iov_base,
                     community_name, comm_len,
                     version, iovec_.iov_len);
  if (status != 0)
    return SNMP_CLASS_INTERNAL_ERROR;

  community_name[comm_len] = 0; // set null based on returned length
  set_request_id( &pdu, raw_pdu->reqid);
  set_error_status( &pdu, (int) raw_pdu->errstat);
  set_error_index( &pdu, (int) raw_pdu->errindex);
  pdu.set_type( raw_pdu->command);

  if (restore_vbs(pdu, raw_pdu)) {
    cmu_snmp::free_pdu(raw_pdu);
    return SNMP_CLASS_INTERNAL_ERROR;
  }

  cmu_snmp::free_pdu(raw_pdu);
  return 0;
}

int wpdu::restore_vbs(Pdu& pdu, const snmp_pdu *raw_pdu) const
{
   Vb tempvb;
   Oid tempoid;
   struct variable_list *vp;

   for(vp = raw_pdu->variables; vp; vp = vp->next_variable) {

    // extract the oid portion
    tempoid.set_data( (unsigned long *)vp->name,
                       ( unsigned int) vp->name_length);
    tempvb.set_oid( tempoid);

    // extract the value portion
    switch(vp->type) {

    // octet string
    case sNMP_SYNTAX_OCTETS:
     case sNMP_SYNTAX_OPAQUE:
    {
       OctetStr octets( (char *) vp->val.string,
                         (long) vp->val_len);
       tempvb.set_value( octets);
    }
    break;

    // object id
    case sNMP_SYNTAX_OID:
    {
         Oid oid( (unsigned long*) vp->val.objid,
              (int) vp->val_len);
      tempvb.set_value( oid);
    }
     break;

    // timeticks
    case sNMP_SYNTAX_TIMETICKS:
    {
       TimeTicks timeticks( (unsigned long) *(vp->val.integer));
       tempvb.set_value( timeticks);
    }
    break;

    // Gauge32
    case sNMP_SYNTAX_GAUGE32:
    {
       Gauge32 gauge32( (unsigned long) *(vp->val.integer));
       tempvb.set_value( gauge32);
    }
    break;

    // 32 bit counter
    case sNMP_SYNTAX_CNTR32:
    {
        Counter32 counter32( (unsigned long) *(vp->val.integer));
       tempvb.set_value( counter32);
      }
    break;

    // ip address
    case sNMP_SYNTAX_IPADDR:
    {
       char buffer[20];
       ACE_OS::sprintf( buffer,"%d.%d.%d.%d",
                        vp->val.string[0],
                        vp->val.string[1],
                        vp->val.string[2],
                        vp->val.string[3]);
       IpAddress ipaddress( buffer);
       tempvb.set_value( ipaddress);
    }
    break;

    // 32 bit integer
    case sNMP_SYNTAX_INT:
    {
       SnmpInt32 int32( (long) *(vp->val.integer));
      tempvb.set_value( int32);
    }
    break;

    // 32 bit unsigned integer
    case sNMP_SYNTAX_UINT32:
    {
      SnmpUInt32 uint32( (unsigned long) *(vp->val.integer));
      tempvb.set_value( uint32);
    }
     break;

    // v2 counter 64's
    case sNMP_SYNTAX_CNTR64:
     break;

    case sNMP_SYNTAX_NULL:
      tempvb.set_null();
    break;

    // v2 vb exceptions
    case sNMP_SYNTAX_NOSUCHOBJECT:
    case sNMP_SYNTAX_NOSUCHINSTANCE:
    case sNMP_SYNTAX_ENDOFMIBVIEW:
       set_exception_status( &tempvb, vp->type);
     break;

    default:
       tempvb.set_null();

    } // end switch

    // append the vb to the pdu
    pdu += tempvb;
  }

  return 0;
}

const unsigned char *wpdu::get_community() const
{
  return community_name;
}
