#ifndef TEMPERATURE_GRAPHER_OPTIONS_H
#define TEMPERATURE_GRAPHER_OPTIONS_H

class Temperature_Grapher_Options
{
public:
  Temperature_Grapher_Options (int argc, ACE_TCHAR **argv)
  {
    ACE_UNUSED_ARG (argc);
    ACE_UNUSED_ARG (argv);
  }

  /// every 20 seconds
  int poll_interval () const
  {
    return 20;
  }
};

#endif /* TEMPERATURE_GRAPHER_OPTIONS_H */
