// User_Interface.cpp
#include "Realize.hpp"
#include <conio.h>
#include <cstdlib>
#include <Windows.h>
using namespace std;

// 选项合法性校验（是否）
bool selected() {
    char choice = '\0';
    do {
        choice = _getch();
    } while (choice != 'Y' && choice != 'y' && choice != 'n' && choice != 'N');
    if (choice == 'Y' || choice == 'y') {
        cout << "图形模式：有向" << endl;
        return true;
    } else {
        cout << "图形模式：无向" << endl;
        return false;
    }
}

// 选项合法性校验（选项）
char selected(const char _min, const char _max) {
    char choice = '\0';
    do {
        choice = _getch();
    } while (choice < _min || choice > _max);
    return choice;
};

// 用户主菜单
void graphicsInterface() {
    cout << "                欢迎使用通用图论研究工具                 " << endl;
    cout << "＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊" << endl;
    cout << "                   1.从键入生成无权图                   " << endl;
    cout << "                     2.显示邻接矩阵                    " << endl;
    cout << "                    3.计算顶点的度数                    " << endl;
    cout << "                     4.深度优先遍历                     " << endl;
    cout << "                      5.哈密顿路径                      " << endl;
    cout << "                      6.保存到文件                      " << endl;
    cout << "                      7.从文件读取                      " << endl;
    cout << "                       8.退出程序                       " << endl;
    cout << "＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊" << endl;
    cout << "          （请选择以上功能，非法按键将被忽略）          " << endl;
}

// 主函数
int main() {
    // 图论对象
    Graph *graph = nullptr;
    while (true) {
        system("cls");
        graphicsInterface();
        switch (selected('1', '8')) {
        case '1':
        {
            system("cls");
            cout << "请输入顶点数量：" << ends;
            int vertexSize;
            cin >> vertexSize;
            cout << "请输入邻边数量：" << ends;
            int edgeSize;
            cin >> edgeSize;
            cout << "这是一个有向图吗？[Y / n]" << ends;
            bool isDirectional = selected();
            graph = new Graph(isDirectional, vertexSize, edgeSize);
            cout << "图形导入成功！" << endl;
            Sleep(1000);
            break;
        }
        case '2':
        {
            system("cls");
            if (graph != nullptr) {
                graph->showGraph();
            } else {
                cerr << "错误：未建立图相关数据！请先通过键盘输入或文件导入图数据......" << endl;
            }
            Sleep(3000);
            break;
        }
        case '3':
        {
            system("cls");
            if (graph != nullptr) {
                cout << "请输入顶点编号：" << ends;
                int vertexID;
                cin >> vertexID;
                cout << endl;
                cout << "       请选择计算度数的类型：       " << endl;
                cout << "               1.总度数             " << endl;
                cout << "               2.入度               " << endl;
                cout << "               3.出度               " << endl;
                cout << "＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊" << endl;
                cout << "（请选择以上功能，非法按键将被忽略）" << endl;
                switch (selected('1', '3')) {
                case '1':
                    cout << "总度数：" << graph->getDegree(vertexID, Graph::all);
                    break;
                case '2':
                    cout << "入度：" << graph->getDegree(vertexID, Graph::in);
                    break;
                case '3':
                    cout << "出度：" << graph->getDegree(vertexID, Graph::out);
                    break;
                }
            } else {
                cerr << "错误：未建立图相关数据！请先通过键盘输入或文件导入图数据......" << endl;
            }
            Sleep(3000);
            break;
        }
        case '4':
        {
            system("cls");
            if (graph != nullptr) {
                cout << "请输入起始顶点编号：" << ends;
                int vertexID;
                cin >> vertexID;
                graph->deepFirstTraverse(vertexID);
            } else {
                cerr << "错误：未建立图相关数据！请先通过键盘输入或文件导入图数据......" << endl;
            }
            Sleep(3000);
            break;
        }
        case '5':
        {
            system("cls");
            if (graph != nullptr) {
                cout << "其中一条哈密顿路径为：" << endl;
                graph->hamitonPath();
            } else {
                cerr << "错误：未建立图相关数据！请先通过键盘输入或文件导入图数据......" << endl;
            }
            Sleep(3000);
            break;
        }
        case '6':
        {
            system("cls");
            if (graph != nullptr) {
                cout << "将文件另存为：" << endl;
                string filePath;
                cin >> filePath;
                graph->saveAs(filePath);
            } else {
                cerr << "错误：未建立图相关数据！请先通过键盘输入或文件导入图数据......" << endl;
            }
            Sleep(3000);
            break;
        }
        case '7':
        {
            system("cls");
            cout << "从文件打开：" << endl;
            string filePath;
            cin >> filePath;
            // 如果存在已经被打开的图，则关闭（释放）图对象
            if (graph != nullptr) {
                delete graph;
                // 图指针置空
                graph = nullptr;
            }
            // 导入数据
            graph = new Graph(filePath);
            // 判断图是否被成功创建（不论导入成功与否，图指针必不为空）
            if (!graph->isCorrectlyInited) {
                // 图没有被正确创建（打开），其他功能仍无法正常使用，
                // 所以还需要关闭（释放）错误的图对象，让菜单屏蔽相应功能
                delete graph;
                // 图指针置空
                graph = nullptr;
            }
            Sleep(3000);
            break;
        }
        case '8':
        {
            cout << "正在退出，请稍候……" << endl;
            Sleep(1000);
            // 程序退出前销毁所有数据
            delete graph;
            graph = nullptr;
            exit(0);
        }
        }
    }

    return 0;
}
