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
#include "InputCL.h"


namespace SANet {
  namespace Default {
    const size_t SA_MAX_STEPS = 1000;
  };  /* SANet::Default namespace */
};  /* SANet namespace */

// Forward declaration of testing functions.
int test_clone (std::string filename);
int test_user_interface (void);
int test_net_gen (void);

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
  std::cout << "Spreading Activation Network User Interface" << std::endl << std::endl;

  // To run a test, uncomment appropriate test function.
//  return test_clone ("simple.san.xml");
//  return test_net_gen ();
//  return test_user_interface ();

  UserInterface::InputCL user_input;

  // Get filename from user.
  std::string filename = "";
  UserInterface::Question file_ques ("Spreading activation network file:");
  if (user_input.ask (file_ques))
    filename = file_ques.get_answer ();

  // Get flag for whether to output network configurations to files.
  bool output_to_file = false;

  // Build network.
  SANetFileIn in;
  SANet::Network *net = in.build_net (filename);

  // Display initial configuration.
  std::cout << "Initial network configuration:" << std::endl;
  net->print (std::cout, true);

  // Print XML representation to output_init.xml.
  if (output_to_file) {
    std::ofstream file_init ("output_init.xml");
    if (file_init == 0){
      std::string msg = "Unable to open output_init.xml for writing";
      throw msg;
    }
    net->print_xml (file_init);
    file_init.close ();
  }

  // Get max steps from user.
  size_t max_steps = SANet::Default::SA_MAX_STEPS;
  UserInterface::QuestionInt steps_ques("Max steps to run (10000 step limit): ", 0, 10000);
  if (user_input.ask (steps_ques))
    max_steps = steps_ques.get_answer_int ();
  

  // Run spreading activation.
  net->update (max_steps);

  // Display final configuration.
  std::cout << "Final network configuration:" << std::endl;
  net->print (std::cout, true);

  // Print XML representation to output_final.xml.
  if (output_to_file) {
    std::ofstream file_final ("output_final.xml");
    if (file_final == 0){
      std::string msg = "Unable to open output_final.xml for writing";
      throw msg;
    }
    net->print_xml (file_final);
    file_final.close ();
  }

  // Delete network.
  delete net;

  // Wait for user to end program.
//  UserInterface::Question end_ques ("Enter any character to end program:");
//  user_input.ask (end_ques);

  return 0;
};

int test_clone (std::string filename, bool output_to_file = false)
{
  UserInterface::InputCL user_input;

  // Build network.
  SANetFileIn in;
  SANet::Network *net = in.build_net (filename);

  // Create initial network clone.
  SANet::Network *net_dupe_init = new SANet::Network (*net);

  // Display initial configurations.
  std::cout << "Initial network configuration:" << std::endl;
  net->print (std::cout, true);
  std::cout << "Initial clone network configuration:" << std::endl;
  net_dupe_init->print (std::cout, true);

  // Print initial XML representation of networks.
  if (output_to_file) {
    // Print initial network XML representation.
    std::ofstream file_init ("output_init.xml");
    if (file_init == 0){
      std::string msg = "Unable to open output_init.xml for writing";
      throw msg;
    }
    net->print_xml (file_init);
    file_init.close ();

    // Print initial clone network XML representation.
    std::ofstream file_dupe_init ("output_clone_init.xml");
    if (file_dupe_init == 0){
      std::string msg = "Unable to open output_clone_init.xml for writing";
      throw msg;
    }
    net_dupe_init->print_xml (file_dupe_init);
    file_dupe_init.close ();
  }

  // Get max steps from user.
  size_t max_steps = SANet::Default::SA_MAX_STEPS;
  UserInterface::QuestionInt steps_ques("Max steps to run (10000 step limit): ", 0, 10000);
  if (user_input.ask (steps_ques))
    max_steps = steps_ques.get_answer_int ();
  
  // Run spreading activation on original network.
  net->update (max_steps);

  // Clone updated network.
  SANet::Network *net_dupe_final = new SANet::Network (*net);

  // Display final configurations.
  std::cout << "Final network configuration (ORIGINAL net) after spreading activation:" << std::endl;
  net->print (std::cout, true);
  std::cout << "Network configuration (CLONE of INITIAL net with no spreading activation performed):" << std::endl;
  net_dupe_init->print (std::cout, true);
  std::cout << "Network configuration (CLONE of FINAL net after spreading activation):" << std::endl;
  net_dupe_final->print (std::cout, true);

  // Print final XML representation of networks.
  if (output_to_file) {
    // Print network XML representation.
    std::ofstream file_final ("output_final.xml");
    if (file_final == 0){
      std::string msg = "Unable to open output_final.xml for writing";
      throw msg;
    }
    net->print_xml (file_final);
    file_final.close ();

    // Print clone network XML representation.
    std::ofstream file_dupe_final ("output_clone_final.xml");
    if (file_dupe_final == 0){
      std::string msg = "Unable to open output_clone_final.xml for writing";
      throw msg;
    }
    net_dupe_init->print_xml (file_dupe_final);
    file_dupe_final.close ();

    // Print second (after SA) clone network XML representation.
    std::ofstream file_dupe2_final ("output_clone2_final.xml");
    if (file_dupe2_final == 0){
      std::string msg = "Unable to open output_clone2_final.xml for writing";
      throw msg;
    }
    net_dupe_final->print_xml (file_dupe2_final);
    file_dupe2_final.close ();
  }


  // Run spreading activation on clones.
  net_dupe_init->update (max_steps);
  net_dupe_final->update (max_steps);

  // Display final configurations.
  std::cout << "Final network configuration (CLONE of INITIAL net) after spreading activation:" << std::endl;
  net_dupe_init->print (std::cout, true);
  std::cout << "Final network configuration (CLONE of FINAL net) after additional spreading activation:" << std::endl;
  net_dupe_final->print (std::cout, true);


  // Delete networks.
  delete net;
  delete net_dupe_init;
  delete net_dupe_final;

  // Wait for user to end program.
  UserInterface::Question end_ques ("Enter any character to end program:");
  user_input.ask (end_ques);

  return 0;
};



namespace UserInterface {
  namespace Testing {
    /// Type of an example enumerated type for user input.
    enum RunKind {CONTINUE, STOP, EXIT, INVALID};
  };  /* UserInterface::Testing namespace */
};  /* UserInterface namespace */


int test_user_interface (void)
{
  std::string answer = "999999";
  int answer_int = 999999;
  UserInterface::Testing::RunKind answer_val = UserInterface::Testing::INVALID;
  std::string answer_val_str = "InVsTr";
  bool answer_bool = false;

  UserInterface::InputCL user_input;

  UserInterface::Question ques ("Testing display info.");

  user_input.info (ques);

  ques.set_prompt ("Testing prompt (enter anything):");
  user_input.ask (ques);
  std::cout << "Your answer was \"" << ques.get_answer () << "\"" << std::endl;

  answer = "999999";
  answer_int = 999999;
  UserInterface::QuestionInt ques_int ("Enter an integer between -100 and 5000:", -100, 5000);

  user_input.ask (ques_int);
  answer = ques_int.get_answer ();
  answer_int = ques_int.get_answer_int ();
  std::cout << "Your answer was \"" << answer << "\" which equals " << answer_int << "." << std::endl;




  std::cout << "QuestionChoice<int> case insensitive" << std::endl;
  UserInterface::QuestionChoice<int> ques_choice_int ("Enter (O)ne, (T)wo, or (F)our:", -1);
  ques_choice_int.add_mapping ("O", 1);
  ques_choice_int.add_mapping ("One", 1);
  ques_choice_int.add_mapping ("T", 2);
  ques_choice_int.add_mapping ("Two", 2);
  ques_choice_int.add_mapping ("F", 4);
  ques_choice_int.add_mapping ("Four", 4);

  answer = "InV";
  answer_val_str = "InVsTr";
  answer_int = -2;

  user_input.ask (ques_choice_int);
  answer = ques_choice_int.get_answer ();
  answer_int = ques_choice_int.get_answer_val ();
  switch (answer_int)
  {
  case 1:
    answer_val_str = "1 value";
    break;
  case 2:
    answer_val_str = "2 value";
    break;
  case 4:
    answer_val_str = "4 value";
    break;
  case -1:
    answer_val_str = "INVALID value";
    break;
  default:
    answer_val_str = "Unknown answer value";
    break;
  }
  std::cout << "Your answer was \"" << answer << "\" which equals " << answer_val_str << "." << std::endl;




  std::cout << "QuestionChoice case insensitive" << std::endl;
  UserInterface::QuestionChoice<UserInterface::Testing::RunKind> ques_choice ("Enter (C)ontinue, (S)top, or (E)xit:", UserInterface::Testing::INVALID);
  ques_choice.add_mapping ("C", UserInterface::Testing::CONTINUE);
  ques_choice.add_mapping ("Continue", UserInterface::Testing::CONTINUE);
  ques_choice.add_mapping ("S", UserInterface::Testing::STOP);
  ques_choice.add_mapping ("Stop", UserInterface::Testing::STOP);
  ques_choice.add_mapping ("E", UserInterface::Testing::EXIT);
  ques_choice.add_mapping ("Exit", UserInterface::Testing::EXIT);

  answer = "InV";
  answer_val_str = "InVsTr";
  answer_val = UserInterface::Testing::INVALID;

  user_input.ask (ques_choice);
  answer = ques_choice.get_answer ();
  answer_val = ques_choice.get_answer_val ();
  switch (answer_val)
  {
  case UserInterface::Testing::CONTINUE:
    answer_val_str = "CONTINUE value";
    break;
  case UserInterface::Testing::STOP:
    answer_val_str = "STOP value";
    break;
  case UserInterface::Testing::EXIT:
    answer_val_str = "EXIT value";
    break;
  case UserInterface::Testing::INVALID:
    answer_val_str = "INVALID value";
    break;
  default:
    answer_val_str = "Unknown answer value";
    break;
  }
  std::cout << "Your answer was \"" << answer << "\" which equals " << answer_val_str << "." << std::endl;


  std::cout << "QuestionChoice case sensitive" << std::endl;
  UserInterface::QuestionChoice<UserInterface::Testing::RunKind> ques_choice_case ("Enter (C)ontinue, (S)top, or (E)xit:", UserInterface::Testing::INVALID, true);
  ques_choice_case.add_mapping ("C", UserInterface::Testing::CONTINUE);
  ques_choice_case.add_mapping ("Continue", UserInterface::Testing::CONTINUE);
  ques_choice_case.add_mapping ("S", UserInterface::Testing::STOP);
  ques_choice_case.add_mapping ("Stop", UserInterface::Testing::STOP);
  ques_choice_case.add_mapping ("E", UserInterface::Testing::EXIT);
  ques_choice_case.add_mapping ("Exit", UserInterface::Testing::EXIT);

  answer = "InV";
  answer_val_str = "InVsTr";
  answer_val = UserInterface::Testing::INVALID;

  user_input.ask (ques_choice_case);
  answer = ques_choice_case.get_answer ();
  answer_val = ques_choice_case.get_answer_val ();
  switch (answer_val)
  {
  case UserInterface::Testing::CONTINUE:
    answer_val_str = "CONTINUE value";
    break;
  case UserInterface::Testing::STOP:
    answer_val_str = "STOP value";
    break;
  case UserInterface::Testing::EXIT:
    answer_val_str = "EXIT value";
    break;
  case UserInterface::Testing::INVALID:
    answer_val_str = "INVALID value";
    break;
  default:
    answer_val_str = "Unknown answer value";
    break;
  }
  std::cout << "Your answer was \"" << answer << "\" which equals " << answer_val_str << "." << std::endl;


  std::cout << "QuestionBool" << std::endl;
  UserInterface::QuestionBool ques_bool ("Enter (Y)es or (N)o:", false);

  answer = "InV";
  answer_val_str = "InVsTr";
  answer_bool = true;

  user_input.ask (ques_bool);
  answer = ques_bool.get_answer ();
  answer_bool = ques_bool.get_answer_val ();
  switch (answer_bool)
  {
  case true:
    answer_val_str = "TRUE value";
    break;
  case false:
    answer_val_str = "FALSE value";
    break;
  default:
    answer_val_str = "Unknown answer value";
    break;
  }
  std::cout << "Your answer was \"" << answer << "\" which equals " << answer_val_str << "." << std::endl;


  // Wait for user to end program.
  UserInterface::Question end_ques ("Enter any character to end program:");
  user_input.ask (end_ques);

  return 0;
};



int test_net_gen (void)
{
  UserInterface::InputCL user_input;

  // Build network.
  SANet::Network *net = new SANet::Network ();

  net->add_cond (1111, "C1", 1, 1, 0, 0, ENVIRON);
  net->add_cond (1112, "C2", 1, 1, 0, 0, ENVIRON);
  net->add_task (2221, "A1", 1, 0, 0.375);
  net->add_cond (1113, "C3", 1, 0, 1, 10, ENVIRON);
  net->add_cond (1114, "C4", 1, 0, 1, 10, ENVIRON);
  net->add_precond_link (1111, 2221, 0.75, 0);
  net->add_precond_link (1112, 2221, 0.5, 0.25);
  net->add_effect_link (2221, 1113, 1);
  net->add_effect_link (2221, 1114, 0.9);

/*
  net->add_cond (1110, "C0", 1, 1, 0, 0, ENVIRON);
  net->add_task (2221, "A1", 1, 0, 0.5);
  net->add_task (2222, "A2", 1, 0, 0.5);
  net->add_cond (1111, "C1", 1, 0, 1, 0, ENVIRON);
  net->add_cond (1112, "C2", 1, 0, 1, 0, ENVIRON);
  net->add_task (2223, "A3", 1, 1, 0.25);
  net->add_cond (1113, "C3", 1, 0, 1, 10, ENVIRON);
  net->add_precond_link (1110, 2221, 1, 0);
  net->add_precond_link (1110, 2222, 1, 0);
  net->add_effect_link (2221, 1111, 1);
  net->add_effect_link (2222, 1112, 1);
  net->add_precond_link (1111, 2223, 0.5, 0);
  net->add_precond_link (1112, 2223, 0.5, 0);
  net->add_effect_link (2223, 1113, 1);
*/

/*
  net->add_cond (1111, "C1", 1, 1, 0, 0, ENVIRON);
  net->add_cond (1112, "C2", 1, 1, 0, 0, ENVIRON);
  net->add_task (2221, "A1", 1, 1, 0.5);
  net->add_task (2222, "A2", 1, 3, 0.5);
  net->add_cond (1113, "C3", 1, 0, 1, 10, ENVIRON);
  net->add_precond_link (1111, 2221, 1, 0);
  net->add_precond_link (1112, 2222, 1, 0);
  net->add_effect_link (2221, 1113, 1);
  net->add_effect_link (2222, 1113, -1);
*/


  // Display initial configuration.
  std::cout << "Initial network configuration:" << std::endl;
  net->print (std::cout, true);

  // Print XML representation to output_init.xml.
  std::ofstream file_init ("output_init.xml");
  if (file_init == 0){
    std::string msg = "Unable to open output_init.xml for writing";
    throw msg;
  }
  net->print_xml (file_init);
  file_init.close ();

  // Get max steps from user.
  size_t max_steps = SANet::Default::SA_MAX_STEPS;
  UserInterface::QuestionInt steps_ques("Max steps to run (10000 step limit): ", 0, 10000);
  if (user_input.ask (steps_ques))
    max_steps = steps_ques.get_answer_int ();
  
  // Run spreading activation.
  net->update (max_steps);

  // Display final configuration.
  std::cout << "Final network configuration:" << std::endl;
  net->print (std::cout, true);

  // Print XML representation to output_final.xml.
  std::ofstream file_final ("output_final.xml");
  if (file_final == 0){
    std::string msg = "Unable to open output_final.xml for writing";
    throw msg;
  }
  net->print_xml (file_final);
  file_final.close ();

  delete net;

  // Wait for user to end program.
  UserInterface::Question end_ques ("Enter any character to end program:");
  user_input.ask (end_ques);

  return 0;
};
