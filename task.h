#pragma once
#include <string>
#include <memory>
#include <vector>
#include <algorithm>  

//abstract
struct Task
{
	bool prioritized = false;
	float priority = 0;

	virtual bool operator<(const Task& t)
	{
		return (this->priority < t.priority);
	}

	//constructor for text file
	virtual void Import(const std::vector<std::string>& params) = 0;
};

//abstract
class PriorityFunctor
{
public:
	//this throwing an error would be it returning 0
	virtual float operator()(Task* t) const { return 0; }
};

//concrete but can still have children
class TaskContainer
{
private:
	std::vector<Task*> tasks;
	PriorityFunctor* pFunc;
public:
	TaskContainer() = delete;
	TaskContainer(std::vector<Task*> tasks, PriorityFunctor* p)
	{
		this->tasks = tasks;
		this->pFunc = p;
	}
	virtual void PrioritizeAll()
	{
		for (auto t : tasks)
		{
			if (this->pFunc->operator()(t) != 0)
			{
				t->prioritized = true;
				t->priority = this->pFunc->operator()(t);
			}
		}
	}

	std::vector<Task*> GetPrioritizedTasks() const
	{
		std::vector<Task*> res;
		for (auto t : this->tasks)
		{
			if (t->prioritized)
			{
				res.push_back(t);
			}
		}
		std::sort(begin(res), end(res));
		return res;
	}

	void Add(Task* t) noexcept
	{
		this->tasks.push_back(t);
	}
	void Remove(Task* t) noexcept
	{
		for (int i = 0; i < tasks.size(); ++i)
		{
			if (t == this->tasks[i])
			{
				if (tasks.size() != 1)
				{
					this->tasks[i] = this->tasks[this->tasks.size() - 1];
					this->tasks.resize(this->tasks.size() - 1);
				}
				else
				{
					this->tasks.resize(0);
				}
				break;
			}
		}
	}

	~TaskContainer()
	{
		for (auto t : tasks)
		{
			delete t;
		}
		delete pFunc;
	}
};