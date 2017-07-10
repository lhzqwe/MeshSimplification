#include "stdafx.h"
#include "Log.h"


Log::~Log()
{
	DestroyLog();
}

void Log::InitializeLog()
{
	m_Ofstream = ofstream(m_FileName, std::ios::out | std::ios::app);
	assert(m_Ofstream.is_open());
}

void Log::DestroyLog()
{
	m_Ofstream.flush();
	m_Ofstream.close();
}

void Log::log(string content)
{
	m_Ofstream << content << endl;
	m_Ofstream.flush();
}
