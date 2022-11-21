/**************************************************************************//**
  \file     csv_parser.h
  \brief    CSV file parser header file.
  \author   Zijie Chou
  \version  V1.0.1
  \date     21. November 2022
 ******************************************************************************/

#ifndef EDA_CHALLENGE_PARSER_CSV_PARSER_H_
#define EDA_CHALLENGE_PARSER_CSV_PARSER_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>
#include "hopscotch_map.h"

/*!
  \struct CSVSignalStatisticStruct
  \brief  Structs for signal statistics
 */
struct CSVSignalStatisticStruct {
  int tc, t1, t0, tx, tg;          ///< The information cut from the csv file
  double sp;
};

/*!
  \struct CSV_VCDSignalStruct
  \brief  Structs for signal properties
 */
struct CSV_VCDSignalStruct {
  unsigned int vcd_signal_width;   ///< Signal bit width
  std::string vcd_signal_label;    ///< Label of the signal
  int declare_width_start = 0;     ///< Initial bits of the vector signal
};

/*!  \brief CSV parser class, which contains all the CSV file parsing functions. */
class CSVParser {
 public:

  /*!  \brief Constructor for the CSV parser class, which opens the CSV file. */
  explicit CSVParser(const std::string &filename) {
      fp_ = fopen64(filename.c_str(), "r");
      std::cout << "\nOpen file: " << filename << "\n";
  }

  /*!  \brief The destructor function of the CSV parser class, which clears all the hash tables to free memory. */
  ~CSVParser() {
      fclose(fp_);
      csv_signal_table_.clear();
      csv_vcd_signal_table_.clear();
      merge_csv_vcd_table_.clear();
  }

  void parse_csv();
  void get_vcd_scope(const std::string &vcd_filename);
  void csv_find_vcd();
  struct CSVSignalStatisticStruct *find_signal(const std::string &signal_label);
 private:
  FILE *fp_{};
  tsl::hopscotch_map<std::string, struct CSVSignalStatisticStruct> csv_signal_table_;
  tsl::hopscotch_map<std::string, struct CSV_VCDSignalStruct> csv_vcd_signal_table_;
  std::unordered_map<std::string, struct CSVSignalStatisticStruct> merge_csv_vcd_table_;
};

#endif //EDA_CHALLENGE_PARSER_CSV_PARSER_H_
