#include "ernaerungsplan.h"

int Ernaerungsplan::BerechneKaloriengehalt()
{
    return 0;
}

Ernaerungsplan::Ernaerungsplan()
{
}

Ernaerungsplan::~Ernaerungsplan()
{
    for (auto m : mahlzeiten) delete m;
}

int Ernaerungsplan::GetID()
{
    return this->id_;
}

int Ernaerungsplan::GetKaloriengehalt()
{
    return this->kaloriengehalt;
}
