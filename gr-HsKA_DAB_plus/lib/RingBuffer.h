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
	T *m_Buffer;
	int m_FillCount;
	int m_Length;
	int m_WriteIndex;
public:
	RingBuffer(int N);
	~RingBuffer();
	
	bool isReady() { return (m_FillCount >= m_Length); };
	int getLength() { return m_Length; };
	
	void pushValue(T newValue);
	T getValueAt(int offset); // Offset: Wie weit zurück gegangen wird. 0 => neuester Übergabewert von pushValue().
};

template<typename T>
RingBuffer<T>::RingBuffer(int N) : m_FillCount(0), m_Length(N), m_WriteIndex(-1)
{
	m_Buffer = new T[N];
	
	for(int i = 0; i < N; ++i)
	{
		m_Buffer[i] = 0;
	}
}

template<typename T>
RingBuffer<T>::~RingBuffer()
{
	if(m_Buffer)
	{
		delete [] m_Buffer;
		m_Buffer = 0;
	}
}

template<typename T>
void RingBuffer<T>::pushValue(T newValue)
{
	if(m_FillCount < m_Length)
		++m_FillCount;
	
	++m_WriteIndex;
	
	if(m_WriteIndex >= m_Length)
		m_WriteIndex = 0;
	
	m_Buffer[m_WriteIndex] = newValue;
}

template<typename T>
T RingBuffer<T>::getValueAt(int offset)
{
	int index = m_WriteIndex - offset;
	
	while(index < 0)
		index += m_Length;
	
	while(index >= m_Length)
		index -= m_Length;
	
	return m_Buffer[index];
}
#endif