#include"CompareRecordApp.hh"

#include<string>
#include<sstream>
#include<cstdlib>
#include <errno.h>

#define COMPARE_APP_STANDARD_DISPLAY_COUNT 10

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/*
int main(void)
{
  for(int i = 0; true; i++)
  {
    std::string tString = std::itoa(i);
  }
}
*/


int
main(int argc, char ** argv)
{
  long tNumDisplay = COMPARE_APP_STANDARD_DISPLAY_COUNT;

  while (1)
  {
    sleep (10);
  }

  // validate args and pass to app
    switch (argc){
        case 6: {
            if ((strcmp(argv[5],"all") == 0) || (strcmp(argv[5],"ALL") == 0)){
                tNumDisplay = LONG_MAX;
            } else {
                errno = 0;
                tNumDisplay = strtoul(argv[5], NULL, 10);
                if ((errno) || (tNumDisplay < 0)){
                    std::cout << "Valid arguments for # "
                              << "of mismatches to display are "
                              << "non-negative integers, or \"all\""
                              << std::endl;
                    exit(1);
                }
            }
            argc--;
        }
        // no break, fall through to standard code
        case 5: {
            // close enough for now
            std::vector<std::string> tArguments;
            for(int i = 1; i < argc; i++)
            {
                tArguments.push_back(std::string(argv[i]));
            }
            std::ostringstream tempstream;
            tempstream << tNumDisplay;
            tArguments.push_back(std::string(tempstream.str()));
            std::cout<<"Starting application \n";
            CompareRecordApp tApplication(tArguments);
        }
        break;
        default:{
            std::cout<<"Usage: "<<std::endl
                     <<"CompareRecords outputpath(old) outputpath(new) dataType "
                     <<"configpath [# of mismatches to show = 10, ALL is valid]\n"
                     <<"Currently supported datatypes:\n"
                     <<"For NG: \"EMI\", \"GBR\", \"ICR\", \"IASA\" \n"
                     <<"For TV: \"TDR\", \"MM\"" << std::endl;
        }
        break;
    }


}


