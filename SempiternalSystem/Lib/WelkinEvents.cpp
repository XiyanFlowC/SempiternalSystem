#include "WelkinEvents.h"

WelkinEngine::Events::Events(int p_cnt)
{
	m_events = new std::vector<void (*) (int, void*)>[p_cnt];
	m_cnt = p_cnt;
}

void WelkinEngine::Events::RegisterEvent(int p_eventID, void(*p_callback)(int, void*))
{
	if (p_eventID >= m_cnt) throw new ParameterException("p_eventID", __LINE__, __FILE__, "p_eventID out of range: m_cnt.");
	if (p_eventID < 0) throw new ParameterException("p_eventID", __LINE__, __FILE__, "p_eventID can not be smaller than 0.");

	m_events[p_eventID].push_back(p_callback);
}

int WelkinEngine::Events::RegisterEvents(int p_eventID, int p_func, void(**p_callbacks)(int, void*))
{
	if (p_eventID >= m_cnt) throw new ParameterException("p_eventID", __LINE__, __FILE__, "p_eventID out of range: m_cnt.");
	if (p_eventID < 0) throw new ParameterException("p_eventID", __LINE__, __FILE__, "p_eventID can not be smaller than 0.");

	int i;
	for (i = 0; i < p_func; ++i)
	{
		m_events[p_eventID].push_back(p_callbacks[i]);
	}

	return i;
}

void WelkinEngine::Events::PullEvnet(int p_eventID, int argc, void* args)
{
	if (p_eventID >= m_cnt) throw new ParameterException("p_eventID", __LINE__, __FILE__, "p_eventID out of range: m_cnt.");
	if (p_eventID < 0) throw new ParameterException("p_eventID", __LINE__, __FILE__, "p_eventID can not be smaller than 0.");

	for (std::vector<void (*) (int, void*)>::iterator itr = m_events[p_eventID].begin(); itr != m_events[p_eventID].end(); ++itr)
	{
		(*itr)(argc, args);
	}
}
