#pragma once

class cGameObject;
class cCharacter;

class cState;
class iStateDelegate
{
public:
	virtual void OnStateFinish(cState* pSender) = 0;
};

enum STATE_TYPE
{
	STATE_MOVE,
	STATE_ATTACK,
	STATE_NULL
};

class cState : public cObject
{
protected:
	SYNTHESIZE(cCharacter*, m_pTarget, Target);
	SYNTHESIZE(iStateDelegate*, m_pDelegate, Delegate);
	SYNTHESIZE(STATE_TYPE, m_eStateType, StateType);

public:
	cState();
	virtual ~cState();

	virtual void Start() PURE;
	virtual void Update() PURE;
};

