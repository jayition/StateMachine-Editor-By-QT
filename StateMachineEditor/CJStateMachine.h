#ifndef CJSTATEMACHINE_H
#define CJSTATEMACHINE_H
#include <iostream>
#include <string>
#include <map>
#define PUGIXML_SOURCE
#include "pugiconfig.hpp"
#include "pugixml.hpp"
using namespace std;

enum CJObjectType {Unknow, Command, Parameter, Switch ,Container};
enum CJObjectState {ShowEnable, ShowDisable,Hide};
class CJObject
{
public:
	string name;//名称
	CJObjectType type;//类型，可以设置对像的类型，分为命令型、参数型、开关型、容器型。
	CJObjectState state;//状态，可以设置对像的状态，可见并可操作，可见不可操作，不可见。
	int value;//对于状态机中控制的对像只用一个int来存其值，对于Command来说，此值无效。对于参数来说，此值表示其不同的参数项，对于开关来说此值0表示关，其余表示开。
	map<string, CJObject> Objects;//存放子对像的清单
public:
	CJObject()
	{
		name = "";
		type = CJObjectType::Unknow;
		state = CJObjectState::ShowEnable;
	}
	CJObject(string sname, CJObjectType ttype, CJObjectState sstate)
	{
		name = sname;
		type = ttype;
		state = sstate;
	}
	~CJObject()
	{
		name = "";
		type = CJObjectType::Unknow;
		state = CJObjectState::ShowEnable;
		Objects.clear();
	}
	CJObject& operator=(CJObject& value)
	{
		this->name = value.name;
		this->type = value.type;
		this->state = value.state;
		this->Objects = value.Objects;
		return *this;
	}
	CJObject(const CJObject& value)
	{
		name = value.name;
		type = value.type;
		state = value.state;
		Objects = value.Objects;
	};

	bool IsContainer()
	{
		if (type == CJObjectType::Container)
			return true;
		else
			return false;
	}

	bool IsSwitch()
	{
		if (type == CJObjectType::Switch)
			return true;
		else
			return false;
	}

	bool IsParameter()
	{
		if (type == CJObjectType::Parameter)
			return true;
		else
			return false;
	}

	bool IsCommand()
	{
		if (type == CJObjectType::Command)
			return true;
		else
			return false;
	}

	bool IsOperatable()
	{
		if (state == CJObjectState::ShowEnable)
			return true;
		else
			return false;
	}
	CJObject& AddSubObject(string sname, CJObjectType ttype, CJObjectState sstate)
	{
		return AddSubObject(CJObject(sname, ttype, sstate));
	}
	//添加一个属于本对像的子对像，
	CJObject& AddSubObject(CJObject obj)
	{
		Objects[obj.name] = obj;
		return Objects[obj.name];
	}
	//获得当前对像是否有子项
	bool HasSub()
	{
		return !Objects.empty();
	}
	//获得当前对像子项数量
	size_t GetSubCount()
	{
		return Objects.size();
	}
	CJObject& GetObject()
	{
		return *this;
	}
	//在本项往下查找指定ID的对像。
	CJObject* GetObject(string value)
	{
		CJObject* ret = nullptr;
		if (name == value)
			return this;
		if (HasSub())
		{
			std::map<string, CJObject>::iterator  it;
			for (it = Objects.begin(); it != Objects.end(); ++it)
			{
				ret = it->second.GetObject(value);
				if (ret)
					return ret;
			}
		}
		return nullptr;
	}
	CJObject& operator[](string a)
	{
		return Objects[a];
	}
};







class CJState;
class CJOperate
{
public:
	string name;//名称
	CJState* m_NextState;
	CJOperate()
	{
		name = "";
		m_NextState = nullptr;
	}
	CJOperate(string sname)
	{
		name = sname;
		m_NextState = nullptr;
	}
	~CJOperate()
	{
		name = "";
		m_NextState = nullptr;
	}
	CJOperate& operator=(CJOperate& value)
	{
		this->name = value.name;
		this->m_NextState = value.m_NextState;
		return *this;
	}
	CJOperate(const CJOperate& value)
	{
		name = value.name;
		m_NextState = value.m_NextState;
	};
	CJState* DoOperate()
	{
		return m_NextState;
	}
};

class CJState
{
public:
	string name;//名称
    CJState* parentstate;
    void* stateitemobject;
	map<string, CJOperate> m_StatusOperateList;//存放子对像的清单
	CJState()
	{
		name = "";
        parentstate=nullptr;
		m_StatusOperateList.clear();
	}
	CJState(string sname)
	{
		name = sname;
        parentstate=nullptr;
		m_StatusOperateList.clear();
	}
	~CJState()
	{
		name = "";
		m_StatusOperateList.clear();
	}
	CJState& operator=(CJState& value)
	{
		this->name = value.name;
        this->parentstate=value.parentstate;
		this->m_StatusOperateList = value.m_StatusOperateList;
		return *this;
	}
	CJState(const CJState& value)
	{
		name = value.name;
        this->parentstate=value.parentstate;
		m_StatusOperateList = value.m_StatusOperateList;
	};
	CJOperate& operator[](string a)
	{
        CJOperate ret;
        if(a.length()<=0)
            return ret;
		return m_StatusOperateList[a];
	}
	CJState* GetState()
	{
		return this;
	}
	CJState* DoOperate(string name)
	{
		return m_StatusOperateList[name].DoOperate();
	}
	bool AddOperate(string operate)
	{
		CJOperate operateobj = CJOperate(operate);
		m_StatusOperateList[operate] = operateobj;
		return true;
	}
    bool DeleteOperate(string name)
    {
         map<string, CJOperate>::iterator key=m_StatusOperateList.find(name);
         if(key!=m_StatusOperateList.end())
         {
             m_StatusOperateList.erase(key);
             return true;
         }
         return false;
    }
};


class CJStateMachine
{
    public:
		CJStateMachine()
		{
			m_StatusList.clear();
			m_AllOperateList.clear();
			m_CurrentState = nullptr;
            m_BootState=nullptr;
		}
		~CJStateMachine()
		{
            Release();
		}
		bool AddStatus(string name)
		{
			CJState state = CJState(name);
			m_StatusList[name] = state;
			return true;
		}
		bool AddOperate(string name)
		{
			CJOperate operate = CJOperate(name);
			m_AllOperateList[name] = operate;
			return true;
		}
		bool SetStatus(string name)
		{
			m_CurrentState = m_StatusList[name].GetState();
			if (m_CurrentState == nullptr)
				return false;
			else
				return true;
		}
		bool SetOperate(string status, string operate)
		{
			return m_StatusList[status].AddOperate(operate);
		}
        bool SetStateParent(string substatus,string parentstatus)
        {
            m_StatusList[substatus].parentstate=m_StatusList[parentstatus].GetState();
        }
		bool SetOperateNextStatus(string status, string operate, string nextstatus)
		{
            m_StatusList[status][operate].name=operate;
			m_StatusList[status][operate].m_NextState = m_StatusList[nextstatus].GetState();
			return true;
		}
        static string wstring2string(wstring str)
        {
            size_t convertedChars = 0;
            string curLocale = setlocale(LC_ALL, NULL);   //curLocale="C"
            setlocale(LC_ALL, "chs");
            const wchar_t* source = str.c_str();
            size_t wcharNum = sizeof(wchar_t) * str.size() + 1;
            char* dest = new char[wcharNum];
            wcstombs_s(&convertedChars, dest, wcharNum, source, _TRUNCATE);
            string result = dest;
            delete[] dest;
            setlocale(LC_ALL, curLocale.c_str());
            return result;
        };
        static wstring string2wstring(string str)
        {
            size_t convertedChars = 0;
            string curLocale = setlocale(LC_ALL, NULL);   //curLocale="C"
            setlocale(LC_ALL, "chs");
            const char* source = str.c_str();
            size_t charNum = sizeof(char) * str.size() + 1;
            wchar_t* dest = new wchar_t[charNum];
            mbstowcs_s(&convertedChars, dest, charNum, source, _TRUNCATE);
            wstring result = dest;
            delete[] dest;
            setlocale(LC_ALL, curLocale.c_str());
            return result;
        };

        bool Save(const string FileName)
        {
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child(L"StateMachine");
            if(m_CurrentState)
                root.append_attribute(L"BootState").set_value(string2wstring(m_BootState->name).c_str());
            else
                root.append_attribute(L"BootState").set_value(L"");
            pugi::xml_node StateList = root.append_child(L"StateList");
            std::map<string, CJState>::iterator  it;
            for (it = m_StatusList.begin(); it != m_StatusList.end(); ++it)
            {
                pugi::xml_node Status = StateList.append_child(L"Status");
                // 增加属性
                Status.append_attribute(L"name").set_value(string2wstring(it->first).c_str());
                if(it->second.parentstate!=nullptr)
                    Status.append_attribute(L"parent").set_value(string2wstring(it->second.parentstate->name).c_str());
                pugi::xml_node StatusOperateList = Status.append_child(L"OperateList");
                std::map<string, CJOperate>::iterator  itoperate;
                for (itoperate = it->second.m_StatusOperateList.begin(); itoperate !=  it->second.m_StatusOperateList.end(); ++itoperate)
                {
                    pugi::xml_node StatusOperate = StatusOperateList.append_child(L"Operate");
                    StatusOperate.append_attribute(L"name").set_value(string2wstring(itoperate->first).c_str());
                    if(itoperate->second.m_NextState)
                    StatusOperate.append_attribute(L"NextState").set_value(string2wstring(itoperate->second.m_NextState->name).c_str());
                }
            }
            pugi::xml_node OperateList = root.append_child(L"OperateList");
            std::map<string, CJOperate>::iterator  itOperate;
            for (itOperate = m_AllOperateList.begin(); itOperate != m_AllOperateList.end(); ++itOperate)
            {
                pugi::xml_node Operate = OperateList.append_child(L"Operate");
                // 增加属性
                Operate.append_attribute(L"name").set_value(string2wstring(itOperate->first).c_str());
            }
            return doc.save_file(FileName.c_str(), L"\t", 1U, pugi::encoding_utf8);
        }
        bool Load(const string FileName)
        {
            Release();
            pugi::xml_document doc;
            if (!doc.load_file(FileName.c_str(), pugi::parse_default, pugi::encoding_auto))
            {
                return false;
            }

            pugi::xml_node root = doc.child(L"StateMachine");
            wstring strRootStateName = root.attribute(L"BootState").value();
            pugi::xml_node OperateList = root.child(L"OperateList");
            if (OperateList)
            {
                for (pugi::xml_node Operate = OperateList.child(L"Operate"); Operate; Operate = Operate.next_sibling())
                {
                      wstring operatename = Operate.attribute(L"name").value();
                     AddOperate(wstring2string(operatename));
                }
            }
            pugi::xml_node StateList = root.child(L"StateList");
            if (StateList)
            {
                for (pugi::xml_node Status = StateList.child(L"Status"); Status; Status = Status.next_sibling())
                {
                    wstring name = Status.attribute(L"name").value();
                    AddStatus(wstring2string(name));
                }
            }
            if (StateList)
            {
                for (pugi::xml_node Status = StateList.child(L"Status"); Status; Status = Status.next_sibling())
                {
                    wstring name = Status.attribute(L"name").value();
                    wstring parentname = Status.attribute(L"parent").value();
                    if(parentname.length()>0)
                        SetStateParent(wstring2string(name),wstring2string(parentname));
                }
            }
            if (StateList)
            {
                for (pugi::xml_node Status = StateList.child(L"Status"); Status; Status = Status.next_sibling())
                {
                    wstring name = Status.attribute(L"name").value();
                    pugi::xml_node StatusOperateList = Status.child(L"OperateList");
                    if (StatusOperateList)
                    {
                        for (pugi::xml_node StatusOperate = StatusOperateList.child(L"Operate"); StatusOperate; StatusOperate = StatusOperate.next_sibling())
                        {
                             wstring operatename = StatusOperate.attribute(L"name").value();
                             wstring nextstatename = StatusOperate.attribute(L"NextState").value();
                             SetOperateNextStatus(wstring2string(name),wstring2string(operatename),wstring2string(nextstatename));
                        }
                    }
                }
            }
            return true;
        }
        void Release()
        {
            m_BootState=nullptr;
            m_CurrentState=nullptr;
            m_StatusList.clear();
            m_AllOperateList.clear();
        }
        bool SetBootState(string name)
        {
            m_BootState=m_StatusList[name].GetState();
            return true;
        }
        CJState& operator[](string a)
        {
            return m_StatusList[a];
        }
        CJState& operator[](int a)
        {
            int count=GetStatusCount();
            if(a>=count)
                a=count-1;
            if(a<0)
                a=0;
            std::map<string, CJState>::iterator  itstate=m_StatusList.begin();
            for(int i=0;i<a;i++)
                itstate++;
            return itstate->second;

        }
        string GetOperate(int index)
        {
            int count=GetOperateCount();
            if(index>=count)
                index=count-1;
            if(index<0)
                index=0;
            std::map<string, CJOperate>::iterator  itoperate=m_AllOperateList.begin();
            for(int i=0;i<index;i++)
                itoperate++;
            return itoperate->first;
        }
        size_t GetOperateCount()
        {
            return m_AllOperateList.size();
        }
        size_t GetStatusCount()
        {
            return m_StatusList.size();
        }
        bool DeleteState(string name)
        {
             map<string, CJState>::iterator key=m_StatusList.find(name);
             if(key!=m_StatusList.end())
             {
                 m_StatusList.erase(key);
                 return true;
             }
             return false;
        }
        bool DeleteOperate(string statename,string operatename)
        {
            if(statename.length()<=0)
                return false;
            return m_StatusList[statename].DeleteOperate(operatename);
        }
    protected:

    private:
        CJState* m_BootState;
		CJState* m_CurrentState;
		map<string, CJState> m_StatusList;//存放子对像的清单
		map<string, CJOperate> m_AllOperateList;//存放子对像的清单
};

#endif // CJSTATEMACHINE_H
