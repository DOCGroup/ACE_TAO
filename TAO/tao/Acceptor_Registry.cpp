// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/Acceptor_Registry.h"
#include "tao/Pluggable.h"
#include "tao/Stub.h"
#include "tao/Environment.h"
#include "tao/GIOP.h"
#include "tao/Protocol_Factory.h"
#include "tao/ORB_Core.h"
#include "ace/Auto_Ptr.h"

TAO_Acceptor_Registry::TAO_Acceptor_Registry (void)
{
}

TAO_Acceptor_Registry::~TAO_Acceptor_Registry (void)
{
}

TAO_Acceptor  *
TAO_Acceptor_Registry::get_acceptor (CORBA::ULong tag)
{
  // @@ Fred&Ossama: Since this is going to be a common operation you
  //    may want to consider using a Hash_Map_Manager, or even a
  //    simple Map_Manager.

  TAO_AcceptorSetItor end =
                this->acceptors_.end ();
  TAO_AcceptorSetItor acceptor =
                this->acceptors_.begin ();

  for (; acceptor != end; acceptor++)
    {
      if ((*acceptor)->tag () == tag)
        return (*acceptor);
    }
  return 0;
}

int
TAO_Acceptor_Registry::is_collocated (const TAO_MProfile &mprofile)
{
  // @@ Fred&Osssama: we should optimize this: we loop over the
  //    profiles here and in the ORB::is_collocated() method, maybe we
  //    should return the index of the profile that matched?
  //    What happens if the address matches but the object key does
  //    not? Should we keep on searching in the ORB loop?

  TAO_AcceptorSetItor end = this->acceptors_.end ();

  for (TAO_AcceptorSetItor i = this->acceptors_.begin (); i != end; ++i)
    {
      for (TAO_PHandle j = 0;
           j != mprofile.profile_count ();
           ++j)
        {
          const TAO_Profile* profile = mprofile.get_profile (j);
          if ((*i)->tag () == profile->tag ()
              && (*i)->is_collocated (profile))
            return 1;
        }
    }
  return 0;
}

int
TAO_Acceptor_Registry::open (TAO_ORB_Core *orb_core)
{
  // @@ Fred&Ossama: This is not the problem you have to solve right
  //    now, but we thought about giving explicit names to each
  //    endpoint, the user could then use those names to setup
  //    policies in the ORB that indicate which endpoints are served
  //    by which thread.
  //    IMHO that is one more reason to keep a list of endpoints in
  //    the ORB_Core, instead of a string.
  //    BTW, that also removes the restriction of using ';' in the
  //    addresses.  Just my two cents.

  // protocol_factories is in the following form
  //   IOP1://addr1,addr2,...,addrN/;IOP2://addr1,...addrM/;...

  ACE_Auto_Basic_Array_Ptr <char>
                        str (orb_core->orb_params ()->endpoints ().rep ());

  ACE_Auto_Basic_Array_Ptr <char> addr_str;
  char *last_iop=0;

  for (char *iop_str = ACE_OS::strtok_r (str.get (), ";", &last_iop);
     iop_str != 0;
     iop_str = ACE_OS::strtok_r (0, ";",  &last_iop))
    {

      ACE_CString iop (iop_str);
      int indx = iop.find ("://", 0);
      if ( indx == iop.npos)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Invalid endpoint epecification.\n"),
                       -1);
        }

      ACE_CString prefix = iop.substring (0, indx);
      ACE_CString addrs  = iop.substring (indx+3);
      if (addrs [addrs.length () - 1] == '/')
        addrs [addrs.length () - 1] = '\0'; // get rid of trailing /

      char *last_addr=0;
      addr_str.reset (addrs.rep ());
      for (char *astr = ACE_OS::strtok_r (addr_str.get (), ",", &last_addr);
                astr != 0 ;
                astr = ACE_OS::strtok_r (0,
                                         ",",
                                         &last_addr))
        {
          ACE_CString address (astr);

          // Now get the list of avaliable protocol factories.
          TAO_ProtocolFactorySetItor end =
                          orb_core->protocol_factories ()->end ();
          TAO_ProtocolFactorySetItor factory =
                        orb_core->protocol_factories ()->begin ();

          TAO_Acceptor *acceptor;
          for (acceptor = 0 ;
               factory != end ;
               factory++)
            {
              if ((*factory)->factory ()->match_prefix (prefix))
                {
                  if ((acceptor = (*factory)->factory ()->make_acceptor ()))
                    {
                      // add acceptor to list.
                      this->acceptors_.insert (acceptor);

                      if (acceptor->open (orb_core, address) == -1)
                        return -1;

                      break;
                    }
                  else
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                                         "(%P|%t) Unable to create an acceptor for %s\n",
                                         str.get ()),
                                        -1);
                    }
                }
              else
                continue;
            }
        }
    }

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<TAO_Acceptor*>;

template class ACE_Unbounded_Set<TAO_Acceptor*>;

template class ACE_Unbounded_Set_Iterator<TAO_Acceptor*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<TAO_Acceptor*>;

#pragma instantiate ACE_Unbounded_Set<TAO_Acceptor*>;

#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_Acceptor*>;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
