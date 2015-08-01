#include "FixedRecord.hh"

#include "StringUtils.hh"

#include<iostream>

/******************************************************************************
 **************************** PUBLIC METHODS **********************************
 *****************************************************************************/

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
FixedRecord::FixedRecord(const RecordDefinition & aRecordDefinition)
  :
  RecordBase(aRecordDefinition),
  _Fields(NULL)
{
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
FixedRecord::FixedRecord(const RecordDefinition & aRecordDefinition,
			 const std::string & aRecordString)
  :
  RecordBase(aRecordDefinition),
  _Fields(NULL)
{
  ReadRecord(aRecordString);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
FixedRecord::~FixedRecord()
{
  free(_Fields);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::string
FixedRecord::DumpRecord() const
{
  std::string tDumpingString = "";
  
  if(_Fields != NULL)
  {
    for(int i = 0; i < _NumberOfFields; i++)
    {
      tDumpingString += GetRecordLine(i);
    }
  }
  return tDumpingString;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::string
FixedRecord::GetCompareRecordString(const FixedRecord & aRecord) const
{ 
  std::string tCompareString("");
  if(_Fields != NULL)
  {
    RecordDefinition::IntVector tDifferingFields(GetDifferingFields(aRecord));
    
    for(std::size_t i = 0 ; i < tDifferingFields.size(); i++)
    {
      tCompareString += GetRecordLine(tDifferingFields[i]);
    }
    
    tCompareString += "\n\n";
    
    for(std::size_t j = 0; j < tDifferingFields.size(); j++)
    {
      tCompareString += aRecord.GetRecordLine(tDifferingFields[j]);
    }
  }
  
  return tCompareString;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
RecordDefinition::IntVector
FixedRecord::GetDifferingFields(const FixedRecord & aRecord) const
{
  RecordDefinition::IntVector tDifferingIndices;
  
  if(*this != aRecord && _Fields != NULL)
  {
    for(std::size_t i = 0; i < _NumberOfFields; i++)
    {
      std::string tOwnField = _RecordString.substr(_Fields[i]._Position,
          _Fields[i]._Size);
      std::string tOtherField = 
        aRecord._RecordString.substr(aRecord._Fields[i]._Position,
            aRecord._Fields[i]._Size);

      StringUtils::ValidatedString tVString(
          _RecordDefinition.IsFieldPartiallyCritical(i));
      // wow, this has gotten seriously out of hand :)
      if((tOwnField == tOtherField) ||
        (_RecordDefinition.IsCritical(i) == false) ||
        (_RecordDefinition.IsBlank1(i) && tOwnField.empty()) ||
        (_RecordDefinition.IsBlank2(i) && tOtherField.empty()) || 
          (tVString.first &&
           ((tOwnField.find(tVString.second) != std::string::npos) ||
            (tOtherField.find(tVString.second) != std::string::npos))))
      {
        continue;
      }
      else
      {
        tDifferingIndices.push_back(i);
      }
    }
  }
  
  return tDifferingIndices;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::string
FixedRecord::GetFieldText(const std::string & aFieldName) const
{
  int tIndex = _RecordDefinition.GetFieldIndex(aFieldName);

  if(tIndex >= 0 && _Fields != NULL)
  {
    return _RecordString.substr(_Fields[tIndex]._Position,
				_Fields[tIndex]._Size);
  }

  return "";
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
FixedRecord::ReadRecord(const std::string & aRecordString)
{
  if(_Fields != NULL)
  {
    free(_Fields);
  }

  AllocateFields();

  RecordBase::ReadRecord(aRecordString);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool
FixedRecord::operator == (const FixedRecord & aRecord) const
{
  bool tReturnValue = true;

  if(_Fields != NULL)
  {
    if(aRecord._RecordString != _RecordString)
    {
      for(std::size_t i = 0; i < _NumberOfFields; i++)
      {
        // If the field is flagged as 'false' (or some other not critical thing), skip
        if(!(_RecordDefinition.IsCritical(i)))
        {
          continue;
        }
        
        // Get the contents of our own field
        std::string tOwnField = _RecordString.substr(_Fields[i]._Position,
            _Fields[i]._Size);
        // If blank1 is flagged and the field is blank, skip
        if (_RecordDefinition.IsBlank1(i) && tOwnField.empty()) {
          continue;
        }    
            
        // Get the contents of the other field
        std::string tOtherField = 
          aRecord._RecordString.substr(aRecord._Fields[i]._Position,
              aRecord._Fields[i]._Size);
        // If blank2 is flagged and the field is blank, skip
        if (_RecordDefinition.IsBlank2(i) && tOtherField.empty()) {
          continue;
        }
        

        StringUtils::ValidatedString tVString(
            _RecordDefinition.IsFieldPartiallyCritical(i));

        if(tOwnField == tOtherField || 
            (tVString.first &&
             ((tOwnField.find(tVString.second) != std::string::npos) ||
              (tOtherField.find(tVString.second) != std::string::npos))))
        {
          continue;
        }

        tReturnValue = false;
        break;
      }
    }
  }
  else
  {
    if(this != &aRecord)
    {
      tReturnValue = false;
    }
  }

  return tReturnValue;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool
FixedRecord::operator != (const FixedRecord & aRecord) const
{
  if(*this == aRecord)
  {
    return false;
  }
  
  return true;
}

/******************************************************************************
 *************************** PROTECTED METHODS ********************************
 *****************************************************************************/

/******************************************************************************
 **************************** PRIVATE METHODS *********************************
 *****************************************************************************/

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
FixedRecord::AllocateFields()
{
  _Fields = reinterpret_cast<FieldBase *>(
    malloc(sizeof(FieldBase)*_NumberOfFields));
  memset(_Fields, 0,sizeof(FieldBase)*_NumberOfFields); 
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
FixedRecord::FillFieldArrayDelimited(const std::string & aRecordString)
{
  RecordDefinition::FieldDefinitionVector::const_iterator tIterator;
  tIterator = _RecordDefinition.GetBeginning();
  std::size_t tDelimiterSeparation;
  int tLengthInDefinition;

  std::size_t tForwardPosition = 0;
  std::size_t tLaggingPosition = 0;

  std::string tDelimiter = _RecordDefinition.GetRecordDelimiter(); 

  bool tDoneWithString = false;

  std::size_t tIndex = 0;

  while(tIterator != _RecordDefinition.GetEnd())
  { 
    if(!tDoneWithString)
    {
      tForwardPosition = _RecordString.find_first_of(
          tDelimiter,  tLaggingPosition);

      if(tForwardPosition == std::string::npos)
      {
        tForwardPosition = _RecordString.length();
        tDoneWithString = true;
      }
    }
    else
    {
      tForwardPosition = 0;
      tLaggingPosition = 0;
    }

    tDelimiterSeparation = tForwardPosition - tLaggingPosition;
    tLengthInDefinition = (*tIterator)->Length;

    if((tLengthInDefinition > 0) && (tLengthInDefinition <= tDelimiterSeparation))
    {
      _Fields[tIndex]._Size = tLengthInDefinition;
    } else {
      _Fields[tIndex]._Size = tDelimiterSeparation;
    }
    _Fields[tIndex]._Position = tLaggingPosition;
    //    _Fields[tIndex]._IsCriticalField = (*tIterator)->IsCritical;

    tLaggingPosition = tForwardPosition + 1;
    tIterator++;
    tIndex++;
  }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
FixedRecord::FillFieldArrayUndelimited(const std::string & aRecordString)
{
  std::size_t tRecordSize = 0;
  RecordDefinition::FieldDefinitionVector::const_iterator tIterator;
  tIterator = _RecordDefinition.GetBeginning();
  std::size_t tIndex  = 0;

  while(tIterator != _RecordDefinition.GetEnd())
  {
    int tFieldOffset = (*tIterator)->Offset;
    if(tFieldOffset == -1)
    {
      tFieldOffset = tRecordSize;
    }
    
    _Fields[tIndex]._Size = (*tIterator)->Length;
    _Fields[tIndex]._Position = tFieldOffset;
//    _Fields[tIndex]._IsCriticalField = (*tIterator)->IsCritical;
    
    tRecordSize += (*tIterator)->Length;
    tIterator++;
    tIndex++;
  }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
FixedRecord::GenerateKey()
{
  const RecordDefinition::IntVector & tKeyDefinition = 
    _RecordDefinition.GetKeyDefinition();
  
  for(std::size_t j = 0; j < tKeyDefinition.size(); j++)
  {
    if(_Key != "")
    {
      _Key += "|";
    }
    if(_Fields[tKeyDefinition[j]]._Size > 0)
    {
      _Key += _RecordString.substr(_Fields[tKeyDefinition[j]]._Position,
				   _Fields[tKeyDefinition[j]]._Size);
    }
  }
  
  //StringUtils::RemoveWhiteSpace(_Key);

  if(!_Key.length())
  {
    std::cout<<"couldn't generate a key!"<<std::endl;
    std::cout<<DumpRecord()<<std::endl;
  }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::string
FixedRecord::GetRecordLine(std::size_t aIndex) const
{
  std::string tReturnString("");
  
  tReturnString += _RecordDefinition.GetFieldName(aIndex);
  tReturnString += ":\t";
  if(_Fields[aIndex]._Size > 0)
  {
    std::string tTempString = _RecordString.substr(_Fields[aIndex]._Position, 
						   _Fields[aIndex]._Size);
    StringUtils::RemoveWhiteSpace(tTempString);
    tReturnString += tTempString;
  }
  tReturnString += "\n";

  return tReturnString;
}
