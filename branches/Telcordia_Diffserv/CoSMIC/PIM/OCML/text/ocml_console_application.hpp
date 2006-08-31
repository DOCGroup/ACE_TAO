#include "application.hpp"
#include "environment.hpp"
#include "engine/configurator.hpp"

class OCML_Console_Application: public Application
{
public:
  OCML_Console_Application(std::istream& is);

  ~OCML_Console_Application();

private:
  OCML::Configurator* configurator_;
  Environment env_;
};

