#include "WelkinManager.h"

inline bool WelkinEngine::Manager::IsHalted()
{
	return halted;
}

inline void WelkinEngine::Manager::Halt()
{
	halted = true;
}

inline void WelkinEngine::Manager::Wake()
{
	halted = false;
}
