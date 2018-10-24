// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if(len<1)
		throw -1;
	BitLen=len;
	if(BitLen%32==0)
	{
		MemLen=BitLen/32;
	}
	else
		MemLen=BitLen/32+1;
	pMem=new TELEM[MemLen];
	for(int i=0;i<MemLen;i++)
		pMem[i]=0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen=bf.BitLen;
	MemLen=bf.MemLen;
	pMem=new TELEM[MemLen];
	for(int i=0;i<MemLen;i++)
		pMem[i]=bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if(n<0||n>=BitLen)
		throw -1;
	return (n/(sizeof(TELEM)*8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if(n<0||n>=BitLen)
		throw -1;
	int sdw=n%32;
	return(1<<sdw);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	int Index=GetMemIndex(n);
	TELEM mask=GetMemMask(n);
	pMem[Index]=mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	pMem[GetMemIndex(n)]&=~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{	
  if ((pMem[GetMemIndex(n)] & GetMemMask(n)) != 0) 
	  return pMem[GetMemIndex(n)&GetMemMask(n)];
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen=bf.BitLen;
	MemLen=bf.MemLen;
	delete[] pMem;
 	pMem=new TELEM[MemLen];
	for(int i=0;i<MemLen;i++)
		pMem[i]=bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if(BitLen!=bf.BitLen)
		return 0;
	for(int i=0;i<MemLen-1;i++)
		if(pMem[i]!=bf.pMem[i])
			return 0;
	for(int i=(MemLen-1)*32;i<BitLen;i++)
		if(GetBit(i)!=bf.GetBit(i))
			return 0;
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if((*this==(bf))==1)
		return 0;
	else
		return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int len;
	if(BitLen>bf.BitLen)
		len=BitLen;
	else
		len=bf.BitLen;
	TBitField tmp(len);
	for(int i=0;i<MemLen;i++)
		tmp.pMem[i]=pMem[i];
	for(int i=0;i<bf.MemLen;i++)
		tmp.pMem[i]|=bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int len;
	if(BitLen>bf.BitLen)
		len=BitLen;
	else
		len=bf.BitLen;
	TBitField tmp(len);
	for(int i=0;i<MemLen;i++)
		tmp.pMem[i]=pMem[i];
	for(int i=0;i<bf.MemLen;i++)
		tmp.pMem[i]&=bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(BitLen);
	for(int i=0;i<MemLen;i++)
		tmp.pMem[i]=pMem[i];
		for(int i=0;i<BitLen;i++)
		{
			if(GetBit(i)==1)
				tmp.ClrBit(i);
			else
				tmp.SetBit(i);
		}
		return tmp;

}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int tmp;
	for(int i=0;i<bf.GetLength();i++)
	{
		istr>>tmp;
		if(tmp==1)
			bf.SetBit(i);
		else if (tmp==0)
			bf.ClrBit(i);
		else
			break;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for(int i=0;i<bf.GetLength();i++)
	{	
		if(bf.GetBit(i)==0)
			ostr<<"0 ";
		else
			ostr<<"1 ";
	}
	return ostr;
}
