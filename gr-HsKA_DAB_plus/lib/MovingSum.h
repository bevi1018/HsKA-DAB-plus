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
- Beschreibung: gleitender Mittelwert der Größe N; Datentyp kann beliebig gewählt werden
- Quellen: keine
*/

#ifndef MOVING_SUM
#define MOVING_SUM

template<typename T>
class MovingSum
{
private:
	T *m_Buffer;
	int m_FillCount;
	int m_Length;
	int m_WriteIndex;
	T m_Sum;
public:
	MovingSum(int N);
	~MovingSum();
	
	bool isReady() { return (m_FillCount >= m_Length); };
	int getLength() { return m_Length; };
	
	T filter(T newValue);
};

template<typename T>
MovingSum<T>::MovingSum(int N) : m_FillCount(0), m_Length(N), m_WriteIndex(0), m_Sum(0)
{
	m_Buffer = new T[N];
	
	for(int i = 0; i < N; ++i)
	{
		m_Buffer[i] = 0;
	}
}

template<typename T>
MovingSum<T>::~MovingSum()
{
	if(m_Buffer)
	{
		delete [] m_Buffer;
		m_Buffer = 0;
	}
}

template<typename T>
T MovingSum<T>::filter(T newValue)
{
	if(m_FillCount < m_Length)
		++m_FillCount;
	
	m_Sum += newValue - m_Buffer[m_WriteIndex];
	m_Buffer[m_WriteIndex] = newValue;
	
	++m_WriteIndex;
	
	if(m_WriteIndex >= m_Length)
		m_WriteIndex = 0;
	
	return m_Sum;
}
#endif