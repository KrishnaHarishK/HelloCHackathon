#include "CompareRecordApp.hh"

#define csv_delim ","

bool
isDir(char* const p_path)
{
  struct stat osDir;
  memset (&osDir, 0, sizeof (&osDir));
  if (    stat (p_path, &osDir) == 0
      and S_ISDIR (osDir.st_mode)
     )
  {
    return true;
  }

  return false;
}

bool
isFile(char* const p_path)
{
  struct stat osDir;
  memset (&osDir, 0, sizeof (&osDir));
  if (    stat (p_path, &osDir) == 0
      and S_ISREG (osDir.st_mode)
     )
  {
    return true;
  }

  return false;
}

void
GenerateCSV_File (char* const p_path)
{
  if (not strcasestr (p_path, "_Comparison.txt"))
  {
	  return;
  }

  ifstream ifs (p_path);

  if (ifs.is_open ())
  {
    string s_line = "";

    while (getline (ifs, s_line))
    {
      if (   strcmp (s_line.c_str(), "") == 0
          or strcasestr (s_line.c_str(), "COMPARING BOTH DIRECTORIES")
          or strcasestr (s_line.c_str(), "Key field(s):")
          or strcasestr (s_line.c_str(), "Field     Name                                           Mismatched")
         )
      {
        #ifdef DEBUG2
        std::cerr << "  Ignored " << s_line << std::endl;
        #endif
        continue;
      }

      if (strcasestr (s_line.c_str (), "Total Number of Keys (version 1):"))
      {
        char* index = (char*) s_line.c_str();
        index += strlen ("Total Number of Keys (version 1):")+1;

        gl_KeyCount_First += atoi (index);
      }
      else if (strcasestr (s_line.c_str (), "Total Number of Keys (version 2):"))
      {
        char* index = (char*) s_line.c_str();
        index += strlen ("Total Number of Keys (version 2):")+1;

        gl_KeyCount_Second += atoi (index);
      }
      else if (strcasestr (s_line.c_str (), "Number of Duplicate keys (version 1):"))
      {
        char* index = (char*) s_line.c_str();
        index += strlen ("Number of Duplicate keys (version 1):")+1;

        gl_DupCount_First += atoi (index);
      }
      else if (strcasestr (s_line.c_str (), "Number of Duplicate keys (version 2):"))
      {
        char* index = (char*) s_line.c_str();
        index += strlen ("Number of Duplicate keys (version 2):")+1;

        gl_DupCount_Second += atoi (index);
      }
      else if (strcasestr (s_line.c_str (), "Total Number of Matched Keys:"))
      {
        char* index = (char*) s_line.c_str();
        index += strlen ("Total Number of Matched Keys:")+1;

        gl_MatchedCount += atoi (index);
      }
      else if (strcasestr (s_line.c_str (), "Number of Unmatched Keys (version 1):"))
      {
        char* index = (char*) s_line.c_str();
        index += strlen ("Number of Unmatched Keys (version 1):")+1;

        gl_UnMatched_First += atoi (index);
      }
      else if (strcasestr (s_line.c_str (), "Number of Unmatched Keys (version 2):"))
      {
        char* index = (char*) s_line.c_str();
        index += strlen ("Number of Unmatched Keys (version 2):")+1;

        gl_UnMatched_Second += atoi (index);
      }
      else if (strcasestr (s_line.c_str (), "Number of records same key w/ differences:"))
      {
        char* index = (char*) s_line.c_str();
        index += strlen ("Number of records same key w/ differences:")+1;

        gl_MatchedWDiff += atoi (index);
      }
    }

    ifs.close ();
  }
  else
  {
    std::cerr << "Failed to open file " << p_path << ": [" << errno << "] " << strerror(errno) << std::endl;
    return;
  }

  #ifdef DEBUG
  std::cout << "Keys in version 1: " << gl_KeyCount_First << std::endl
       << "Keys in version 2: " << gl_KeyCount_Second << std::endl
       << "Dups in version 1: " << gl_DupCount_First << std::endl
       << "Dups in version 2: " << gl_DupCount_Second << std::endl
       << "Matched Keys: " << gl_MatchedCount << std::endl
       << "Unmatched in version 1: " << gl_UnMatched_First << std::endl
       << "Unmatched in version 2: " << gl_UnMatched_Second << std::endl
       << "Matched Keys with diff: " << gl_MatchedWDiff << std::endl;
  #endif

  ofstream ofs ("Report.csv");

  if (ofs.is_open ())
  {
    ofs << "Process-name" << csv_delim
        << "Total Keys in Version 1" << csv_delim
        << "Total Keys in Version 2" << csv_delim
        << "Duplicate Keys in Version 1" << csv_delim
        << "Duplicate Keys in Version 2" << csv_delim
        << "Matched Keys" << csv_delim
        << "Unmatched Keys in Version 1" << csv_delim
        << "Unmatched Keys in Version 2" << csv_delim
        << "Matched Differences" << csv_delim
        << "Reasons for differences" << std::endl;

    ofs << "ns_matchmerge" << csv_delim
        << gl_KeyCount_First << csv_delim
        << gl_KeyCount_Second << csv_delim
        << gl_DupCount_First << csv_delim
        << gl_DupCount_Second << csv_delim
        << gl_MatchedCount << csv_delim
        << gl_UnMatched_First << csv_delim
        << gl_UnMatched_Second << csv_delim
        << gl_MatchedWDiff << csv_delim
        << "-----------------" << std::endl;

    ofs.close ();
  }
  else
  {
    std::cerr << "Failed to open output file" << ": [" << errno << "] " << strerror(errno) << std::endl;
  }
}

void
GenerateCSV_Dir (char* const p_path)
{
  struct dirent *infile = NULL;
  DIR    *indir         = NULL;
  char *p_filename      = NULL;

  if((indir = opendir(p_path)) == NULL)
  {
    std::std::cerr << "Unable to opendir " << p_path << std::endl;
    return;
  }

  while( (infile = readdir(indir)) != NULL )
  {
    if(   (infile->d_name[0] == '.')
       or (strstr (infile->d_name, ".tmp" ) != NULL)
       or (strstr (infile->d_name, ".temp" ) != NULL)
       or (strstr (infile->d_name, "_Comparison.txt") == NULL)
      )
    {
      continue;
    }

    p_filename = g_strdup_printf ("%s/%s", p_path, infile->d_name);

    GenerateCSV (p_filename);

    free (p_filename);
    p_filename = NULL;

  }

  closedir (indir);
  indir = NULL;
}

void
GenerateCSV (char* const p_path)
{
  if (isDir (p_path))
  {
    GenerateCSV_Dir (p_path);
  }
  else if (isFile (p_path))
  {
    GenerateCSV_File (p_path);
  }
  else
  {
    std::cerr << p_path << " is neither a file nor a dir, nothing to do, returning to main!!" << std::endl;
  }

  return;
}
