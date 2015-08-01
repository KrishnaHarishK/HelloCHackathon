#ifndef _IPTSException__h_
#define _IPTSException__h_

#include <string>

using std::string;

class IPTSException 
{
    public:
        // Constructors
        IPTSException(const string &p_info);
        IPTSException();
    
        // Destructor
        virtual ~IPTSException();
    
        void addInfo(const string &p_info);
        const string printInfo(void);

    private:
        string _info;
};

#endif // _IPTSException__h_

