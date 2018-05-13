#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <QWidget>
#include <QTime>
#include "gamescore.h"

namespace Ui {
class ScoreBoard;
}

class ScoreBoard : public QWidget
{
    Q_OBJECT

public:
    explicit ScoreBoard(QWidget *parent = 0);
    ~ScoreBoard();
    static const QString fileNames[];

private:
    Ui::ScoreBoard *ui;
    class QTimer *timer;
    class QTime time;
    class QTimer *writeFileTimer;

    QString tournamentName;
    QString homeName;
    QString guestName;

    class GameScore *gameScore;

    QString rootPath = "/";
    void updateFile(QString, QString);
    void updateField(QString, QString);

private slots:
    void addTwoGuest();
    void addTwoHome();
    void minusTwoGuest();
    void minusTwoHome();
    void startTimer();
    void stopTimer() ;
    void setCountUp(bool) ;
    void set1HTGuest(QString) ;
    void set2HTGuest(QString);
    void setOTGuest(QString) ;
    void setGuestName(QString);
    void setOTHome(QString) ;
    void set2HTHome(QString) ;
    void set1HTHome(QString) ;
    void setHomeLiveScore(QString);
    void setGuestLiveScore(QString);
    void setHomeName(QString) ;
    void setTimerSeconds(QString);
    void setTimerMinutes(QString) ;
    void setTournamentName(QString) ;
    void setContinuousUpdate(bool) ;
    void updateOBSFiles();
    void openFileBrowser();
    void setHomePoints(QString);
    void setGuestPoints(QString);
    void loadStateFromFiles();
    void setRootPath(QString);

    void setSOG1(QString);
    void setSOG2(QString);
    void setSOG3(QString);
    void setSOG4(QString);
    void setSOG5(QString);
    void setSOG6(QString);
    void setSOG7(QString);
    void setSOG8(QString);
    void setSOG9(QString);
    void setSOG10(QString);

    void setSOH1(QString);
    void setSOH2(QString);
    void setSOH3(QString);
    void setSOH4(QString);
    void setSOH5(QString);
    void setSOH6(QString);
    void setSOH7(QString);
    void setSOH8(QString);
    void setSOH9(QString);
    void setSOH10(QString);

    void copyLiveScoreToSO();
    void copyLiveScoreToHT1();
    void copyLiveScoreToHT2();

    void updateTime();
};

#endif // SCOREBOARD_H
