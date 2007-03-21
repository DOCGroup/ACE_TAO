// $Id$
#include "Cookies.h"

#if !defined (__ACE_INLINE__)
#include "Cookies.inl"
#endif /* !defined INLINE */

ACE_RCSID (ciao,
           Cookies,
           "$Id$")

namespace CIAO
{
  Cookie_Impl::Cookie_Impl (CORBA::ULong const &key)
  {
    this->cookieValue ().length (sizeof (CORBA::ULong));
    ACE_OS::memcpy (this->cookieValue ().get_buffer (false),
                    &key,
                    sizeof (CORBA::ULong));
  }

  Cookie_Impl::~Cookie_Impl (void)
  {
  }

  bool
  Cookie_Impl::extract (Components::Cookie const * const ck,
                        CORBA::ULong &key)
  {
    Cookie_Impl const * const c =
      dynamic_cast <Cookie_Impl const * const> (ck);

    if (c == 0)
      {
        return false;
      }

    CORBA::OctetSeq const &x = c->cookieValue ();

    if (x.length () != sizeof (CORBA::ULong))
      {
        return false;
      }

    key = *reinterpret_cast<const CORBA::ULong *> (x.get_buffer ());

    return true;
  }

  //========================================================

  CORBA::ValueBase *
  Cookie_Impl_init::create_for_unmarshal (void)
  {
    CORBA::ValueBase *ret_val = 0;

    ACE_NEW_THROW_EX (ret_val,
                      CIAO::Cookie_Impl,
                      CORBA::NO_MEMORY ());

    return ret_val;
  }

  Cookie_Impl_init::~Cookie_Impl_init (void)
  {
  }
}
