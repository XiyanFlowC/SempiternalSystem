#include "WelkinObject.h"

const static WelkinEngine::Size WelkinEngine::StandardBoundingSize = { 1.0, 1.0, 1.0 };

int WelkinEngine::Object::GetID()
{
    return m_ID;
}

WelkinEngine::string WelkinEngine::Object::GetMnemonic()
{
    return m_mnemonic_name;
}

void WelkinEngine::Object::SetMenemonic(WelkinEngine::string p_mnemonic_name)
{
    m_mnemonic_name = p_mnemonic_name;
}

WelkinEngine::Object::Object(int ID)
    : m_ID(ID)
{
}

WelkinEngine::Object::Object(int ID, string p_mnemonic_name)
    : m_mnemonic_name(p_mnemonic_name), m_ID(ID)
{
}

WelkinEngine::Entity::Entity(int ID)
    :Object(ID)
{
    EmptyInit();
}

void WelkinEngine::Entity::EmptyInit()
{
    m_boundingSize = StandardBoundingSize;
    m_interactiveSize = StandardBoundingSize;
    m_event = new Events(DoNotUse_EventCount);
    m_map_loc = StandardBoundingSize;
}

WelkinEngine::Entity::Entity(int ID, string p_mnemonic_name, Position p_loc, Size p_interS, Size p_boundS)
    :Object(ID, p_mnemonic_name), m_map_loc(p_loc), m_boundingSize(p_boundS), m_interactiveSize(p_interS)
{
    EmptyInit();
}
