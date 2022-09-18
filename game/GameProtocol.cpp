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

/*���������ͨ��������ԭʼ����
����ֵ��ת�������Ϣ��������MutiMsg
ת����ʽ,TCPճ������*/
UserData* GameProtocol::raw2request(std::string _szInput)
{
	MultiMsg* pRet = new MultiMsg();
	szLast.append(_szInput);
	while (1) {
		if (szLast.size() < 8)
		{
			break;
		}
		/*��ǰ�ĸ��ֽ��ж�ȡ��Ϣ���ݳ���*/
		int iLength = 0;
		//һ��ʼiLength��00000032��0, szLast���Ԫ�ض����ֽ�
		//�� 1��2,3,4��� 4*2��24�η�+3*2��16�η�+2*2��8�η�+1*2��0�η���������˼
		iLength |= szLast[0] << 0;//������32��0|24��0������szLast[0]
		iLength |= szLast[1] << 8;//24��0������szLast[0] | 16��0+szLast[1]+8��0
		iLength |= szLast[2] << 16;
		iLength |= szLast[3] << 24;

		int id = 0;
		id |= szLast[4] << 0;
		id |= szLast[5] << 8;
		id |= szLast[6] << 16;
		id |= szLast[7] << 24;

		/*ͨ�������ĳ����жϺ��������Ƿ�Ϸ�*/
		if (szLast.size() - 8< iLength)
		{
			/*�������Ļ�û����ɶ������*/
			break;
		}
		/*����һ���û�����*/
		GameMsg* pMsg = new GameMsg((GameMsg::MSG_TYPE)id, szLast.substr(8, iLength));
		pRet->m_Msgs.push_back(pMsg);
		/*�����Ѿ�����ɹ��ı���*/
		szLast.erase(0, 8 + iLength);
	}
	return pRet;
}

/*��������ҵ��㣬�����͵���Ϣ  ������ Zinx_SendOut
����ֵת������ֽ���*/
std::string* GameProtocol::response2raw(UserData& _oUserData)
{
	int iLength = 0;
	int id = 0;
	std::string MsgContent;
	//�Ƚ�����Ϣת������Ϸ��Ϣ����
	GET_REF2DATA(GameMsg, oOutput, _oUserData);
	
	id = oOutput.enMsgType;
	MsgContent = oOutput.serialize();
	iLength = MsgContent.size();
	
	//����Щ��Ϣת�����ֽ��� Ȼ�󽻸���һ������ȥ��ӡ
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
/*�������ݷ��͵�ͨ��*/
Ichannel* GameProtocol::GetMsgSender(BytesMsg& _oBytes)
{
	return m_channel;
}
