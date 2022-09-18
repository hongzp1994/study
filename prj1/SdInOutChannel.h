#pragma once
#include <zinx.h>
#include<ZinxTCP.h>

class StdInChannel :
    public Ichannel
{
public:
    StdInChannel();
    virtual bool Init();
    virtual bool ReadFd(std::string& _input);
    virtual bool WriteFd(std::string& _output);
    virtual void Fini();
    virtual int GetFd();
    virtual std::string GetChannelInfo();
    virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput);
    virtual ~StdInChannel();
};
class StdOutChannel :
    public Ichannel
{
public:
    StdOutChannel();
    virtual bool Init();
    virtual bool ReadFd(std::string& _input);
    virtual bool WriteFd(std::string& _output);
    virtual void Fini();
    virtual int GetFd();
    virtual std::string GetChannelInfo();
    virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput);
    virtual ~StdOutChannel();
};

class myTcpData :public ZinxTcpData
{
public:
    myTcpData(int _fd) :ZinxTcpData(_fd) {}
    virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput);
};

class myFact :public IZinxTcpConnFact
{
    virtual ZinxTcpData* CreateTcpDataChannel(int _fd);
};

