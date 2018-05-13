#ifndef GAMESCORE_H
#define GAMESCORE_H

#include <QString>

class GameScore
{
public:
    GameScore();
    QString HomeHT1Score;
    QString HomeHT2Score;
    QString HomeOTScore;

    QString GuestHT1Score;
    QString GuestHT2Score;
    QString GuestOTScore;

    QString HomeLiveScore;
    QString GuestLiveScore;

    QString HomePoints;
    QString GuestPoints;

    QString HomeSOGoals[10];
    QString GuestSOGoals[10];
};

#endif // GAMESCORE_H
