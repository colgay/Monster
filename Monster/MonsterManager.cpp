#include "MonsterManager.h"

MonsterManager::~MonsterManager()
{
    this->Clear();
}

BaseMonster* MonsterManager::Create(const char* pszName)
{
    BaseMonsterClassHelper* pHelper = GetMonsterClass(pszName);
    if (pHelper == nullptr)
        return nullptr;

    BaseMonster *pMonster = pHelper->Instantiate();
    if (pMonster == nullptr)
        return nullptr;

    m_edicts[pMonster->m_pEdict] = pMonster;
    return pMonster;
}

void MonsterManager::Remove(BaseMonster* pMonster)
{
    m_edicts.erase(pMonster->m_pEdict);
    delete pMonster;
}

BaseMonster* MonsterManager::GetMonster(edict_t* pEntity) const
{
    auto iter = m_edicts.find(pEntity);
    if (iter == m_edicts.end())
        return nullptr;

    return iter->second;
}

BaseMonsterClassHelper* MonsterManager::GetMonsterClass(const char* pszName)
{
    auto iter = m_helpers.find(pszName);
    if (iter == m_helpers.end())
        return nullptr;

    return iter->second;
}

void MonsterManager::Clear()
{
    for (auto const& x : m_edicts)
    {
        delete x.second;
    }

    m_edicts.clear();
}

void MonsterManager::Register(const char* pszName, BaseMonsterClassHelper* pHelper)
{
    m_helpers.insert_or_assign(pszName, pHelper);
}

void MonsterManager::Precache()
{
    for (auto const& x : m_helpers)
    {
        BaseMonster* pMonster = x.second->Instantiate();
        pMonster->Precache();
        delete pMonster;
    }
}

void MonsterManager::OnThink(edict_t* pEntity)
{
    BaseMonster* pMonster = GetMonster(pEntity);
    if (pMonster != nullptr)
        pMonster->Think();
}

void MonsterManager::OnTouch(edict_t* pEntity, edict_t* pOther)
{
    BaseMonster* pMonster = GetMonster(pEntity);
    if (pMonster != nullptr)
        pMonster->Touch(pOther);
}

void MonsterManager::OnKilled(edict_t* pVictim, edict_t* pKiller, int shouldgib)
{
    BaseMonster* pMonster = GetMonster(pVictim);
    if (pMonster != nullptr)
        pMonster->Killed(pKiller, shouldgib);
}

void MonsterManager::OnTraceAttack(edict_t* pVictim, entvars_t*& pAttacker, float& damage, Vector& dir, TraceResult*& ptr, int& damagebits)
{
    BaseMonster* pMonster = GetMonster(pVictim);
    if (pMonster != nullptr)
        pMonster->TraceAttack(pAttacker, damage, dir, ptr, damagebits);
}

MonsterManager& GetMonsterManager()
{
    static MonsterManager s_MonsterManger;
    return s_MonsterManger;
}