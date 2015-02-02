/*
 *  Module store info about player health.
 *
 *  Maxim Lyasnikov 2014 
 *  
 *  Module is state machine and are three states:
 *   - Normal - then player can shoot
 *   - Shock - then player takes enough damage, his state is critical,
 *       and if he takes more damage he died, else if no more damage, he
 *       bring back to Normal
 *   - Dead - then player can't shoot and it's game over.
 * 
 *  Switching between states depend on how much damage (hit_to_shock or
 * hit_to_dead) he takes in hit_timeout. For ex. takes 3  * times, in 15 sec
 * will bring to shock state. Then in shock state if player do not take damage 
 * in 30 sec (shock_timeout), he will bring back to normal state. Same for dead 
 * state, depending on hit_timeout.
 * 
 *  Taking damage means call playerState_proceed(>0).
 * 
 *  The poling_rate means how often external code will call 
 * playerState_proceed(). It's for clk undependable timeout set.
 * 
 *  Rates and damages can bee configured in player.c
 */

#ifndef laser_gun_playerState_h_include
        #define laser_gun_playerState_h_include

enum playerStateEnum
{
    playerState_normal,
    playerState_shock,
    playerState_dead
};

struct playerState_info 
{
    enum playerStateEnum currentState;
    
    int hitCount;
    int hitTimeoutCounter;
    int shockTimeoutCounter;

};

typedef struct playerState_info * PlayerState_Info;
typedef enum playerStateEnum * PlayerState_Enum;

PlayerState_Info playerState_init(int polingrate);

void playerState_proceed(int isDamage);

PlayerState_Enum playerState_CurentState();

int playerState_isDead();
int playerState_isNormal();
int playerState_isShock();



#endif
