#pragma once
#include <ostream>
#include <string>
#include <vector>

template <size_t numberOfBits = 8>
    class InfiniteInt
    {
        private:
            std::vector<bool> bits;

            //member functions
            void IncrementBit(size_t pos)
            {
                if( (pos >= bits.size()) || (pos < 0) ) throw std::out_of_range("indexing bits must be within the range of bits size");
                if(bits[pos])
                {
                    bits[pos] = false;
                    if(pos+1 != bits.size()) IncrementBit(pos+1);
                }
                else
                {
                    bits[pos] = true;
                }
            }
            void DecrementBit(size_t pos)
            {
                if( (pos >= bits.size()) || (pos < 0) ) throw std::out_of_range("indexing bits must be within the range of bits size");
                if(bits[pos])
                {
                    bits[pos] = false;
                }
                else
                {
                    bits[pos] = true;
                    if(pos+1 != bits.size()) DecrementBit(pos+1);
                }  
            }
            
        public:
            //constructors
            InfiniteInt() { bits.resize(numberOfBits); }

            InfiniteInt operator=(unsigned long long int x)
            {
                for(unsigned long long int i = 0; i < x; ++i)
                {
                    IncrementBit(0);
                }
            }

            //member functions
            InfiniteInt operator++() { IncrementBit(0); return *this; }
            InfiniteInt operator--() { DecrementBit(0); return *this; }
            InfiniteInt operator+(unsigned long long x) const { auto temp(*this); for(int i  = 0; i < x; ++i) ++temp; return temp;}
            InfiniteInt operator-(unsigned long long x) const { auto temp(*this); for(int i  = 0; i < x; ++i) --temp; return temp;}
            auto ToNumber() const //messiest implmenation possible but alteast it works
            {
                struct
                {
                    unsigned res : numberOfBits = 0;
                }output;
                InfiniteInt<numberOfBits> temp = *this;
                auto func = [](const std::vector<bool>& bools){ for(auto b : bools) if (b) {return false;} return true; };
                while(!func(temp.bits))
                {
                    --temp;
                    ++output.res;
                }

                return output.res;
            }
            std::string ToBinary() const
            {
                std::string res;
                for(int i = bits.size()-1; i >= 0; --i)
                {
                    res+=std::to_string(bits[i]);
                }
                return res;
            }
            size_t size() const { return bits.size(); }
            
            //other functions
            template <size_t sz>
            friend std::ostream& operator<<(std::ostream& os, const InfiniteInt<sz>& number)
            {
                os<<number.ToNumber();
                return os;
            }
            
        //TODO:
            //more arithmetic functions
            //int related constructors
    };
