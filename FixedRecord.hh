#ifndef FixedRecord_h
#define FixedRecord_h

#include "RecordDefinition.hh"
#include "RecordBase.hh"

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
 * The base class for records.  It takes a string and parses itself out of it.
 */
class FixedRecord: public RecordBase
{
 public:
  typedef struct
  {
    bool operator()(const std::string * s1, const std::string * s2) const
      {
	return strcmp(s1->c_str(), s2->c_str()) < 0;
      }
  }CompareKeys;
  typedef std::map<const std::string *, 
		   const FixedRecord *, 
		   CompareKeys> RecordMap;
  typedef std::set<const std::string *,
		   CompareKeys> KeyMap;

  /// a data type to use for storing records that should match, but don't
  typedef std::pair<const FixedRecord *, const FixedRecord *> RecordPair;
  
  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  FixedRecord(const RecordDefinition & aRecordDefinition);

  //---------------------------------------------------------------------------
  /**
   * @param aRecordString a string, from which to construct this record
   */
  //---------------------------------------------------------------------------
  FixedRecord(const RecordDefinition & aRecordDefinition,
	      const std::string & aRecordString); 

  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  virtual ~FixedRecord();

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
   */
  //---------------------------------------------------------------------------
  std::string 
  GetCompareRecordString(const FixedRecord & aFixedRecord) const;

  //---------------------------------------------------------------------------
  /**
   */
  //------------------------------------------------------------------------
  RecordDefinition::IntVector
  GetDifferingFields(const FixedRecord & aFixedRecord) const;

  //---------------------------------------------------------------------------
  /**
   */
  //------------------------------------------------------------------------
  std::string
  GetFieldText(const std::string & aFieldName) const;

  //---------------------------------------------------------------------------
  /**
   */
  //------------------------------------------------------------------------
  virtual void
  ReadRecord(const std::string & aRecordString);

  //---------------------------------------------------------------------------
  /**
   * Override the equals operator for easier record comparison.
   */
  //---------------------------------------------------------------------------
  bool
  operator == (const FixedRecord & aFixedRecord) const;

  //---------------------------------------------------------------------------
  /**
   * Override the unequal operator for easier record comparison.
   */
  //---------------------------------------------------------------------------
  bool 
  operator != (const FixedRecord & aFixedRecord) const;
  
protected:

  typedef struct 
  {
    unsigned short _Size;
    unsigned short  _Position;
  } FieldBase;

  // map of actual data fields
  FieldBase   *_Fields;

 private:

  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  void
  AllocateFields();

  /// TODO make these one function that just use a different mechanism to 
  /// break up the string
  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  virtual void
  FillFieldArrayDelimited(const std::string & aRecordString);

  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  virtual void
  FillFieldArrayUndelimited(const std::string & aRecordString);

  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  void
  GenerateKey();

  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  std::string
  GetRecordLine(std::size_t aIndex) const;

};    
#endif
