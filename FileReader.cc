#include "FileReader.hh"
#include "FixedRecord.hh"
#include "IPTSException.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
FileReader::FileReader(const RecordDefinition & aRecordDefinition)
  :
  _RecordDefinition(aRecordDefinition),
  _DuplicateKeys(0), 
  _BytesRead(0)
{
    _DupKeysVector.clear();
    int aIndex = 0;
    int numFields = aRecordDefinition.GetNumberOfFields();
    while (aIndex < numFields && !aRecordDefinition.IsHashkey(aIndex)) {
        ++aIndex;
    }
    
    FixedRecord::RecordMap tempMap;
    
    if (aIndex == numFields) {
        _RecordMapVector.push_back(tempMap);
        _NumberOfRecordMaps = 1;
    }
    else {
        _NumberOfRecordMaps = aRecordDefinition.GetHashkeyValue(aIndex);
        for (int i=0; i < _NumberOfRecordMaps; ++i) {
            // push_back pushes a copy of the object onto the vector
            _RecordMapVector.push_back(tempMap);
        }
    }        

    _NumberOfRecordMaps = 1;

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
FileReader::~FileReader()
{
    for (size_t i=0; i<_RecordMapVector.size(); ++i) {
        _RecordMapVector[i].clear();
    }
    _RecordMapVector.clear();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
FileReader::CreateRecords(std::ifstream & aInputFileStream)
{
    const int tBuffSize = 2048;
    char tCharBuffer [tBuffSize];
    std::string tRecordString;

    while(!aInputFileStream.eof() && aInputFileStream.good()) {
        aInputFileStream.getline(tCharBuffer, tBuffSize);
        tCharBuffer[aInputFileStream.gcount()] = '\0';
        tRecordString = tCharBuffer;

        if(tRecordString.length() >= _RecordDefinition.GetMinimumRecordLength()) {
            FixedRecord * tNewRecord = new FixedRecord(_RecordDefinition, tRecordString);
            _BytesRead += tNewRecord->GetTextSize();
            const std::string * tKey = &(tNewRecord->GetKey());

            int rmvIndex = 0;
            // Only hash if necessary
            if(_NumberOfRecordMaps > 1) {
                rmvIndex = GetHashResult(tKey->c_str());
            }
            
            if(_RecordMapVector[rmvIndex].find(tKey) != _RecordMapVector[rmvIndex].end()) {
                ++_DuplicateKeys;

                UpdateDupKeyMap (*tKey);

                delete(tNewRecord);
            }
            else {
                _RecordMapVector[rmvIndex][tKey] = tNewRecord;
                ++_NumberOfRecords;
                if((_NumberOfRecords % 1000) == 0) {
                    std::cout << "\rHave read " << _NumberOfRecords << " records.";
                    std::cout.flush();
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::size_t
FileReader::GetHashResult(const char* tKey) const {
    // Get the value we'll be basing the hash on
    int hashValue=0;
    int tKeyLength = strlen(tKey);
	int spacing = tKeyLength/6;  	// So that we will have 6 characters to go off of
	if (spacing == 0) {spacing=1;}	// In case there are less than 6 characters
	
    for (int i=0; i<tKeyLength; i+=spacing) {
        hashValue += *(tKey+i);
    }
    
    // If our number of RecordMaps is a power of two, use a bitmask to hash.  It's fast.
    if ( !(_NumberOfRecordMaps & _NumberOfRecordMaps-1) ) {
        return (hashValue & _NumberOfRecordMaps-1);
    }
    else {
        return (hashValue % _NumberOfRecordMaps-1);
    }    
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::size_t
FileReader::GetNumberOfBytesRead() const
{
  return _BytesRead;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::size_t
FileReader::GetNumberOfDuplicateKeys() const
{
  return _DuplicateKeys;
}

std::string
FileReader::DumpDuplicateKeys () const
{
  std::string strToBeReturned = "";
  std::map<std::string,int>::const_iterator iter_DuplicateMap = _DuplicateMap.begin ();

  while (iter_DuplicateMap != _DuplicateMap.end ())
  {
    strToBeReturned += iter_DuplicateMap->first;
    strToBeReturned += "\n";
    ++ iter_DuplicateMap;
  }

  return strToBeReturned;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::size_t
FileReader::GetNumberOfRecords() const
{
  return _NumberOfRecords;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::size_t
FileReader::GetNumberOfRecordMaps() const
{
  return _NumberOfRecordMaps;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
const FixedRecord::RecordMap & 
FileReader::GetRecordMap(int i=0) const
{
  return _RecordMapVector[i];
}

void
FileReader::UpdateDupKeyMap (std::string strKey)
{
  std::map<std::string,int>::iterator iter_DuplicateMap = _DuplicateMap.find (strKey);

  if (iter_DuplicateMap != _DuplicateMap.end ())
  {
    _DuplicateMap[strKey] ++;
  }
  else
  {
    _DuplicateMap[strKey] = 1;
  }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
FileReader::CreateTDRRecords(std::ifstream & aInputFileStream)
{
    const int tBuffSize = 2048;
    char tCharBuffer [tBuffSize];
    std::string tRecordString;

    while(!aInputFileStream.eof() && aInputFileStream.good()) {
        aInputFileStream.getline(tCharBuffer, tBuffSize);
        tCharBuffer[aInputFileStream.gcount()] = '\0';
        tRecordString = tCharBuffer;
        if(tRecordString.length() >= _RecordDefinition.GetMinimumRecordLength()) {
            FixedRecord * tNewRecord = new FixedRecord(_RecordDefinition, tRecordString);
            _BytesRead += tNewRecord->GetTextSize();
            const std::string * tKey = &(tNewRecord->GetKey());

            int rmvIndex = 0;
            // Only hash if necessary
            if(_NumberOfRecordMaps > 1) {
                rmvIndex = GetHashResult(tKey->c_str());
            }
            
            if(_RecordMapVector[rmvIndex].find(tKey) != _RecordMapVector[rmvIndex].end()) {
                ++_DuplicateKeys;

                delete(tNewRecord);
            }
            else {
                _RecordMapVector[rmvIndex][tKey] = tNewRecord;
                ++_NumberOfRecords;
                if((_NumberOfRecords % 1000) == 0) {
                    std::cout << "\rHave read " << _NumberOfRecords << " records.";
                    std::cout.flush();
                }
            }
        }
    }
}
