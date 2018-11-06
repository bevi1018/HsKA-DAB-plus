/*
Hochschule Karlsruhe Technik u. Wirtschaft (HsKa)
Projektarbeit Master
- Projekt: DAB+ Empfänger mit Gnu Radio
- Studenten: David Kohler, Vivian Becher
- Professor: M. Litzenburger
- Datum: 28.08.18
--------------------------------------------------
Informationen
- Bibliothek: RingBuffer.h
- Hierarchieebene: untere Ebene
- Beschreibung: Ringbuffer der Größe N; Datentyp kann beliebig gewählt werden
- Quellen: keine
*/

#ifndef RING_BUFFER
#define RING_BUFFER

template<typename T>
class RingBuffer
{
private:
	T * m_Buffer;		///< Buffer fuer die letzen N Werte
	int m_FillCount;	///< Gibt den Fuellstand des Buffers an
	int m_Length;		///< Gibt die Laenge des Ringbuffers an
	int m_WriteIndex;	///< Index fuer den naechsten zu schreibende Wert
public:
	/**
	 * Konstruktor
	 * \param N Gibt die Laenge des Ringbuffers an
	 */
	RingBuffer(int N);
	
	/**
	 * Destruktor
	 */
	~RingBuffer();
	
	/**
	 * Gibt an, ob der Ringbuffer komplett gefuellt ist (erst dann sind alle Werte gueltig
	 * \return True, wenn der Ringbuffer komplett gefuellt ist, sonst False
	 */
	bool isReady() { return (m_FillCount >= m_Length); };
	
	/**
	 * Gibt die Laenge des Ringbuffers zurueck
	 * \return Laenge des Ringbuffers
	 */
	int getLength() { return m_Length; };
	
	/**
	 * Speichert einen neuen Wert in den Ringbuffer. Der letzte Wert wird dabei geloescht.
	 * \param newValue Neuer Wert, der in den Ringbuffer geschrieben wird.
	 */
	void pushValue(T newValue);
	
	/**
	 * Gibt den Wert zurueck, der mit dem pushValue()-Aufruf uebergeben wurde, der offset zurueck liegt
	 * \param offset 0 => neuester Uebergabewert von pushValue(). getLength() - 1: aeltester Wert.
	 * \return
	 */
	T getValueAt(int offset);
};

template<typename T>
RingBuffer<T>::RingBuffer(int N) : m_FillCount(0), m_Length(N), m_WriteIndex(-1)
{
	// Buffer für die Werte erzeugen
	m_Buffer = new T[N];
	
	// Bufferwerte auf 0 setzen
	for(int i = 0; i < N; ++i)
	{
		m_Buffer[i] = 0;
	}
}

template<typename T>
RingBuffer<T>::~RingBuffer()
{
	// Buffer löschen
	if(m_Buffer)
	{
		delete [] m_Buffer;
		m_Buffer = 0;
	}
}

template<typename T>
void RingBuffer<T>::pushValue(T newValue)
{
	// Wenn der Buffer noch nicht gefüllt war => Füllstandszähler erhöhen
	if(m_FillCount < m_Length)
		++m_FillCount;
	
	// Schreibindex erhöhen
	++m_WriteIndex;
	
	// Wenn der Schreibindex übergelaufen ist => Schreibindex auf 0 setzen
	if(m_WriteIndex >= m_Length)
		m_WriteIndex = 0;
	
	// Neuer Wert in den Buffer schreiben
	m_Buffer[m_WriteIndex] = newValue;
}

template<typename T>
T RingBuffer<T>::getValueAt(int offset)
{
	// Echter Leseindex berechnen
	int index = m_WriteIndex - offset;
	
	// Den Bereich des Leseindex anpassen
	while(index < 0)
		index += m_Length;
	
	while(index >= m_Length)
		index -= m_Length;
	
	// Den jeweiligen Wert zurückgeben
	return m_Buffer[index];
}
#endif
