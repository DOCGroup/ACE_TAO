// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/Pluggable.h"
#include "tao/Stub.h"
#include "tao/Environment.h"
#include "tao/GIOP.h"
#include "tao/CDR.h"
#include "tao/Object_KeyC.h"
#include "tao/debug.h"

#include "ace/ACE.h"

#if !defined (__ACE_INLINE__)
# include "tao/Pluggable.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Pluggable, "$Id$")

TAO_IOP_Version::~TAO_IOP_Version (void)
{
}

TAO_IOP_Version::TAO_IOP_Version (const TAO_IOP_Version &src)
  : major (src.major),
    minor (src.minor)
{
}

TAO_IOP_Version::TAO_IOP_Version (CORBA::Octet maj, CORBA::Octet min)
  : major (maj),
    minor (min)
{
}

void
TAO_IOP_Version::set_version (CORBA::Octet maj, CORBA::Octet min)
{
  this->major = maj;
  this->minor = min;
}

int
TAO_IOP_Version::operator== (const TAO_IOP_Version *&src)
{
  return this->major == src->major && this->minor == src->minor;
}

int
TAO_IOP_Version::operator== (const TAO_IOP_Version &src)
{
  return this->major == src.major && this->minor == src.minor;
}

TAO_IOP_Version &
TAO_IOP_Version::operator= (const TAO_IOP_Version &src)
{
  this->major = src.major;
  this->minor = src.minor;
  return *this;
}

// ****************************************************************

TAO_Profile::~TAO_Profile (void)
{
}

// Generic Profile
CORBA::ULong
TAO_Profile::tag (void) const
{
  return this->tag_;
}

CORBA::ULong
TAO_Profile::_incr_refcnt (void)
{
  // OK, think I got it.  When this object is created (guard) the
  // lock is automatically acquired (refcount_lock_).  Then when
  // we leave this method the destructir for guard is called which
  // releases the lock!
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->refcount_lock_, 0);

  return this->refcount_++;
}

CORBA::ULong
TAO_Profile::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, mon, this->refcount_lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  // refcount is 0, so delete us!
  // delete will call our ~ destructor which in turn deletes stuff.
  delete this;
  return 0;
}

// ****************************************************************

TAO_Unknown_Profile::TAO_Unknown_Profile (CORBA::ULong tag)
  : TAO_Profile (tag)
{
}

int
TAO_Unknown_Profile::parse_string (const char *,
                                   CORBA::Environment &)
{
  // @@ THROW something????
  return -1;
}

CORBA::String
TAO_Unknown_Profile::to_string (CORBA::Environment &)
{
  // @@ THROW something?
  return 0;
}

const TAO_opaque&
TAO_Unknown_Profile::body (void) const
{
  return this->body_;
}

int
TAO_Unknown_Profile::decode (TAO_InputCDR& cdr)
{
  if ((cdr >> this->body_) == 0)
    return -1;
  return 0;
}

int
TAO_Unknown_Profile::encode (TAO_OutputCDR &stream) const
{
  stream.write_ulong (this->tag ());
  return (stream << this->body_);
}

const TAO_ObjectKey &
TAO_Unknown_Profile::object_key (void) const
{
  // @@ TODO this is wrong, but the function is deprecated anyway....
  static TAO_ObjectKey empty_key;
  return empty_key;
}

TAO_ObjectKey *
TAO_Unknown_Profile::_key (CORBA::Environment &) const
{
  // @@ THROW something???
  return 0;
}

CORBA::Boolean
TAO_Unknown_Profile::is_equivalent (TAO_Profile* other_profile,
                                    CORBA::Environment &)
{
  if (other_profile->tag () != this->tag ())
    return 0;

  TAO_Unknown_Profile *op =
    ACE_dynamic_cast (TAO_Unknown_Profile*, other_profile);

  return (this->body_ == op->body_);
}

CORBA::ULong
TAO_Unknown_Profile::hash (CORBA::ULong max,
                           CORBA::Environment &)
{
  return (ACE::hash_pjw (ACE_reinterpret_cast (const char*,
                                               this->body_.get_buffer ()),
                         this->body_.length ()) % max);
}

ASYS_TCHAR *
TAO_Unknown_Profile::addr_to_string(void)
{
  return 0;
}

void
TAO_Unknown_Profile::reset_hint (void)
{
  // do nothing
}

// ****************************************************************

// Transport ...
TAO_Transport::TAO_Transport (CORBA::ULong tag)
  :  tag_ (tag)
{
}

TAO_Transport::~TAO_Transport (void)
{
}

CORBA::ULong
TAO_Transport::tag (void) const
{
  return this->tag_;
}

// Connector
TAO_Connector::TAO_Connector (CORBA::ULong tag)
  : tag_(tag)
{
}

CORBA::ULong
TAO_Connector::tag (void) const
{
  return this->tag_;
}



int
TAO_Connector::make_mprofile (const char *string,
                              TAO_MProfile &mprofile,
                              CORBA::Environment &ACE_TRY_ENV)
{
  // This method utilizes the "Template Method" design pattern to
  // parse the given URL style IOR for the protocol being used
  // and create an mprofile from it.
  //
  // The methods that must be defined by all Connector sub-classes are:
  //      make_profile
  //      check_prefix

  // Check for a valid string
  if (!string || !*string)
    return 1;  // Failure

  // Check for the proper prefix in the IOR.  If the proper prefix isn't
  // in the IOR then it is not an IOR we can use.
  if (this->check_prefix (string) != 0)
    {
      return 1;
      // Failure: not the correct IOR for this protocol.
      // DO NOT throw an exception here since the Connector_Registry
      // should be allowed the opportunity to continue looking for
      // an appropriate connector.
    }

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - TAO_Connector::make_mprofile <%s>\n",
                  string));
    }

  ACE_CString ior;

  ior.set (string, ACE_OS::strlen (string), 1);

  // Find out where the protocol name ends
  int ior_index = ior.find (':');

  if (ior_index == ACE_CString::npos)
    {
      ACE_THROW_RETURN (CORBA::INITIALIZE (), -1);
      // No colon ':' in the IOR!
    }
  else
    {
      ior_index++;
      // Add the length of the colon to the IOR index (i.e. 1)
    }


  const char endpoint_delimiter = ',';
  // The delimiter used to seperate inidividual addresses.

  // Count the number of endpoints in the IOR.  This will be the number
  // of entries in the Mprofile.

  CORBA::ULong profile_count = 1;
  // Number of endpoints in the IOR  (initialized to 1)

  for (size_t j = 0; j < ior.length (); ++j)
    {
      if (ior[j] == endpoint_delimiter)
        profile_count++;
    }

  // Tell the MProfile object how many Profiles it should hold.
  // Mprofile::set(size) returns the number profiles it can hold.
  if (mprofile.set (profile_count) != ACE_static_cast (int, profile_count))
    {
      ACE_THROW_RETURN (CORBA::INITIALIZE (), -1);
      // Error while setting the MProfile size!
    }

  // Find the version, if it exists, and keep track of it so that it
  // may be passed to each Profile.

  int version_index;
  // Index denoting where version is located in the IOR

  if (ior.find ("//") == ior_index)
    {
      version_index = 0;
      // No version provided
      // `protocol://'
    }
  else if (ior.find ("//") == ior_index + 3)
    {
      version_index = ior_index;
      // Version provided
      // `protocol:N.n//'

      ior_index += 5;
      // Skip over the `N.n//'
    }
  else
    {
      ACE_THROW_RETURN (CORBA::INITIALIZE (), -1);
      // Problem in IOR between protocol prefix and double slash "//"
    }

  // The behind the following loop is to split the IOR into several
  // strings that can be parsed by each profile.
  // For example,
  //    `1.3//moo,shu,chicken/arf'
  // will be parsed into:
  //    `1.3//moo/arf'
  //    `1.3//shu/arf'
  //    `1.3//chicken/arf'
  //
  // If no version is provided then the string will be of the form:
  //    `//moo/arf'

  int objkey_index = ior.find (this->object_key_delimiter (), ior_index);
  // Find the object key
  //
  // Typically, a forward slash '/' is used to delimit endpoints from the
  // object key.  However, some protocols may use forward slashes in
  // the endpoints themselves.  To prevent ambiguities from arising, a
  // protocol specific object key delimiter can be used.

  if (objkey_index == 0 || objkey_index == ACE_CString::npos)
    {
      ACE_THROW_RETURN (CORBA::INITIALIZE (), -1);
      // Failure: No endpoints specified or no object key specified.
    }

  int begin = 0;
  int end = ior_index - 1;
  // Initialize the end of the endpoint index

  for (CORBA::ULong j = 0; j < profile_count; ++j)
    {
      begin += end + 1;

      if (j < profile_count - 1)
        end = ior.find (endpoint_delimiter, begin);
      else
        end = objkey_index - begin;  // Handle last endpoint differently

      if (end < ACE_static_cast (int, ior.length ()) && end != ior.npos)
        {
          ACE_CString endpoint = ior.substring (begin, end);

          if (version_index > 0)
            {
              endpoint = ior.substring (version_index, 5) + endpoint;
              // Concatenate version string and endpoint
            }
          else
            {
              endpoint = ior.substring (ior_index - 2, 2) + endpoint;
              // No version provided
            }

          endpoint += ior.substring (objkey_index);
          // Add the object key to the string.

          // The endpoint should now be of the form:
          //    `N.n://endpoint/object_key'
          // or
          //    `//endpoint/object_key'

          TAO_Profile *profile = 0;
          // Must initialize since pointer is passed as a reference!

          if (this->make_profile (endpoint.c_str (),
                                  profile,
                                  ACE_TRY_ENV) != 0)
            {
              ACE_THROW_RETURN (CORBA::INITIALIZE (), -1);
              // Failure:  Problem during profile creation
            }

          // Create a Profile using the individual endpoint string

          // Give up ownership of the profile.
          if (mprofile.give_profile (profile) != 0)
            {
              ACE_THROW_RETURN (CORBA::INITIALIZE (), -1);
              // Failure presumably only occurs when MProfile is full!
              // This should never happen.
            }
        }
      else
        {
          ACE_THROW_RETURN (CORBA::INITIALIZE (), -1);
          // Unable to seperate endpoints
        }
    }

  return 0;  // Success
}

TAO_Connector::~TAO_Connector (void)
{
}

// Acceptor
TAO_Acceptor::TAO_Acceptor (CORBA::ULong tag)
  :  tag_ (tag)
{
}

CORBA::ULong
TAO_Acceptor::tag (void) const
{
  return this->tag_;
}

TAO_Acceptor::~TAO_Acceptor (void)
{
}
