// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/performance-test/Demux/CodeGen
//
// = FILENAME
//    codegen.cpp
//
//    Implements the code generator class
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "codegen.h"

ACE_RCSID(CodeGen, codegen, "$Id$")

Demux_Test_CodeGenerator::Demux_Test_CodeGenerator (void)
  : num_POAs_ (1), // 1 child POA
    num_objs_ (1),
    num_ops_ (1),
    state_ (Demux_Test_CodeGenerator::CG_NONE),
    oneway_ (0),
    debug_ (0)
{
}

Demux_Test_CodeGenerator::~Demux_Test_CodeGenerator (void)
{
}

int 
Demux_Test_CodeGenerator::init (int argc, char *argv[])
{
  if (this->parse_args (argc, argv) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) Demux_Test_CodeGenerator::init - "
                         "Failed to initialize\n"),
                        -1);
    }
  return 0;
}

int 
Demux_Test_CodeGenerator::parse_args (int argc, char *argv[])
{
  // for parsing the arguments
  ACE_Get_Opt get_opt (argc, argv, "dhm:o:p:u");

  for (int c; (c = get_opt ()) != EOF;)
    {
      switch (c)
	{
        case 'd':
          this->debug_ = 1;
          break;
	case 'm': // number of operations
	  this->num_ops_ = ACE_OS::atoi (get_opt.optarg);
	  break;
	case 'o': // number of object keys
	  this->num_objs_ = ACE_OS::atoi (get_opt.optarg);
	  break;
	case 'p': // number of POAs
	  this->num_POAs_ = ACE_OS::atoi (get_opt.optarg);
	  break;
        case 'u':
          // unidirectional i.e., oneway operations
          this->oneway_ = 1;
          break;
	case 'h': // usage
	  this->usage ();
          break;
	default:
          ACE_ERROR ((LM_ERROR,
                      "Bad option or insufficient arguments\n"));
          this->usage ();
	}
    }
  return 0;
}

void 
Demux_Test_CodeGenerator::usage (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Usage: tao [-m <#operations>] [-o <#objects>] "
              "[-p <#POAs>] [-h] [-d] [-u]\n"
              "Default: 1 child POA, 1 operation, 1 object\n"
              "[-h] dislay this help message\n"
              "[-d] generate debugging\n"
              "[-u] generate oenway operations\n"));
  ACE_OS::exit (0);
}

int
Demux_Test_CodeGenerator::run (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Generating code for %d POAs, %d objects, and %d operations\n",
              this->num_POAs_,
              this->num_objs_,
              this->num_ops_));
  
  
  // generate the POA names
  if (this->gen_poa_names () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) Demux_Test_CodeGenerator::run - "
                         "Failed to generate POA names\n"),
                        -1);
    }

  // generate the object names
  if (this->gen_object_names () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) Demux_Test_CodeGenerator::run - "
                         "Failed to generate object names\n"),
                        -1);
    }

  // generate method names
  if (this->gen_operation_names () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) Demux_Test_CodeGenerator::run - "
                         "Failed to generate operation names\n"),
                        -1);
    }

  this->state_ = Demux_Test_CodeGenerator::CG_NONE;

  // debugging
  if (this->debug_)
    {
      if (this->print_names () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) Demux_Test_CodeGenerator::run - "
                             "Failed to print all generated names\n"),
                            -1);
        }
    }
  
  // generate include code for client-side main program
  if (this->gen_idl_interface () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) Demux_Test_CodeGenerator::run - "
                         "Failed to generate IDL interface\n"),
                        -1);
    }

  // generate the impl header
  if (this->gen_servant_hdr_code () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) Demux_Test_CodeGenerator::run - "
                         "Failed to generate servant header code\n"),
                        -1);
    }

  // generate impl 
  if (this->gen_servant_impl_code () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) Demux_Test_CodeGenerator::run - "
                         "Failed to generate servant impl code\n"),
                        -1);
    }

  // generate include code for client-side main program
  if (this->gen_client_incl_code () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) Demux_Test_CodeGenerator::run - "
                         "Failed to generate client code\n"),
                        -1);
    }

  // success
  return 0;
}

