// $Id$
#include "CCF/CompilerElements/FileSystem.hpp"
#include "CCF/CompilerElements/TokenStream.hpp"
#include "CCF/CompilerElements/Preprocessor.hpp"
#include "CCF/CompilerElements/Diagnostic.hpp"
#include "CCF/CompilerElements/Context.hpp"

#include "CCF/CodeGenerationKit/CommandLine.hpp"
#include "CCF/CodeGenerationKit/CommandLineParser.hpp"
#include "CCF/CodeGenerationKit/CommandLineDescriptor.hpp"

#include "CCF/CIDL/LexicalAnalyzer.hpp"
#include "CCF/CIDL/Parser.hpp"
#include "CCF/CIDL/SyntaxTree.hpp"
#include "CCF/CIDL/SemanticAction/Impl/Factory.hpp"

#include "ExecutorMappingGenerator.hpp"
#include "ServantGenerator.hpp"
#include "RepositoryIdGenerator.hpp"
#include "DescriptorGenerator.hpp"

#include <iostream>

using std::cerr;
using std::endl;

using namespace CCF;
using namespace CIDL;
using namespace SyntaxTree;

int main (int argc, char* argv[])
{

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
    ServantGenerator svnt_gen (cl);
    RepositoryIdGenerator repid_gen;
    DescriptorGenerator desc_gen;

    if (cl.get_value ("help", false) || cl.get_value ("help-html", false))
    {
      CL::Description d (argv[0]);

      lem_gen.options (d);
      svnt_gen.options (d);
      desc_gen.options (d);

      d.add_option (CL::OptionDescription (
                      "trace-semantic-actions",
                      "Turn on semnatic actions tracing facility.",
                      true));

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
        ifs.open (file_path, std::ios_base::in);
      }
      catch (fs::filesystem_error const&)
      {
        cerr << *i << ": error: unable to open in read mode" << endl;
        return -1;
      }
      catch (std::ios_base::failure const&)
      {
        cerr << *i << ": error: unable to open in read mode" << endl;
        return -1;
      }
    }

    //@@ for some reason ifs throws exception if I don't reset it to
    //   original state. It probably has something to do with call to
    //   get after eof.
    ifs.exceptions (ios_base::iostate (0));

    std::istream& is = 
      ifs.is_open () ? static_cast<std::istream&> (ifs) 
                     : static_cast<std::istream&> (std::cin);

    InputStreamAdapter isa (is);
    Preprocessor pp (isa);

    if (cl.get_value ("preprocess-only", false))
    {
      while (true)
      {
        Preprocessor::int_type i = pp.next ();

        if (pp.eos (i)) break;

        Preprocessor::char_type c = pp.to_char_type (i);

        std::cout << c ;
      }
      
      return 0;
    }


    Diagnostic::Stream diagnostic_stream;


    LexicalAnalyzer lexer (pp);
    TokenList token_stream;

    //@@ bad token comparison
    for (TokenPtr token = lexer.next ();; token = lexer.next ())
    {
      token_stream.push_back (token);
      if (ReferenceCounting::strict_cast<EndOfStream> (token) != 0) break;
    }

    if (token_stream.size () == 1)
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
        new TranslationRegion (fs::path (".builtin"),
                               unit->table (),
                               unit->create_order ()));
      unit->insert (builtin);

      // Inject built-in types into the file scope of this
      // translation region

      ScopePtr s = builtin->scope ();

      s->insert (BuiltInTypeDefPtr (new Object           (s)));
      s->insert (BuiltInTypeDefPtr (new ValueBase        (s)));
      s->insert (BuiltInTypeDefPtr (new Any              (s)));
      s->insert (BuiltInTypeDefPtr (new Boolean          (s)));
      s->insert (BuiltInTypeDefPtr (new Char             (s)));
      s->insert (BuiltInTypeDefPtr (new Double           (s)));
      s->insert (BuiltInTypeDefPtr (new Float            (s)));
      s->insert (BuiltInTypeDefPtr (new Long             (s)));
      s->insert (BuiltInTypeDefPtr (new LongDouble       (s)));
      s->insert (BuiltInTypeDefPtr (new LongLong         (s)));
      s->insert (BuiltInTypeDefPtr (new Octet            (s)));
      s->insert (BuiltInTypeDefPtr (new Short            (s)));
      s->insert (BuiltInTypeDefPtr (new String           (s)));
      s->insert (BuiltInTypeDefPtr (new UnsignedLong     (s)));
      s->insert (BuiltInTypeDefPtr (new UnsignedLongLong (s)));
      s->insert (BuiltInTypeDefPtr (new UnsignedShort    (s)));
      s->insert (BuiltInTypeDefPtr (new Void             (s)));
      s->insert (BuiltInTypeDefPtr (new Wchar            (s)));
      s->insert (BuiltInTypeDefPtr (new Wstring          (s)));
    }

    //Create implied #include <Components.idl>
    {
      TranslationRegionPtr builtin (
        new ImpliedIncludeTranslationRegion (fs::path ("Components.idl"),
                                             unit->table (),
                                             unit->create_order ()));
      unit->insert (builtin);

      ScopePtr fs = builtin->scope ();
      ModulePtr m (new Module (SimpleName("Components"), fs));
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


    CompilerElements::Context context;
    context.set ("file-path", file_path);

    bool trace = cl.get_value ("trace-semantic-actions", false);

    context.set ("idl2::semantic-action::trace", trace);
    context.set ("idl3::semantic-action::trace", trace);
    context.set ("cidl::semantic-action::trace", trace);


    SemanticAction::Impl::Factory actions (context, diagnostic_stream, tr);

    //-----------------------------------------------------------------

    Parser parser (context, diagnostic_stream, lexer, actions);

    IDL2::Parsing::parse (token_stream.begin (),
                          token_stream.end (),
                          parser.start ());

    if (diagnostic_stream.error_count () != 0) return -1;

    // Generate executor mapping.
    {
      lem_gen.generate (cl, unit);
    }

    // Generate servant code.
    {
      svnt_gen.generate (unit);
    }
    
    // Compute repository IDs in a separate pass.
    {
      repid_gen.generate (unit);
    }

    // Generate descriptor code.
    {
      desc_gen.generate (cl, unit);
    }
  }
  catch (Declaration::NotInScope const&)
  {
    cerr << "exception: " << "Declaration::NotInScope" << endl;
  }
  catch (...)
  {
    cerr << "exception: " << "unknown" << endl;
  }
}
