#pragma once
#include <zinx.h>
class EchoRole : public Irole
{
public:
	EchoRole();
	virtual bool Init();
	virtual UserData* ProcMsg(UserData& _poUserData);
	virtual void Fini();
	virtual ~EchoRole();
};

class OutputCtrl :public Irole {
public:
	OutputCtrl();
	virtual bool Init();
	virtual UserData* ProcMsg(UserData& _poUserData);
	virtual void Fini();
	Ichannel* pOut = NULL;
	~OutputCtrl();
};

class  DatePreRole :public Irole
{
public:

	virtual bool Init();
	virtual UserData* ProcMsg(UserData& _poUserData);
	virtual void Fini();
	bool needAdd = false;
};

