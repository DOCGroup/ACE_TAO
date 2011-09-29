// $Id$

#include "IF_EXE_M_R_StructsC.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_string.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
        case 'k':
          ior = get_opts.opt_arg ();
          break;

        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                                     "usage:  %s "
                                     "-k <ior> "
                                     "\n",
                                     argv [0]),
                                    -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}


CORBA::ORB_var m_ORB_p;

class IF_Test_client
{
public:
  int
  foo (int argc, ACE_TCHAR *argv[], bool shutdown)
  {
    IF_EXE_M_R::IF_ExeCtrlData_var client;
    IF_EXE_M_R::Test_Struct ts;
    IF_EXE_M_R::CORBA_BaseFOOG FOOG;
    IF_EXE_M_R::CORBA_FOOIInPlan bs;

    FOOG.FOOD = IF_EXE_M_R::ENUM_FOOD (0);
    FOOG.FOOIID = CORBA::string_dup ("A_FOOG");
    ACE_OS::memset (&FOOG.startPosition,
                    0, sizeof(IF_EXE_M_R::CORBA_LatLongFOOB));
    ACE_OS::memset (&FOOG.endPosition,
                    0, sizeof(IF_EXE_M_R::CORBA_LatLongFOOB));
    FOOG.FOOBReferenceStartPosition = IF_EXE_M_R::FOOBMode_E (0);
    FOOG.averageFOOHFOOB = 0.0;
    FOOG.FOOBControlMode = IF_EXE_M_R::ENUM_ControlMode (0);
    FOOG.datasetVersion = 1L;
    FOOG.maxHorizontalFOOU = 0L;
    FOOG.maxVerticalFOOU = 0L;
    FOOG.FOOTDistance = 0.0;
    FOOG.startFOOA = 0.0;
    FOOG.FOOTDuration = 0.0;
    FOOG.endFOOA = 0.0;
    FOOG.FOOTEnergyConsum = 0.0;
    FOOG.FOOAReference = IF_EXE_M_R::FOOAMode_E (0);
    FOOG.FOOAControlMode = IF_EXE_M_R::ENUM_ControlMode (0);
    FOOG.FOOBReferenceEndPosition = IF_EXE_M_R::FOOBMode_E (0);
    FOOG.economyMode = IF_EXE_M_R::ENUM_Selection (0);
    FOOG.cl_FOOSMode = IF_EXE_M_R::ENUM_CL_FOOSMode (0);
    FOOG.transitionStatus = IF_EXE_M_R::ENUM_TransitionStatus (0);
    FOOG.FOOKTypeStart = IF_EXE_M_R::ENUM_FOOKType (0);
    FOOG.activeComponents.length (0);
    FOOG.FOOKTypeEnd = IF_EXE_M_R::ENUM_FOOKType (0);

    bs.type = IF_EXE_M_R::BASE_FOOG;
    bs.data.FOOVFOOG (FOOG);

    ts.l = 42L;
    ts.whatEver <<= bs;

    try
      {
        if (parse_args (argc, argv) != 0)
          return 1;

        CORBA::Object_var obj = m_ORB_p->string_to_object (ior);
        if (CORBA::is_nil (obj.in ()))
          {
            ACE_ERROR_RETURN ((LM_DEBUG,
                                               "Nil Test::Hello reference <%s>\n",
                                               ior),
                                              1);
          }
        ACE_DEBUG ((LM_DEBUG, "RepositoryID: %s\n", obj->_interface_repository_id()));
        client = IF_EXE_M_R::IF_ExeCtrlData::_narrow (obj.in ());
        if (CORBA::is_nil (client.in ()))
          {
            ACE_ERROR_RETURN ((LM_DEBUG,
                                         "Reference is not of type IF_EXE_M_R::IF_ExeCtrlData\n"),
                                        1);

          }
        client->foo (ts);
        if (shutdown)
          {
            client->shutdown ();
          }
      }
    catch (const CORBA::Exception &ex)
      {
        ex._tao_print_exception ("Exception caught:");
      }
    return 0;
  }
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int retval = 0;
  try
    {
      m_ORB_p = CORBA::ORB_init (argc, argv);
      IF_Test_client testclient;

      for (int i = 0; i < 10; i++)
        {
          retval += testclient.foo (argc, argv, i == 9);
        }
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception("ERROR : unexpected CORBA exception caugth :");
      ++retval;
    }
  return retval;
}

