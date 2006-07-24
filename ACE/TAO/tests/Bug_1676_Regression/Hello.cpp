//
// $Id$
//
#include "Hello.h"

ACE_RCSID(Hello, Hello, "$Id$")

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

#if 0
char *
Hello::get_string (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("Hello there!");
}
#endif

::Test::StringList * 
Hello::get_stringList (
	// ACE_ENV_SINGLE_ARG_DECL_NOT_USED
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
	)
	ACE_THROW_SPEC ((
	::CORBA::SystemException
	))
{
	Test::StringList * seq;
    ACE_NEW_RETURN (seq,
		Test::StringList(10),
                    0);
    seq->length(10);
	for (CORBA::ULong i = 0; i<seq->length(); i++)
	{
		char tmp[255] = {0};
		sprintf(tmp, "Hello World %d", i);
		(*seq)[i] = CORBA::string_dup(tmp);
	}

	return seq;
}

void 
Hello::get_stringList2 (
        ::CORBA::Boolean initialize,
        ::Test::StringList_out osl
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        ::CORBA::SystemException
      ))
{
    // CORBA::String_var the_string = osl->length();
    // ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Entering get_stringList2(%.4d)\n", osl));

    if (initialize) {
        ACE_NEW (osl,
		    Test::StringList(10));
        
        osl->length(5);
        for (CORBA::ULong i = 0; i<osl->length(); i++)
	    {
		    char tmp[255] = {0};
		    sprintf(tmp, "Hello Again %d", i);
		    (*osl)[i] = CORBA::string_dup(tmp);
	    }
    }
}
    
void 
Hello::mod_stringList (
        ::Test::StringList & iosl
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        ::CORBA::SystemException
      ))
{
    // osl->length(10);
    for (CORBA::ULong i = 0; i<iosl.length(); i++)
	{
		char tmp[255] = {0};
		sprintf(tmp, "Hello Client %d", i);
        if (i%2) {
		    iosl[i] = CORBA::string_dup(tmp);
        }
	}
}
    
void
Hello::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
