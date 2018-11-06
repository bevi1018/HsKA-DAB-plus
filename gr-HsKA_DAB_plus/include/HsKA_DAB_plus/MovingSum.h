/*
Hochschule Karlsruhe Technik u. Wirtschaft (HsKa)
Projektarbeit Master
- Projekt: DAB+ Empfänger mit Gnu Radio
- Studenten: David Kohler, Vivian Becher
- Professor: M. Litzenburger
- Datum: 28.08.18
--------------------------------------------------
Informationen
- Bibliothek: MovingSum.h
- Hierarchieebene: untere Ebene
- Beschreibung: gleitende Summe der Größe N; Datentyp kann beliebig gewählt werden
- Quellen: keine
*/

#ifndef MOVING_SUM
#define MOVING_SUM

#include <stdint.h>

template<typename T>
class MovingSum
{
private:
	T * 	m_Buffer;		///< Buffer fuer die letzen N Werte
	int32_t m_FillCount;	///< Gibt an, wie weit der Buffer gefuellt ist
	int32_t m_Length;		///< Gibt an, wieviele Werte aufsummiert werden (N)
	int32_t m_WriteIndex;	///< Naechster Index, auf den geschrieben wird
	T   	m_Sum;			///< Die aktuelle Summe.
public:
	/**
	 * Konstruktor
	 * \param N Die Anzahl der Werte, die aufsummiert werden.
	 */
	MovingSum(int32_t N);
	
	/**
	 * Destruktor
	 */
	~MovingSum();
	
	/**
	 * Gibt an, ob der Buffer gefuellt ist und damit die Summe verwendet werden kann
	 * \return True, wenn der Buffer komplett gefuellt ist, sonst False
	 */
	bool isReady() { return (m_FillCount >= m_Length); };
	
	/**
	 * Gibt die Anzahl der Werte an, die aufsummiert werden
	 * \return Anzahl der Werte, die aufsummiert werden.
	 */
	int32_t getLength() { return m_Length; };
	
	/**
	 * Addiert einen neuen Wert auf die Summe und gibt die gleitende Summe zurueck.
	 * \param newValue Neuer Wert, der addiert werden soll
	 * \return Der neue gleitende Mittelwert
	 */
	T filter(T newValue);
};

template<typename T>
MovingSum<T>::MovingSum(int32_t N) : m_FillCount(0), m_Length(N), m_WriteIndex(0), m_Sum(0)
{
	// Buffer erzeugen
	m_Buffer = new T[N];
	
	// Buffer mit 0 initialisieren, damit die Summe später stimmt.
	for(int32_t i = 0; i < N; ++i)
	{
		m_Buffer[i] = 0;
	}
}

template<typename T>
MovingSum<T>::~MovingSum()
{
	// Den Buffer löschen
	if(m_Buffer)
	{
		delete [] m_Buffer;
		m_Buffer = 0;
	}
}

template<typename T>
T MovingSum<T>::filter(T newValue)
{
	// Wenn der Buffer noch nicht gefüllt war => Füllstand erhöhen
	if(m_FillCount < m_Length)
		++m_FillCount;
	
	// Neuer Wert auf die Summe addieren und der letze Wert subtrahieren
	m_Sum += newValue - m_Buffer[m_WriteIndex];
	
	// Neuer Wert in den Buffer schreiben
	m_Buffer[m_WriteIndex] = newValue;
	
	// Schreibindex erhöhen
	++m_WriteIndex;
	
	// Wenn der Schreibindex übergelaufen ist => Schreibindex auf 0 setzen
	if(m_WriteIndex >= m_Length)
		m_WriteIndex = 0;
	
	// Neuer gleitender Mittelwert zurückgeben
	return m_Sum;
}
#endif
