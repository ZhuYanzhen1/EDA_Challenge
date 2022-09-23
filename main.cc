/**************************************************************************//**
  \file     main.cc
  \brief    This source file is the program entry file.
  \author   Lao·Zhu
  \version  V1.0.1
  \date     20. September 2022
 ******************************************************************************/

#ifdef IS_NOT_RUNNING_GOOGLE_TEST
#include <iostream>
#include "system.h"
#include "gui.h"
#include "tclap/CmdLine.h"
#else
#include "gtest/gtest.h"
#endif

/*!
    \brief      Program entry
    \param[in]  argc: Number of command-line arguments sent to the main function at program runtime
    \param[in]  argv: Store an array of pointers to string parameters
    \return     Program execution results
*/
int main(int argc, char **argv) {
#ifdef IS_NOT_RUNNING_GOOGLE_TEST
    try {
        TCLAP::CmdLine cmd("This software is VCD file parsing and statistics software, optimized for large files."
                           " You can visit https://github.com/ZhuYanzhen1/EDA_Challenge to get more information about this software.",
                           ' ', "1.0.1");

        /* Define value arguments and add it to the command line */
        TCLAP::ValueArg<std::string> filename_arg("f", "file", "The vcd file to be parsed",
                                                  true, "./test.vcd", "string");
        TCLAP::SwitchArg using_gui_switch("g", "gui", "Whether to display the gui interface",
                                          cmd, false);
        cmd.add(filename_arg);

        /* Parse the argv array */
        cmd.parse(argc, argv);

        /* Get the value parsed by each arg */
        std::string filepath = filename_arg.getValue();
        bool using_gui_flag = using_gui_switch.getValue();

        if (using_gui_flag) {
            std::cout << "Using gui with file: " << filepath << "\n";
            return gui_display_main_window(1, argv);
        } else {
            std::cout << "No gui with file: " << filepath << "\n";
        }
    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }
    return 0;
#else
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#endif
}
