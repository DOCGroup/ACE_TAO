// $Id$
#include "CCF/CompilerElements/FileSystem.hpp"

#include "CCF/CodeGenerationKit/CommandLine.hpp"
#include "CCF/CodeGenerationKit/CommandLineParser.hpp"
#include "CCF/CodeGenerationKit/CommandLineDescriptor.hpp"

#include "CCF/CIDL/CIDL_LexicalAnalyzer.hpp"
#include "CCF/CIDL/CIDL_Parser.hpp"
#include "CCF/CIDL/CIDL_SyntaxTree.hpp"
#include "CCF/CIDL/CIDL_SemanticActionImpl.hpp"

#include "ExecutorMappingGenerator.hpp"
#include "ServantGenerator.hpp"

#include "CCF/CompilerElements/TokenStream.hpp"
#include "CCF/CompilerElements/Preprocessor.hpp"

#include <iostream>

using std::cerr;
using std::endl;

int main (int argc, char* argv[])
{
  using namespace IDL2::SyntaxTree;

  try
  {
    // Parsing command line options and arguments
    //
    //
    CommandLine cl;

    if (!parse (argc, argv, cl))
    {
      cerr << "command line syntax error" << endl;
      cerr << "try " << argv[0] << " --help for usage information" << endl;
      return -1;
    }

    ExecutorMappingGenerator lem_gen;

    if (cl.get_value ("help", false) || cl.get_value ("help-html", false))
    {
      CL::Description d (argv[0]);

      lem_gen.options (d);

      d.add_option (CL::OptionDescription (
                      "preprocess-only",
                      "Run preprocessor only and output result to stdout.",
                      true));

      d.add_option (CL::OptionDescription (
                      "help",
                      "Display usage information and exit.",
                      true));

      d.add_option (CL::OptionDescription (
                      "help-html",
                      "Dump usage information in html format and exit.",
                      true));

      d.add_argument ("cidl file");

      if (cl.get_value ("help-html", false)) CL::print_html (cerr, d);
      else CL::print_text (cerr, d);

      return 0;
    }


    fs::ifstream ifs;
    ifs.exceptions (ios_base::badbit | ios_base::failbit);

    fs::path file_path;

    CommandLine::ArgumentIterator i = cl.argument_begin ();

    if (i != cl.argument_end ())
    {
      try
      {
        file_path = fs::path (*i, fs::native);
        ifs.open (file_path, std::ios_base::out);
      }
      catch (fs::filesystem_error const&)
      {
        cerr << *i << ": error: unable to open in read mode" << endl;
        return -1;
      }
      catch (ios_base::failure const&)
      {
        cerr << *i << ": error: unable to open in read mode" << endl;
        return -1;
      }
    }

    //@@ for some reason ifs throws exception if I don't reset it to
    //   original state. It probably has something to do with call to
    //   get after eof.
    ifs.exceptions (ios_base::iostate (0));

    std::istream& is = ifs.is_open () 
      ? static_cast<std::istream&> (ifs)
      : static_cast<std::istream&> (std::cin);

    CCF::InputStreamAdapter isa (is);
    CCF::Preprocessor pp (isa);

    if (cl.get_value ("preprocess-only", false))
    {
      while (true)
      {
        CCF::Preprocessor::int_type i = pp.next ();

        if (pp.eos (i)) break;

        CCF::Preprocessor::char_type c = pp.to_char_type (i);

        std::cout << c ;
      }
      return 0;
    }


    CIDL::LexicalAnalyzer lexer (pp);
    TokenStream token_stream;

    //@@ bad token comparison
    for (TokenPtr token = lexer.next ();
         token.in () != lexer.eos.in ();
         token = lexer.next ())
    {
      token_stream.push_back (token);
    }

    if (token_stream.size () == 0)
    {
      cerr << "no tokens produced so nothing to parse" << endl;
      return 0;
    }

    TranslationUnitPtr unit (new TranslationUnit);

    //-----------------------------------------------------------------
    //@@ exeprimental code

    //Create .builtin region
    {
      TranslationRegionPtr builtin (
        new TranslationRegion (unit->table (),
                               unit->create_order ()));
      unit->insert (builtin);

      // Inject built-in types into the file scope of this
      // translation region

      ScopePtr s = builtin->scope ();

      // Note: I know what I am doing here (and if you don't then
      //       read MExC++#17 again).

      s->insert (BuiltInTypeDefPtr (new Void (s)));
      s->insert (BuiltInTypeDefPtr (new Long (s)));
      s->insert (BuiltInTypeDefPtr (new Boolean (s)));
      s->insert (BuiltInTypeDefPtr (new String (s)));
    }

    //@@ This should be in IDL3 or even CIDL part I just need
    //   a mechanism to create them in proper order.
    {
      TranslationRegionPtr builtin (
        new ImpliedIncludeTranslationRegion (fs::path ("Components.idl"),
                                             unit->table (),
                                             unit->create_order ()));
      unit->insert (builtin);

      ScopePtr fs = builtin->scope ();
      ModulePtr m (new SyntaxTree::Module (SimpleName("Components"), fs));
      fs->insert (m);

      LocalInterfaceDefPtr i (
        new LocalInterfaceDef (SimpleName ("EnterpriseComponent"),
                               m,
                               ScopedNameSet ()));

      m->insert (i);
    }

    TranslationRegionPtr tr (
      new PrincipalTranslationRegion (file_path,
                                      unit->table (),
                                      unit->create_order ()));
    unit->insert (tr);

    CIDL::SemanticActionFactoryImpl action_factory (tr);

    //-----------------------------------------------------------------

    CIDL::Parser parser (lexer, action_factory);

    bool result = Details::parse (token_stream.begin (),
                                  token_stream.end (),
                                  parser.start ());

    if (!result) return -1;

    // Generate executor mapping

    {
      lem_gen.generate (cl, unit);
    }

    // Generate servant code
    {
      ServantGenerator gen (cl);
      gen.generate (unit);
    }

  }
  catch (Declaration::NotInScope const&)
  {
    cerr << "exception: " << "Declaration::NotInScope" << endl;
  }
  catch (...)
  {
    cerr << "exception: " << "unknow" << endl;
  }
}
