#include"CompareRecordApp.hh"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <map>
#include <errno.h>
#include <sys/stat.h>

#define COMPARE_APP_STANDARD_DISPLAY_COUNT 10

char* const p_comparefile = ".compare";

typedef std::map<unsigned int,std::string> CompSuite;

CompSuite gl_mapCompSuite;

void
parse_dot_compare_file (void)
{
  std::ifstream ifs (p_comparefile);

  if (ifs.is_open ())
  {
    std::string s_line = "";
    unsigned int l_counter = 0;

    while (getline (ifs, s_line))
    {
      ++ l_counter;

      gl_mapCompSuite[l_counter] = s_line;
    }

    ifs.close ();
  }
  else
  {
    std::cerr << "Failed to open file .compare: [" << errno << "] " << strerror(errno) << std::endl;
    exit (-1);
  }
}

bool
ready_to_compare (void)
{
  struct stat osFile;
  memset (&osFile, 0, sizeof (struct stat));

  if (stat (".compare", &osFile) == 0)
  {
    parse_dot_compare_file ();
    return true;
  }

  return false;
}

std::vector<std::string>
&split (const std::string &strLine, char cDelim, std::vector<std::string> &vectElements)
{
  std::stringstream sstrLine(strLine);
  std::string strSplitItem;
  while (std::getline(sstrLine, strSplitItem, cDelim)) {
      vectElements.push_back(strSplitItem);
  }
  return vectElements;
}

std::vector<std::string>
split(const std::string &strStringToSplit, char cDelim)
{
  std::vector<std::string> vectSplitElements;
  split(strStringToSplit, cDelim, vectSplitElements);
  return vectSplitElements;
}

int
main(int argc, char ** argv)
{
  long tNumDisplay = COMPARE_APP_STANDARD_DISPLAY_COUNT;

  unsigned int i = 3;

  do
  {
	-- i;
    if (ready_to_compare())
    {
      std::cout << "Found " << gl_mapCompSuite.size() << " commands to compare. Forking now!!" << std::endl;

      CompSuite::iterator l_iterCompSuite = gl_mapCompSuite.begin ();

      while (l_iterCompSuite not_eq gl_mapCompSuite.end ())
      {
        std::vector<std::string> tArguments = split (l_iterCompSuite->second, ' ');

        CompareRecordApp tApplication (tArguments);

        char p_outfile[PATH_MAX] = {0};
        char p_destfile[PATH_MAX] = {0};

        sprintf (p_outfile, "%s_Comparison.txt", tArguments[2].c_str ());
        sprintf (p_destfile, "%d_%s_Comparison.txt",l_iterCompSuite->first,tArguments[2].c_str ());

        if (isFile(p_outfile))
        {
        	rename (p_outfile, p_destfile);
        }

        ++ l_iterCompSuite;
      }

      unlink (p_comparefile);

      GenerateCSV (".");
    }
    else
    {
      std::cout << "Nothing to do yet...sleeping!!!" << std::endl;
      sleep (10);
    }
    
    if (i < 0)
    	break;

  } while (true);

  /* validate args and pass to app
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
    */

  return 0;
}


