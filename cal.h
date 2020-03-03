#pragma once
#include <ctime>
#include <random>
#include <vector>
#include <unordered_map>
#include "date.h"
#include "parse.h"

namespace Calander {

    struct EventInstance sealed
    {
        string name = "empty";
        date::Date d;
        unsigned short int time;
        unsigned int ID;
        bool isDone;
    };

    class Event sealed
    {
    private:
        string name = "NULL";
        date::Date start;
        date::Date end;
        unsigned short int timeTotal = 0;
        unsigned int GetHash()
        {
            unsigned long int sum = 0;

            sum += start;
            sum += end;
            sum += timeTotal;

            int endOfStr = name.size();
            if (endOfStr > 7) endOfStr = 7;
            for (int i = 0; i < endOfStr; ++i) {
                sum += name.at(i);
            }

            return sum;
        }

    public:
        unsigned int ID = NULL;
        bool preserveTime = false;

        Event() = delete;
        Event(string n, date::Date s, date::Date e, unsigned short int t, bool preserved)
        {
            if (s > e) throw new exception("start date can't be after end");
            name = n;
            start = s;
            end = e;
            timeTotal = t;
            preserveTime = preserved;
            ID = GetHash();
        }
        vector<EventInstance> GetInstances() const //for pre-distribution of a new event
        {
            vector<EventInstance> result;
            for (int i = start+1; i <= end+2; ++i) {
                result.push_back(EventInstance{ name, date::Date(i), (unsigned short int)(timeTotal / (end - start)), ID, 0 });
            }
            return result;
        }

    };

    class CalanderContainer sealed
    {
    private:
        std::unordered_map<unsigned short int, Calander::Event> references;
        std::vector < Calander::EventInstance > instances;

    public:
        CalanderContainer() {
            Generate();
        }
        void Generate()
        {
            instances.resize(365);        
        }
        //void Import(string filename); //finish
        void Save(string filename) //for saving all events to txt file
        {
            std::vector< std::vector<std::string> > contents(instances.size());
            for (int i = 0; i < instances.size(); ++i) {
                if (instances[i].name != "empty") {
                    contents[i].push_back(instances[i].name);
                    contents[i].push_back(to_string(instances[i].d.day()) +  ' ' +  to_string(instances[i].d.month())+ ' ' + to_string(instances[i].d.year()) );
                    contents[i].push_back(to_string(instances[i].ID));
                    contents[i].push_back(to_string(instances[i].time));
                }
            }
            TextParser::Save(filename, contents, ' ', 4);
            //add reference saving
        }
        void Distribute(const Event& e) //for distributing new events
        {
            vector<EventInstance> inst = e.GetInstances();
            for (int i = 0; i < inst.size(); ++i) {
                Distribute(inst[i]);
            }
            references.insert({ e.ID, e });
        }
        void Distribute(const EventInstance& e) //for re-distributing previously saved event instances
        {
            instances[(e.d - date::Date(0, 1, e.d.year())) - 1] = e;
        }
    };

}