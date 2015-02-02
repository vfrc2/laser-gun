/*
 * File:   player_test.c
 * Author: vfrc2
 *
 * Created on 25.11.2014, 15:15:02
 */

#include <stdio.h>
#include <stdlib.h>
#include "CUnit/Basic.h"
#include "src/playerState/player.h"

/*
 * CUnit Test Suite
 */

int init_suite(void) {
    return 0;
}

int clean_suite(void) {
    return 0;
}

int POLLING_INTERVAL = 5; //

void _initPlayerState()
{
    playerState_init(POLLING_INTERVAL);
}   

void Player_Born_Test()
{
    _initPlayerState();
    
    CU_ASSERT_NOT_EQUAL(playerState_IsNormal(),0);
}

void Shock_Player_Test()
{
    _initPlayerState();
    
    _shootPlayer(3,1);
    
    playerState_proceed(1); //last shot
    //playerState_proceed(0); //empty frame
    
    CU_ASSERT_NOT_EQUAL(playerState_IsShock(),0);
}

void Player_Health_Recovering_Test()
{
    _initPlayerState();
    
     PlayerState_Info _currentInfo = playerState_init(POLLING_INTERVAL);
    
     int max = 4;
     
    _shootPlayer(max/4,0); //for three shots
    _shootPlayer(1,1);
    _shootPlayer(max/4,0);
    _shootPlayer(1,1);
    _shootPlayer(max/4,0);
    _shootPlayer(1,1);
    
    //playerState_proceed(1); //last shot
    playerState_proceed(0); //empty frame
    
    CU_ASSERT_NOT_EQUAL(playerState_IsNormal(),0);
}

void Normal_Healing_Player_Test()
{
    _initPlayerState();
    
    _shootPlayer(3,1);
    
    _shootPlayer(_currentInfo->hitTimeoutCounter + 3, 0); //do not shoot
    
    playerState_proceed(1); //last shot
    
    
    CU_ASSERT_EQUAL(_currentInfo->hitCount, 1);
    CU_ASSERT_EQUAL(_currentInfo->currentState, playerState_normal);
    
}

void _shootPlayer(int times,int isDamage)
{
    int i = 0;
    for(i=0; i < times;  i++)
            playerState_proceed(isDamage); //damage

}

void Kill_Player_Test()
{
    PlayerState_Info _currentInfo = playerState_init(6);
    
    _shootPlayer(3,1);
    
    playerState_proceed(1); //last shoot
    playerState_proceed(0); //empty frame
    
    _shootPlayer(3,1);
    
    playerState_proceed(1); //last shot
    
    CU_ASSERT_NOT_EQUAL(playerState_isDead(),0);
    
}


int main() {
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("player_test", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if (    (NULL == CU_add_test(pSuite, "Player_Born_Test",Player_Born_Test)) ||
            (NULL == CU_add_test(pSuite, "Shock_Player_Test",Shock_Player_Test)) ||
            //(NULL == CU_add_test(pSuite, "Normal_Player_Timeout_Test",Normal_Player_Timeout_Test)) ||
            //(NULL == CU_add_test(pSuite, "Normal_Healing_Player_Test", Normal_Healing_Player_Test)) ||
            (NULL == CU_add_test(pSuite, "Kill_Player_Test",Kill_Player_Test)) 
            )   
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
