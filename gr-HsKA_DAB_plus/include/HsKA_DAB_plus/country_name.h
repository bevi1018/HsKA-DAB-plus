#ifndef COUNTRY_NAME_H
#define COUNTRY_NAME_H

#include <stdint.h>

namespace CountryName
{
	void get_country_name(uint8_t ecc, uint8_t id, char *name);
}

#endif