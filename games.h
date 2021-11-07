

#ifndef GAMES_H
#define	GAMES_H

#ifdef	__cplusplus
// extern "C" {
#endif

    /** @brief A enum to track the state of the game */
    typedef enum
    {
        PRE_GAME,
        START_GAME,
        RUNNING_GAME,
        END_GAME,
    } GameState_e;

#ifdef	__cplusplus
// }
#endif

#endif	/* GAMES_H */