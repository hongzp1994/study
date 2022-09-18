#include"CmdCheck.h"
#include"CmdMsg.h"
#include"EchoRole.h"

using namespace std;

CmdCheck* CmdCheck::poSingle = new CmdCheck();

CmdCheck::CmdCheck()
{}
CmdCheck::~CmdCheck()
{}
/*这里判断输入进来字符是否是命令字符，如果是改变标签
并判断如果是exit直接关闭程序，如果是open 或者close
改变对应标签 返回自定义信息类对象给下一个函数*/
UserData* CmdCheck::raw2request(std::string _szInput)
{
	if ("exit" == _szInput)
	{
		ZinxKernel::Zinx_Exit();
		return NULL;
	}
	/*赋值原始数据字符串到用户数据字符串字段*/
	auto pret = new CmdMsg();
	pret->szUserData = _szInput;
	if ("open" == _szInput)
	{
		pret->isCmd = true;
		pret->isOpen = true;
	}
	if ("close" == _szInput)
	{
		pret->isCmd = true;
		pret->isOpen = false;
	}
	if ("date" == _szInput)
	{
		pret->needDatePre = true;
		pret->isCmd = true;
	}
	if ("cleardate" == _szInput)
	{
		pret->needDatePre = false;
		pret->isCmd = true;
	}
	return pret;
}


std::string* CmdCheck::response2raw(UserData& _oUserData)
{
	/*用户数据转成原始数据*/
	GET_REF2DATA(CmdMsg, output, _oUserData);
	return new string(output.szUserData);
}
/*判断输入对象的命令标志位，如果是命令 就将对象转换成输出控制类的对象
如果不是就将对象转换成回显类对象
*/
Irole* CmdCheck::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{
	szOutChannel = _oUserDataMsg.szInfo;
	if ("stdin" == szOutChannel)
	{
		szOutChannel = "stdout";
	}
	/*根据命令不同，交给不同的处理者role对象*/
	auto rolelist = ZinxKernel::Zinx_GetAllRole();

	auto pCmdMsg = dynamic_cast<CmdMsg*>(_oUserDataMsg.poUserData);
	/*读取当前消息是否是命令*/
	bool isCmd = pCmdMsg->isCmd;
	Irole* pRetRole = NULL;


	for (Irole* prole : rolelist)
	{
		if (isCmd)
		{
			auto pOutCtrl = dynamic_cast<OutputCtrl*>(prole);
			if (NULL != pOutCtrl)
			{
				pRetRole = pOutCtrl;
				break;
			}
		}
		else
		{
			auto pDate = dynamic_cast<DatePreRole*>(prole);
			if (NULL != pDate)
			{
				pRetRole = pDate;
				break;
			}
		}
	}
	return pRetRole;
}
Ichannel* CmdCheck::GetMsgSender(BytesMsg& _oBytes)
{
	/*根据发送信息获取发送者 这里是指定数据通过标准输出输出*/
	return ZinxKernel::Zinx_GetChannel_ByInfo(szOutChannel);
}