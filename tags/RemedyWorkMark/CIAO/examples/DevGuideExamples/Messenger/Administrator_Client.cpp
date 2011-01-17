// $Id$

/**
 *  @file Administrator_Client.cpp
 *
 *  Do NOT put this file in a project; it is included by
 *  Administrator_Client_IDL2.cpp and Administrator_Client_IDL3.cpp
 *  This file will not build on its own.
 *
 *  @author Don Busch <busch_d@ociweb.com>
 */

#include "ace/Get_Opt.h"
#include <iostream>

const ACE_TCHAR* ior = ACE_TEXT ("file://Messenger.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
  {
      case 'k':
        ior = get_opts.optarg;
        break;

      case '?':
      default:
        std::cerr << "usage: " << argv[0] << " -k <ior>" << std::endl;
        return -1;
        break;
  }
  return 0;
}

int
svc( Runnable_ptr runnable, Publication_ptr publication )
{
    enum SelectionType { START=1, STOP, CHANGE_PERIOD, CHANGE_TEXT, EXIT };

    bool done = false;
    do {
      std::cout << "\nWhat do you want to do to the Messenger(s)?" << std::endl;
      std::cout << START << ".  Start" << std::endl;
      std::cout << STOP << ".  Stop" << std::endl;
      std::cout << CHANGE_PERIOD << ".  Change Publication Period" << std::endl;
      std::cout << CHANGE_TEXT << ".  Change Publication Text" << std::endl;
      std::cout << EXIT << ".  Exit" << std::endl;

      char selection_text[10];
      std::cout << "Please enter a selection: ";
      std::cin.getline( selection_text, sizeof(selection_text) );
      int selection = ACE_OS::atoi(selection_text);

      switch (selection) {
        case START: {
          runnable->start();
          break;
        }
        case STOP: {
          runnable->stop();
          break;
        }
        case CHANGE_PERIOD: {
          char period[10];
          std::cout << "Please enter a new period in seconds: ";
          std::cin.getline( period, sizeof( period ) );
          publication->period( ACE_OS::atoi(period) );
          break;
        }
        case CHANGE_TEXT: {
          char buffer[1024];
          std::cout << "Please enter new text: ";
          std::cin.getline( buffer, sizeof(buffer) );
          publication->text( buffer );
          break;
        }
        case EXIT: {
          done = true;
          break;
        }
        default:
          std::cout << "Please enter a valid option" << std::endl;
      }
    } while ( !done );

    return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (parse_args (argc, argv) != 0) {
    return 1;
  }

  try {
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    // get Messenger object
    CORBA::Object_var obj = orb->string_to_object(ior);
    Messenger_var messenger = Messenger::_narrow (obj.in());

    obj = messenger->provide_facet ("control");
    Runnable_var runnable = Runnable::_narrow (obj.in ());
    obj = messenger->provide_facet ("content");
    Publication_var publication = Publication::_narrow (obj.in ());

    svc( runnable.in(), publication.in() );

    return 0;
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "Caught CORBA::Exception: " << std::endl << ex << std::endl;
  }

  return 1;
}
