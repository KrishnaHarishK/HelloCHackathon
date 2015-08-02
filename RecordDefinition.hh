    #ifndef RecordDefinition_H
    #define RecordDefinition_H

    #include"StringUtils.hh"

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

    /// this class is intended to parse a config file that defines a record
    /// and create the corresponding record definition. Intended to be created once
    /// and then handed to the constructors of all the records for a given record
    /// type.
    class RecordDefinition
    {
        public:
            /// a type for field definitions
            class FieldDefinition{
                public:
                    FieldDefinition() :
                        Name(""),
                        Length(-1),
                        Offset(-1),
                        Index(0),
                        IsCritical(true),
                        IsBlank1(false),
                        IsBlank2(false),
                        IsHashkey(false),
                        IsPartiallyCritical(false),
                        MatchString("")
                    {};
        
                    std::string Name;
                    int         Length; 
                    int         Offset;
                    std::size_t Index;
                    bool IsCritical;
                    bool IsBlank1;            // Old record field should be skipped if blank
                    bool IsBlank2;            // New record field should be skipped if blank
                    bool IsHashkey;           // Record field should be used as the hashkey for splitting
                    int  HashkeyValue;        //   records into different RecordMap
                    bool IsPartiallyCritical;
                    std::string MatchString;
                    int  FieldDifferenceCount;
            };

            // Typedefs
            typedef std::vector<FieldDefinition *> FieldDefinitionVector;
            typedef std::vector<std::size_t>       IntVector;
            typedef std::map<std::string, std::size_t> FieldIndexMap;
            
            // Constructors
            RecordDefinition(const std::string & aFileName);
            // For MCDR records
            RecordDefinition() {} ;
      
            // Deconstructor
            virtual ~RecordDefinition();

            // Field Definition Accessors
            bool IsCritical(std::size_t aIndex) const {
                return _FieldDefinitions[aIndex]->IsCritical;
            }
            bool IsBlank1(std::size_t aIndex) const {
                return _FieldDefinitions[aIndex]->IsBlank1;
            }
            bool IsBlank2(std::size_t aIndex) const {
                return _FieldDefinitions[aIndex]->IsBlank2;
            }
            bool IsHashkey(std::size_t aIndex) const {
                return _FieldDefinitions[aIndex]->IsHashkey;
            }
            int  GetHashkeyValue(std::size_t aIndex) const {
                return _FieldDefinitions[aIndex]->HashkeyValue;
            }
            const std::string & GetFieldName(std::size_t aIndex) const {
                return _FieldDefinitions[aIndex]->Name;
            }
            FieldDefinitionVector::const_iterator GetBeginning() const {
                return _FieldDefinitions.begin();
            }
            FieldDefinitionVector::const_iterator GetEnd() const {
                return _FieldDefinitions.end();
            }
            
            // Record Definition Accessors
            int GetRecordFixedLength() const {
                return _FixedRecordLength;
            }
            const std::string & GetRecordDelimiter() const {
                return _RecordDelimiter;
            }
            int GetNumberOfFields() const {
                return _FieldDefinitions.size();
            }
            std::size_t GetMinimumRecordLength() const {
                return _MinimumRecordLength;
            }
            const IntVector & GetKeyDefinition() const {
                return _KeyDefinition;
            }
            bool IsRecordDelimited() const {
                return (_RecordDelimiter != "");
            }
            bool IsRecordFixedLength() const {
                return (_FixedRecordLength != -1);
            }
            // Implemented in .cc file
            int GetFieldIndex(const std::string & aFieldName) const;
            std::string GetKeyDefinitionString() const;
            
            // Other functions
            StringUtils::ValidatedString IsFieldPartiallyCritical(std::size_t aIndex) const;
            //Surya bool RecordDefinition::IsFixedLengthInfoComplete() const;
            bool IsFixedLengthInfoComplete() const;
      
        private:
            std::ifstream           _InputFileStream;
            std::filebuf *          _FileBuffer;
            std::string             _LineString;

            IntVector               _KeyDefinition;

            FieldDefinitionVector   _FieldDefinitions;
            FieldIndexMap           _FieldIndexMap;

            // These define the look of the config file
            const std::string       _FieldsTag;
            const std::string       _FieldsEndTag;
            const std::string       _PropertiesTag;
            const std::string       _PropertiesEndTag;
            const std::string       _RecordLength;
            const std::string       _Delimiter;
            
            std::string             _RecordDelimiter;
            int                     _FixedRecordLength;
            std::size_t             _MinimumRecordLength;
            
            const static int        _BuffSize           = 2048;
          
            // Functions
            std::string & GetLine();
            std::string GetPropertyValueString(
                            const std::string & aStringToSearch,
                            const std::string & aPropertyPrefix,
                            std::size_t aPosition
                            ) const;
            void ReadFieldDefinitions();
            void ReadInputFile();
            void ReadProperties();
    };

    #endif

