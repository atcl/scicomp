///header
/* author:	Christian Himpe (2013)
 * 
 * license:	BSD 2-Clause License ( http://opensource.org/licenses/BSD-2-Clause )
 * 
 * filename:	config.cc
 * 
 * content:	Config class (handles ini files and configuration)
 */
///*

///includes
#include <stdio.h>
#include <ctype.h>
#include <map>
#include <cstring>
#include <string>
#include <algorithm>
///*

///definitions
class Config
{
	private:
		mutable std::map<std::string,double> param;			//map associating keys with values
		inline std::string trim(const char* c);				//trim whitespaces
	public:
		Config(const char* f);						//Contructor
		inline double operator()(const char* p,double d) const;		//Get Value by Name
};
///*

///implementation
std::string Config::trim(const char* c)
{
	std::string s(c);
	s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
	return s;
}

Config::Config(const char* f) : param()
{
	FILE* fp;
	fp = fopen(f,"r");
	if(fp==0) { exit(1); return; }

	char* line = 0;
	size_t length = 0;

	while(getline(&line,&length,fp)!=-1)
	{
		if(strpbrk(line,"=")!=0)					//check for '='
		{
			std::string name(trim(strtok(line,"=")));		//extract parameter name
			std::transform(name.begin(),name.end(),name.begin(),::tolower); //convert to lower case
			const double value = atof(strtok(0,";"));			//extract and convert parameter value
			param[name] = value;					//map current pair
		}
	}
	fclose(fp);
}

double Config::operator()(const char* p,double d) const
{
	const std::map<std::string,double>::const_iterator i = param.find(std::string(p));
	if(i != param.end()) { d = i->second; }
	return d;
}
///*

#include <iostream>

int main(int argc,char** argv)
{
	if(argc!=2) return 1;
	Config conf(argv[1]);

	std::cout << conf("symmetric",0) << std::endl;
	std::cout << conf("fast",1) << std::endl;
	std::cout << conf("drunk",0) << std::endl;

	return 0;
}




