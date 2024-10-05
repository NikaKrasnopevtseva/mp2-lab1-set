// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <cstring>
#include <algorithm>
#define KolBits (sizeof(TELEM) * 8)
TBitField::TBitField(int len)
{
    if (len < 0) throw "len < 0";
    BitLen = len;
    MemLen = BitLen / KolBits + 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < (MemLen); i++) pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return (n / KolBits);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n > BitLen) {
        throw "Invalid bit index";
    }
    int k = n % KolBits;
    return (1 << k);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n > BitLen) {
        throw "Invalid bit index";
    }
    TELEM index = GetMemIndex(n);
    pMem[index] = pMem[index] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n > BitLen) {
        throw "Invalid bit index";
    }
    TELEM index = GetMemIndex(n);
    pMem[index] = pMem[index] & ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n > BitLen) {
        throw "Invalid bit index";
    }
    TELEM index = GetMemIndex(n);
    return (pMem[index] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    delete[] pMem;
    this->BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (MemLen != bf.MemLen) {
        return 0;
    }
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return 0;
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return ~((*this) == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    TBitField or(max(BitLen, bf.BitLen));
    if (MemLen == bf.MemLen)
    for (int i = 0; i <= or.MemLen; i++) or.pMem[i] = bf.pMem[i] | pMem[i];
    else
    {
        for (int i = 0; i <= or.MemLen; i++) {
            if (i < MemLen && i >= bf.MemLen) or.pMem[i] = pMem[i];
                else
                    if (i >= MemLen && i < bf.MemLen) or.pMem[i] = bf.pMem[i];
        }
    }
    return or; 
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    TBitField and (max(BitLen, bf.BitLen));
    if (MemLen == bf.MemLen)
        for (int i = 0; i <= and.MemLen; i++) and.pMem[i] = bf.pMem[i] & pMem[i];
    else
    {
        for (int i = 0; i <= and.MemLen; i++) {
            if (i < MemLen && i >= bf.MemLen) and.pMem[i] = pMem[i];
            else
                if (i >= MemLen && i < bf.MemLen) and.pMem[i] = bf.pMem[i];
        }
    }
    return and;
}

TBitField TBitField::operator~(void) // отрицание
{
 TBitField not(BitLen);
  for (int i = 0; i < BitLen; i++) {
      if (this->GetBit(i)) not.ClrBit(i);
      else not.SetBit(i);
   }
  return not;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    int len,bit;
    cout << "Enter the Bitlen" << endl;
    cin >> len;
    bf = TBitField(len);
    cout << "Enter the BitField" << endl;
    for (int i = 0; i < len; ++i) {
        cin >> bit;
        if (bit == 1)
            bf.SetBit(i);
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    cout << "BitField:" << endl;
    for (TELEM i = 0; i < bf.BitLen; i++)
    {
        if (bf.GetBit(i)) cout << i << " ";
    }
    cout<<endl;
    return ostr;
}
