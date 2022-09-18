#include <zinx.h>
#include<iostream>
#include"SdInOutChannel.h"
#include"EchoRole.h"
#include"CmdCheck.h"
#include"CmdMsg.h"
#include<ZinxTCP.h>
#include "ZinxTimer.h"
using namespace std;


class timerhello :public TimerOutProc {
	//Í¨¹ýTimerOutproc ¼Ì³Ð
	virtual void Proc()override
	{
		auto pchannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
		std::string output = "hello world";
		ZinxKernel::Zinx_SendOut(output, *pchannel);
		TimerOutMng::GetInstance().DelTask(this);
	}
	virtual int GetTimeSec()override
	{
		return 3;
	}
};

class timerbye :public TimerOutProc
{
	virtual void Proc()override
	{
		auto pchannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
		std::string output = "bye";
		ZinxKernel::Zinx_SendOut(output, *pchannel);
	}
	virtual int GetTimeSec()override
	{
		return 5;
	}
};

int main()
{
	ZinxKernel::ZinxKernelInit();

	TimerOutMng::GetInstance().AddTask(new timerhello());
	TimerOutMng::GetInstance().AddTask(new timerbye());

	ZinxKernel::Zinx_Add_Channel(*(new  StdInChannel ()));
	ZinxKernel::Zinx_Add_Channel(*(new StdOutChannel()));
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(51111, new myFact())));
	ZinxKernel::Zinx_Add_Role(*(new EchoRole()));
	ZinxKernel::Zinx_Add_Role(*(new DatePreRole()));
	ZinxKernel::Zinx_Add_Role(*(new OutputCtrl()));
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTimerChannel()));

	ZinxKernel::Zinx_Run();

	ZinxKernel::ZinxKernelFini();
	return 0;
}