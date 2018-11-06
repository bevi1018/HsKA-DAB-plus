#ifndef PY_CALLBACK_H
#define PY_CALLBACK_H

#include <HsKA_DAB_plus/api.h>

/**
 * Klasse, die einen Python Callback darstellt, um z.B. einen GnuRadio Parameter zu ändern
 * Von dieser Klasse in Python ableiten
 */
class HSKA_DAB_PLUS_API py_callback
{
public:
	/**
	 * Virtueller Destruktor
	 */
	virtual ~py_callback();
	
	/**
	 * Virtuelle Funktion für den Aufruf der set-Funktion
	 */
	virtual void call(float value);
};

#endif
