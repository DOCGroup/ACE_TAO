// $Id$

// Defines the Internet domain address family address format.

#define ACE_BUILD_DLL
#include "ace/ATM_Addr.h"

#if !defined (__ACE_INLINE__)
#include "ace/ATM_Addr.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, ATM_Addr, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_ATM_Addr)

#if defined (ACE_HAS_FORE_ATM_XTI)
#define BHLI_MAGIC "FORE_ATM"

// This is line rate in cells/s for an OC-3 MM interface.
const long ACE_ATM_Addr::LINE_RATE = 353207;
const int ACE_ATM_Addr::OPT_FLAGS_CPID = 0x1;
const int ACE_ATM_Addr::OPT_FLAGS_PMP = 0x2;
const int ACE_ATM_Addr::SELECTOR = 0x99;
#endif /* ACE_HAS_FORE_ATM_XTI */

// Default constructor

ACE_ATM_Addr::ACE_ATM_Addr (void)
#if defined (ACE_HAS_FORE_ATM_XTI)
  : ACE_Addr (AF_ATM,
#else
  : ACE_Addr (AF_UNSPEC,
#endif /* ACE_HAS_FORE_ATM_XTI */

              sizeof this->atm_addr_)
{
  // ACE_TRACE ("ACE_ATM_Addr::ACE_ATM_Addr");
  (void) ACE_OS::memset ((void *) &this->atm_addr_,
                         0,
                         sizeof this->atm_addr_);
  this->init ();
}

// Copy constructor.

ACE_ATM_Addr::ACE_ATM_Addr (const ACE_ATM_Addr &sap)
{
  ACE_TRACE ("ACE_ATM_Addr::ACE_ATM_Addr");
  this->set (sap);
}

ACE_ATM_Addr::ACE_ATM_Addr (const ATMSAPAddress *sap)
{
  ACE_TRACE ("ACE_ATM_Addr::ACE_ATM_Addr");
  this->set (sap);
}

ACE_ATM_Addr::ACE_ATM_Addr (const ASYS_TCHAR sap[])
{
  ACE_TRACE ("ACE_ATM_Addr::ACE_ATM_Addr");
  this->set (sap);
}

void
ACE_ATM_Addr::init (void)
{
#if defined (ACE_HAS_FORE_ATM_XTI)
  // Note: this approach may be FORE implementation-specific.  When we
  // bind with tag_addr ABSENT and tag_selector PRESENT, only the
  // selector (i.e. address[19]) is used by the TP.  The rest of the
  // local address is filled in by the TP and can be obtained via the
  // 'ret' parameter or with t_getname()/t_getprotaddr().

  atm_addr_.addressType = (u_int16_t) AF_ATM;

  atm_addr_.sap.t_atm_sap_addr.SVE_tag_addr = (int8_t) T_ATM_ABSENT;
  atm_addr_.sap.t_atm_sap_addr.SVE_tag_selector = (int8_t) T_ATM_PRESENT;

  atm_addr_.sap.t_atm_sap_addr.address_format = (u_int8_t) T_ATM_ENDSYS_ADDR;
  atm_addr_.sap.t_atm_sap_addr.address_length = ATMNSAP_ADDR_LEN;
  atm_addr_.sap.t_atm_sap_addr.address[ATMNSAP_ADDR_LEN - 1] = SELECTOR;

  atm_addr_.sap.t_atm_sap_layer2.SVE_tag = (int8_t) T_ATM_ABSENT;
  atm_addr_.sap.t_atm_sap_layer3.SVE_tag = (int8_t) T_ATM_ABSENT;

  atm_addr_.sap.t_atm_sap_appl.SVE_tag = (int8_t) T_ATM_PRESENT;
  atm_addr_.sap.t_atm_sap_appl.ID_type = (u_int8_t) T_ATM_USER_APP_ID;

  ACE_OS::memcpy (atm_addr_.sap.t_atm_sap_appl.ID.user_defined_ID,
                  BHLI_MAGIC,
                  sizeof atm_addr_.sap.t_atm_sap_appl.ID);
#else
#endif /* ACE_HAS_FORE_ATM_XTI */
}

int
ACE_ATM_Addr::set (const ACE_ATM_Addr &sap)
{
  ACE_TRACE ("ACE_ATM_Addr::set");

  this->init ();

  this->ACE_Addr::base_set (sap.get_type (),
                            sap.get_size ());

#if defined (ACE_HAS_FORE_ATM_XTI)
  ACE_ASSERT (sap.get_type () == AF_ATM);
#endif /* ACE_HAS_FORE_ATM_XTI */

  (void) ACE_OS::memcpy ((void *) &this->atm_addr_,
                         (void *) &sap.atm_addr_,
                         sizeof this->atm_addr_);
  return 0;
}

int
ACE_ATM_Addr::set (const ATMSAPAddress *sap)
{
  ACE_TRACE ("ACE_ATM_Addr::set");

  this->init ();

#if defined (ACE_HAS_FORE_ATM_XTI)
  this->ACE_Addr::base_set (AF_ATM,
#else
  this->ACE_Addr::base_set (AF_UNSPEC,
#endif /* ACE_HAS_FORE_ATM_XTI */
                            sizeof (*sap));

  (void) ACE_OS::memcpy ((void *) &this->atm_addr_,
                         (void *) sap,
                         sizeof this->atm_addr_);
  return 0;
}

int
ACE_ATM_Addr::set (const ASYS_TCHAR address[])
{
  ACE_TRACE ("ACE_ATM_Addr::set");

  this->init ();

#if defined (ACE_HAS_FORE_ATM_XTI)
  atm_addr_.sap.t_atm_sap_addr.SVE_tag_addr =
    (int8_t) T_ATM_PRESENT;
#endif /* ACE_HAS_FORE_ATM_XTI */
  return this->string_to_addr (address);
}

// Transform the string into the current addressing format.

int
ACE_ATM_Addr::string_to_addr (const ASYS_TCHAR sap[])
{
  ACE_TRACE ("ACE_ATM_Addr::string_to_addr");

#if defined (ACE_HAS_FORE_ATM_XTI)
  this->ACE_Addr::base_set (AF_ATM,
#else
  this->ACE_Addr::base_set (AF_UNSPEC,
#endif /* ACE_HAS_FORE_ATM_XTI */
                            sizeof this->atm_addr_);
#if defined (ACE_HAS_FORE_ATM_XTI)
  struct hostent *entry;
  struct atmnsap_addr *nsap;

  // Yow, someone gave us a NULL ATM address!
  if (sap == 0)
    {
      errno = EINVAL;
      return -1;
    }
  else if ((entry = gethostbyname_atmnsap((ASYS_TCHAR *)sap)) != 0)
    {
      ACE_OS::memcpy (atm_addr_.sap.t_atm_sap_addr.address, 
                      entry->h_addr_list[0],
                      ATMNSAP_ADDR_LEN - 1);
    }
  else if ((nsap = atmnsap_addr (sap)) != 0)
    {
      ACE_OS::memcpy (atm_addr_.sap.t_atm_sap_addr.address,
                      nsap->atmnsap,
                      ATMNSAP_ADDR_LEN);
    }
  else
#else
  ACE_UNUSED_ARG (sap);
#endif /* ACE_HAS_FORE_ATM_XTI */
    {
      errno = EINVAL;
      return -1;
    }

#if defined (ACE_HAS_FORE_ATM_XTI)
  return 0;
#endif /* ACE_HAS_FORE_ATM_XTI */
}

// Transform the current address into string format.

int
ACE_ATM_Addr::addr_to_string (ASYS_TCHAR addr[],
                              size_t addrlen) const
{
  ACE_TRACE ("ACE_ATM_Addr::addr_to_string");

#if defined (ACE_HAS_FORE_ATM_XTI)
  ASYS_TCHAR buffer[MAXNAMELEN + 1];
  struct atmnsap_addr nsap;
  ACE_OS::memcpy (nsap.atmnsap,
                  atm_addr_.sap.t_atm_sap_addr.address,
                  ATMNSAP_ADDR_LEN);
  ACE_OS::sprintf (buffer,
                   ASYS_TEXT ("%s"),
                   ASYS_WIDE_STRING (atmnsap_ntoa (nsap)));

  size_t total_len = ACE_OS::strlen (buffer)
    + sizeof ('\0'); // For trailing '\0'.

  if (addrlen < total_len)
    return -1;
  else
    ACE_OS::strcpy(addr, buffer);

  return 0;
#else
  ACE_UNUSED_ARG (addr);
  ACE_UNUSED_ARG (addrlen);
  return -1;
#endif /* ACE_HAS_FORE_ATM_XTI */
}

const ASYS_TCHAR *
ACE_ATM_Addr::addr_to_string (void) const
{
  ACE_TRACE ("ACE_ATM_Addr::addr_to_string");

  static ASYS_TCHAR addr[MAXHOSTNAMELEN + 1];
  this->addr_to_string (addr,
                        MAXHOSTNAMELEN + 1);
  return addr;
}

// Set a pointer to the address.
void
ACE_ATM_Addr::set_addr (void *addr, int len)
{
  ACE_TRACE ("ACE_ATM_Addr::set_addr");

#if defined (ACE_HAS_FORE_ATM_XTI)
  this->ACE_Addr::base_set (AF_ATM,
#else
  this->ACE_Addr::base_set (AF_UNSPEC,
#endif /* ACE_HAS_FORE_ATM_XTI */
                            len);
  ACE_OS::memcpy ((void *) &this->atm_addr_,
                  (void *) addr, len);
}

// Compare two addresses for inequality.

int
ACE_ATM_Addr::operator != (const ACE_ATM_Addr &sap) const
{
  ACE_TRACE ("ACE_ATM_Addr::operator !=");
  return !((*this) == sap);
}

// Compare two addresses for equality.

int
ACE_ATM_Addr::operator == (const ACE_ATM_Addr &sap) const
{
  ACE_TRACE ("ACE_ATM_Addr::operator ==");

  return ACE_OS::memcmp (&atm_addr_,
                         &sap.atm_addr_,
#if defined (ACE_HAS_FORE_ATM_XTI)
                         sizeof (struct ATMSAPAddress)) == 0;
#else
                         sizeof (ATMSAPAddress)) == 0;
#endif /* ACE_HAS_FORE_ATM_XTI */
}

void
ACE_ATM_Addr::dump (void) const
{
  ACE_TRACE ("ACE_ATM_Addr::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ASYS_TCHAR s[ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 16];
  ACE_OS::sprintf (s,
                   ASYS_TEXT ("%s"),
                   ASYS_WIDE_STRING (this->addr_to_string ()));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("%s"), s));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// Some OSes (namely, AIX) have implemented the v5 t_getprotaddr()
// command.  Others, like Irix, still use the v4 t_getname().  Both
// are offered here.

#ifdef aix
#define T_GETPROTADDR(fd, laddr, raddr)	ACE_OS::t_getprotaddr(fd, laddr, raddr)
#else
#define T_GETPROTADDR(fd, laddr, raddr) ACE_OS::t_getname(fd, laddr.addr, LOCALNAME)
#endif /* aix */

int
ACE_ATM_Addr::get_local_address (ACE_HANDLE fd,
                                 u_char addr[])
{
#if defined (ACE_HAS_FORE_ATM_XTI)
  ATMSAPAddress local_addr;
  struct t_bind boundaddr;

  boundaddr.addr.maxlen = sizeof local_addr;
  boundaddr.addr.buf = (char *) &local_addr;

  if (T_GETPROTADDR (fd, &boundaddr, 0) == -1)
    {
      ACE_OS::t_error ("T_GETPROTADDR (local_address)");
      return -1;
    }

  ACE_OS::memcpy (addr,
                  local_addr.sap.t_atm_sap_addr.address,
                  ATMNSAP_ADDR_LEN);
  return 0;
#else
  ACE_UNUSED_ARG (fd);
  ACE_UNUSED_ARG (addr);
  return -1;
#endif /* ACE_HAS_FORE_ATM_XTI */
}

char *
ACE_ATM_Addr::construct_options(ACE_HANDLE fd,
                                int qos_kb,
                                int flags,
                                long *optsize)
{
#if defined (ACE_HAS_FORE_ATM_XTI)
  struct t_opthdr *popt;
  char *buf;
  int qos_cells;
  struct t_info info;

  if (ACE_OS::t_getinfo (fd, &info) == -1)
    {
      ACE_OS::t_error ("t_getinfo");
      return 0;
    }
 
  buf = (char *) ACE_OS::malloc (info.options);

  if (buf == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("Unable to allocate %ld bytes for options\n"),
                       info.options),
                      0);

  popt = (struct t_opthdr *) buf;

  if (flags & OPT_FLAGS_CPID)
    {
      // This constructs the T_ATM_ORIG_ADDR option, which is used to
      // signal the UNI 3.1 Calling Party ID Information Element.
      t_atm_addr *source_addr;

      popt->len	= sizeof (struct t_opthdr) + sizeof (t_atm_addr);
      popt->level = T_ATM_SIGNALING;
      popt->name = T_ATM_ORIG_ADDR;
      popt->status = 0;

      source_addr = 
        (t_atm_addr *)((char *) popt + sizeof (struct t_opthdr));

      source_addr->address_format = T_ATM_ENDSYS_ADDR;
      source_addr->address_length = ATMNSAP_ADDR_LEN;

      if (get_local_address (fd, source_addr->address))
        {
          ACE_ERROR ((LM_ERROR,
                      ASYS_TEXT ("Can't get local address!\n")));
          ACE_OS::free (buf);
          return 0;
        }

      popt = T_OPT_NEXTHDR (buf, info.options , popt);
    }

  // This constructs all options necessary (bearer cap., QoS, and
  // Traffic Descriptor) to signal for a CBR connection with the
  // specified QoS in kbit/sec., and/or specify a PMP connection.

  // For FORE 200e cards, the adapter shapes traffic to CBR with rate
  // equal to PCR CLP=0+1 (traffic.forward.PCR_all_traffic)

  qos_cells = (qos_kb * 1000) / (48*8);

  if ((qos_cells > 0 && qos_cells < LINE_RATE) 
      || (ACE_BIT_ENABLED (flags, OPT_FLAGS_PMP)))
    {
      struct t_atm_bearer *bearer;
      struct t_atm_traffic *traffic;

      // T_ATM_BEARER_CAP: Broadband bearer capability
      popt->len	= sizeof (struct t_opthdr) + sizeof (struct t_atm_bearer);
      popt->level = T_ATM_SIGNALING;
      popt->name = T_ATM_BEARER_CAP;
      popt->status = 0;

      bearer = (struct t_atm_bearer *)((char *) popt + sizeof (struct t_opthdr));
      bearer->bearer_class = T_ATM_CLASS_X;

      if (qos_cells)
        {
          bearer->traffic_type = T_ATM_CBR;
          bearer->timing_requirements = T_ATM_END_TO_END;
        }
      else
        {
          bearer->traffic_type	 = 0; // UBR
          bearer->timing_requirements = 0;
        }
      bearer->clipping_susceptibility = T_ATM_NULL;

      if (ACE_BIT_ENABLED (flags, OPT_FLAGS_PMP))
        bearer->connection_configuration = T_ATM_1_TO_MANY;
      else
        bearer->connection_configuration = T_ATM_1_TO_1;

      popt = T_OPT_NEXTHDR (buf, info.options, popt);

      // T_ATM_TRAFFIC: traffic descriptor
      popt->len	= sizeof (struct t_opthdr) + sizeof (struct t_atm_traffic);
      popt->level = T_ATM_SIGNALING;
      popt->name = T_ATM_TRAFFIC;
      popt->status = 0;

      traffic = (struct t_atm_traffic *)((char *) popt + sizeof (struct t_opthdr));

      traffic->forward.PCR_high_priority = T_ATM_ABSENT;
      traffic->forward.PCR_all_traffic = qos_cells ? qos_cells : LINE_RATE;
      traffic->forward.SCR_high_priority = T_ATM_ABSENT;
      traffic->forward.SCR_all_traffic = T_ATM_ABSENT;
      traffic->forward.MBS_high_priority = T_ATM_ABSENT;
      traffic->forward.MBS_all_traffic = T_ATM_ABSENT;
      traffic->forward.tagging = T_NO;

      traffic->backward.PCR_high_priority = T_ATM_ABSENT;
      traffic->backward.PCR_all_traffic	= 
        (ACE_BIT_ENABLED (flags, OPT_FLAGS_PMP)) 
        ? 0 : qos_cells ? qos_cells : LINE_RATE;
      traffic->backward.SCR_high_priority = T_ATM_ABSENT;
      traffic->backward.SCR_all_traffic	= T_ATM_ABSENT;
      traffic->backward.MBS_high_priority = T_ATM_ABSENT;
      traffic->backward.MBS_all_traffic	= T_ATM_ABSENT;
      traffic->backward.tagging = T_NO;

      traffic->best_effort = qos_cells ? T_NO : T_YES;

      popt = T_OPT_NEXTHDR (buf,
                            info.options,
                            popt);
    }

  if (qos_cells > 0 && qos_cells < LINE_RATE)
    {
      struct t_atm_qos *qos;

      // T_ATM_QOS: Quality of Service
      popt->len	= sizeof (struct t_opthdr) + sizeof (struct t_atm_qos);
      popt->level = T_ATM_SIGNALING;
      popt->name = T_ATM_QOS;
      popt->status = 0;

      qos = (struct t_atm_qos *)((char *) popt + sizeof (struct t_opthdr));
      qos->coding_standard = T_ATM_ITU_CODING;
      qos->forward.qos_class = T_ATM_QOS_CLASS_1;
      qos->backward.qos_class = T_ATM_QOS_CLASS_1;

      popt = T_OPT_NEXTHDR (buf, info.options, popt);
    }

  // return actual size of options and option buffer to user
  *optsize = (char *) popt - buf;

  return buf;
#else
  ACE_UNUSED_ARG (fd);
  ACE_UNUSED_ARG (qos_kb);
  ACE_UNUSED_ARG (flags);
  ACE_UNUSED_ARG (optsize);
  return 0;
#endif /* ACE_HAS_FORE_ATM_XTI */
}
