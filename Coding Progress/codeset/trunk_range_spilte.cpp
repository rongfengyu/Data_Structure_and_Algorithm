#include <iostream>
#include <set>//STL set, for multi-ip check!
#include <stdlib.h>
#include <string>
#include <string.h>
using namespace std;


set<int> getTrunkrange(string vlanidrange)
{
	set<int> vlanidset;
	vlanidset.clear();

	char temp[100];
	strcpy(temp, vlanidrange.c_str());
	char *buf = (char *) temp;
	char *Ptr;

	Ptr = strtok(buf, ",");
	while (Ptr != NULL)
	{
		size_t pos;
		string ptr = string(Ptr);
		pos = ptr.find("-");
		if (pos != std::string::npos)
		{//find!
			char *idstart = (char *) ptr.substr(0, pos).c_str();
			char temp_idstart[100];
			strcpy(temp_idstart, idstart);
			int idstart_id = atoi(temp_idstart);

			char *idstop = (char *) ptr.substr(pos + 1,ptr.length() - pos - 1).c_str();
			char temp_idstop[100];
			strcpy(temp_idstop, idstop);
			int idstop_id = atoi(temp_idstop);

			for (unsigned int i = idstart_id; i <= idstop_id;i++)
			{
				vlanidset.insert(i);
			}
		}
		else //single IP
		{
			unsigned int id = atoi(ptr.c_str());
			vlanidset.insert(id);
		}
		//next IP/MASK
		Ptr = strtok(NULL, ",");
	}

	return vlanidset;
}


int main() {
    set<int> vlanidset;
    set<int>::iterator iter;
    
    vlanidset = getTrunkrange("10,20-30,41,33,9,102,90-100");
	
    for(iter = vlanidset.begin() ; iter != vlanidset.end() ; ++iter)
    {
        cout<<*iter<<" ";
    }
	
	return 0;
}
