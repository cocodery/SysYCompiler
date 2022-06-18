## DAG

###dagNode
```cpp
class dagNode{
public:
    int32_t dag_idx;
    operatorType op;
    VirtReg reg;
    vector<VirtReg> addi_tags;
    dagLeafConstNode *cr;
    dagLeafConstNode *cl;
    dagLeafVarNode *vr;
    dagLeafVarNode *vl;
    dagNode *left;
    dagNode *right;
public:
    int32_t getIdx() { return dag_idx; }
    bool isLeaf() { return ((right==nullptr)&&(left==nullptr)); }
    void createOpNode(int32_t dag_idx, VirtReg reg, operatorType op, vector<VirtReg> addi_tags, dagNode *right, dagNode *left);
    void createOpNode(int32_t dag_idx, VirtReg reg, operatorType op, vector<VirtReg> addi_tags, dagLeafConstNode *cr, dagLeafConstNode *cl);
    void createOpNode(int32_t dag_idx, VirtReg reg, operatorType op, vector<VirtReg> addi_tags, dagLeafVarNode *vr, dagLeafVarNode *vl);
};
```
`dagNode`是结点编号，`op`是操作符，`reg`是寄存器，`addi_tags`是附加标记，`cr`是右子树的常量结点，`cl`是左子树的常量结点，`vr`是右子树的变量结点，`vl`是左子树的变量结点，`left`是左子树一般结点，`right`是右子树一般节点。

###dagLeafConstNode
```cpp
class dagLeafConstNode{
public:
    int32_t dag_idx;
    CTValue value;
    vector<VirtReg> addi_tags;
};
```
结点编号，结点的常数值，附加的标记。
###dagLeafVarNode
```cpp
class dagLeafVarNode{
public:
    int32_t dag_idx;
    VirtReg reg;
    vector<VirtReg> addi_tags;
};
```
结点编号，结点的虚拟内存，附加的标记。

###dag
```cpp
class dag{
public:
    int32_t node_cnt;
    vector<dagNode> dag_nodes;
    vector<dagLeafConstNode> dag_leaf_const_nodes;
    vector<dagLeafVarNode> dag_leaf_var_nodes;
public:
    dag() : node_cnt(0), dag_nodes(vector<dagNode>()) {}
    void printDag();
    int32_t isExist(CTValue value);
    int32_t isExist(Variable *var);
    int32_t isExist(VirtReg reg, bool erase);
    int32_t isExprExist(VirtReg left, VirtReg right, operatorType op);
    int32_t isExprExist(VirtReg src, operatorType op);
    void printDagNode();
    operatorType getOp(BinaryOp bop);
    operatorType getOp(UnaryOp uop);
    string turnOp(operatorType op);
};
```
`node_cnt`是指结点的数量, `dag_nodes`是结点的集合, `dag_leaf_const_nodes`是常量结点的集合, `dag_leaf_var_nodes`是变量结点的集合
`printDag`函数被我暂时注释掉了, 内容没有更新, 取消注释后可能有问题
`isExist`函数用于判断是否某个`CTValue` `Variable` `VirtReg`是否存在于某个结点,返回值为结点在该类型数组中的标号
`isExprExist`函数用于判断某个表达式是否存在, 返回值为表达式计算结果的结点在数组中的标号
`getOp`函数用于获取操作符, 返回值为operatorType类型, `turnOp`函数用于获取操作符的字符串表示(方便打印dag)

###未解决的问题
1. ReadFromBasicBlock函数始终报错CTValue有多个默认构造函数. 之前似乎已经解决过这个问题, 但是晚上头脑太混乱了不知道怎么改好了.
2. reGenerateBB函数没有完成. 目前只能生成含附加标记的带符号的表达式ir.