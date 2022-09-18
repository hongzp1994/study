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
	/*д��ȥ*/
	GET_REF2DATA(CmdMsg, input, _poUserData);
	CmdMsg* pout = new CmdMsg(input);
	/*ZinxKernel::Zinx_SendOut��������Ĳ���1 ֻ���ǶѶ��� ��һ��ʼ�����_poUserData��ջ����*/
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
	/*������һ�������ڵĶ�����date�Ķ���*/
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
	/*��ȡ�û����õĿ��ر�־*/
	GET_REF2DATA(CmdMsg, input, _poUserData);
	if (true == input.isOpen)
	{
		if (NULL != pOut)
		{
			//�����
			ZinxKernel::Zinx_Add_Channel(*pOut);
			pOut = NULL;
		}
	}
	else
	{
		//����� ��ȡͨ��
		auto pchannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
		ZinxKernel::Zinx_Del_Channel(*(pchannel));
		pOut = pchannel;
	}
	/*�����߹����*/
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
	/*������һ�������ڵĶ�����echo�Ķ���*/
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
	/* ����û�����ʱ����---���ı��־λ*/
	GET_REF2DATA(CmdMsg, input, _poUserData);
	if (input.isCmd)
	{
		needAdd = input.needDatePre;
	}
	else /*�������Ҫ���Ե�����---������ַ��������Թ��ܶ���*/
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