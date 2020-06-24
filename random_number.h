#pragma once
#include <random>
#include <stdexcept>

unsigned Random(unsigned lowest, unsigned highest)
{
	return ((rand()%(unsigned)(highest))+offset);
}
