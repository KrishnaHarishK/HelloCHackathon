#include "RecordDefinition.hh"
#include "StringUtils.hh"


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
RecordDefinition::RecordDefinition(const std::string & aFileName) :
    _InputFileStream(),
    _FileBuffer(NULL),
    _LineString(""),
    _FieldDefinitions(),
    _RecordDelimiter(""),
    _FixedRecordLength(-1),
    _MinimumRecordLength(0),
    _FieldsTag("<Fields>"),
    _FieldsEndTag("</Fields>"),
    _PropertiesTag("<Properties>"),
    _PropertiesEndTag("</Properties>"),
    _RecordLength("RecordLength="),
    _Delimiter("Delimiter=")
{
    _FileBuffer = _InputFileStream.rdbuf();
  
    if(!_FileBuffer->open(aFileName.c_str(), std::ios::in)) {
        std::cout   << "FATAL ERROR: RecordDefinition could not open file."
                    << "EXITING" << std::endl;
        exit(1);
    }
    else {
        ReadInputFile();
    
        if (IsRecordFixedLength()) {
            std::cout << "Checking if fixed record information is complete..." << std::endl;
            if (!IsFixedLengthInfoComplete()) {        
                std::cout << "Please change the definition file.  Exiting CompareRecords." << std::endl;
                exit(1);
            }
            std::cout << "Check finished." << std::endl;
        }
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
RecordDefinition::~RecordDefinition()
{
    register std::size_t tVectorSize = _FieldDefinitions.size();
    while(tVectorSize > 0) {
        delete _FieldDefinitions[--tVectorSize];
        _FieldDefinitions.pop_back();
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int
RecordDefinition::GetFieldIndex(const std::string & aName) const
{
    FieldIndexMap::const_iterator tIterator = _FieldIndexMap.find(aName);
  
    if(tIterator != _FieldIndexMap.end()) {
        return tIterator->second;
    }
  
    return -1;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::string
RecordDefinition::GetKeyDefinitionString() const
{
    std::string tString("");

    for(std::size_t i = 0; i < _KeyDefinition.size(); i++) {
        if(i != 0) {
            tString += "|";
        }

        tString += GetFieldName(_KeyDefinition[i]);
    }

    return tString;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
StringUtils::ValidatedString
RecordDefinition::IsFieldPartiallyCritical(std::size_t aIndex) const
{
    StringUtils::ValidatedString tValidatedString(false, "");
  
    if(aIndex < _FieldDefinitions.size()) {
        if(_FieldDefinitions[aIndex]->IsPartiallyCritical) {
            tValidatedString.first = true;
            tValidatedString.second = _FieldDefinitions[aIndex]->MatchString;
        }
    }

    return tValidatedString;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool RecordDefinition::IsFixedLengthInfoComplete() const
{
	bool isComplete = true;
    for (int i=0; i<_FieldDefinitions.size(); i++) {
        // If Length is still -1, neither it nor the Offset have been set
        if (_FieldDefinitions[i]->Length == -1) {
			std::cout   << " - No Length/Offest values seen for " 
                        << _FieldDefinitions[i]->Name << "." << std::endl;
			isComplete=false;
        }
    
        // If Length is set to a negative number, that's a problem
        else if (_FieldDefinitions[i]->Length < -1) {
            std::cout   << " - Length for " << _FieldDefinitions[i]->Name
                        << " has been set to a negative number." << std::endl;
			isComplete=false;
        }
    
        // If Offset is still -1, it has not been set
		else if (_FieldDefinitions[i]->Offset == -1) {
            std::cout   << " - Only one Length/Offset value seen for " 
                        << _FieldDefinitions[i]->Name << "." << std::endl;
			isComplete=false;
        }
    
        // If Offset is set to a negative number, that's a problem
        else if (_FieldDefinitions[i]->Offset < -1) {
            std::cout   << " - Offset for " << _FieldDefinitions[i]->Name
                        << " has been set to a negative number." << std::endl;
			isComplete=false;
        }
    }
    return isComplete;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::string &
RecordDefinition::GetLine()
{
    _LineString = "";

    while(!_InputFileStream.eof()) {
        static char sCharBuffer[_BuffSize];
        _InputFileStream.getline(sCharBuffer, _BuffSize);
		if (sCharBuffer[0] == '#') {
			continue;
		}
		else if (sCharBuffer[0] == '\0') {
			continue;
		}
		if (sCharBuffer[0] != ' ' && sCharBuffer[0] != '\t') {
			int valid=0;
			for (int i=0; i<strlen(sCharBuffer); ++i) {
				if (sCharBuffer[i] != ' ' && sCharBuffer[i] != '\t') {
					_LineString = sCharBuffer;
					valid=1;
					break;
				}
			}
			if (valid) {break;}
			else {continue;}
		}
    }

    return _LineString;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::string
RecordDefinition::GetPropertyValueString(const std::string & aStringToSearch,
                                         const std::string & aPropertyPrefix,
                                         std::size_t aPosition) const
{
    std::size_t tPropertyStringIndex = aPosition + aPropertyPrefix.length();
  
    std::size_t tPropertyStringLength = (aStringToSearch.length() - 
                                         tPropertyStringIndex);
  
    std::string tSubString = aStringToSearch.substr(tPropertyStringIndex,
                                                    tPropertyStringLength);
  
    return tSubString;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
RecordDefinition::ReadFieldDefinitions()
{
  std::string &tCurrentLine = GetLine();
  std::size_t tFieldIndex = 0;

  // While we haven't hit </Fields>
  // Skipping any blank lines
  while ((tCurrentLine != _FieldsEndTag) &&
         (tCurrentLine != ""))
  {
    std::vector<std::string> tSubStrings;
    std::size_t tSpaceIndex = 0;
    std::size_t tNextSpaceIndex = 0;

    // Break the line up into individual "words" (defined as being
    //   separated by spaces).  Push those words onto tSubStrings.
    while(tNextSpaceIndex != std::string::npos)
    {
      tNextSpaceIndex = tCurrentLine.find(' ', tSpaceIndex);

      tSubStrings.push_back(
        tCurrentLine.substr(tSpaceIndex, 
                            tNextSpaceIndex - tSpaceIndex));
      
      tSpaceIndex = tNextSpaceIndex + 1;
    }

    // Grab the last word too.
    tSubStrings.push_back(
      tCurrentLine.substr(tSpaceIndex,
                          tCurrentLine.length() - tSpaceIndex));

    // Make a new Field Definition to hold the information from this line.
    FieldDefinition * tField = new FieldDefinition();

    // Field name is defined as being the first 'word'
    tField->Name = tSubStrings[0];

    
    // If there is more than one word in the line, do the following logic on the
    //   additional words
    //
    // If it's an int,
    //   If we don't already have a field length, set Length to that int
    //   If we already have a field length, set Offset to that int
    // If it's a boolean, set IsCritical to that boolean
    // If it matches "false=*",
    //   Set IsPartiallyCritical to true
    //   Set MatchString to *
    // If it matches "key", push back [SOMETHING] onto _KeyDefinition
    if(tSubStrings.size() > 1)
    {
        bool tAlreadyHasFieldLength = false;
        bool foundHashkey = false;  // We only want one field to be a hashkey

        for(std::size_t i = 1; i < tSubStrings.size(); i++) {
            StringUtils::ValidatedString tIsPartiallyIgnored(StringUtils::IsConditionalExclusion(tSubStrings[i]));
        
            if(StringUtils::IsInt(tSubStrings[i])) {
                if(!tAlreadyHasFieldLength) {
                    tField->Length = StringUtils::GetInt(tSubStrings[i]);
                    tAlreadyHasFieldLength = true;
                }
                else {
                    tField->Offset = StringUtils::GetInt(tSubStrings[i]);
                }
            }
            else if(StringUtils::IsBool(tSubStrings[i])) {
                tField->IsCritical = StringUtils::GetBool(tSubStrings[i]);
            }
            else if(tIsPartiallyIgnored.first) {
                tField->IsPartiallyCritical = true;
                tField->MatchString = tIsPartiallyIgnored.second;
            }
            else if(tSubStrings[i] == "key") {
                _KeyDefinition.push_back(_FieldDefinitions.size());
            }
            // Old record field i should be skipped if blank
            else if (tSubStrings[i] == "blank1") {
                tField->IsBlank1 = true;
            }
            // New record field i should be skipped if blank
            else if (tSubStrings[i] == "blank2") {
                tField->IsBlank2 = true;
            }
            else if (!foundHashkey && tSubStrings[i] == "hashkey") {
                tField->IsHashkey = true;
                
                // We only want one field to be a hashkey
                foundHashkey = true;
                
                // The next field should be a number specifying how many hashes to use
                // If it's not, something is wrong
                if (StringUtils::IsInt(tSubStrings[i+1])) {
                    tField->HashkeyValue = StringUtils::GetInt(tSubStrings[++i]);
                }
                else {
                    // TODO: FIRE OFF AN ERROR
                }
            }
        }
    }

    // Default is true.  If was flagged 'false' this is skipped
    if(tField->IsCritical) {
        // The default value for Length, i.e. we didn't get a
        //   specfic length in the line.
        if(tField->Length == -1) {
            _MinimumRecordLength += 1;
        }
        // We did get a value for Length in the line.
        else {
            _MinimumRecordLength += tField->Length;
        }
    }

    // Set tField's index, THEN increment tFieldIndex
    tField->Index = tFieldIndex++;
    
    _FieldDefinitions.push_back(tField);
    
    _FieldIndexMap[tField->Name] = tField->Index;

    // Get the next line
    tCurrentLine = GetLine();
  }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
RecordDefinition::ReadInputFile()
{
  std::string & tCurrentLine = GetLine();
  // skip to a tag we care about
  while(tCurrentLine != "")
  {
    while((tCurrentLine != _FieldsTag) && 
          (tCurrentLine != _PropertiesTag) &&
          (tCurrentLine != ""))
    {
      tCurrentLine = GetLine();
    }
    
    if(tCurrentLine == _FieldsTag)
    {
      ReadFieldDefinitions();
    }
    else if(tCurrentLine == _PropertiesTag)
    {
      ReadProperties();
    }
  }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
RecordDefinition::ReadProperties()
{
  std::string &tCurrentLine = GetLine();
  while ((tCurrentLine != _PropertiesEndTag) &&
         (tCurrentLine != ""))
  {
    // check whether the line represents record length
    std::size_t tPosition;
    if((tPosition = tCurrentLine.find(_RecordLength)) != std::string::npos)
    {
      _FixedRecordLength = StringUtils::GetInt(
        GetPropertyValueString(tCurrentLine, _RecordLength, tPosition));
    }
    else if((tPosition = tCurrentLine.find(_Delimiter)) != std::string::npos)
    {
      _RecordDelimiter = GetPropertyValueString(tCurrentLine, 
                                                _Delimiter, 
                                                tPosition);
    }

    // if not recognized, just advance to the next line for now
    tCurrentLine = GetLine();
  }
}
