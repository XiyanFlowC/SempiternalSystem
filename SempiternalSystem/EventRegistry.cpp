#include "EventRegistry.h"

int EventRegistry::Register(void(*p_func)(int, void*), int key)
{
	if (size >= funcSize) return -1;
	terms[size].key = key;
	terms[size].index = size;
	registeredFunc[size++] = p_func;
	return 0;
}

int EventRegistry::Register(void(*p_func)(int, void*))
{
	if (size >= funcSize) return -1;
	registeredFunc[size++] = p_func;
	return 0;
}

void EventRegistry::HandleEvent(int p_key, int argc, void* args)
{
	for (int i = 0; i < size; ++i)
	{
		if (terms[i].key == p_key)
		{
			registeredFunc[terms[i].index](argc, args);
		}
	}
}

void EventRegistry::ExecFunc(int p_index, int argc, void* args)
{
	registeredFunc[p_index](argc, args);
}

EventRegistry::EventRegistry() : funcSize(128), size(0)
{
	registeredFunc = (void(**)(int, void*))malloc(sizeof(void*) * funcSize);
	terms = (RegistryTerm*)malloc(sizeof(RegistryTerm) * funcSize);
}

EventRegistry::EventRegistry(int p_size) : size(0)
{
	funcSize = p_size;
	registeredFunc = (void(**)(int, void*))malloc(sizeof(void*) * funcSize);
	terms = (RegistryTerm*)malloc(sizeof(RegistryTerm) * funcSize);
}

EventRegistry::~EventRegistry()
{
	free(registeredFunc);
	free(terms);
}
