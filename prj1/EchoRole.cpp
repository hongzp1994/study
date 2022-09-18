#include"EchoRole.h"
#include"CmdMsg.h"
#include"CmdCheck.h"
using namespace std;
EchoRole::EchoRole()
{
}
bool EchoRole::Init()
{
	return true;
}
UserData* EchoRole::ProcMsg(UserData& _poUserData)
{
	/*写出去*/
	GET_REF2DATA(CmdMsg, input, _poUserData);
	CmdMsg* pout = new CmdMsg(input);
	/*ZinxKernel::Zinx_SendOut这个函数的参数1 只能是堆对象 而一开始传入的_poUserData是栈对象*/
	ZinxKernel::Zinx_SendOut(*pout, *(CmdCheck::GetInstance()));
	return nullptr;
}

void EchoRole::Fini()
{

}
EchoRole::~EchoRole()
{

}


bool OutputCtrl::Init()
{
	Irole* pRetRole = NULL;
	/*设置下一个处理环节的对象是date的对象*/
	for (auto pRole : ZinxKernel::Zinx_GetAllRole())
	{
		auto pDate = dynamic_cast<DatePreRole*>(pRole);
		if (NULL != pDate)
		{
			pRetRole = pDate;
			break;
		}
	}
	if (NULL != pRetRole)
	{
		SetNextProcessor(*pRetRole);
	}

	return true;
}
UserData* OutputCtrl::ProcMsg(UserData& _poUserData)
{
	/*读取用户设置的开关标志*/
	GET_REF2DATA(CmdMsg, input, _poUserData);
	if (true == input.isOpen)
	{
		if (NULL != pOut)
		{
			//开输出
			ZinxKernel::Zinx_Add_Channel(*pOut);
			pOut = NULL;
		}
	}
	else
	{
		//关输出 获取通道
		auto pchannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
		ZinxKernel::Zinx_Del_Channel(*(pchannel));
		pOut = pchannel;
	}
	/*开或者关输出*/
	return new CmdMsg(input);
}
void OutputCtrl::Fini()
{

}

OutputCtrl::OutputCtrl() {}
OutputCtrl::~OutputCtrl() {}

bool DatePreRole::Init()
{
	Irole* pRetRole = NULL;
	/*设置下一个处理环节的对象是echo的对象*/
	for (auto pRole : ZinxKernel::Zinx_GetAllRole())
	{
		auto pEcho = dynamic_cast<EchoRole*>(pRole);
		if (NULL != pEcho)
		{
			pRetRole = pEcho;
			break;
		}
	}
	if (NULL != pRetRole)
	{
		SetNextProcessor(*pRetRole);
	}

	return true;
}
UserData* DatePreRole::ProcMsg(UserData& _poUserData)
{
	CmdMsg* pret = NULL;
	/* 如果用户数据时命令---》改变标志位*/
	GET_REF2DATA(CmdMsg, input, _poUserData);
	if (input.isCmd)
	{
		needAdd = input.needDatePre;
	}
	else /*如果是需要回显的内容---》添加字符串给回显功能对象*/
	{
		if (needAdd)
		{
			time_t tmp;
			time(&tmp);
			string szNew = ctime(&tmp) + input.szUserData;
			pret = new CmdMsg();
			pret->szUserData = szNew;
		}
		else
		{
			pret = new CmdMsg();
			pret->szUserData = input.szUserData;
		}
	}
	return pret;
}

void DatePreRole::Fini()
{

}