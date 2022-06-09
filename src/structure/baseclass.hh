#pragma once

// 基类

class Info {
public:
    virtual ~Info() { }
};

class Inst: public Info {
public:
    virtual ~Inst() { }
};