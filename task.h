#pragma once
#include <string>
#include <memory>
#include <vector>
#include <algorithm>  

struct Task //abstract
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

template <typename T> // T should be type of Task
class PriorityFunctor //abstract
{
public:
	//this throwing an error would be it returning 0
	virtual float operator()(T t) const { return 0; }
};

template <typename T>
class TaskContainer  //concrete but can still have children
{
private:
	std::vector<T> tasks;
	PriorityFunctor<T>* pFunc;
public:
	TaskContainer() = delete;
	TaskContainer(PriorityFunctor<T>* p)
	{
		this->pFunc = p;
	}
	TaskContainer(std::vector<T> tasks, PriorityFunctor<T>* p)
	{
		this->tasks = tasks;
		this->pFunc = p;
	}
	virtual void PrioritizeAll()
	{
		for (auto& t : tasks)
		{
			if (this->pFunc->operator()(t) != 0)
			{
				t.prioritized = true;
				t.priority = this->pFunc->operator()(t);
			}
		}
	}

	std::vector<T> GetPrioritizedTasks() const
	{
		std::vector<T> res;
		for (T t : this->tasks)
		{
			if (t.prioritized)
			{
				res.push_back(t);
			}
		}
		std::sort(begin(res), end(res));
		return res;
	}

	void Add(const T& t) noexcept
	{
		this->tasks.push_back(t);
	}
	void Remove(const T& t) noexcept
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
		delete pFunc;
	}
};
