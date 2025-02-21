#include "ace/OS_main.h"

#if !defined (ACE_DOESNT_DEFINE_MAIN)

#if defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER) && !defined (ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER)

#include "ace/Init_ACE.h"

#  if !defined (ACE_HAS_MINIMAL_ACE_OS)
#    include "ace/Object_Manager.h"
#  endif /* ! ACE_HAS_MINIMAL_ACE_OS */

// Rename "main ()" on platforms that don't allow it to be called "main ()".

// Also, create ACE_Object_Manager static instance(s) in "main ()".
// ACE_MAIN_OBJECT_MANAGER defines the ACE_Object_Manager(s) that will
// be instantiated on the stack of main ().  Note that it is only used
// when compiling main ():  its value does not affect the contents of
// ace/OS.o.
#  if !defined (ACE_MAIN_OBJECT_MANAGER)
#    define ACE_MAIN_OBJECT_MANAGER \
        ACE_OS_Object_Manager ace_os_object_manager; \
        ACE_Object_Manager ace_object_manager;
#  endif /* ! ACE_MAIN_OBJECT_MANAGER */

#  if !defined (ACE_WIN32)

#    if !(defined (ACE_VXWORKS) && !defined (__RTP__))
/* forward declaration */
extern int ace_main_i (int, char *[]);
#    endif

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

#    if defined (ACE_VXWORKS) && !defined (__RTP__)
ace_main_proc_ptr vx_ace_main_i_ptr = 0;

int ace_os_main_i (int argc, char *argv[]) /* user's entry point, e.g., main */
{
  ACE_MAIN_OBJECT_MANAGER
  return vx_ace_main_i_ptr ? (*vx_ace_main_i_ptr) (argc, argv) : (-1);   /* what the user calls "main" */
}
#    else /* !ACE_VXWORKS && !__RTP__ */
ACE_Export int
ace_os_main_i (int argc, char *argv[]) /* user's entry point, e.g., main */
{
  ACE_MAIN_OBJECT_MANAGER
  return ace_main_i (argc, argv);           /* what the user calls "main" */
}
#    endif

ACE_END_VERSIONED_NAMESPACE_DECL

#  else

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

#    if defined (ACE_WIN32) && defined (ACE_USES_WCHAR)
int ACE_Main_Base::run (int argc, ACE_TCHAR *argv[])
{
  return this->run_i (argc, argv);
}

ACE_Export int
ace_os_wmain_i (ACE_Main_Base &mbase, int argc, ACE_TCHAR *argv[]) /* user's entry point, e.g., main */
{
  ACE_MAIN_OBJECT_MANAGER
  return mbase.run (argc, argv);           /* what the user calls "main" */
}
#    else /* ! (ACE_WIN32 && ACE_USES_WCHAR) */

ACE_Main_Base::ACE_Main_Base ()
{
  ACE::init ();
}

ACE_Main_Base::~ACE_Main_Base ()
{
  ACE::fini ();
}

int ACE_Main_Base::run (int argc, char *argv[])
{
  return this->run_i (argc, argv);
}

ACE_Export int
ace_os_main_i (ACE_Main_Base &mbase, int argc, char *argv[]) /* user's entry point, e.g., main */
{
  ACE_MAIN_OBJECT_MANAGER
  return mbase.run (argc, argv);           /* what the user calls "main" */
}
#    endif /* ACE_WIN32 && ACE_USES_WCHAR */

ACE_END_VERSIONED_NAMESPACE_DECL

#  endif   /* !ACE_WIN32 */

#endif /* ACE_HAS_NONSTATIC_OBJECT_MANAGER && !ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER */

#endif /* ACE_DOESNT_DEFINE_MAIN */

