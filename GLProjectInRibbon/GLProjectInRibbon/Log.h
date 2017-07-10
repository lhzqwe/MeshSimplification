#pragma once
#include <string>
#include <fstream>

using namespace std;

class Log
{
public:
	Log() : m_FileName("Running.log") { InitializeLog(); };
	~Log();

	void log(string content);

private:
	string m_FileName;
	ofstream m_Ofstream;

private:
	void InitializeLog();
	void DestroyLog();
};

