#include <HsKA_DAB_plus/py_callback.h>
#include <stdio.h>

// Defaultimplementierungen, damit die Klasse im Pythonmodul sichbar ist
py_callback::~py_callback()
{
}
	
void py_callback::call(float value)
{
	printf("WARNING: Python-Callback was not called!\n");
}