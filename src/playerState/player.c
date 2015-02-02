

#include "player.h"

/* PLAYER OPTIONS */

static int HIT_TO_SHOCK = 3; //num shoots for shock in hit_timeout
static int HIT_TO_DEATH = 3; // num shoots for dead in hit_timeout 

static int HIT_TIMEOUT = 15 * 1000; //msec Timeout for norm->shock, shock->dead
static int HEAL_TIMEOUT = 30 * 1000; //msec healing timeout for shock->norm

/******************/

int _poling_rate = 5; // msec default; playerState_proceed calling rate
int _hit_timeout_count;
int _heal_timeout_count; //counters for 

struct playerState_info _current;

PlayerState_Info _currentInfo;

PlayerState_Info playerState_init(int poling)
{
    _poling_rate = poling;
    
    _hit_timeout_count  = HIT_TIMEOUT / _poling_rate;
    _heal_timeout_count = HEAL_TIMEOUT / _poling_rate; 
    
    _currentInfo = &_current;
    
    _currentInfo->currentState = playerState_normal;
    _currentInfo->hitCount = 0;
    _currentInfo->hitTimeoutCounter = _hit_timeout_count;
    _currentInfo->shockTimeoutCounter = 0;
    
    return _currentInfo;
}

void _proceedNormal(int isDamage);
void _proceedShock(int isDamage);

void playerState_proceed(int isDamage)
{
    if (_currentInfo->currentState == playerState_normal)
        _proceedNormal(isDamage);
    else if (_currentInfo->currentState == playerState_shock)
        _proceedShock(isDamage);
    //else dead not walks)))
    return;
}

void _setState(enum playerStateEnum state)
{
    if (state == playerState_normal)    {
        _currentInfo->hitCount = 0;
        _currentInfo->hitTimeoutCounter = _hit_timeout_count;
        _currentInfo->shockTimeoutCounter = 0;
        _currentInfo->currentState = playerState_normal;
    } else if (state == playerState_shock)
    {
        _currentInfo->hitCount = 0;
        _currentInfo->hitTimeoutCounter = _hit_timeout_count;
        _currentInfo->shockTimeoutCounter = _heal_timeout_count;
        _currentInfo->currentState = playerState_shock;
    } else if (state == playerState_dead)
    {
        _currentInfo->currentState = playerState_dead;
    }
}

void _proceedNormal(int isDamage)
{
    if (isDamage)
        _currentInfo->hitCount++;
    
    if (_currentInfo->hitCount > HIT_TO_SHOCK)
    {
        _setState(playerState_shock);
        return;
    }
    
    if (_currentInfo->hitTimeoutCounter == 0)
    {
        _setState(playerState_normal);
        return;
    }
    
    _currentInfo->hitTimeoutCounter--; //time running 
}

void _proceedShock(int isDamage)
{
    if (isDamage){
        _currentInfo->hitCount++;
        _currentInfo->shockTimeoutCounter = _heal_timeout_count; //to heal need no damege during timeout
    }
    
    if (_currentInfo->hitTimeoutCounter == 0)
        _currentInfo->hitCount = 0;
    
    if (_currentInfo->shockTimeoutCounter == 0) 
    {
        _setState(playerState_normal); //healed
        return;
    }
    
    if (_currentInfo->hitCount > HIT_TO_DEATH)
    {
        _setState(playerState_dead);
        return;
    }
    
    _currentInfo->shockTimeoutCounter--; //healing state
    _currentInfo->hitTimeoutCounter--;
       
}

PlayerState_Enum playerState_CurrentState()
{
    return &_currentInfo->currentState;
}

int playerState_isDead()
{
    return _currentInfo->currentState == playerState_dead;
}

int playerState_isShock()
{
    return _currentInfo->currentState == playerState_shock;
}

int playerState_isNormal()
{
    return _currentInfo->currentState == playerState_normal;
}