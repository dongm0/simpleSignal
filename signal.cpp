#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
 
#define Connect(sender, signal, receiver, method) ((sender)->signal.Bind(receiver, method))
/*
* func: 槽函数基类
* parm:
* return:
*/
template<class T>
class SlotBase
{
public:
    virtual void Exec(T param1) = 0;  //纯虚函数
    virtual ~SlotBase(){}
};
 
 
/*
* func: 槽函数
* parm:
* return:
*/
template<class T, class T1>
class Slot : public SlotBase<T1>
{
public:
 
    /* 定义Slot的时候，获取槽函数信息 */
    Slot(T* pObj, void (T::*func)(T1))
    {
        m_pSlotBase = pObj;
        m_Func = func;
    }
 
    /* signal触发时，调用 */
    void Exec(T1 param1)
    {
        (m_pSlotBase->*m_Func)(param1);
    }
 
private:
    /* 槽函数信息 暂存 */
    T* m_pSlotBase = NULL;
    void (T::*m_Func)(T1);
};
 
/*
* func: 信号
* parm:
* return:
*/
template<class T1>
class Signal
{
public:
 
    /* 模板函数 -> Bind时获取槽函数指针 */
    template<class T>
    void Bind(T* pObj, void (T::*func)(T1))
    {
        m_pSlotSet.push_back(new Slot<T,T1>(pObj,func));
    }
 
    /* 重载操作符 -> signal触发机制 */
    void operator()(T1 param1)
    {
        for(int i=0;i<(int)m_pSlotSet.size();i++)
        {
            m_pSlotSet[i]->Exec(param1);
        }
    }
 
    ~Signal()
    {
        for(int i=0;i<(int)m_pSlotSet.size();i++)
        {
            delete m_pSlotSet[i];
        }
    }
 
private:
    vector<SlotBase<T1>*> m_pSlotSet; //这一句很重要，靠基类的指针来存储 信号槽指针
};
 
 
 
 
class TestFunc1
{
public:
    void FuncOfA(int parm)
    {
        printf("enter FuncOfA parm = %d\n", parm);
    }
};
class TestFunc2
{
public:
    void FuncOfB(int parm)
    {
        printf("enter FuncOfB parm = %d\n", parm);
    }
};
 
 
class TestSignal
{
public:
    TestSignal()
    {
    }
    void emit(int value)
    {
        ValueChanged(value);
    }
 
public:
    Signal<int> ValueChanged;
 
 
};
 
int main()
{
    /* 1、定义信号和槽 */
    TestFunc1* pFunc1 = new TestFunc1;
    TestFunc2* pFunc2 = new TestFunc2;
    TestSignal* pSignal = new TestSignal;
 
    /* 2、1个信号绑定2个槽 */
    Connect(pSignal, ValueChanged, pFunc1, &TestFunc1::FuncOfA);
    Connect(pSignal, ValueChanged, pFunc2, &TestFunc2::FuncOfB);
 
    /* 3、触发信号 */
    pSignal->emit(1);
    pSignal->emit(2);
 
 
 
    delete pFunc1;
    delete pFunc2;
    delete pSignal;
 
}