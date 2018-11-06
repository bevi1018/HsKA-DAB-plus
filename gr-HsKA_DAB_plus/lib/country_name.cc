#include <HsKA_DAB_plus/country_name.h>
#include <string.h>

namespace CountryName
{
	void get_country_name(uint8_t ecc, uint8_t id, char *name)
	{
		// Leerer Standardtext
		name[0] = 0;
		
		switch(ecc)
		{
			case 0xE0:
				{
					switch(id)
					{
						case 0x1:
						case 0xD:
							strcpy(name, "Germany");
							break;
						case 0x2:
							strcpy(name, "Algeria");
							break;
						case 0x3:
							strcpy(name, "Andorra");
							break;
						case 0x4:
							strcpy(name, "Israel");
							break;
						case 0x5:
							strcpy(name, "Italy");
							break;
						case 0x6:
							strcpy(name, "Belgium");
							break;
						case 0x7:
							strcpy(name, "Russian Federation");
							break;
						case 0x8:
							strcpy(name, "Azores (Portugal)/Palestine");
							break;
						case 0x9:
							strcpy(name, "Albania");
							break;
						case 0xA:
							strcpy(name, "Austria");
							break;
						case 0xB:
							strcpy(name, "Hungary");
							break;
						case 0xC:
							strcpy(name, "Malta");
							break;
						case 0xE:
							strcpy(name, "Canaries (Spain)");
							break;
						case 0xF:
							strcpy(name, "Egypt");
							break;
					}
				}
				break;
				
			case 0xE1:
				{
					switch(id)
					{
						case 0x1:
							strcpy(name, "Greece");
							break;
						case 0x2:
							strcpy(name, "Cyprus");
							break;
						case 0x3:
							strcpy(name, "San Marino");
							break;
						case 0x4:
							strcpy(name, "Switzerland");
							break;
						case 0x5:
							strcpy(name, "Jordan");
							break;
						case 0x6:
							strcpy(name, "Finland");
							break;
						case 0x7:
							strcpy(name, "Luxembourg");
							break;
						case 0x8:
							strcpy(name, "Bulgaria");
							break;
						case 0x9:
							strcpy(name, "Denmark");
							break;
						case 0xA:
							strcpy(name, "Gibraltar (UK)");
							break;
						case 0xB:
							strcpy(name, "Iraq");
							break;
						case 0xC:
							strcpy(name, "United Kingdom");
							break;
						case 0xD:
							strcpy(name, "Libya");
							break;
						case 0xE:
							strcpy(name, "Romania");
							break;
						case 0xF:
							strcpy(name, "France");
							break;
					}
				}
				break;
				
			case 0xE2:
				{
					switch(id)
					{
						case 0x1:
							strcpy(name, "Morocco");
							break;
						case 0x2:
							strcpy(name, "Czech Republic");
							break;
						case 0x3:
							strcpy(name, "Poland");
							break;
						case 0x4:
							strcpy(name, "Vatican");
							break;
						case 0x5:
							strcpy(name, "Slovakia");
							break;
						case 0x6:
							strcpy(name, "Syria");
							break;
						case 0x7:
							strcpy(name, "Tunisia");
							break;
						case 0x8:
							strcpy(name, "Madeira");
							break;
						case 0x9:
							strcpy(name, "Liechtenstein");
							break;
						case 0xA:
							strcpy(name, "Iceland");
							break;
						case 0xB:
							strcpy(name, "Monaco");
							break;
						case 0xC:
							strcpy(name, "Lithuania");
							break;
						case 0xD:
							strcpy(name, "Serbia");
							break;
						case 0xE:
							strcpy(name, "Spain");
							break;
						case 0xF:
							strcpy(name, "Norway");
							break;
					}
				}
				break;
				
			case 0xE3:
				{
					switch(id)
					{
						case 0x1:
							strcpy(name, "Montenegro");
							break;
						case 0x2:
							strcpy(name, "Ireland");
							break;
						case 0x3:
							strcpy(name, "Turkey");
							break;
						case 0x5:
							strcpy(name, "Tajikistan");
							break;
						case 0x8:
							strcpy(name, "Netherlands");
							break;
						case 0x9:
							strcpy(name, "Latvia");
							break;
						case 0xA:
							strcpy(name, "Lebanon");
							break;
						case 0xB:
							strcpy(name, "Azerbaijan");
							break;
						case 0xC:
							strcpy(name, "Croatia");
							break;
						case 0xD:
							strcpy(name, "Kazakhstan");
							break;
						case 0xE:
							strcpy(name, "Sweden");
							break;
						case 0xF:
							strcpy(name, "Belarus");
							break;
					}
				}
				break;
				
			case 0xE4:
				{
					switch(id)
					{
						case 0x1:
							strcpy(name, "Moldova");
							break;
						case 0x2:
							strcpy(name, "Estonia");
							break;
						case 0x3:
							strcpy(name, "Kyrgyzstan/Macedonia");
							break;
						case 0x4:
							strcpy(name, "Ukraine");
							break;
						case 0x7:
							strcpy(name, "Kosovo");
							break;
						case 0x8:
							strcpy(name, "Portugal");
							break;
						case 0x9:
							strcpy(name, "Slovenia");
							break;
						case 0xA:
							strcpy(name, "Armenia");
							break;
						case 0xB:
							strcpy(name, "Uzbekistan");
							break;
						case 0xC:
							strcpy(name, "Georgia");
							break;
						case 0xE:
							strcpy(name, "Turkmenistan");
							break;
						case 0xF:
							strcpy(name, "Bosnia Herzegovina");
							break;
					}
				}
				break;
		}
	}
}