#include "Card.h"

#include "Unit.h"

Card::Card()
{

}

Card::~Card()
{

}

void Card::Use(Unit& unit)
{
	switch (m_type)
	{
	case (CardType::Attack):
	{
		unit.Attack();
	}break;
	default:
		break;
	}
};
