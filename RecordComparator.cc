#include "RecordComparator.hh"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
RecordComparator::RecordComparator(const FileReader & aFileReader1,
                                         const FileReader & aFileReader2)
  :
  _FileReader1(aFileReader1),
  _FileReader2(aFileReader2)
{
    _ComparatorData cdTemp;
    for(int tableIndex=0; tableIndex < _FileReader1.GetNumberOfRecordMaps(); ++tableIndex) {
        _ComparatorDataVector.push_back(cdTemp);
    }
    
    FindInconsistencies();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
RecordComparator::~RecordComparator()
{
  for(int tableIndex=0; tableIndex < _ComparatorDataVector.size(); ++tableIndex) {
    while(std::size_t tIndex = _ComparatorDataVector[tableIndex]._DifferentRecords.size()) {
        FixedRecord::RecordPair * tPair = _ComparatorDataVector[tableIndex]._DifferentRecords[tIndex - 1];
        _ComparatorDataVector[tableIndex]._DifferentRecords.pop_back();
        delete tPair;
    }
  }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
const RecordDefinition::IntVector &
RecordComparator::GetDifferingFieldCounts() const
{
  return _DifferingFieldCounts;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
FixedRecord::RecordPair
RecordComparator::GetDifferingRecord(std::size_t aIndex, int tableIndex) const
{
  return *(_ComparatorDataVector[tableIndex]._DifferentRecords[aIndex]);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::size_t
RecordComparator::GetNumberOfDifferentRecords(int tableIndex) const
{
  return _ComparatorDataVector[tableIndex]._DifferentRecords.size();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::size_t
RecordComparator::GetNumberOfMatchedKeys(int tableIndex) const
{
  return _ComparatorDataVector[tableIndex].numberOfMatchedKeys;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::size_t
RecordComparator::GetNumberOfUnmatchedKeysNewVersion(int tableIndex) const
{
  return _ComparatorDataVector[tableIndex]._NewRecordsUnmatchedKeys.size();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::size_t
RecordComparator::GetNumberOfUnmatchedKeysOldVersion(int tableIndex) const
{
  return _ComparatorDataVector[tableIndex]._OldRecordsUnmatchedKeys.size();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::string
RecordComparator::GetRecordDifferences(std::size_t aIndex, int tableIndex) const
{
  std::string tString("");

  if(aIndex < _ComparatorDataVector[tableIndex]._DifferentRecords.size())
  {
    const FixedRecord & tRecord1 = *(_ComparatorDataVector[tableIndex]._DifferentRecords[aIndex]->first);
    const FixedRecord & tRecord2 = *(_ComparatorDataVector[tableIndex]._DifferentRecords[aIndex]->second);

    tString = tRecord1.GetCompareRecordString(tRecord2);
  }
  
  return tString;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
const std::vector<std::string> &
RecordComparator::GetUnmatchedKeysNewVersion(int tableIndex) const {
    return _ComparatorDataVector[tableIndex]._NewRecordsUnmatchedKeys;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
const std::vector<std::string> &
RecordComparator::GetUnmatchedKeysOldVersion(int tableIndex) const {
    return _ComparatorDataVector[tableIndex]._OldRecordsUnmatchedKeys;
}

std::size_t
RecordComparator::GetNumberOfDataStructs() {
    return _ComparatorDataVector.size();
}

std::size_t
RecordComparator::GetTotalNumberOfMatchedKeys() {
    std::size_t returnValue = 0;
    for(int tableIndex=0; tableIndex < _ComparatorDataVector.size(); ++tableIndex) {
        returnValue += GetNumberOfMatchedKeys(tableIndex);
    }
    return returnValue;
}

std::size_t
RecordComparator::GetTotalNumberOfUnmatchedKeysOldVersion() {
    std::size_t returnValue = 0;
    for(int tableIndex=0; tableIndex < _ComparatorDataVector.size(); ++tableIndex) {
        returnValue += GetNumberOfUnmatchedKeysOldVersion(tableIndex);
    }
    return returnValue;
}

std::size_t
RecordComparator::GetTotalNumberOfUnmatchedKeysNewVersion() {
    std::size_t returnValue = 0;
    for(int tableIndex=0; tableIndex < _ComparatorDataVector.size(); ++tableIndex) {
        returnValue += GetNumberOfUnmatchedKeysNewVersion(tableIndex);
    }
    return returnValue;
}

std::size_t
RecordComparator::GetTotalNumberOfDifferentRecords() {
    std::size_t returnValue = 0;
    for(int tableIndex=0; tableIndex < _ComparatorDataVector.size(); ++tableIndex) {
        returnValue += GetNumberOfDifferentRecords(tableIndex);
    }
    return returnValue;
}

const RecordDefinition::IntVector &
RecordComparator::GetDifferingFieldCounts() {
    return _DifferingFieldCounts;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
RecordComparator::FindInconsistencies() { 
    for(int tableIndex=0; tableIndex<_FileReader1.GetNumberOfRecordMaps(); ++tableIndex) {
        const FixedRecord::RecordMap & tMap1 = _FileReader1.GetRecordMap(tableIndex);
        const FixedRecord::RecordMap & tMap2 = _FileReader2.GetRecordMap(tableIndex);
        
        std::cout   << "Processing table " << tableIndex << "..." << std::endl;
        // true = populate the matched keys deque
        FindInconsistentKeys(_ComparatorDataVector[tableIndex]._OldRecordsUnmatchedKeys, tMap1, tMap2, tableIndex, true);
        std::cout   << "Finished processing table " << tableIndex << "'s first set of keys at "
                    << time(NULL) << std::endl;
        // false = do not populate the matched keys deque.  We already did.
        FindInconsistentKeys(_ComparatorDataVector[tableIndex]._NewRecordsUnmatchedKeys, tMap2, tMap1, tableIndex, false);
        std::cout   << "Finished processing table " << tableIndex << "'s second set of keys at "
                    << time(NULL) << std::endl;
  
        FindDifferingRecords(tMap1, tMap2, tableIndex);
        
        _ComparatorDataVector[tableIndex].numberOfMatchedKeys = _ComparatorDataVector[tableIndex]._MatchedKeys.size();
        _ComparatorDataVector[tableIndex]._MatchedKeys.clear();
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
RecordComparator::FindDifferingRecords(
    const FixedRecord::RecordMap & aOldRecords,
    const FixedRecord::RecordMap & aNewRecords,
    int   tableIndex)
{
    FixedRecord::RecordMap::const_iterator tMap1Iterator;
    FixedRecord::RecordMap::const_iterator tMap2Iterator;
    int tNumberOfRecords = 0;

    std::deque<const std::string *>::iterator tMatchedKeysIterator;

    for(tMatchedKeysIterator = _ComparatorDataVector[tableIndex]._MatchedKeys.begin(); 
        tMatchedKeysIterator != _ComparatorDataVector[tableIndex]._MatchedKeys.end();
        tMatchedKeysIterator++)
    {
        // Find a given key in both maps
        tMap1Iterator = aOldRecords.find(*tMatchedKeysIterator);
        tMap2Iterator = aNewRecords.find(*tMatchedKeysIterator);

        // If the values are different...
        if(*(tMap1Iterator->second) != *(tMap2Iterator->second)) {
            // Add the differing records to _DifferentRecords
            _ComparatorDataVector[tableIndex]._DifferentRecords.push_back(
                new FixedRecord::RecordPair(tMap1Iterator->second, tMap2Iterator->second)
            );

            RecordDefinition::IntVector tDifferingFields = 
                tMap1Iterator->second->GetDifferingFields(*(tMap2Iterator->second));

            for(int i = 0; i < tDifferingFields.size(); i++) {
                // This while-loop initializes the _DifferingFieldCounts structure
                while(tDifferingFields[i] >= _DifferingFieldCounts.size()) {
                    _DifferingFieldCounts.push_back(0);
                }
                
                // Adding up how many times a field is different (for later display)
                _DifferingFieldCounts[tDifferingFields[i]]++;
            }
        }
        tNumberOfRecords++;
        if((tNumberOfRecords % 1000) == 0) {
            std::cout<<"\rHave compared "<<tNumberOfRecords<<" records in table " << tableIndex;
            std::cout.flush();
        }
    }
    std::cout << std::endl;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
RecordComparator::FindInconsistentKeys(
  std::vector<std::string> & aKeyVector,
  const FixedRecord::RecordMap & aRecordMap1,
  const FixedRecord::RecordMap & aRecordMap2,
  int   tableIndex,
  bool  populateMatchedKeys)
{
  FixedRecord::RecordMap::const_iterator tMap1Iterator;
  FixedRecord::RecordMap::const_iterator tMap1End = aRecordMap1.end();
  FixedRecord::RecordMap::const_iterator tMap2Iterator;
  FixedRecord::RecordMap::const_iterator tMap2End = aRecordMap2.end();
  std::deque<const std::string*> *matchedKeys = &_ComparatorDataVector[tableIndex]._MatchedKeys;

  tMap1Iterator = aRecordMap1.begin();
  
  int numRecords = 0;
  std::cout << aRecordMap1.size() << " records to process." << std::endl;

  while(tMap1Iterator != tMap1End)
  {
    if ((numRecords % 1000) == 0) {
        std::cout << "\rProcessed " << numRecords << " entries...";
        std::cout.flush();
    }
	tMap2Iterator = aRecordMap2.find(tMap1Iterator->first);
    if(tMap2Iterator == tMap2End)	// Unmatched key
    {
      aKeyVector.push_back(*(tMap1Iterator->first));
    }
    else if (populateMatchedKeys) // Matched key
    {
      matchedKeys->push_back(tMap1Iterator->first);
    }
    tMap1Iterator++;
    ++numRecords;
  }
  std::cout << std::endl;
}
