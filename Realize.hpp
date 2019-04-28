// Realize.hpp
#pragma once
#include "Definition.hpp"

// 创建邻接矩阵
Graph::Graph(const bool _isDirectional, const int _vertexSize, const int _edgeSize)
{
    isDirectional = _isDirectional;
    vertexSize = _vertexSize;
    edgeSize = _edgeSize;
    for (int i = 0; i < _edgeSize; i++)
    {
        cout << "请输入邻边的2个顶点编号：" << ends;
        // 起点与终点编号
        int departure, arrival;
        cin >> departure >> arrival;
        adjacency[departure - 1][arrival - 1] = true;
        // 若是无向图，需要将有向邻接矩阵进行对称
        if (!_isDirectional)
            adjacency[arrival - 1][departure - 1] = true;
    }
    // 对象被正确创建
    isCorrectlyInited = true;
}

// 显示邻接矩阵
void Graph::showGraph() const
{
    cout << "邻接矩阵为：" << endl;
    for (int i = 0; i < vertexSize; i++)
    {
        for (int j = 0; j < vertexSize; j++)
            cout << adjacency[i][j] << ends;
        cout << endl;
    }
}

// 计算度数、入度、出度
int Graph::getDegree(int _vertexID, DegreeType _type = all) const
{
    int degree = 0;
    switch (_type)
    {
        // 出度
    case out:
        for (const bool &edge : adjacency[_vertexID - 1])
            // 遍历特定行中的每一项
            degree += edge;
        break;

        // 入度
    case in:
        for (const bool(&edgeList)[10] : adjacency)
            // 遍历每一行中的特定项
            degree += edgeList[_vertexID - 1];
        break;

        // 总度
    case all:
        // 有向图：入度＋出度
        degree = getDegree(_vertexID, in) + getDegree(_vertexID, out);
        if (!isDirectional)
            // 无向图：有向图÷２
            degree = degree / 2;
        break;
    }
    return degree;
}

// 深度优先遍历（公开）
void Graph::deepFirstTraverse(const int _vertexID)
{
    _dft(_vertexID);
    _rv();
}

// 深度优先遍历（内核）
void Graph::_dft(const int _vertexID)
{
    // 输出当前顶点
    cout << _vertexID << ends;
    // 将当前顶点标记为已访问
    isVisited[_vertexID - 1] = true;
    // 遍历所有顶点
    for (int i = 1; i <= vertexSize; i++)
        // 若存在连通的边，且另一端的顶点未被访问
        if (adjacency[_vertexID - 1][i - 1] && !isVisited[i - 1])
            // 递归地进行深度优先遍历
            _dft(i);
}

// 重置节点访问记录
void Graph::_rv()
{
    for (bool &vertex : isVisited)
        vertex = false;
}

// 另存为
void Graph::saveAs(const string _fileName)
{
    ofstream fileWriter;
    // 以输出方式打开文件
    fileWriter.open(_fileName, ios::out);
    // 若文件打开成功
    if (fileWriter.is_open())
    {
        // 导出方向性
        fileWriter << isDirectional << endl;
        // 导出顶点数量
        fileWriter << vertexSize << endl;
        // 导出邻边数量
        fileWriter << edgeSize << endl;
        // 导出顶点连接表
        for (int i = 0; i < vertexSize; i++)
            // 对于有向图
            if (isDirectional)
            {
                // 遍历所有顶点（可能有反向边）
                for (int j = 0; j < vertexSize; j++)
                    if (adjacency[i][j])
                        fileWriter << i + 1 << ends << j + 1 << endl;
            }
            else
            {
                // 跳过反向边（可能导致重复输出）
                for (int j = i; j < vertexSize; j++)
                    if (adjacency[i][j])
                        fileWriter << i + 1 << ends << j + 1 << endl;
            }
        cout << "文件" << _fileName << "导出成功！" << endl;
    }
    else
        cerr << "错误：导出异常！" << endl;
    fileWriter.close();
}

// 打开
Graph::Graph(const string _fileName)
{
    ifstream fileReader;
    // 以输入方式打开文件
    fileReader.open(_fileName, ios::in);
    if (fileReader.is_open())
    {
        // 读取方向
        fileReader >> isDirectional;
        // 读取顶点数量
        fileReader >> vertexSize;
        // 读取邻边数量
        fileReader >> edgeSize;
        for (int i = 0; i < edgeSize; i++)
        {
            int departure, arrival;
            // 输入与临边相关的2个顶点编号
            fileReader >> departure >> arrival;
            // 生成2个顶点之间的单项边
            adjacency[departure - 1][arrival - 1] = true;
            // 若是无向图，需要将有向邻接矩阵进行对称
            if (!isDirectional)
                adjacency[arrival - 1][departure - 1] = true;
        }
        cout << "文件" << _fileName << "导入成功！" << endl;
        // 对象已被正确创建，可以正常使用了
        isCorrectlyInited = true;
    }
    else
    {
        cerr << "错误：导入异常！" << endl;
        // 对象未被正确创建，仍不可使用！
        isCorrectlyInited = false;
    }
}

// 深度优先搜索（内核）
void Graph::_dfs(const int _vertexID)
{
    // 将当前顶点加入路径栈
    pathStack.push(_vertexID);
    // 标记当前顶点访问状态
    isVisited[_vertexID - 1] = true;
    // 对于每一个起始顶点
    for (int i = 1; i <= vertexSize; i++)
    {
        // 若存在未访问的相邻顶点
        if (adjacency[_vertexID - 1][i - 1] && !isVisited[i - 1])
        {
            // 递归进行深度优先搜索
            _dfs(i);
        }
        // 存在哈密顿路径，快速返回至递归顶层
        if (pathStack.size() == vertexSize)
        {
            // 留存路径并退出（后续再还原）
            return;
        }
    }
    // 将当前顶点撤出路径栈
    pathStack.pop();
    // 撤销当前顶点访问状态
    isVisited[_vertexID - 1] = false;
}

// 哈密顿路径
void Graph::hamitonPath()
{
    // 从每个顶点开始
    for (int i = 1; i <= vertexSize; i++)
    {
        // 深度优先搜索
        _dfs(i);
        // 当路径栈满（一条路径遍历所有顶点）
        if (pathStack.size() == vertexSize)
        {
            // 已找到路径，退出循环
            break;
        }
    }
    // 判断是否存在路径
    if (pathStack.size() > 0)
    {
        // 存在路径，进行输出
        for (long long i = pathStack.size() - 1; i >= 0; i--)
        {
            cout << pathStack._Get_container()[i] << ends;
        }
    }
    else
    {
        cerr << "错误：不存在哈密顿路径！" << endl;
    }
    // 重置路径栈
    while (!pathStack.empty())
    {
        pathStack.pop();
    }
    // 重置顶点访问记录
    _rv();
}
