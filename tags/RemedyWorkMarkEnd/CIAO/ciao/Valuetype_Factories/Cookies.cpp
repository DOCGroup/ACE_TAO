// $Id$

#include "Cookies.h"

#if !defined (__ACE_INLINE__)
#include "Cookies.inl"
#endif /* !defined INLINE */

namespace CIAO
{
  Cookie_Impl::Cookie_Impl (ptrdiff_t const &key)
  {
    this->cookieValue ().length (sizeof (ptrdiff_t));
    ACE_OS::memcpy (this->cookieValue ().get_buffer (false),
                    &key,
                    sizeof (ptrdiff_t));
  }

  Cookie_Impl::~Cookie_Impl (void)
  {
  }

  bool
  Cookie_Impl::extract (Components::Cookie const * const ck,
                        ptrdiff_t &key)
  {
    Cookie_Impl const * const c =
      dynamic_cast <Cookie_Impl const *> (ck);

    if (c == 0)
      {
        return false;
      }

    CORBA::OctetSeq const &x = c->cookieValue ();

    if (x.length () != sizeof (ptrdiff_t))
      {
        return false;
      }

    key = *reinterpret_cast<const ptrdiff_t*> (x.get_buffer ());

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
