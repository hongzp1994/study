#include "GameProtocol.h"
#include <iostream>
#include"GameMsg.h"
#include "GameChannel.h"
#include"msg.pb.h"
#include"GameRole.h"
using namespace std;

GameProtocol::GameProtocol()
{
}


GameProtocol::~GameProtocol()
{
	if (NULL != m_Role)
	{
		ZinxKernel::Zinx_Del_Role(*m_Role);
		delete m_Role;
	}
}

/*输入参数是通道传来的原始报文
返回值是转换后的消息对象链表MutiMsg
转换方式,TCP粘包处理*/
UserData* GameProtocol::raw2request(std::string _szInput)
{
	MultiMsg* pRet = new MultiMsg();
	szLast.append(_szInput);
	while (1) {
		if (szLast.size() < 8)
		{
			break;
		}
		/*在前四个字节中读取消息内容长度*/
		int iLength = 0;
		//一开始iLength是00000032个0, szLast里得元素都是字节
		//将 1，2,3,4变成 4*2的24次方+3*2的16次方+2*2的8次方+1*2的0次方这样的意思
		iLength |= szLast[0] << 0;//这里变成32个0|24个0后面是szLast[0]
		iLength |= szLast[1] << 8;//24个0后面是szLast[0] | 16个0+szLast[1]+8个0
		iLength |= szLast[2] << 16;
		iLength |= szLast[3] << 24;

		int id = 0;
		id |= szLast[4] << 0;
		id |= szLast[5] << 8;
		id |= szLast[6] << 16;
		id |= szLast[7] << 24;

		/*通过读到的长度判断后续报文是否合法*/
		if (szLast.size() - 8< iLength)
		{
			/*本条报文还没够，啥都不干*/
			break;
		}
		/*构造一条用户请求*/
		GameMsg* pMsg = new GameMsg((GameMsg::MSG_TYPE)id, szLast.substr(8, iLength));
		pRet->m_Msgs.push_back(pMsg);
		/*弹出已经处理成功的报文*/
		szLast.erase(0, 8 + iLength);
	}
	return pRet;
}

/*参数来自业务层，待发送的信息  于来自 Zinx_SendOut
返回值转换后的字节流*/
std::string* GameProtocol::response2raw(UserData& _oUserData)
{
	int iLength = 0;
	int id = 0;
	std::string MsgContent;
	//先将该信息转换成游戏消息对象
	GET_REF2DATA(GameMsg, oOutput, _oUserData);
	
	id = oOutput.enMsgType;
	MsgContent = oOutput.serialize();
	iLength = MsgContent.size();
	
	//将这些信息转换成字节流 然后交给下一个对象去打印
	auto pret = new std::string();

	pret->push_back((iLength>>0) & 0xff);
	pret->push_back((iLength >> 8) & 0xff);
	pret->push_back((iLength >> 16) & 0xff);
	pret->push_back((iLength >> 24) & 0xff);
	pret->push_back((id >> 0) & 0xff);
	pret->push_back((id >> 8) & 0xff);
	pret->push_back((id >> 16) & 0xff);
	pret->push_back((id >> 24) & 0xff);
	pret->append(MsgContent);

	return pret;
}

Irole* GameProtocol::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{
	return m_Role;
}
/*返回数据发送的通道*/
Ichannel* GameProtocol::GetMsgSender(BytesMsg& _oBytes)
{
	return m_channel;
}
