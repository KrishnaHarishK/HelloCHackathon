#ifndef CompareRecordApp_h
#define CompareRecordApp_h

#include <iostream>
#include <fstream>
#include <cstring>
#include <csignal>
#include <map>
#include <vector>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <exception>
#include <errno.h>
#include <dirent.h>
#include <ctime>

#include "glib.h"

void GenerateCSV (char* const);

uint32_t gl_KeyCount_First   = 0,
         gl_KeyCount_Second  = 0,
         gl_DupCount_First   = 0,
         gl_DupCount_Second  = 0,
         gl_MatchedCount     = 0,
         gl_UnMatched_First  = 0,
         gl_UnMatched_Second = 0,
         gl_MatchedWDiff     = 0;

#include "FileReader.hh"
#include "FixedRecord.hh"
#include "RecordComparator.hh"
#include "RecordDefinition.hh"

#define COMPARE_APP_STANDARD_DISPLAY_COUNT 10

void GenerateCSV (char* const);
bool isFile (char* const);
bool isDir  (char* const);

/**
 * This is an application that compares the contents of two directories, based
 * on a couple of input parameters.  It parses the contents into maps of data
 * records and then compares them.
 */
class CompareRecordApp
{
 public:
  //---------------------------------------------------------------------------
  /**
   * This constructor actually executes all of the functionality of the 
   * Comparison.  When it returns the output has already been written. 
   * (calls Run()).
   * @param aArguments: The arguments list, repackaged as a string vector
   */
  //---------------------------------------------------------------------------
  CompareRecordApp(const std::vector<std::string> & aArguments);

  virtual ~CompareRecordApp();
  

 private:
  /// the arguments list, repackaged as a string vector
  const std::vector<std::string> & _Arguments;
  /// the name of the output file
  std::string _OutputFileName;
  /// definition of GBR record
  RecordDefinition * _RecordDefinition;
  long _NumMismatchesToDisplay;

  typedef std::pair<std::string, std::size_t> TableEntry;
  typedef std::vector<TableEntry> TableLineDef;

  //---------------------------------------------------------------------------
  /**
   * Uses the argument vector to determine what the output file should be 
   * called (fills _OutputFileName).  TODO: implemented addition of timestamp 
   * to the file name.
   */
  //---------------------------------------------------------------------------
  void GenerateOutputFileName();

  //---------------------------------------------------------------------------
  /**
   * Called after the comparisons have been performed.  Prints the results 
   * to file (_OutputFileName)
   * @param aOutputFile: the output file to be written to
   * @param aFileReader0: The FileReader object which has scanned the 
   * the first directory for FixedRecord Records.
   * @param aFileReader1: the FileReader object which has scanned the 
   * the second directory for FixedRecord Records.
   */
  //---------------------------------------------------------------------------
  void PrintComparisons(std::ofstream & aOutputFile,
			const FileReader & aFileReader0,
			const FileReader & aFileReader1);

  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  void PrintFieldListing(std::ofstream & aOutputFile,
			 const RecordDefinition::IntVector & aDiffCountVector);

  //---------------------------------------------------------------------------
  /**
   * @param aOutputFile: The file to be written to.
   * @param aKeyVector: aVector of keys to be written to aOutputFile
   */
  //---------------------------------------------------------------------------
  void PrintKeys(std::ofstream & aOutputFile,
                 const std::vector<std::string> & aKeyVector);
  
  //---------------------------------------------------------------------------
  /**
   *
   */
  //---------------------------------------------------------------------------
  std::string PrintTableLine(const TableLineDef & aLineDef) const;

  //---------------------------------------------------------------------------
  /**
   * Runs the application (opens files, hands them to file readers, launches
   * PrintComparisons)
   */
  //---------------------------------------------------------------------------
  void Run();  
};

#endif
