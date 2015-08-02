#ifndef RecordBase_h
#define RecordBase_h

#include"RecordDefinition.hh"

#include <vector>
#include <deque>
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
#include <stdlib.h>
#include <string>
#include <set>

/**
 * The base class for records.  Takes the record string and definition, and 
 * stores it. Defines some basic types.
 */
class RecordBase
{
 public:
  
  //---------------------------------------------------------------------------
  /**
   * @param aRecordString a string, from which to construct this record
   */
  //---------------------------------------------------------------------------
  RecordBase(const RecordDefinition & aRecordDefinition); 

  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  virtual 
  ~RecordBase();

  //---------------------------------------------------------------------------
  /**
   * @return  This function returns the human-readable representation of the 
   * record as a string.
   */
  //---------------------------------------------------------------------------
  virtual std::string 
  DumpRecord() const;

  //---------------------------------------------------------------------------
  /**
   * This function writes the record's internal string representation to the 
   * stream.
   * @param a file input stream to write to
   */
  //---------------------------------------------------------------------------
  virtual void
  DumpRecordToStream(std::ostream & aOutputStream) const;

  //---------------------------------------------------------------------------
  /**
   * @param the name of the field you're looking for
   * @return the Field by the given name, if available, else empty string 
   *         constant "" but not in this class.
   */
  //---------------------------------------------------------------------------
  virtual std::string 
  GetFieldText(const std::string & aFieldName) const = 0;
  
  //---------------------------------------------------------------------------
  /**
   * @return: The key for this record.Has to be implemented to return a 
   *          string made from a set of fields that is unique for all records.
   */
  //------------------------------------------------------------------------
  const std::string & 
  GetKey() const;

  //---------------------------------------------------------------------------
  /**
   * Returns the length of the record
   */
  //---------------------------------------------------------------------------
  virtual std::size_t
  GetTextSize() const;

protected:

  /// how many fields are in the record definition
  unsigned short  _NumberOfFields;
  /// does the record have a delimiter
  bool        _IsDelimitedRecord;

  /// record definition reference
  const RecordDefinition & _RecordDefinition;
  /// the key
  std::string _Key;
  /// the record string
  std::string _RecordString;

  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  void
  FillFieldArray(const std::string & aRecordString);

  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  virtual void
  FillFieldArrayDelimited(const std::string & aRecordString) = 0;

  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  virtual void
  FillFieldArrayUndelimited(const std::string & aRecordString) = 0;

  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  virtual void
  GenerateKey() = 0;

  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  void
  ReadRecord(const std::string & aRecordString);

private:

};    
#endif
