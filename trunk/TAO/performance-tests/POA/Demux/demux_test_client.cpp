//=============================================================================
/**
 *  @file    demux_test_client.cpp
 *
 *  $Id$
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#include "demux_test_client.h"
#include "tao/debug.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_string.h"

// Constructor
Demux_Test_Client::Demux_Test_Client (void)
  : argc_ (0),
    argv_ (0),
    is_ (Demux_Test_Client::LINEAR),
    num_POAs_ (1),
    // default number of child POAs is 1 and each one will always have 1 object
    num_objs_ (1),
    num_ops_ (1),
    demux_test_ (TAO_DEMUX_TEST_MAX_POAS * TAO_DEMUX_TEST_MAX_OBJS),
    loop_count_ (1),
    ior_fp_ (0),
    result_fp_ (0),
    step_ (5)
{
}

// destructor
Demux_Test_Client::~Demux_Test_Client (void)
{
}

//
// initialize the Demux_Test_Client
//

int
Demux_Test_Client::init (int argc, ACE_TCHAR *argv [])
{
  this->argc_ = argc;
  this->argv_ = argv;

  // Grab the ORB
  try
    {
      // get the underlying ORB
      this->orb_ =
        CORBA::ORB_init (argc, argv);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("ORB_init");
      throw;
    }

  // now parse the rest of the arguments to determine the POA depth, the number
  // of objects with each POA and other info

  if (this->parse_args () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%N:%l) Demux_Test_Client::init - "
                       "parse_args failed\n"),
                      -1);

  if (this->init_operation_db () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%N:%l) Demux_Test_Client::init - "
                       "init_operation_db failed\n"),
                      -1);

  // now read all the IORS
  CORBA::ULong i, j;

  for (i = 0; i < this->num_POAs_; ++i)
    for (j = 0; j < this->num_objs_; ++j)
      {
        char str [1024 * 10];
        ACE_OS::memset (str, 0, sizeof (str));

        if (fscanf (this->ior_fp_, "%s", str) == EOF)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "IOR database has less entries than required\n"),
                              -1);
          }

        // Get the IOR and output it to the file
        try
          {
            CORBA::Object_var objref = this->orb_->string_to_object (str);

            // now narrow to Demux_Test object

            this->demux_test_[i * j] = Demux_Test::_narrow (objref.in ());


            if (CORBA::is_nil (this->demux_test_[i * j].in ()))
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "ObjRef for IOR %s (POA %d, OBJ %d) is NULL\n",
                                   str, i, j),
                                  -1);
              }
            }
        catch (const CORBA::Exception& ex)
          {
            ex._tao_print_exception ("object_to_string");
            throw;
          }
      } // j and i loop

  ACE_OS::fclose (this->ior_fp_);

  // success
  return 0;

}

// parse command line arguments (if any).
int
Demux_Test_Client::parse_args (void)
{

  ACE_Get_Opt get_opts (this->argc_, this->argv_, ACE_TEXT("df:m:n:o:p:i:s:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'f':
        this->ior_fp_ = ACE_OS::fopen (get_opts.opt_arg (), "w");
        if (this->ior_fp_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.opt_arg ()), -1);
        break;
      case 'm':
        this->num_ops_ = ACE_OS::atoi (get_opts.opt_arg ());
        if (this->num_ops_ > TAO_DEMUX_TEST_MAX_OPS)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%d exceeds the maximum of "
                               "%d objects per POA\n",
                               this->num_objs_,
                               TAO_DEMUX_TEST_MAX_OPS),
                              -1);
          }
        break;
      case 'n':
        this->loop_count_ = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'o':
        this->num_objs_ = ACE_OS::atoi (get_opts.opt_arg ());
        if (this->num_objs_ > TAO_DEMUX_TEST_MAX_OBJS)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%d exceeds the maximum of "
                               "%d objects per POA\n",
                               this->num_objs_,
                               TAO_DEMUX_TEST_MAX_OBJS),
                              -1);
          }
        break;
      case 'p':
        this->num_POAs_ = ACE_OS::atoi (get_opts.opt_arg ());
        if (this->num_POAs_ > TAO_DEMUX_TEST_MAX_POAS)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%d exceeds the maximum of "
                               "%d POAs\n",
                               this->num_objs_,
                               TAO_DEMUX_TEST_MAX_POAS),
                              -1);
          }
        break;
      case 'i':
        switch (*get_opts.opt_arg ())
          {
          case 'L':
            this->is_ = Demux_Test_Client::LINEAR;
            break;
          case 'R':
            this->is_ = Demux_Test_Client::RANDOM;
            break;
          case 'B':
            this->is_ = Demux_Test_Client::BEST;
            break;
          case 'W':
            this->is_ = Demux_Test_Client::WORST;
            break;
          }
        break;
      case 's':
        this->step_ = ACE_OS::atoi (get_opts.opt_arg ());
        if (this->step_ > this->num_objs_)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%d exceeds the no. of Objs specified"),
                            this->step_);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-m <num ops>]"
                           " [-o <num objs>]"
                           " [-p <num POAs]"
                           " [-i <invoke strategy>"
                           " [-f <IOR file>]"
                           " [-n <loop count>]"
                           "\n"
                           "Invocation Strategy: L(linear), R(random)"
                           "B(best), W(worst)\n",
                           this->argv_ [0]),
                          -1);
      }

  if (!this->ior_fp_)
    {
      // open default IOR file
      this->ior_fp_ = ACE_OS::fopen ("ior.dat", "r");
      if (this->ior_fp_ == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to open file ior.dat for reading\n"), -1);
    }
  return 0;
}

// The main program for Demux_Test
int
Demux_Test_Client::run (void)
{
  // open a temporary results file
  if ((this->result_fp_ = ACE_OS::fopen ("results.dat", "w")) == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Demux_Test_Client::run - "
                         "Failed to open the results file for writing\n"),
                        -1);
    }
  try
    {
      switch (this->is_)
        {
        case Demux_Test_Client::LINEAR:
          (void) this->run_linear_test ();
          break;
        case Demux_Test_Client::RANDOM:
          (void) this->run_random_test ();
          break;
        case Demux_Test_Client::BEST:
          (void) this->run_best_test ();
          break;
        case Demux_Test_Client::WORST:
          (void) this->run_worst_test ();
          break;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("run failed");
      ACE_ERROR ((LM_ERROR,
                  "(%N:%l) Demux_Test_Client::run - "
                  "Error running the Client\n"));
      throw;
    }

  ACE_OS::fclose (this->result_fp_);
  this->result_fp_ = 0;

  try
    {
      // call the shutdown method one the first object
      this->demux_test_[0]->shutdown ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("shutdown failed");
      ACE_ERROR ((LM_ERROR,
                  "(%N:%l) Demux_Test_Client::run - "
                  "Error running the Client\n"));
      throw;
    }

      // now print the results
  if (this->print_results () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Demux_Test_Client::run - "
                         "Print results failed\n"),
                        -1);
    }

  return 0;
}

int
Demux_Test_Client::run_linear_test (void)
{
  CORBA::ULong j, k, l, m;
  ACE_hrtime_t start, end;

  m = 0;
  for (j = 0; j < this->num_POAs_; ++j)
    for (k = 0; k < this->num_objs_; k+=this->step_)
      for (l = 0; l < this->num_ops_; ++l)
          {
            start = ACE_OS::gethrtime ();

            // invoke the method
            this->op_db_[l].op_ (this->demux_test_[j * k].in ());

            end = ACE_OS::gethrtime ();

            m++;
            ACE_OS::fprintf (this->result_fp_, "%d %f\n", m,
                             1.0 * ACE_UINT64_DBLCAST_ADAPTER (end - start));
          }

  return 0;
}

int
Demux_Test_Client::run_random_test (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "ERROR : Random test\n"));
  return 0;
}

int
Demux_Test_Client::run_best_test (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "ERROR : Best Test\n"));
  return 0;
}

int
Demux_Test_Client::run_worst_test (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "ERROR : Worst test\n"));
  return 0;
}

int
Demux_Test_Client::print_results (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "******** "));
  switch (this->is_)
    {
    case Demux_Test_Client::LINEAR:
      ACE_DEBUG ((LM_DEBUG,
                  "Linear Strategy ******\n"));
      break;
    case Demux_Test_Client::RANDOM:
      ACE_DEBUG ((LM_DEBUG,
                  "Random Strategy ******\n"));
      break;
    case Demux_Test_Client::BEST:
      ACE_DEBUG ((LM_DEBUG,
                  "Best Strategy ******\n"));
      break;
    case Demux_Test_Client::WORST:
      ACE_DEBUG ((LM_DEBUG,
                  "Worst Strategy ******\n"));
      break;
    }

  return 0;
}

// include the generated code
#include "demux_test_client.inl"
