//Muhammad Shayaan Nofil
//i210450@nu.edu.pk

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

template <class T, class F>
struct pa {
    T first;
    F second;
};

template<class T>
class adjlist
{
    T* arr;
    char s, c;
    void extend()
    {
        T* narr = new T[this->c * 2];
        for (int i = 0; i < this->s; i++)
        {
            narr[i] = this->arr[i];
        }
        delete[]this->arr;
        this->arr = narr;
        this->c *= 2;
    }
public:
    adjlist(const adjlist<T>& obj)
    {
        *this = obj;
    }
    adjlist()
    {
        this->s = 0;
        this->c = 16;
        this->arr = new T[c];
    }
    adjlist(unsigned n, const T& v)
    {
        this->assign(n, v);
    }
    ~adjlist()
    {
        if (this->arr) { delete[]this->arr; }
    }
    void assign(unsigned n, const T& v)
    {
        this->s = n;
        this->c = 1;
        while (c < n) { c *= 2; }
        this->arr = new T[c];
        for (unsigned int i = 0; i < this->s; i++) { this->arr[i] = v; }
    }
    adjlist<T>& operator=(const adjlist<T>& obj)
    {
        this->s = obj.s;
        this->c = obj.c;

        this->arr = new T[this->c];
        for (unsigned i = 0; i < this->s; i++) {
            this->arr[i] = obj.arr[i];
        }
        return *this;
    }
    void push_back(const T& v)
    {
        if (this->s == this->c) {
            this->extend();
        }
        arr[this->s++] = v;
    }
    void pop_back()
    {
        this->s--;
    }
    T& back()
    {
        return this->arr[this->s - 1];
    }
    T& operator[](unsigned idx)
    {
        return *(this->arr + idx);
    }
    char size()
    {
        return this->s;
    }
};

adjlist<string> split(string s, char c)
{
    string tmp = "";
    adjlist<string> out;
    for (int i = 0; s[i]; i++)
    {
        if (s[i] == c)
        {
            out.push_back(tmp);
            tmp.clear();
        }
        else
        {
            tmp.push_back(s[i]);
        }
    }
    out.push_back(tmp);
    return out;
}

class Graph
{
    adjlist<adjlist<pa<int, int>>> al;
    adjlist<string> nodeNames;
    bool needComputation = true;
    adjlist<adjlist<int>> sDist;
    adjlist<adjlist<int>> path;

    int getNodeNum(const string& nodeName)
    {
        for (int i = 0; i < nodeNames.size(); i++)
        {
            if (nodeNames[i] == nodeName)return i;
        }
        return -1;
    }
    void remNode(int num)
    {
        for (int i = num; i < al.size(); i++)
        {
            nodeNames[i] = nodeNames[i + 1];
            adjlist<pa<int, int>> tmp;
            i++;
            for (int j = 0; j < al[i].size(); j++)
            {
                if (al[i][j].first != num)
                {
                    tmp.push_back({ al[i][j].first, al[i][j].second });
                    if (al[i][j].first > num)
                    {
                        tmp.back().first--;
                    }
                }
            }
            i--;
            al[i] = tmp;
        }
        al.pop_back();
        nodeNames.pop_back();
    }
    void computeShortest()
    {
        int n = this->al.size();
        this->sDist.assign(n, adjlist<int>(n, 1e9 + 7));
        this->path.assign(n, adjlist<int>(n, -1));
        for (int i = 0; i < n; i++)
        {
            this->sDist[i][i] = 0;
            this->path[i][i] = i;
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < al[i].size(); j++)
            {
                sDist[i][al[i][j].first] = al[i][j].second;
                path[i][al[i][j].first] = al[i][j].first;
            }
        }
        for (int k = 0; k < n; k++)
        {
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    if (this->sDist[i][k] + this->sDist[k][j] < this->sDist[i][j])
                    {
                        this->sDist[i][j] = this->sDist[i][k] + this->sDist[k][j];
                        this->path[i][j] = this->path[i][k];
                    }
                }
            }
        }
        this->needComputation = false;
    }
    bool isNum(const string& s)
    {
        for (int i = 0; s[i]; i++) {
            if (s[i] < '0' || s[i]>'9') {
                return false;
            }
        }
        return true;
    }
public:
    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;
    Graph() {}
    Graph(const string& filename)
    {
        ifstream mfile(filename);
        string s;
        getline(mfile, s);
        adjlist<string> heading = split(s, '\t');
        for (int i = 1; i < heading.size(); i++)
        {
            addNode(heading[i]);
        }
        int n = this->al.size();
        for (int i = 1; i <= n; i++)
        {
            getline(mfile, s);
            adjlist<string> data = split(s, '\t');
            for (int j = 1; j < data.size(); j++)
            {
                int weight = stoi(data[j]);
                if (weight > 0) {
                    addEdge(data[0], heading[j], weight);
                }
            }
        }
    }
    void addNode(const string& name)
    {
        this->nodeNames.push_back(name);
        this->al.push_back(adjlist<pa<int, int>>());
    }
    void removeNode(const string& name)
    {
        int nodeNum = getNodeNum(name);
        if (nodeNum != -1) {
            this->remNode(getNodeNum(name));
        }
        this->needComputation = true;
    }
    void addEdge(const string& name1, const string& name2, int weight)
    {
        int nodeNum1 = getNodeNum(name1);
        int nodeNum2 = getNodeNum(name2);
        if (nodeNum1 != -1 && nodeNum2 != -1) {
            this->al[nodeNum1].push_back({ nodeNum2, weight });
        }
        this->needComputation = true;
    }
    string ShortestRoutefrompair(const string& node1, const string& node2)
    {
        int nodeNum1 = getNodeNum(node1);
        int nodeNum2 = getNodeNum(node2);
        if (nodeNum1 == -1 || nodeNum2 == -1) {
            return "";
        }
        if (this->needComputation) { this->computeShortest(); }
        int dist = this->sDist[nodeNum1][nodeNum2];
        string path = "";
        if (dist == 1e9 + 7)
        {
            cout << "Path length infinity: " << node1 << " to " << node2 << '\n';
        }
        else
        {
            int start = nodeNum1;
            while (start != nodeNum2)
            {
                path += nodeNames[start] + "-> ";
                start = this->path[start][nodeNum2];
            }
            path += nodeNames[nodeNum2];
            cout << "Path length " << dist << ": " << path << '\n';
        }
        return path;
    }
    string secondShortestRoutefrompair(const string& node1, const string& node2)
    {
        int nodeNum1 = this->getNodeNum(node1), nodeNum2 = this->getNodeNum(node2);
        if (nodeNum1 == -1 || nodeNum2 == -1)
        {
            return "";
        }
        if (this->needComputation) { this->computeShortest(); }
        int length = 1e9 + 7;
        string path = "";
        int sDist = this->sDist[nodeNum1][nodeNum2];
        if (sDist == 1e9 + 7)
        {
            cout << "! Path length infinity: " << node1 << " to " << node2 << '\n';
        }
        else
        {
            int start = nodeNum1;
            int cl = 0;
            while (start != nodeNum2)
            {
                int add = -1;
                for (int i = 0; i < this->al[start].size(); i++)
                {
                    int neighbour = this->al[start][i].first;
                    int weight = this->al[start][i].second;
                    if (neighbour == this->path[start][nodeNum2])
                    {
                        add = weight;
                        continue;
                    }
                    length = min(length, cl + weight + this->sDist[neighbour][nodeNum2]);
                }
                if (add == -1) { cout << "ERROR\n"; }
                cl += add;
                start = this->path[start][nodeNum2];
            }
            start = nodeNum1;
            cl = 0;
            bool end = false;
            while (start != nodeNum2)
            {
                int add = -1;
                path += this->nodeNames[start] + "-> ";
                for (int i = 0; i < al[start].size(); i++)
                {
                    int neighbour = this->al[start][i].first;
                    int weight = this->al[start][i].second;
                    if (neighbour == this->path[start][nodeNum2])
                    {
                        add = weight;
                        continue;
                    }
                    if (length == cl + weight + this->sDist[neighbour][nodeNum2])
                    {
                        int ss = neighbour;
                        while (ss != nodeNum2)
                        {
                            path += this->nodeNames[ss] + "-> ";
                            ss = this->path[ss][nodeNum2];
                        }
                        path += this->nodeNames[ss];
                        end = true;
                        break;
                    }
                }
                if (end) { break; }
                if (add == -1) { cout << "ERROR\n"; }
                cl += add;
                start = this->path[start][nodeNum2];
            }
            cout << "! Path length " << length << ": " << path << '\n';
        }
        return path;
    }
    void shortestPathfromalltoone(const string& node) {
        int nodeNum = this->getNodeNum(node);
        int n = al.size();
        if (nodeNum == -1)
        {
            return;
        }
        for (int i = 0; i < n; i++)
        {
            if (i != nodeNum)
            {
                ShortestRoutefrompair(nodeNames[i], node);
            }
        }
    }
    void secondshortestPathfromalltoone(const string& node) {
        int nodeNum = this->getNodeNum(node);
        int n = al.size();
        if (nodeNum == -1)
        {
            return;
        }
        for (int i = 0; i < n; i++)
        {
            if (i != nodeNum)
            {
                secondShortestRoutefrompair(nodeNames[i], node);
            }
        }
    }
    void shortestPathfromOnetoall(const string& node)
    {
        int nodeNum = this->getNodeNum(node);
        int n = al.size();
        if (nodeNum == -1)
        {
            return;
        }
        for (int i = 0; i < n; i++)
        {
            if (i != nodeNum)
            {
                ShortestRoutefrompair(node, nodeNames[i]);
            }
        }
    }
    void secondshortestPathfromOnetoall(const string& node)
    {
        int nodeNum = this->getNodeNum(node);
        int n = al.size();
        if (nodeNum == -1)
        {
            return;
        }
        for (int i = 0; i < n; i++)
        {
            if (i != nodeNum)
            {
                secondShortestRoutefrompair(node, nodeNames[i]);
            }
        }
    }
};
