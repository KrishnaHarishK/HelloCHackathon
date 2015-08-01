#ifndef StringUtils_H
#define StringUtils_H

#include<string>
#include<vector>

class StringUtils
{
public:
  typedef std::vector<std::string> StringVector;
  typedef std::pair<bool, std::string> ValidatedString;
  
  typedef struct{
    std::size_t _Offset;
    std::size_t _Length;
  } StringSegment;

  typedef enum REMOVAL_MODE{
    eRemoveWhiteSpaceAll,
    eRemoveWhiteSpaceChomp
  };

  typedef enum VERIFICATION_MODE{
    eTrueForWhiteSpaceOrEmpty,
    eFalseForWhiteSpaceOrEmpty
  };

  typedef std::vector<StringSegment> StringSegmentVector;

  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  static StringSegmentVector
  BreakUpString(const std::string & aString, const std::string & aDelimiters);

  //---------------------------------------------------------------------------
  /**
   * This is not a safe method. Memory has to be provided for it and it has
   * no way to verify how much space was provisioned.
   */
  //---------------------------------------------------------------------------
  static void
  FillBuffer(char * rpBuffer, const std::string & aString);

  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  static bool 
  GetBool(std::string aString);

  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  static int  
  GetInt(std::string aString);

  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  static std::string
  GetString(int aInt);

  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  static bool 
  IsBool(std::string aString);

  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  static ValidatedString
  IsConditionalExclusion(std::string aString);

  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  static bool 
  IsInt(std::string aString, 
	VERIFICATION_MODE aMode = eFalseForWhiteSpaceOrEmpty);

  //---------------------------------------------------------------------------
  /**
   */
  //---------------------------------------------------------------------------
  static void
  RemoveWhiteSpace(std::string & aString, 
		   REMOVAL_MODE aMode = eRemoveWhiteSpaceAll);

};
#endif
