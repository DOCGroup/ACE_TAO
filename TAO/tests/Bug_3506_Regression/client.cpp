#include "IF_EXE_M_R_StructsC.h"
#include "ace/Get_Opt.h"

namespace
{
  int
  parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior)
  {
    ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
    int option {};

    while ((option = get_opts ()) != -1)
      switch (option)
        {
        case 'k':
          ior = get_opts.opt_arg ();
          break;

        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage: %s -k <ior>\n",
                             argv [0]),
                            -1);
        }
    return 0;
  }
}

class IF_Test_client
{
public:
  int
  foo (CORBA::ORB_ptr orb, ACE_TCHAR const *ior, bool const shutdown) const
  {
    IF_EXE_M_R::IF_ExeCtrlData_var client {};
    IF_EXE_M_R::Test_Struct ts {};
    IF_EXE_M_R::CORBA_BaseFOOG FOOG {};
    IF_EXE_M_R::CORBA_FOOIInPlan bs {};

    FOOG.FOOIID = CORBA::string_dup ("A_FOOG");
    FOOG.datasetVersion = 1;
    bs.type = IF_EXE_M_R::BASE_FOOG;
    bs.data.FOOVFOOG (FOOG);
    ts.l = 42;
    ts.whatEver <<= bs;

    try
      {
        CORBA::Object_var const obj = orb->string_to_object (ior);
        if (CORBA::is_nil (obj.in ()))
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);

        ACE_DEBUG ((LM_DEBUG, "RepositoryID: %s\n", obj->_interface_repository_id ()));
        client = IF_EXE_M_R::IF_ExeCtrlData::_narrow (obj.in ());
        if (CORBA::is_nil (client.in ()))
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Reference is not of type IF_EXE_M_R::IF_ExeCtrlData\n"),
                            1);

        client->foo (ts);
        if (shutdown)
          client->shutdown ();
      }
    catch (CORBA::Exception const &ex)
      {
        ex._tao_print_exception ("Exception caught:");
      }
    return 0;
  }
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int retval {};
  ACE_TCHAR const *ior = ACE_TEXT ("file://test.ior");

  try
    {
      if (parse_args (argc, argv, ior) != 0)
        return 1;

      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      IF_Test_client const testclient;
      constexpr int request_count {10};

      for (int request {}; request < request_count; ++request)
        retval += testclient.foo (orb.in (), ior, request == request_count - 1);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("ERROR : unexpected CORBA exception caugth :");
      ++retval;
    }
  return retval;
}
