// $Id$

#include "ace/High_Res_Timer.h"
#include "ace/Basic_Stats.h"
#include "ace/Sample_History.h"
#include "ace/OS_NS_ctype.h"
#include "ace/Get_Opt.h"

#include "StringMember_Test.h"

StringMember_Test::StringMember_Test (void)
  : iterations_ (10000),
    str_len_ (40),
    str_copy_ (false)
{
}

void
StringMember_Test::shutdown (void)
{
  this->root_poa_->destroy (true, true);
  this->orb_->destroy ();
}

int
StringMember_Test::init (int argc, ACE_TCHAR *argv[])
{
  // Initialize the ORB.
  this->orb_ = CORBA::ORB_init (argc, argv);

  int result = this->parse_args (argc, argv);

  if (result != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("parse_args failed\n")),
                        result);
    }

  // Get an Object reference to RootPOA.
  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("RootPOA");

  // Narrow the Object reference to a POA reference
  this->root_poa_ =
    PortableServer::POA::_narrow (obj.in ());

  // Get the POAManager of RootPOA
  this->poa_manager_ =
    this->root_poa_->the_POAManager ();

  // Activate the servant under RootPOA.
  // With the alternate mapping, ObjectId is an std::vector,
  // no need for a _var.
  PortableServer::ObjectId id =
    this->root_poa_->activate_object (&this->servant_);

  // Create an object reference.
  this->test_obj_ = this->root_poa_->id_to_reference (id);

  return 0;
}

int
StringMember_Test::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("n:l:c"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        this->iterations_ = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'l':
        this->str_len_ = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'c':
        this->str_copy_ = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-n <iterations> "
                           "-l <string length> "
                           "-c toggle string copy on "
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates sucessful parsing of the command line
  return 0;
}

int
StringMember_Test::run (void)
{
  this->poa_manager_->activate ();
  Test_var objref = Test::_narrow (this->test_obj_.in ());

  CORBA::String_var str = gen_string ();
  std::string data (str.in ());

  Foo arg;

  // Set the arg to anything and make a call to set up and cache
  // the CORBA connection. Subsequent timed calls will not incur
  // the connection overhead.
  arg.strmem = "who cares?";
  objref->test_op (arg);

  ACE_Sample_History history (this->iterations_);

  if (this->str_copy_)
    {
      for (int i = 0; i < this->iterations_; ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();
          arg.strmem = data.c_str ();
          objref->test_op (arg);
          ACE_hrtime_t stop = ACE_OS::gethrtime ();
          history.sample (stop - start);
        }
    }
  else
    {
      for (int i = 0; i < this->iterations_; ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();
          arg.strmem = data;
          objref->test_op (arg);
          ACE_hrtime_t stop = ACE_OS::gethrtime ();
          history.sample (stop - start);
        }
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("string length = \t%d\n")
              ACE_TEXT ("number of iterations = %d\n"),
              this->str_len_,
              this->iterations_));

  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
  ACE_Basic_Stats stats;
  history.collect_basic_stats (stats);
  stats.dump_results (
    (this->str_copy_ ? ACE_TEXT ("Copy") : ACE_TEXT ("No Copy")),
    gsf);

  return 0;
}

char *
StringMember_Test::gen_string (void)
{
  char *buf = CORBA::string_alloc (this->str_len_);
  CORBA::ULong i = 0;

  while (i < this->str_len_)
    {
      int c = ACE_OS::rand () % 128;

      if (ACE_OS::ace_isprint (c) && !ACE_OS::ace_isspace (c))
        {
          buf [i] = c;
          i++;
        }
    }

  buf[i] = 0;
  return buf;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      StringMember_Test test;

      test.init (argc, argv);

      test.run ();

      test.shutdown ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client: CORBA Exception caught");
      return -1;
    }
  catch (...)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "client: unknown exception caught\n"),
                        -1);
    }

  return 0;
}
