#ifndef ANNOTEST_IDL_BE_UTIL_HEADER
#define ANNOTEST_IDL_BE_UTIL_HEADER

class TAO_OutStream;
class be_module;
class AST_Decl;
class AST_Generator;

class be_util
{
public:
  /// Special BE arg call factored out of DRV_args.
  static void prep_be_arg (char *s);

  /// Checks made after parsing args.
  static void arg_post_proc ();

  /// Display usage of BE-specific options.
  static void usage ();

  /// Create an AST node generator.
  static AST_Generator *generator_init ();
};

#endif
