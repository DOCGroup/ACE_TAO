#include <cstdio>
#include <memory>

#include "IF_EXE_M_R_StructsS.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

namespace
{
  int
  parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior_output_file)
  {
    ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
    int option {};

    while ((option = get_opts ()) != -1)
      switch (option)
        {
        case 'o':
          ior_output_file = get_opts.opt_arg ();
          break;

        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage: %s -o <iorfile>\n",
                             argv [0]),
                            -1);
        }
    return 0;
  }
}

class IF_Test_impl : public POA_IF_EXE_M_R::IF_ExeCtrlData
{
public:
  explicit IF_Test_impl (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  IF_Test_impl (IF_Test_impl const &) = delete;
  IF_Test_impl (IF_Test_impl &&) = delete;
  IF_Test_impl &operator= (IF_Test_impl const &) = delete;
  IF_Test_impl &operator= (IF_Test_impl &&) = delete;

  void shutdown () override
  {
    this->orb_->shutdown (false);
  }

  void foo (::IF_EXE_M_R::Test_Struct const &ts) override
  {
    // The Any owns the extracted union; this pointer is borrowed.
    IF_EXE_M_R::CORBA_FOOIInPlan const *anUnion_p {};
    if (ts.whatEver >>= anUnion_p)
      {
        ACE_DEBUG ((LM_DEBUG, "Any successfully marshalled\nAny is %@\nID: %C\nNAME: %C\nKIND: %d\n",
                    &ts.whatEver,
                    ts.whatEver._tao_get_typecode ()->id (),
                    ts.whatEver._tao_get_typecode ()->name (),
                    ts.whatEver._tao_get_typecode ()->kind ()));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR Any not successfully marshalled\nAny is %@\n", &ts.whatEver));
        if (ts.whatEver._tao_get_typecode ()->kind () != CORBA::tk_null)
          ACE_ERROR ((LM_ERROR, "ID: %C\nNAME: %C\nKIND: %d\n",
                      ts.whatEver._tao_get_typecode ()->id (),
                      ts.whatEver._tao_get_typecode ()->name (),
                      ts.whatEver._tao_get_typecode ()->kind ()));
        else
          ACE_ERROR ((LM_ERROR, "Typecode is NULL\nKIND: %d\n",
                      ts.whatEver._tao_get_typecode ()->kind ()));
      }
  }

private:
  CORBA::ORB_var const orb_;
};

class Main_C
{
public:
  int Create (CORBA::ORB_ptr orb, int argc, ACE_TCHAR *argv[]) const;
};

int
Main_C::Create (CORBA::ORB_ptr orb, int argc, ACE_TCHAR *argv[]) const
{
  ACE_TCHAR const *ior_output_file = ACE_TEXT ("test.ior");

  try
    {
      if (parse_args (argc, argv, ior_output_file) != 0)
        return 1;

      CORBA::Object_var const obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (obj.in ());
      PortableServer::POAManager_var const mgr = poa->the_POAManager ();
      mgr->activate ();

      IF_Test_impl *servant {};
      ACE_NEW_RETURN (servant, IF_Test_impl (orb), 1);
      // ServantBase_var takes ownership of the raw servant pointer.
      PortableServer::ServantBase_var owner_transfer (servant);

      PortableServer::ObjectId_var const id = poa->activate_object (servant);
      CORBA::Object_var const object = poa->id_to_reference (id.in ());
      IF_EXE_M_R::IF_ExeCtrlData_var const hello =
        IF_EXE_M_R::IF_ExeCtrlData::_narrow (object.in ());
      CORBA::String_var const ior = orb->object_to_string (hello.in ());

      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior_output_file, "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s\n",
                             ior_output_file),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      orb->run ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
  Main_C const main_cs;
  return main_cs.Create (orb.in (), argc, argv);
}
