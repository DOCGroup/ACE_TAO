// $Id$
//
// @(#)iioporb.cpp      1.8 95/09/19
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// IIOP:                ORB pseudo-object
//
// This includes objref stringification/destringification for IIOP
// object references.

#include "tao/corba.h"

#if !defined (__ACE_INLINE__)
# include "tao/IIOP_ORB.i"
#endif /* ! __ACE_INLINE__ */

static const char ior_prefix [] = "IOR:";
static const char iiop_prefix [] = "iiop:";

// Objref stringification.

CORBA::String
IIOP_ORB::object_to_string (CORBA::Object_ptr obj,
                            CORBA::Environment &env)
{
  env.clear ();

  // Application writer controls what kind of objref strings they get,
  // maybe along with other things, by how they initialize the ORB.

  if (use_omg_ior_format_)
    {
      // By default, orbs use IOR strings; these are ugly (and error
      // prone) but specified by CORBA.
      //
      // XXX there should be a simple way to reuse this code in other
      // ORB implementations ...

      char *bytes;
      // @@ Is BUFSIZ the right size here?
      char buf [BUFSIZ];
      CDR cdr (buf, sizeof buf, TAO_ENCAP_BYTE_ORDER);

      bytes = buf;
      // support limited oref ACE_OS::strcmp.
      (void) ACE_OS::memset (bytes, 0, BUFSIZ);

      // Marshal the objref into an encapsulation bytestream.
      (void) cdr.put_char (TAO_ENCAP_BYTE_ORDER);
      if (cdr.encode (CORBA::_tc_Object,
                        &obj, 0,
                        env) != CORBA::TypeCode::TRAVERSE_CONTINUE)
        return 0;

      // Now hexify the encapsulated CDR data into a string, and
      // return that string.

      CORBA::String cp;
      size_t len = cdr.length ();

      CORBA::String string = CORBA::string_alloc (sizeof ior_prefix + 2 * len);

      ACE_OS::strcpy ((char *) string, ior_prefix);

      bytes = cdr.buffer ();

      for (cp = (CORBA::String) ACE_OS::strchr ((char *) string, ':') + 1;
           len--;
           bytes++)
        {
          *cp++ = ACE::nibble2hex ((*bytes) >> 4);
          *cp++ = ACE::nibble2hex (*bytes);
        }

      *cp = 0;
      if (this->optimize_collocation_objects_)
        {
          IIOP_Object *iiopobj;
          if (obj->QueryInterface (IID_IIOP_Object, (void **) &iiopobj) != TAO_NOERROR)
            {
              env.exception (new CORBA_DATA_CONVERSION (CORBA::COMPLETED_NO));
              return 0;
            }
          this->_register_collocation (iiopobj->profile.object_addr ());
        }
      return string;
    }
  else
    {
      // The "internet" ORB uses readable URL style objrefs, as used
      // in the World Wide Web.
      //
      // NOTE: the version ID in the string is ugly but we can't
      // realistically eliminate it by any "assume 1.0" strategy...
      // Similarly with the port, because there's no single IIOP port
      // to which we could default.

      static const char digits [] = "0123456789";

      // This only works for IIOP objrefs.  If we're handed an objref
      // that's not an IIOP objref, fail -- application must use an
      // ORB that's configured differently.

      IIOP_Object *obj2;

      if (obj->QueryInterface (IID_IIOP_Object,
                               (void **) &obj2) != TAO_NOERROR)
        {
          env.exception (new CORBA_DATA_CONVERSION (CORBA::COMPLETED_NO));
          return 0;
        }

      if (!obj2)                        // null?
        return CORBA::string_copy ((CORBA::String) iiop_prefix);

      CORBA::String_var key;
      TAO_POA::encode_sequence_to_string (key.inout(),
                                          obj2->profile.object_key);

      u_int buflen = (ACE_OS::strlen (iiop_prefix) +
                      1 /* major # */ + 1 /* minor # */ +
                      2 /* double-slash separator */ +
                      ACE_OS::strlen (obj2->profile.host) +
                      1 /* colon separator */ +
                      5 /* port number */ +
                      1 /* slash separator */ +
                      ACE_OS::strlen (key) +
                      1 /* zero terminator */);
      CORBA::String buf = CORBA::string_alloc (buflen);

      ACE_OS::sprintf (buf, "%s%c.%c//%s:%d/%s", iiop_prefix,
                       digits [obj2->profile.iiop_version.major],
                       digits [obj2->profile.iiop_version.minor],
                       obj2->profile.host, obj2->profile.port,
                       key.in ());

      this->_register_collocation (obj2->profile.object_addr ());
      return buf;
    }
}

// Destringify OMG-specified "IOR" string.
//
// XXX there should be a simple way to reuse this code in other ORB
// implementations ...

static CORBA::Object_ptr
ior_string_to_object (CORBA::String str,
                      CORBA::Environment &env)
{
  // Unhex the bytes, and make a CDR deencapsulation stream from the
  // resulting data.

  char *buffer;
  ACE_NEW_RETURN (buffer,
                  char [1 + ACE_OS::strlen ((char *) str) / 2],
                  CORBA_Object::_nil ());

  char *tmp = (char *) str;
  size_t len = 0;

  while (tmp [0] && tmp [1])
    {
      u_char byte;

      if (!(isxdigit (tmp [0]) && isxdigit (tmp [1])))
        break;

      byte = (u_char) (ACE::hex2byte (tmp [0]) << 4);
      byte |= ACE::hex2byte (tmp [1]);

      buffer [len++] = byte;
      tmp += 2;
    }

  if (tmp [0] && !isspace (tmp [0]))
    {
      delete [] buffer;
      env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO));
      return 0;
    }

  // Create deencapsulation stream ... then unmarshal objref from that
  // stream.

  CDR stream;
  CORBA::Object_ptr objref;

  stream.setup_encapsulation (buffer, len);
  if (stream.decode (CORBA::_tc_Object,
                    &objref, 0,
                    env) != CORBA::TypeCode::TRAVERSE_CONTINUE)
    objref = 0;

  delete [] buffer;
  return objref;
}

// Destringify URL style IIOP objref.

static CORBA::Object_ptr
iiop_string_to_object (CORBA::String string,
                       CORBA::Environment &env)
{
  // NIL objref encodes as just "iiop:" ... which has already been
  // removed, so we see it as an empty string.

  if (!string || !*string)
    return 0;

  // type ID not encoded in this string ... makes narrowing rather
  // expensive, though it does ensure that type-safe narrowing code
  // gets thoroughly excercised/debugged!  Without a typeID, the
  // _narrow will be required to make an expensive remote "is_a" call.

  IIOP_Object *data;

  // null type ID.
  ACE_NEW_RETURN (data, IIOP_Object ((char *) 0), 0);

  // Remove the "N.N//" prefix, and verify the version's one
  // that we accept

  if (isdigit (string [0]) && isdigit (string [2]) && string [1] == '.'
      && string [3] == '/' && string [4] == '/')
    {
      data->profile.iiop_version.major = (char) (string [0] - '0');
      data->profile.iiop_version.minor = (char) (string [2] - '0');
      string += 5;
    }
  else
    {
      env.exception (new CORBA_DATA_CONVERSION (CORBA::COMPLETED_NO));
      data->Release ();
      return 0;
    }

  if (data->profile.iiop_version.major != IIOP::MY_MAJOR
      || data->profile.iiop_version.minor > IIOP::MY_MINOR)
    {
      env.exception (new CORBA_DATA_CONVERSION (CORBA::COMPLETED_NO));
      data->Release ();
      return 0;
    }

  // Pull off the "hostname:port/" part of the objref

  char *cp = ACE_OS::strchr (string, ':');
  if (cp == 0)
    {
      env.exception (new CORBA_DATA_CONVERSION (CORBA::COMPLETED_NO));
      data->Release ();
      return 0;
    }

  data->profile.host = CORBA::string_alloc (1 + cp - string);
  for (cp = data->profile.host;
       *string != ':';
       *cp++ = *string++)
    continue;

  *cp = 0;
  string++;

  cp = ACE_OS::strchr ((char *) string, '/');

  if (cp == 0)
    {
      env.exception (new CORBA_DATA_CONVERSION (CORBA::COMPLETED_NO));
      CORBA::string_free (data->profile.host);
      data->profile.host = 0;
      data->Release ();
      return 0;
    }

  data->profile.port = (short) ACE_OS::atoi ((char *) string);
  data->profile.object_addr (0);
  string = ++cp;

  // Parse the object key
  TAO_POA::decode_string_to_sequence (data->profile.object_key,
                                      string);

  // Create the CORBA level proxy.
  TAO_ServantBase *servant = TAO_ORB_Core_instance ()->orb ()->_get_collocated_servant (data);

  CORBA_Object *obj = new CORBA_Object (data, servant, servant != 0);

  // Clean up in case of error
  if (obj == 0)
    data->Release ();

  return obj;
}

// Destringify arbitrary objrefs.

CORBA::Object_ptr
IIOP_ORB::string_to_object (CORBA::String str,
                            CORBA::Environment &env)
{
  env.clear ();

  CORBA::Object_ptr obj = 0;

  // Use the prefix code to choose which destringify algorithm to use.
  if (ACE_OS::strncmp ((char *)str,
                       iiop_prefix, sizeof iiop_prefix - 1) == 0)
    obj = iiop_string_to_object (str + sizeof iiop_prefix - 1, env);

  else if (ACE_OS::strncmp ((char *)str,
                            ior_prefix,
                            sizeof ior_prefix - 1) == 0)
    obj = ior_string_to_object (str + sizeof ior_prefix - 1, env);

  // Return the object
  return obj;
}

int
IIOP_ORB::_register_collocation (ACE_Addr &addr)
{
  if (this->optimize_collocation_objects_)
    return this->collocation_record_.insert ((ACE_INET_Addr&) addr);
  else
    return 0;
}

TAO_ServantBase *
IIOP_ORB::_get_collocated_servant (STUB_Object *sobj)
{
  if (this->optimize_collocation_objects_ && sobj != 0)
    {
      IIOP_Object *iiopobj;
      // Make sure users passed in an IIOP_Object otherwise, we don't know what to do next.
      if (sobj->QueryInterface (IID_IIOP_Object, (void **) &iiopobj) != TAO_NOERROR)
        {
          ACE_ERROR ((LM_ERROR, "%p: Passing IIOP ORB and non-IIOP object\n", "_get_collocated_object"));
          return 0;               // Something must be wrong!
        }

      if (this->collocation_record_.find (iiopobj->profile.object_addr ()) == 0)
          // Check if the object requested is a collocated object.
        {
          CORBA::Environment env;
          TAO_ObjectKey *objkey = iiopobj->key (env);

          if (env.exception ())
            {
              delete objkey;
              return 0;
            }
          TAO_POA *poa = (TAO_POA *) this->resolve_poa ();

          if (poa != 0)
            {
              PortableServer::Servant servant = poa->find_servant (*objkey,
                                                                      env);
              if (env.exception ())
                {
                  delete objkey;
                  return 0;
                }
              return servant;
            }
        }
    }

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<IIOP_ORB, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Unbounded_Set<ACE_INET_Addr>;
template class ACE_Unbounded_Set_Iterator<ACE_INET_Addr>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<IIOP_ORB, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Unbounded_Set<ACE_INET_Addr>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_INET_Addr>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
