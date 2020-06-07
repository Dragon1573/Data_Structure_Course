// Definition.hpp
#pragma once
#include <deque>
#include <fstream>
#include <iostream>
#include <string>

// 最大顶点数量
constexpr int MAX_VERTEX = 9;

class Graph {
private:
    // 图的类型
    bool isDirectional;
    // 邻接矩阵
    bool adjacency[10][10] = {0};
    // 顶点数量
    int vertexSize;
    // 邻边数量
    int edgeSize;
    // 节点访问记录
    bool isVisited[10] = {0};
    // 深度优先遍历（内核）
    void _dft(const int);
    // 重置节点访问记录（内核）
    void _rv();
    // 遍历路径
    std::deque<int> pathStack;
    // 深度优先搜索（内核）
    void _dfs(const int);

public:
    // 记录对象是否被正确初始化
    bool isCorrectlyInited;
    // 度的类型
    const enum DegreeType {
        in = 1,
        out = 2,
        all = 3
    };
    // 创建邻接矩阵
    Graph(const bool, const int, const int);
    // 析构函数
    ~Graph() {}
    // 显示邻接矩阵
    void showGraph() const;
    // 计算度数、入度、出度
    int getDegree(int, DegreeType) const;
    // 深度优先遍历（公开）
    void deepFirstTraverse(const int);
    // 哈密顿路径
    void hamitonPath();
    // 另存为
    void saveAs(const std::string);
    // 打开
    Graph(const std::string);
};
