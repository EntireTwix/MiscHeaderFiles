#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <ostream>

class Hoffman final
{
private:
    struct Node
    {
        //constructor
        Node() = default;
        Node(char c, size_t s) : character(c), sum(s) { }
        Node(int p, int lc, int rc)
        {
            parent = p;
            leftChild = lc;
            rightChild = rc;
        }

        //functions
        size_t GetSum(const std::vector<Node>& nodes) const
        {
            return sum + (leftChild != -1 ? nodes[leftChild].GetSum(nodes) : 0) + (rightChild != -1 ? nodes[rightChild].GetSum(nodes) : 0);  
        }
        friend std::ostream& operator<<(std::ostream& os, const Node& n)
        {
            return os<<n.character<<','
            <<n.sum<<','
            <<std::to_string(n.branched)<<',';
        }
        std::vector<bool> GetRoute(const std::vector<Node>& nodes) const
        {
            std::vector<bool> choices;

            //add this choice
            if(nodes[nodes[parent].leftChild].character == this->character) 
            {
                choices.push_back(0);
            }
            else
            {
                choices.push_back(1);
            }
            

            if(parent != (nodes.size()-1) )
            {
                //add parents choices
                for(auto c : nodes[parent].GetRoute(nodes))
                {
                    choices.push_back(c);
                }
            }
            
            return choices;
        }

        //members
        char character = 0;
        size_t sum = 0;
        bool branched = false;

        int parent = -1, leftChild = -1, rightChild = -1;
    };

    struct NodeComparisonFunctor
    {
        NodeComparisonFunctor(const std::vector<Node>& nodes)
        {
            this->nodes = nodes;
        }
        bool operator()(int a, int b)
        {
            return nodes[a].GetSum(nodes) < nodes[b].GetSum(nodes);
        }
        std::vector<Node> nodes;
    };

    static bool Increment(std::vector<Node>& nodes, char c)
    {
        for (auto& n : nodes)
        {
            if (n.character == c)
            {
                ++n.sum;
                return true;
            }
        }
        return false;
    }

    static size_t UnBranched(const std::vector<Node>& nodes)
    {
        size_t sum = 0;
        for (auto n : nodes)
        {
            if (!n.branched) ++sum;
        }
        return sum;
    }

public:
    static std::string NavigateTree(std::vector<bool> path, const std::vector<Node>& nodes)
    {
        size_t location = nodes.size()-1;
        std::string result="";
        while(path.size()!=0)
        {
            if(path[path.size()-1])
            {
                //right path
                location = nodes[location].rightChild;
            }
            else
            {
                //left path 
                location = nodes[location].leftChild;
            }
            if(nodes[location].character != 0)
            {
                result+=nodes[location].character;

                //reset at tree start
                location = nodes.size()-1;
            }  
            path.resize(path.size()-1);
        }
        std::reverse(begin(result), end(result));
        return result;
    }

    static std::vector<bool> Compress(const std::string& contents) //for text compression
    {
        //popullating nodes
        std::vector<Node> nodes;
        for (auto c : contents)
        {
            if (!Increment(nodes, c)) nodes.push_back(Node(c, 1));
        }
        size_t originalSize = nodes.size();

        //making into tree
        {
            std::vector<int> NodesCopy;
            while (UnBranched(nodes) > 1)
            {
                //copying over non branched nodes
                for (int i = 0; i < nodes.size(); ++i)
                {
                    if (!nodes[i].branched) NodesCopy.push_back(i);
                }

                //sorting said indexes from least to greatest
                std::sort(begin(NodesCopy), end(NodesCopy), NodeComparisonFunctor(nodes));
                nodes.push_back(Node(nodes.size(), NodesCopy[0], NodesCopy[1]));
                for (int i = 0; i <= 1; ++i)
                {
                    nodes[NodesCopy[i]].branched = true;
                    nodes[NodesCopy[i]].parent = nodes.size() - 1;
                }
                NodesCopy.resize(0);
            }
        }
        //now the last member of vector is the trees stump, and the first few ${originalSize} members
        
        std::vector<bool> output, vec;
        for(auto c : contents)
        {
            for(auto n : nodes) 
            {
                if(n.character == c) 
                {
                    vec = n.GetRoute(nodes);
                    std::reverse(begin(vec), end(vec));
                    for(auto b : vec)
                    {
                        output.push_back(b);
                    }
                }
            }
        }
        return output;
    }

};
