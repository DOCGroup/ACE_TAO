// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/Acceptor_Registry.h"
#include "tao/Pluggable.h"
#include "tao/Stub.h"
#include "tao/Environment.h"
#include "tao/GIOP.h"
#include "tao/Protocol_Factory.h"
#include "tao/ORB_Core.h"
#include "tao/params.h"
#include "tao/MProfile.h"

#include "ace/Auto_Ptr.h"

typedef ACE_Unbounded_Set_Iterator<TAO_Acceptor*>
        TAO_AcceptorSetItor;

TAO_Acceptor_Registry::TAO_Acceptor_Registry (void)
{
}

TAO_Acceptor_Registry::~TAO_Acceptor_Registry (void)
{
}

size_t
TAO_Acceptor_Registry::endpoint_count (void)
{
  int count = 0;
  TAO_AcceptorSetItor end =
                this->acceptors_.end ();
  TAO_AcceptorSetItor acceptor =
                this->acceptors_.begin ();

  for (; acceptor != end; acceptor++)
    {
      count += (*acceptor)->endpoint_count ();
    }

  return count;
}

int
TAO_Acceptor_Registry::make_mprofile (const TAO_ObjectKey &object_key,
                                      TAO_MProfile &mprofile)
{
  TAO_AcceptorSetItor end =
                this->acceptors_.end ();
  TAO_AcceptorSetItor acceptor =
                this->acceptors_.begin ();

  for (; acceptor != end; ++acceptor)
    {
      if ((*acceptor)->create_mprofile (object_key, mprofile) == -1)
        return -1;
	    }

  return 0;
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

  for (; acceptor != end; ++acceptor)
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

  // protocol_factories is in the following form
  //   IOP1://addr1,addr2,...,addrN/;IOP2://addr1,...addrM/;...

  TAO_EndpointSetIterator first_endpoint =
    orb_core->orb_params ()->endpoints ().begin ();

  TAO_EndpointSetIterator last_endpoint =
    orb_core->orb_params ()->endpoints ().end ();

  ACE_Auto_Basic_Array_Ptr <char> addr_str;

  for (TAO_EndpointSetIterator endpoint = first_endpoint;
       endpoint != last_endpoint;
       ++endpoint)
    {
      ACE_CString iop = (*endpoint);

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
          

          for (TAO_ProtocolFactorySetItor factory =
                 orb_core->protocol_factories ()->begin ();
               factory != end;
               ++factory)
            {
              if ((*factory)->factory ()->match_prefix (prefix))
                {
                  TAO_Acceptor *acceptor =
                    (*factory)->factory ()->make_acceptor ();
                  if (acceptor != 0)
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
                                         "(%P|%t) Unable to create an "
                                         "acceptor for %s\n",
                                         iop.c_str ()),
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
