// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/Acceptor_Registry.h"
#include "tao/Stub.h"
#include "tao/Environment.h"
#include "tao/GIOP.h"
#include "tao/ORB_Core.h"
#include "tao/params.h"
#include "tao/MProfile.h"
#include "tao/debug.h"

#include "ace/Auto_Ptr.h"

#if !defined(__ACE_INLINE__)
#include "tao/Acceptor_Registry.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Acceptor_Registry, "$Id$")

TAO_Acceptor_Registry::TAO_Acceptor_Registry (void)
  : acceptors_ ()
{
}

TAO_Acceptor_Registry::~TAO_Acceptor_Registry (void)
{
  this->close_all ();
}

size_t
TAO_Acceptor_Registry::endpoint_count (void)
{
  int count = 0;
  TAO_AcceptorSetItor end = this->end ();

  for (TAO_AcceptorSetItor i = this->begin (); i != end; ++i)
    count += (*i)->endpoint_count ();

  return count;
}

int
TAO_Acceptor_Registry::make_mprofile (const TAO_ObjectKey &object_key,
                                      TAO_MProfile &mprofile)
{
  TAO_AcceptorSetItor end = this->end ();

  for (TAO_AcceptorSetItor i = this->begin (); i != end; ++i)
    if ((*i)->create_mprofile (object_key,
                               mprofile) == -1)
      return -1;

  return 0;
}

int
TAO_Acceptor_Registry::is_collocated (const TAO_MProfile &mprofile)
{
  TAO_AcceptorSetItor end = this->end ();

  for (TAO_AcceptorSetItor i = this->begin (); i != end; ++i)
    {
      for (TAO_PHandle j = 0;
           j != mprofile.profile_count ();
           ++j)
        {
          const TAO_Profile *profile = mprofile.get_profile (j);

          // Check the address for equality.
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

  if (first_endpoint == last_endpoint)
    {
      // No endpoints were specified, we let each protocol pick its
      // own default.

      // All TAO pluggable protocols are expected to have the ability
      // to create a default endpoint.

      return this->open_default (orb_core);
    }

  ACE_Auto_Basic_Array_Ptr <char> addr_str;

  for (TAO_EndpointSetIterator endpoint = first_endpoint;
       endpoint != last_endpoint;
       ++endpoint)
    {
      ACE_CString iop = (*endpoint);

      // IOP://address1,address2
      //    ^ slot
      int slot = iop.find ("://", 0);
      if (slot == iop.npos)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Invalid endpoint epecification: "
                           "<%s>.\n",
                           iop.c_str ()),
                          -1);
      ACE_CString prefix = iop.substring (0, slot);

      // @@ We could move the protocol factory loop to the outermost
      //    level but for now we leave it inside the endpoint loop
      //    under the assumption that there will be more endpoints
      //    than protocols.

      // Now get the list of available protocol factories.
      TAO_ProtocolFactorySetItor end =
        orb_core->protocol_factories ()->end ();

      int found = 0;
      // If usable protocol (factory) is found then this will be
      // set equal to 1.

      for (TAO_ProtocolFactorySetItor factory =
             orb_core->protocol_factories ()->begin ();
           factory != end;
           ++factory)
        {
          if ((*factory)->factory ()->match_prefix (prefix))
            {
              found = 1;  // A usable protocol was found.

              // check for the presence of addresses.  Get length and
              // subtract  3 for the three chars `://'
              if (slot == ACE_static_cast (int, iop.length () - 3))
                {
                  // Protocol was specified without an endpoint.
                  // According to the "iioploc" spec, this is valid.
                  // As such, we extend this feature to all pluggable
                  // protocols.  All TAO pluggable protocols are
                  // expected to have the ability to create a default
                  // endpoint.

                  if (this->open_default (orb_core, factory) == 0)
                    continue;
                  else
                    return -1;  // Problem creating default server.
                }

              // increment slot past the "://" (i.e. add 3)
              ACE_CString addrs = iop.substring (slot + 3);

              // @@ Should we check for something else besides '/',
              // i.e. make it protocol dependent?  -- Ossama
              if (addrs [addrs.length () - 1] == '/')
                // Get rid of trailing '/'.
                addrs [addrs.length () - 1] = '\0';

              char *last_addr=0;
              addr_str.reset (addrs.rep ());

              // Iterate over the addrs specified in the endpoint.
              for (char *astr = ACE_OS::strtok_r (addr_str.get (),
                                                  ",",
                                                  &last_addr);
                   astr != 0 ;
                   astr = ACE_OS::strtok_r (0,
                                            ",",
                                            &last_addr))
                {
                  ACE_CString address (astr);

                  TAO_Acceptor *acceptor =
                    (*factory)->factory ()->make_acceptor ();
                  if (acceptor != 0)
                    {
                      // Check if an "N.n@" version prefix was
                      // specified.
                      // @@ For now, we just drop the version prefix.
                      // At some point in the future it may become
                      // useful.
                      int major = -1;
                      int minor = -1;
                      const char *temp_iop = address.c_str ();
                      if (isdigit (temp_iop[0])
                          && temp_iop[1] == '.'
                          && isdigit (temp_iop[2])
                          && temp_iop[3] == '@')
                        {
                          major = temp_iop[0] - '0';
                          minor = temp_iop[2] - '0';
                          address = address.substring (4);
                        }

                      if (acceptor->open (orb_core,
                                          major, minor,
                                          address) == -1)
                        {
                          delete acceptor;

                          return -1;
                        }

                      // add acceptor to list.
                      if (this->acceptors_.insert (acceptor) == -1)
                        {
                          delete acceptor;

                          ACE_ERROR_RETURN ((LM_ERROR,
                                             "TAO (%P|%t) unable to add <%s> "
                                             "to acceptor registry.\n",
                                             address.c_str ()),
                                            -1);
                        }
                    }
                  else
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                                         "TAO (%P|%t) unable to create "
                                         "an acceptor for <%s>\n",
                                         iop.c_str ()),
                                        -1);
                    }
                }
            }
          else
            continue;
        }

      if (found == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "TAO (%P|%t) no usable transport protocol "
                             "was found\n"),
                            -1);
        }
    }

  return 0;
}

// Iterate through the loaded transport protocols and create a default 
// server for each protocol.
int TAO_Acceptor_Registry::open_default (TAO_ORB_Core *orb_core)
{
  TAO_ProtocolFactorySetItor end =
    orb_core->protocol_factories ()->end ();

  // loop through all the loaded protocols...
  for (TAO_ProtocolFactorySetItor i =
         orb_core->protocol_factories ()->begin ();
       i != end;
       ++i)
    {
      // if the protocol requires an explicit -ORBendpoint option then 
      // don't use it, otherwise open a default endpoint for that
      // protocol, this solves the problem with persistent endpoints
      // (such as UNIX domain rendesvouz points), that are not cleaned 
      // up if the server crashes.
      if (!(*i)->factory ()->requires_explicit_endpoint ())
        {
          if (this->open_default (orb_core, i) != 0)
            return -1;
        }
    }

  return 0;
}

// Used when no endpoints were specified.  Open a default server for
// the indicated protocol.
int
TAO_Acceptor_Registry::open_default (TAO_ORB_Core *orb_core,
                                     TAO_ProtocolFactorySetItor &factory)
{
  // No endpoints were specified, we let each protocol pick its own
  // default endpoint.

  // Make an acceptor
  TAO_Acceptor *acceptor =
    (*factory)->factory ()->make_acceptor ();

  if (acceptor == 0)
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "TAO (%P|%t) unable to create "
                    "an acceptor for <%s>\n",
                    (*factory)->protocol_name ().c_str ()));

      return -1;
    }

  // Initialize the acceptor to listen on a default endpoint.
  if (acceptor->open_default (orb_core) == -1)
    {
      delete acceptor;

      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "TAO (%P|%t) unable to open "
                    "default acceptor for <%s>%p\n",
                    (*factory)->protocol_name ().c_str (), ""));

      return -1;
    }

  if (this->acceptors_.insert (acceptor) == -1)
    {
      delete acceptor;

      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                           "TAO (%P|%t) unable to add <%s> default_acceptor "
                           "to acceptor registry.\n",
                           (*factory)->protocol_name ().c_str ()));

      return -1;
    }

  return 0;
}

int
TAO_Acceptor_Registry::close_all (void)
{
  TAO_AcceptorSetItor end =
                this->acceptors_.end ();

  for (TAO_AcceptorSetItor i = this->acceptors_.begin ();
       i != end;
       ++i)
    {
      if (*i == 0)
        continue;

      (*i)->close ();

      delete *i;
    }

  this->acceptors_.reset ();
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node<TAO_Acceptor*>;
template class ACE_Unbounded_Set<TAO_Acceptor*>;
template class ACE_Unbounded_Set_Iterator<TAO_Acceptor*>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node<TAO_Acceptor*>
#pragma instantiate ACE_Unbounded_Set<TAO_Acceptor*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_Acceptor*>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
