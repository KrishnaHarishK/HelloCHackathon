#include"StringUtils.hh"

#include<sstream>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
StringUtils::StringSegmentVector
StringUtils::BreakUpString(const std::string & aString, 
                           const std::string & aDelimiters)
{
  StringSegmentVector tStringChunks;

  std::size_t tForwardPosition = 0;
  std::size_t tLaggingPosition = 0;

  while((tForwardPosition = aString.find_first_of(aDelimiters, 
                                                  tLaggingPosition)) != 
        std::string::npos)
  {
    StringSegment tSegment = {tLaggingPosition, 
                              tForwardPosition- tLaggingPosition};
    tStringChunks.push_back(tSegment);

    tLaggingPosition = tForwardPosition + 1;
  }

  StringSegment tSegment = {tLaggingPosition, 
                            aString.length() - tLaggingPosition};
  tStringChunks.push_back(tSegment);
  
  return tStringChunks;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
StringUtils::FillBuffer(char * rpBuffer, const std::string & aString)
{
  strncpy(rpBuffer, aString.c_str(), aString.length());
  rpBuffer[aString.length()] = 0;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool
StringUtils::GetBool(std::string aString)
{
  bool tReturnValue = false;
  
  RemoveWhiteSpace(aString);

  if(aString == "true")
  {
    tReturnValue = true;
  }

  return tReturnValue;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int
StringUtils::GetInt(std::string aString)
{
  int tReturnValue;
  RemoveWhiteSpace(aString);

  std::istringstream tStream(aString);
  
  tStream>>tReturnValue;
  
  return tReturnValue;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::string
StringUtils::GetString(int aInt)
{
  std::ostringstream tStream;

  tStream<<aInt;
  
  return tStream.str();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool
StringUtils::IsBool(std::string aString)
{
  bool tReturnValue = false;
  RemoveWhiteSpace(aString);

  if(aString == "true" || aString == "false")
  {
    tReturnValue = true;
  }

  return tReturnValue;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
StringUtils::ValidatedString
StringUtils::IsConditionalExclusion(std::string aString)
{
  ValidatedString tReturnPair(false, "");

  RemoveWhiteSpace(aString);

  if((aString.find("false") == 0) &&
     (aString.length() > 6) &&
     (aString[5] == '='))
  {
    tReturnPair.first = true;
    tReturnPair.second = aString.substr(6, aString.length());
  }

  return tReturnPair;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool
StringUtils::IsInt(std::string aString, 
		   VERIFICATION_MODE aMode)
{
  bool tReturnValue = false;
  RemoveWhiteSpace(aString);

  if((aString.length() ||
      aMode == eTrueForWhiteSpaceOrEmpty)&&
     (aString.find_first_not_of("-0123456789") == std::string::npos))
  {
    std::size_t tDashIndex = aString.find('-');
    if(tDashIndex != std::string::npos)
    {
      if((tDashIndex == 0) && 
	 aString.find('-', ++tDashIndex) == std::string::npos)
      {
	tReturnValue = true;
      }
    }
    else
    {
      tReturnValue = true;
    }
  }

  return tReturnValue;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
StringUtils::RemoveWhiteSpace(std::string & aString, REMOVAL_MODE aMode)
{
  if(aMode == eRemoveWhiteSpaceAll)
  {
    while((*(aString.begin()) == ' ') ||
	  (*(aString.begin()) == '\t') ||
	  (*(aString.begin()) == '\n') ||
	  (*(aString.begin()) == '\f') ||
	  (*(aString.begin()) == '\r'))
    {
      aString.erase(aString.begin());
    }
    
    std::string::iterator tIterator = aString.begin();
    
    while(tIterator != aString.end())
    {
      std::string::iterator tOldValue = tIterator;
      tIterator ++;
      if((*tIterator == ' ') ||
	 (*tIterator == '\t') ||
	 (*tIterator == '\n') ||
	 (*tIterator == '\f') ||
	 (*tIterator == '\r'))
      {
	aString.erase(tIterator);
	tIterator = tOldValue;
      }
    }
  }
  else if(aMode == eRemoveWhiteSpaceChomp)
  {
    std::string::iterator tIterator = aString.end();

    while((*(--tIterator) == ' ') ||
	  (*(--tIterator) == '\t') ||
	  (*(--tIterator) == '\n') ||
	  (*(--tIterator) == '\f') ||
	  (*(--tIterator) == '\r'))
    {
      aString.erase(tIterator);
      tIterator = aString.end();
    }
  }
}

