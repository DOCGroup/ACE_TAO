/* FUZZ: disable check_for_improper_main_declaration */

#include <string>
#include <iostream>

using std::cerr;
using std::endl;

class Command
{
public:
  enum Value
  {
    HELP = 0,
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

  friend std::ostream&
  operator<< (std::ostream& os, Command c);

  friend std::istream&
  operator>> (std::istream& is, Command& c);

private:
  Value v_;
  static char* labels_[];
};

char* Command::labels_[] = {"help", "version", "default"};


std::ostream&
operator<< (std::ostream& os, Command c)
{
  return os << Command::labels_[c.v_];
}

std::istream&
operator>> (std::istream& is, Command& c)
{
  std::string s;
  is >> s;
  if (is)
  {
    if (s == Command::labels_[Command::HELP]) c.v_ = Command::HELP;
    else if (s == Command::labels_[Command::VERSION]) c.v_ = Command::VERSION;
    else is.setstate (std::ios::failbit);
  }
  return is;
}

int
main ()
{
  Command c = Command::HELP;

  c = Command::DEFAULT;

  Command c1 (Command::HELP);

  c = c1;

  cerr << c << endl;

  switch (c)
  {
  case Command::HELP:
    {
      cerr << "iiihuuu!!!" << endl;
    }
  }

  std::cin >> c1;
  if (std::cin) cerr << c1 << endl;
  else cerr << "*failed" << endl;
}
//$Id$
