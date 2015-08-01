#include"CompareRecordApp.hh"

#include"RecordComparator.hh"
#include <cstdlib>
#include <sstream>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CompareRecordApp::CompareRecordApp(
  const std::vector<std::string> & aArguments)
  :
  _Arguments(aArguments),
  _OutputFileName("UnnamedOutput.txt"),
  _RecordDefinition(NULL)
{
  std::cout<<"app got constructed \n";
  GenerateOutputFileName();

  _RecordDefinition= new RecordDefinition(_Arguments[3]+"/"+
                                          _Arguments[2]+"Record.def");
  
  std::istringstream i(_Arguments[4]);
  i >> _NumMismatchesToDisplay;
  if (_NumMismatchesToDisplay == LONG_MAX) {
    std::cout<<"Showing ALL mismatches" << std::endl;
  } else {
    std::cout<<"Mismatch limit is " << _NumMismatchesToDisplay << std::endl;
  }
  Run();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CompareRecordApp::~CompareRecordApp()
{
  delete _RecordDefinition;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void 
CompareRecordApp::GenerateOutputFileName()
{
  _OutputFileName = _Arguments[2]+"_Comparison.txt";
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void 
CompareRecordApp::PrintFieldListing(
  std::ofstream & aOutputFile,
  const RecordDefinition::IntVector & aDiffCountVector)
{
  // declare some variables and print the table header while we're at it
  TableEntry tFieldIndexEntry("Field", 10);
  TableEntry tNameEntry("Name", 47);
  TableEntry tDiffCountEntry("Mismatched", 20);
  
  TableLineDef tLineDef;
  tLineDef.push_back(tFieldIndexEntry);
  tLineDef.push_back(tNameEntry);
  tLineDef.push_back(tDiffCountEntry);

  std::string tLineString(PrintTableLine(tLineDef));

  aOutputFile<<tLineString;
  
  RecordDefinition::FieldDefinitionVector::const_iterator tIterator(
    _RecordDefinition->GetBeginning());
  std::size_t tIndex = 0;

  // iterate through the fields and print them.
  while(tIterator != _RecordDefinition->GetEnd())
  {
    // field index
    tFieldIndexEntry.first = StringUtils::GetString(tIndex + 1);
    // field name
    tNameEntry.first = (*tIterator)->Name;
    // difference count
    if(!((*tIterator)->IsCritical))
    {
      tDiffCountEntry.first = "Always Ignored";
    }
    else if((*tIterator)->IsPartiallyCritical)
    {
      tDiffCountEntry.first = "Selectively Ignored";
    }
    else
    {
      std::size_t tDiffs = 0;
      if(aDiffCountVector.size() > tIndex)
      {
        tDiffs = aDiffCountVector[tIndex];
      }
      tDiffCountEntry.first = StringUtils::GetString(tDiffs);
      if ((*tIterator)->IsBlank1) 
      {
        tDiffCountEntry.first += " (blanks in set 1 ignored)";
      }
      if ((*tIterator)->IsBlank2) 
      {
        tDiffCountEntry.first += " (blanks in set 2 ignored)";
      }
    }

    tLineDef.clear();
    tLineDef.push_back(tFieldIndexEntry);
    tLineDef.push_back(tNameEntry);
    tLineDef.push_back(tDiffCountEntry);
    
    tLineString = PrintTableLine(tLineDef);

    aOutputFile<<tLineString;

    tIterator++;
    tIndex++;
  }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
CompareRecordApp::PrintKeys(std::ofstream & aOutputFile,
                            const std::vector<std::string> & aKeyVector)
{
  std::vector<std::string>::const_iterator tIterator = 
    aKeyVector.begin();
  
  while(tIterator != aKeyVector.end())
  {
    aOutputFile<<*tIterator<<std::endl;
    tIterator++;
  }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::string
CompareRecordApp::PrintTableLine(const TableLineDef & aLineDef) const
{
  std::string tLineString("");
  TableLineDef::const_iterator tIterator(aLineDef.begin());

  while(tIterator != aLineDef.end())
  {
    std::string tFieldString("");
    tFieldString += (*tIterator).first;
    while(tFieldString.length() < (*tIterator).second)
    {
      tFieldString += " ";
    }
    tLineString += tFieldString;
    tIterator++;
  }
  
  tLineString += "\n";
  
  return tLineString;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
CompareRecordApp::PrintComparisons(std::ofstream & aOutputFile,
                                   const FileReader & aFileReader0,
                                   const FileReader & aFileReader1)
{
  aOutputFile<<"\n\n \tCOMPARING BOTH DIRECTORIES' CONTENTS: \n\n";

  RecordComparator tComparator(aFileReader0, aFileReader1);

  std::size_t tNumberOfMatchedKeys;
  std::size_t tNumberOfUnmatchedKeysOld;
  std::size_t tNumberOfUnmatchedKeysNew;
  std::size_t tNumberOfDifferentRecords;
  std::size_t tNumberOfDuplicateKeys_v1,
              tNumberOfDuplicateKeys_v2;

  tNumberOfMatchedKeys      = tComparator.GetTotalNumberOfMatchedKeys();
  tNumberOfUnmatchedKeysOld = tComparator.GetTotalNumberOfUnmatchedKeysOldVersion();
  tNumberOfUnmatchedKeysNew = tComparator.GetTotalNumberOfUnmatchedKeysNewVersion();
  tNumberOfDifferentRecords = tComparator.GetTotalNumberOfDifferentRecords();

  tNumberOfDuplicateKeys_v1 = aFileReader0.GetNumberOfDuplicateKeys();
  tNumberOfDuplicateKeys_v2 = aFileReader1.GetNumberOfDuplicateKeys();

  const RecordDefinition::IntVector & tDifferingFieldCounts = 
                              tComparator.GetDifferingFieldCounts();
  
  // Output the basic information (everything seen above the listing of specific different or
  //    missing records).
  aOutputFile<< "Type: "<<_Arguments[2]<<std::endl<<std::endl;

  aOutputFile<< "Key field(s): " << _RecordDefinition->GetKeyDefinitionString()
	     << std::endl;
  aOutputFile<<"Total Number of Keys (version 1): "
             << tComparator.GetTotalNumberOfMatchedKeys() + tComparator.GetTotalNumberOfUnmatchedKeysOldVersion() << std::endl;
  aOutputFile<<"Total Number of Keys (version 2): "
             << tComparator.GetTotalNumberOfMatchedKeys() + tComparator.GetTotalNumberOfUnmatchedKeysNewVersion() << std::endl
	     << std::endl;

  aOutputFile<< "Number of Duplicate keys (version 1): "
             << tNumberOfDuplicateKeys_v1 << std::endl;
  aOutputFile<< "Number of Duplicate keys (version 2): "
             << tNumberOfDuplicateKeys_v2 << std::endl;
  aOutputFile<<"Total Number of Matched Keys: "
             << tNumberOfMatchedKeys << std::endl;
  aOutputFile<<"Number of Unmatched Keys (version 1): \t"
             << tNumberOfUnmatchedKeysOld << std::endl;
  aOutputFile<<"Number of Unmatched Keys (version 2): \t"
             << tNumberOfUnmatchedKeysNew << std::endl;
  
  aOutputFile<<"\n\nNumber of records same key w/ differences: "
             << tNumberOfDifferentRecords << std::endl;
  
  PrintFieldListing(aOutputFile, tDifferingFieldCounts);
 
  // If there are different records and the user wants them printed, start printing
  if(tNumberOfDifferentRecords > 0) {
    aOutputFile << std::endl << "Sample of differing records: " << std::endl << std::endl;

    std::size_t differingRecordCount = 0;
    for (int tableIndex=0; tableIndex < tComparator.GetNumberOfDataStructs(); ++tableIndex) {
        for (int tIndex=0; tIndex < tComparator.GetNumberOfDifferentRecords(tableIndex); ++tIndex) {
            aOutputFile << "Differing Record "<< differingRecordCount << std::endl;
            aOutputFile	<< "Key: " << tComparator.GetDifferingRecord(tIndex, tableIndex).first->GetKey() << std::endl << std::endl;
            aOutputFile << tComparator.GetRecordDifferences(tIndex);
            aOutputFile << std::endl << std::endl;
            ++differingRecordCount;
        }
    }
  }    

  if(tNumberOfUnmatchedKeysOld > 0) {
    aOutputFile << "\n\n Keys from version 1 that do not exist in version 2" << std::endl << std::endl;
  
    for (int tableIndex=0; tableIndex < tComparator.GetNumberOfDataStructs(); ++tableIndex) {
        const std::vector<std::string> & tUnmatchedKeys = tComparator.GetUnmatchedKeysOldVersion(tableIndex);
        PrintKeys(aOutputFile, tUnmatchedKeys);
    }
  }

  if(tNumberOfUnmatchedKeysNew > 0) {
    aOutputFile << "\n\n Keys from version 2 that do not exist in version 1" << std::endl << std::endl;
    
    for (int tableIndex=0; tableIndex < tComparator.GetNumberOfDataStructs(); ++tableIndex) {
        const std::vector<std::string> & tUnmatchedKeys = tComparator.GetUnmatchedKeysNewVersion(tableIndex);
        PrintKeys(aOutputFile, tUnmatchedKeys);
    }
  }

  if (tNumberOfDuplicateKeys_v1 > 0)
  {
    aOutputFile << "\n\n Duplicate unique keys from version 1" << std::endl << std::endl;

    aOutputFile << aFileReader0.DumpDuplicateKeys () << std::endl << std::endl;
  }

  if (tNumberOfDuplicateKeys_v2 > 0)
  {
    aOutputFile << "\n\n Duplicate unique keys from version 2" << std::endl << std::endl;

    aOutputFile << aFileReader1.DumpDuplicateKeys () << std::endl << std::endl;
  }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
CompareRecordApp::Run()
{
  bool bValidComparison = true;
  time_t tTime;

  // Errors message go in here
  std::string tErrorString("");
  // the output will go to this file
  std::ofstream tOutputFile;
  tOutputFile.open (_OutputFileName.c_str());
  
  // file readers to process the input files
  FileReader  * tFileReader [2];
  tFileReader[0] = new FileReader(*_RecordDefinition);
  tFileReader[1] = new FileReader(*_RecordDefinition);

  time(&tTime);
  std::cout<<"starting to read files at: "<< ctime(&tTime)  <<std::endl;

  for(int i = 1; i <=2 && bValidComparison; i++)
  {
    // let's convert the cstring to strings asap
    std::string tDirName(_Arguments[i-1]);
    if(tDirName[tDirName.length()-1] != '/')
    {
      tDirName += "/";
    }
    
    // we need two directories
    DIR * tDirectory;
    
    tDirectory  = opendir(tDirName.c_str());
    
    if(tDirectory)
    {
      struct dirent * tDirEntry;
      
      while(tDirEntry = readdir(tDirectory))
      {
        if((std::string(tDirEntry->d_name) != ".") &&
           (std::string(tDirEntry->d_name) != ".."))
        {
          std::string tCurrentName = tDirName + tDirEntry->d_name;
          std::ifstream tInputFileStream;
//          std::filebuf * tFileBuffer;
//          tFileBuffer = tInputFileStream.rdbuf();
          
          tInputFileStream.open(tCurrentName.c_str(), std::ios::in);
          
          if(tInputFileStream.good())
          {
            tFileReader[i - 1]->CreateRecords(tInputFileStream);
            tInputFileStream.close();
          }    
        }
      }
      if (tFileReader[i-1]->GetNumberOfBytesRead()  == 0){
        tErrorString = "It looks like directory '" + tDirName +
          "' does not have record files to compare. Exiting.";
        bValidComparison = false;
        continue;
      }
      std::cout<<std::endl;
    }
    else
    {
      tErrorString = "Directory was invalid: "+tDirName;
      bValidComparison = false;
    }
  }

  if(bValidComparison){
    std::cout<<"FileReaders done.  1 read: "
      <<tFileReader[0]->GetNumberOfBytesRead() 
      <<" 2 read: "
      << tFileReader[1]->GetNumberOfBytesRead()
      << " total: "
      << (tFileReader[0]->GetNumberOfBytesRead() +
          tFileReader[1]->GetNumberOfBytesRead())
      <<std::endl;

    time(&tTime);
    std::cout<<"Started comparison at: "  << ctime(&tTime) <<std::endl;

    PrintComparisons(tOutputFile,
        *tFileReader[0],
        *tFileReader[1]);

    time(&tTime);
    std::cout<<"\nfinished comparison at "<< ctime(&tTime) <<std::endl;

  } 

  delete tFileReader[0];
  delete tFileReader[1];
  
  if(tErrorString != "")
  {
    tOutputFile  << tErrorString << std::endl;
    std::cout << tErrorString << std::endl;
  }
  
// close output file
  tOutputFile.close();
}

