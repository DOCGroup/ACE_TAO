// $Id$
#include "ACE_Mem_Map.h"
#include "new.h"
#include "Multicast_Manager.h"
#include "global.h"

// Initialize all the static member vars.
int Multicast_Manager::received_host_count	= 0;
Multicast_Manager::Host_Elem	*Multicast_Manager::drwho_list		= 0;
Multicast_Manager::Host_Elem	*Multicast_Manager::current_ptr		= 0;

/* Names of hosts to query for friend info. */
char *Multicast_Manager::host_names[] =
{
  "mabillon.ics.uci.edu",   /* "128.195.14.4", */
  "beaver.ics.uci.edu",     /* "128.195.2.1", */
  "convention.ics.uci.edu", /* "128.195.8.9", */
  "valentine.ics.uci.edu",  /* "128.200.38.10", */
  "abp.ics.uci.edu",	    /* "128.200.39.23", */
  "cress.ics.uci.edu",      /* "128.195.5.8", */
#if 0
  "murphy.ics.uci.edu",     /* "128.195.4.11", */
  "madeleine.ics.uci.edu",  /* "128.200.32.2", */
  "liege.ics.uci.edu",      /* "128.195.13.1", */
  "ics.uci.edu",	    /* "128.195.1.1", */
  "128.195.10.16", 	/* vlsi.ics.uci.edu */
#endif
  0			/* The NULL entry... ;-) */
};

void
Multicast_Manager::insert_default_hosts (void)
{
  /* Enter the static list of hosts into the dynamic table! */

  for (char **np = host_names; *np != 0; np++)
    Multicast_Manager::add_host (*np);
}

/* Inserts all the names in FILENAME into the list of hosts to contact. */

int    
Multicast_Manager::insert_hosts_from_file (char *filename)
{
  ACE_Mem_Map mmap (filename);
  char *host_ptr = mmap.addr ();

  if (host_ptr == 0)
    return -1;
  else
    {

      for (char *end_ptr = host_ptr + mmap.size (); host_ptr < end_ptr; )
	{
	  Multicast_Manager::add_host (host_ptr);

	  while (*host_ptr != '\n')
	    host_ptr++;

	  *host_ptr++ = '\0';
	}

      return 0;
    }
}

/* Returns the IP host address for the next unexamined host in the list. 
   If no more unexamined hosts remain a 0 is returned, else a 1. */

int	
Multicast_Manager::get_next_host_addr (in_addr &host_addr)
{
  for (Multicast_Manager::current_ptr = Multicast_Manager::current_ptr == 0 ? Multicast_Manager::drwho_list : Multicast_Manager::current_ptr->next;
       Multicast_Manager::current_ptr != 0;
       Multicast_Manager::current_ptr = Multicast_Manager::current_ptr->next)
    
      {
	hostent *hp;
	char	*host_name = Multicast_Manager::current_ptr->host_name;

	if ((hp = Multicast_Manager::get_host_entry (host_name)) == 0)
	  {
	    fprintf (stderr, "%s: host unknown.\n", host_name);
	    continue;
	  }

	Multicast_Manager::received_host_count++;
	ACE_OS::memcpy (&host_addr,
                        hp->h_addr,
                        sizeof host_addr);
	ACE_OS::memcpy (&Multicast_Manager::current_ptr->host_addr,
                        hp->h_addr,
                        sizeof host_addr);
	return 1;
      }

  return 0;
}

/* This function attempts to get the internet address for either a
   hostname or hostnumber.  The function makes the simplifying
   assumption that hostnames begin with an alphabetic character! */

hostent *
Multicast_Manager::get_host_entry (char *host)
{
  static hostent host_entry;
  auto	 hostent *hp;
  
  if (isdigit (*host)) /* IP address. */
    { 
      unsigned long ia = inet_addr (host);

      hp = ia == (unsigned long) -1 ? 0 : gethostbyaddr ((char *) &ia, sizeof ia, AF_INET);
    } 
  else /* Host name. */
    hp = gethostbyname (host);

  return hp == 0 ? 0 : (hostent *) memcpy (&host_entry, hp, sizeof *hp);
}

/* Adds an additional new host to the list of host machines. */

void 
Multicast_Manager::add_host (char *host_name)
{
  Multicast_Manager::drwho_list = new (PRIVATE_POOL) Host_Elem (host_name, Multicast_Manager::drwho_list);
}

void 
Multicast_Manager::checkoff_host (in_addr host_addr)
{
  for (Host_Elem *tmp = Multicast_Manager::drwho_list; tmp != 0; tmp = tmp->next)
    if (ACE_OS::memcmp (&tmp->host_addr.s_addr,
                        &host_addr.s_addr,
                        sizeof host_addr.s_addr) == 0)
      {
	tmp->checked_off = 1;
	Multicast_Manager::received_host_count--;
	return;
      }
}

int 
Multicast_Manager::get_next_non_responding_host (char *&host_name)
{
  for (Multicast_Manager::current_ptr = Multicast_Manager::current_ptr == 0 ? Multicast_Manager::drwho_list : Multicast_Manager::current_ptr->next;
       Multicast_Manager::current_ptr != 0;
       Multicast_Manager::current_ptr = Multicast_Manager::current_ptr->next)
    if (!Multicast_Manager::current_ptr->checked_off)
      {
	host_name = Multicast_Manager::current_ptr->host_name;
	return 1;
      }
  
  return 0;
}

#if !defined (__OPTIMIZE__)
Multicast_Manager::Host_Elem::Host_Elem (char *h_name, Multicast_Manager::Host_Elem *n)
: host_name (h_name), checked_off (0), next (n)
{
}

int 
Multicast_Manager::outstanding_hosts_remain (void)
{
  return Multicast_Manager::received_host_count > 0;
}
#endif				/* __OPTIMIZE__ */
