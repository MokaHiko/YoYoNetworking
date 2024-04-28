#pragma once

#include <Core/Memory.h>

enum class CardType
{
    Attack, 
    Defense,
};

class Unit;
class Card
{
public:
    Card();
    ~Card();

    virtual void Use(Unit& unit);
    virtual void Animate() {};

    CardType GetType() const {return m_type;}
    void SetType(CardType type) {m_type = type;}
private:
    uint32_t m_energy_cost;
    std::string m_description;

    CardType m_type;
};