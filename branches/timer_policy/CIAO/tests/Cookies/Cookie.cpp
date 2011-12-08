// $Id$

#include "Cookie.h"
#include "ciao/Valuetype_Factories/Cookies.h"

#define KEY_VALUE 123456789

Cookie::Cookie (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

::Components::Cookie *
Cookie::get_cookie (void)
{
  ::Components::Cookie_var ck;
  ptrdiff_t const &key = KEY_VALUE;
  ACE_NEW_THROW_EX (ck.inout (),
                    ::CIAO::Cookie_Impl (key),
                    ::CORBA::NO_MEMORY ());

  return ck._retn ();
}

void
Cookie::check_cookie (::Components::Cookie * cookie)
{
  ptrdiff_t key = 0UL;
  try
    {
      if (cookie == 0 || ! ::CIAO::Cookie_Impl::extract (cookie, key))
        {
          ACE_ERROR ((LM_ERROR, "Cookie::check_cookie - "
                    "ERROR: Unable to extract cookie.\n"));
        }
      else
        {
          if (key == KEY_VALUE)
            {
              ACE_DEBUG ((LM_DEBUG, "Cookie::check_cookie - "
                        "OK: Cookie seems ok.\n"));
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG, "Cookie::check_cookie - "
                        "ERROR: Unexpected cookie: expected <%u> - received <%u>\n",
                        KEY_VALUE, key));
            }
        }
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Cookie::check_cookie - "
                "ERROR: Unexpected exception caught\n"));
    }

}

void
Cookie::shutdown (void)
{
  this->orb_->shutdown (0);
}
