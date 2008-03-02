#include <typeinfo>
#include <string>
#include <iostream>

/* FUZZ: disable check_for_improper_main_declaration */

namespace CommandLine
{
  struct Parser
  {
  };
}

using std::string;
using std::cerr;
using std::endl;

using namespace CommandLine;

class Command
{
public:
  enum Value
  {
    HELP,
    VERSION,
    DEFAULT
  };

  Command (Value v = Command::DEFAULT)
      : v_ (v)
  {
  }

  operator Value () const
  {
    return v_;
  }

private:
  Value v_;
};


int
version ();

int
help (int argc, char* argv[]);

int
main (int argc, char* argv[])
{

  // Step 1: determine command
  //
  // * there is usually one command
  // * command can be optional
  // * command usually takes up one argument
  //

  CommandParser<Command> cp;

  switch (cp.parse (argc, argv))
  {
  case Command::VERSION:
    {
      return version ();
    }
  case Command::HELP:
    {
      return help (argc, argv);
    }
  }

  // Step 2: parse options
  //
  // * options are usually optional
  // * options are usually position-independant
  // * options usually do not repeat
  // * options can take up more than one argument
  //

  OptionMap om;

  CompositeParser op;

  op.add (OptionParser<string> ("string", "--string", "-s"));
  op.add (OptionParser<unsigned long> ("number", "--number", "-n"));

  while (argc != 1 && !op.empty ())
  {
    om.insert (op.parse (argc, argv));
  }

  // Step 3: parse operands
  //
  // * operands usually position-dependant
  // * operand usually take up one argument
  //

  OperandParser<string> odp;

  string str = odp.parse (argc, argv);

  unsigned long num = 0;

  if (argc != 1)
  {
    OperandParser<unsigned long> op;
    num = op.parse (argc, argv);
  }

  string s = om.count ("string") ? om["string"] : "default";
  unsigned long l = om["number"];

  // om.at ()
  // om.get ()
  // om.resolve ()
  // om.option ()
  // om.value ()

  cerr << "opreation settings are:" << endl << endl
       << "option string  : " << om.get<string> ("string", "default") << endl
       << "option number  : " << om.get ("number", 10UL) << endl
       << "operand string : " << str << endl
       << "operand number : " << num << endl;
}


//
//
//
int
version ()
{
  cerr << "foo 1.0" << endl;
  return 0;
}


//
//
//
int
help (int argc, char* argv[])
{
  Command subject;

  if (argc != 1)
  {
    OperandParser<Command> op;
    subject = op.parse (argc, argv);
  }

  switch (subject)
  {
  case Command::HELP:
    {
      cerr << "foo help [<command>]" << endl << endl
           << "\t If <command> is specified then print extended help" << endl
           << "\t information for specified command. Otherwise print" << endl
           << "\t general usage information." << endl;
      break;
    }
  case Command::VERSION:
    {
      cerr << "foo version" << endl << endl
           << "\t Print version information." << endl;
      break;
    }
  default:
    {
      cerr << "foo version" << endl
           << "foo help [<command>]" << endl
           << "foo [-s|--string <str>] [-n|--number <num>] <str> [<num>]"
           << endl;
      break;
    }
  }

  return 0;
}
//$Id$
