#ifndef FileReader_h
#define FileReader_h

#include "FixedRecord.hh"
#include "RecordDefinition.hh"

#include<iostream>
#include<fstream>
#include<map>
#include<vector>

/**
 * This class takes a record type as a template parameter.  It can be used
 * to read and parse files that are expected to contain that record type an
 * aggregate them.
 */
class FileReader
{
 public:

  //Constructors
  FileReader(const RecordDefinition & aRecordDefinition);

  //---------------------------------------------------------------------------
  /**
   * Removes the records from memory.
   */
  //---------------------------------------------------------------------------
  ~FileReader();

  //---------------------------------------------------------------------------
  /**
   * @param aFileBuffer: This file is read until the end and all the records in
   * it are added to the _RecordMap.  Can be used to read any number of files.
   */
  //---------------------------------------------------------------------------
  void CreateRecords(std::ifstream & aInputFileStream);

  /**
   * @param aFileBuffer: This file is read until the end and all the records in
   * it are added to the _RecordMap.  Can be used to read any number of files.
   */
  //---------------------------------------------------------------------------
  void CreateTDRRecords(std::ifstream & aInputFileStream);
  
  /**
     * @param aFileBuffer: This file is read until the end and all the records in
     * it are added to the _RecordMap.  Can be used to read any number of files.
     */
    //---------------------------------------------------------------------------
  void CreateMCDRRecords(std::ifstream & aInputFileStream);

  // Accessors
  std::size_t GetNumberOfBytesRead() const;
  std::size_t GetNumberOfDuplicateKeys() const;
  std::size_t GetNumberOfRecords() const;
  std::size_t GetNumberOfRecordMaps() const; 
  std::size_t GetHashResult(const char*) const;

  const FixedRecord::RecordMap & GetRecordMap(int) const;

  std::vector<std::string> GetDupKeys() const {return _DupKeysVector;};

  void UpdateDupKeyMap (std::string);
  std::string DumpDuplicateKeys () const;
    
 private:

    const RecordDefinition &                _RecordDefinition;
    std::vector<FixedRecord::RecordMap>     _RecordMapVector;
    std::vector<std::string>                _DupKeysVector;
    int                                     _DuplicateKeys;
    int                                     _NumberOfRecords;
    int                                     _NumberOfRecordMaps;
    std::size_t                             _BytesRead;
    std::map<std::string,int>               _DuplicateMap;
};

#endif
