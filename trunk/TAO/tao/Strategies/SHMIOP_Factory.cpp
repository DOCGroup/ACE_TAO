// $Id$

#include "tao/Strategies/SHMIOP_Factory.h"

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "tao/Strategies/SHMIOP_Acceptor.h"
#include "tao/Strategies/SHMIOP_Connector.h"

#include "tao/ORB_Constants.h"

#include "ace/Arg_Shifter.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/OS_NS_strings.h"

static const char prefix_[] = "shmiop";

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_SHMIOP_Protocol_Factory::TAO_SHMIOP_Protocol_Factory (void)
  : TAO_Protocol_Factory (TAO_TAG_SHMEM_PROFILE),
    mmap_prefix_ (0),
    min_bytes_ (10*1024)        // @@ Nanbor, remove this magic number!!
{
}

TAO_SHMIOP_Protocol_Factory::~TAO_SHMIOP_Protocol_Factory (void)
{
  delete [] this->mmap_prefix_;
}

int
TAO_SHMIOP_Protocol_Factory::match_prefix (const ACE_CString &prefix)
{
  // Check for the proper prefix for this protocol.
  return (ACE_OS::strcasecmp (prefix.c_str (), ::prefix_) == 0);
}

const char *
TAO_SHMIOP_Protocol_Factory::prefix (void) const
{
  return ::prefix_;
}

char
TAO_SHMIOP_Protocol_Factory::options_delimiter (void) const
{
  return '/';
}

TAO_Acceptor *
TAO_SHMIOP_Protocol_Factory::make_acceptor (void)
{
  TAO_SHMIOP_Acceptor *acceptor = 0;

  ACE_NEW_RETURN (acceptor,
                  TAO_SHMIOP_Acceptor,
                  0);

  acceptor->set_mmap_options (this->mmap_prefix_,
                              this->min_bytes_);

  return acceptor;
}

int
TAO_SHMIOP_Protocol_Factory::init (int argc,
                                   ACE_TCHAR* argv[])
{
  // Copy command line parameter not to use original as well as type conversion.
  ACE_Argv_Type_Converter command_line(argc, argv);

  ACE_Arg_Shifter arg_shifter (command_line.get_argc(), command_line.get_TCHAR_argv());

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR *current_arg = 0;

      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-MMAPFileSize"))))
        {
          this->min_bytes_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-MMAPFilePrefix"))))
        {
          this->mmap_prefix_ = ACE::strnew (current_arg);
          arg_shifter.consume_arg ();
        }
      else
        // Any arguments that don't match are ignored so that the
        // caller can still use them.
        arg_shifter.ignore_arg ();
    }

  return 0;
}

TAO_Connector *
TAO_SHMIOP_Protocol_Factory::make_connector (void)
{
  TAO_Connector *connector = 0;

  ACE_NEW_RETURN (connector,
                  TAO_SHMIOP_Connector,
                  0);
  return connector;
}

int
TAO_SHMIOP_Protocol_Factory::requires_explicit_endpoint (void) const
{
  return 1;
}


ACE_STATIC_SVC_DEFINE (TAO_SHMIOP_Protocol_Factory,
                       ACE_TEXT ("SHMIOP_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_SHMIOP_Protocol_Factory),
                       ACE_Service_Type::DELETE_THIS |
                                  ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_Strategies, TAO_SHMIOP_Protocol_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */
