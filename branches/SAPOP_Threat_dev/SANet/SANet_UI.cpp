// -*- C++ -*-

//=============================================================================
/**
 * @file  SANet_UI.cpp
 *
 * This file contains the user interface for running and visualizing spreading
 * activation networks.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SANET_ONLY
#define SANET_ONLY
#endif

#include <iostream>
#include <fstream>
#include <string>
#include "SANet.h"
#include "SANetFileIn.h"

using namespace SANet;

/// User interface main function.
/**
 * @param argc  The number of command line arguments.
 *
 * @param argv  An array of char *, containing the command line arguments.
 *
 * @return -1 on error, else 0.
 */
int main (int argc, char *argv[])
{
  std::cout << "Spreading Activation Network User Interface" << std::endl;

  // Get filename from user.
  std::cout << "Network file: ";
  std::string filename = "";
  std::cin >> filename;

  // Build network.
  SANetFileIn in;
  SANet::Network *net = in.build_net (filename);

  // Print XML representation to output1.xml.
  std::ofstream out_file ("output1.xml");
  if (out_file == 0){
    std::string msg = "Unable to open output1.xml for writing";
    throw msg;
  }
  net->print_xml (out_file);
  out_file.close ();

/*
  net->add_cond (1111, "C1", 1, 1, 0, 0);
  net->add_cond (1112, "C2", 1, 1, 0, 0);
  net->add_task (2221, "A1", 1, 0, 0.375);
  net->add_cond (1113, "C3", 1, 0, 1, 10);
  net->add_cond (1114, "C4", 1, 0, 1, 10);
  net->add_precond_link (1111, 2221, 0.75, 0);
  net->add_precond_link (1112, 2221, 0.5, 0.25);
  net->add_effect_link (2221, 1113, 1);
  net->add_effect_link (2221, 1114, 0.9);
*/

/*
  net->add_cond (1110, "C0", 1, 1, 0, 0);
  net->add_task (2221, "A1", 1, 0, 0.5);
  net->add_task (2222, "A2", 1, 0, 0.5);
  net->add_cond (1111, "C1", 1, 0, 1, 0);
  net->add_cond (1112, "C2", 1, 0, 1, 0);
  net->add_task (2223, "A3", 1, 1, 0.25);
  net->add_cond (1113, "C3", 1, 0, 1, 10);
  net->add_precond_link (1110, 2221, 1, 0);
  net->add_precond_link (1110, 2222, 1, 0);
  net->add_effect_link (2221, 1111, 1);
  net->add_effect_link (2222, 1112, 1);
  net->add_precond_link (1111, 2223, 0.5, 0);
  net->add_precond_link (1112, 2223, 0.5, 0);
  net->add_effect_link (2223, 1113, 1);
*/

/*
  net->add_cond (1111, "C1", 1, 1, 0, 0);
  net->add_cond (1112, "C2", 1, 1, 0, 0);
  net->add_task (2221, "A1", 1, 1, 0.5);
  net->add_task (2222, "A2", 1, 3, 0.5);
  net->add_cond (1113, "C3", 1, 0, 1, 10);
  net->add_precond_link (1111, 2221, 1, 0);
  net->add_precond_link (1112, 2222, 1, 0);
  net->add_effect_link (2221, 1113, 1);
  net->add_effect_link (2222, 1113, -1);
*/

/*
  net->print ();

  net->update (10);

  std::cout << std::endl;
  net->print (std::cout, true);
*/

/*
  std::string filename = "";
  Parser::XML_Parser parser;
  Parser::XML_Object *obj;
  Parser::SA_Net_Builder builder;
  Network *net;

  // Get network file from user.
  std::cout << "Network file: ";
  std::cin >> filename;
  
  try {
    obj = parser.parse_file (filename);
  } catch (Parser::Invalid_File e) {
    std::cerr << "Invalid File exception occurred while opening network file." << std::endl;
    std::cerr << e.err_msg ();
    return -1;
  } catch (Parser::Duplicate_Param e) {
    std::cerr << "Duplicate Parameter exception occurred while parsing network file." << std::endl;
    std::cerr << e.err_msg ();
    return -1;
  } catch (Parser::Invalid_Character e) {
    std::cerr << "Invalid Character exception occurred while parsing network file." << std::endl;
    std::cerr << e.err_msg ();
    return -1;
  } catch (Parser::Invalid_Param_Name e) {
    std::cerr << "Invalid Parameter Name exception occurred while parsing network file." << std::endl;
    std::cerr << e.err_msg ();
    return -1;
  } catch (Parser::Invalid_Param_Type e) {
    std::cerr << "Invalid Parameter Type exception occurred while parsing network file." << std::endl;
    std::cerr << e.err_msg ();
    return -1;
  } catch (Parser::Invalid_Tag_Name e) {
    std::cerr << "Invalid Tag Name exception occurred while parsing network file." << std::endl;
    std::cerr << e.err_msg ();
    return -1;
  } catch (char *e) {
    std::cerr << "Exception occurred while parsing network file." << std::endl;
    std::cerr << e;
    return -1;
  } catch (...) {
    std::cerr << "Unknown exception occurred while parsing network file.";
    std::cerr << std::endl;
    return -1;
  }

  try {
    net = builder.build_net (obj);
  } catch (char *e) {
    std::cerr << "Exception occurred while building network." << std::endl;
    std::cerr << e;
    return -1;
  } catch (...) {
    std::cerr << "Unknown exception occurred while building network.";
    std::cerr << std::endl;
    return -1;
  }
*/
  std::cout << "Initial network configuration:" << std::endl;
  net->print ();

  // Get max steps from user.
  std::cout << "Max steps to run: ";
  std::string max_steps_str = "";
  std::cin >> max_steps_str;

  // Run spreading activation.
  net->update (atoi (max_steps_str.c_str ()));

  std::cout << "Final network configuration:" << std::endl;
  net->print (std::cout, true);

  std::ofstream out2_file ("output2.xml");
  if (out2_file == 0){
    std::string msg = "Unable to open output2.xml for writing";
    throw msg;
  }
  net->print_xml (out2_file);
  out2_file.close ();

//  delete obj;
  delete net;

//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  std::cout << "Enter any character to end program: ";
  char temp_;
  std::cin>>temp_;
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

  return 0;
}
