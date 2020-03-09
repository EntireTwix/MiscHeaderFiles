#pragma once
#include <Windows.h>
#include <thread>
#include <chrono>

struct Key
{
    unsigned char pos, neg;
    double posRate, idleRate, snapThreshold, limit;
    double counter = 0;
};

class Keyboard
{
private:
    Key* keys = nullptr;
public:
    int keySz = 0;
    Keyboard() = delete;
    Keyboard(std::initializer_list<Key> kys)
    {
        for (Key k : kys)
        {
            ++keySz;
        }
        keys = new Key[keySz];
        int i = 0;
        for (Key k : kys)
        {
            keys[i] = k;
            ++i;
        }
    }
    Key& operator[](int pos)
    {
        if ((pos < 0) || (pos >= keySz))
        {
            throw new std::out_of_range("can't index list of arrays with that value");
        }
        else 
        {
            return keys[pos];
        }
    }
    ~Keyboard()
    {
        if (keys != nullptr)
        {
            delete[] keys;
        }
        else
        {
            delete keys;
        }
    }
};

//to be called with new thread
void KeyBoardHandler(Keyboard* kybrd, bool* end)
{
    Key temp;
    std::chrono::high_resolution_clock::time_point t1;

    if ((kybrd == nullptr) || (end == nullptr)) throw std::exception("Null exception: kybrd and/or end can't be null for this function");

    while (*end)
    {
        //start timer
        t1 = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < kybrd->keySz; ++i)
        {
            temp = kybrd->operator[](i);

            if (kybrd->operator[](i).counter >= kybrd->operator[](i).limit)
            {
                kybrd->operator[](i).counter = temp.limit;
            }
            if (kybrd->operator[](i).counter <= -kybrd->operator[](i).limit)
            {
                kybrd->operator[](i).counter = -temp.limit;
            }

            if ((GetKeyState(temp.pos) & 0x80) || (GetKeyState(temp.neg) & 0x80)) {
                if (GetKeyState(temp.pos) & 0x80)
                {
                    kybrd->operator[](i).counter += temp.posRate;
                }
                if (GetKeyState(temp.neg) & 0x80)
                {
                    kybrd->operator[](i).counter -= temp.posRate;
                }
            }
            else
            {
                if ((temp.counter >= - temp.snapThreshold) && (temp.counter <= temp.snapThreshold))
                {
                    kybrd->operator[](i).counter = 0;
                }
                else if (temp.counter > temp.snapThreshold)
                {
                    if ((temp.counter - temp.idleRate) < 0)
                    {
                        kybrd->operator[](i).counter = 0;
                    }
                    else
                    {
                        kybrd->operator[](i).counter -= temp.idleRate;
                    }
                }
                else if (temp.counter < temp.snapThreshold)
                {
                    if ((temp.counter - temp.idleRate) > 0)
                    {
                        kybrd->operator[](i).counter = 0;
                    }
                    else
                    {
                        kybrd->operator[](i).counter += temp.idleRate;
                    }
                }
            }
        }

        //maintain loop duration so that its not effected by cpu performance (ie:speeding up or slowing)
        while ((std::chrono::duration_cast<std::chrono::duration<double> >(std::chrono::high_resolution_clock::now() - t1)).count() <= 0.016);
    }
}
