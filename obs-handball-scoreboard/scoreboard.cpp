#include "scoreboard.h"
#include "ui_scoreboard.h"
#include <QTimer>
#include <QString>
#include <QFileDialog>

ScoreBoard::ScoreBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScoreBoard)
{
    this->gameScore = new GameScore();
    this->timer = new QTimer(this);
    this->writeFileTimer = new QTimer(this);
    this->time = QTime(0,0,0,0);
    this->rootPath = "C:/DEV/beachtour/obs";

    connect(this->timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    connect(this->writeFileTimer, SIGNAL(timeout()), this, SLOT(updateOBSFiles()));
    ui->setupUi(this);
    ui->rootPath->setText(this->rootPath);
}

ScoreBoard::~ScoreBoard()
{
    delete ui;
}
const QString ScoreBoard::fileNames[] = {"HomeHT1", "HomeHT2", "HomeOT", "HomeLive", "HomePoints",
                                      "GuestHT1", "GuestHT2", "GuestOT", "GuestLive", "GuestPoints",
                                         "Timer", "GuestName", "HomeName", "TournamentName" };

void ScoreBoard::addTwoGuest()
{
    ui->liveGuest->stepUp();
    ui->liveGuest->stepUp();
}
void ScoreBoard::addTwoHome()
{
    ui->liveHome->stepUp();
    ui->liveHome->stepUp();
}
void ScoreBoard::minusTwoGuest()
{
    ui->liveGuest->stepDown();
    ui->liveGuest->stepDown();
}
void ScoreBoard::minusTwoHome()
{
    ui->liveHome->stepDown();
    ui->liveHome->stepDown();
}

void ScoreBoard::startTimer()
{
    this->timer->start(1000);
}
void ScoreBoard::stopTimer() {
    this->timer->stop();
}
void ScoreBoard::setTimerSeconds(QString seconds)
{
    this->time = QTime(this->time.hour(), this->time.minute(), seconds.toInt());
}
void ScoreBoard::setTimerMinutes(QString minutes)
{
    this->time = QTime(this->time.hour(), minutes.toInt(), this->time.second());
}
void ScoreBoard::updateTime() {
    this->time = this->time.addSecs(1);
    QString minutes = this->time.toString("mm");
    ui->timerMinutes->setText(minutes);

    QString seconds = this->time.toString("ss");
    ui->timerSeconds->setText(seconds);
}
void ScoreBoard::setCountUp(bool) {}

void ScoreBoard::set1HTGuest(QString value)
{
    this->gameScore->GuestHT1Score = value;
}
void ScoreBoard::set2HTGuest(QString value)
{
    this->gameScore->GuestHT2Score = value;
}
void ScoreBoard::setOTGuest(QString value)
{
    this->gameScore->GuestOTScore = value;
}
void ScoreBoard::setGuestLiveScore(QString value)
{
    this->gameScore->GuestLiveScore = value;
}
void ScoreBoard::setGuestPoints(QString value)
{
    this->gameScore->GuestPoints = value;
}

void ScoreBoard::set1HTHome(QString value)
{
    this->gameScore->HomeHT1Score = value;
}
void ScoreBoard::set2HTHome(QString value)
{
    this->gameScore->HomeHT2Score = value;
}
void ScoreBoard::setOTHome(QString value)
{
    this->gameScore->HomeOTScore = value;
}
void ScoreBoard::setHomeLiveScore(QString value)
{
    this->gameScore->HomeLiveScore = value;
}
void ScoreBoard::setHomePoints(QString value)
{
    this->gameScore->HomePoints = value;
}

void ScoreBoard::setGuestName(QString value)
{
    this->guestName = value;
    this->updateFile("GuestName", this->guestName);
}
void ScoreBoard::setHomeName(QString value)
{
    this->homeName = value;
    this->updateFile("HomeName", this->homeName);
}
void ScoreBoard::setTournamentName(QString value)
{
    this->tournamentName = value;
    this->updateFile("TournamentName", this->tournamentName);
}

void ScoreBoard::openFileBrowser()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    this->rootPath = dir;
    ui->rootPath->setText(dir);
}

void ScoreBoard::setContinuousUpdate(bool active)
{
    if (active) {
        this->writeFileTimer->start(500);
    } else {
        this->writeFileTimer->stop();
    }
}
void ScoreBoard::updateOBSFiles()
{
    QString values[14] = {this->gameScore->HomeHT1Score, this->gameScore->HomeHT2Score,
                       this->gameScore->HomeOTScore, this->gameScore->HomeLiveScore,
                        this->gameScore->HomePoints, this->gameScore->GuestHT1Score,
                         this->gameScore->GuestHT2Score, this->gameScore->GuestOTScore,
                          this->gameScore->GuestLiveScore, this->gameScore->GuestPoints,
                           this->time.toString("mm") + " : " + this->time.toString("ss"),
                            this->guestName, this->homeName, this->tournamentName};
    for (unsigned int i = 0; i<14;i++) {
        this->updateFile(ScoreBoard::fileNames[i], values[i]);
    }

    if (!QDir(this->rootPath + "/SO").exists())
    {
        QDir().mkdir(this->rootPath + "/SO");
    }

    for (unsigned int i = 0; i<10;i++) {
        this->updateFile("SO/SOH"+ QString::number(i+1), this->gameScore->HomeSOGoals[i]);
    }
    for (unsigned int i = 0; i<10;i++) {
        this->updateFile("SO/SOG"+ QString::number(i+1), this->gameScore->GuestSOGoals[i]);
    }
}
void ScoreBoard::updateFile(QString fileName, QString value)
{
    QFile file(this->rootPath + "/" + fileName + ".txt");
    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QString text = value;
        file.write(text.toUtf8());
        file.close();

    }
}

void ScoreBoard::loadStateFromFiles()
{
    QByteArray fileData;
    for (unsigned int i = 0; i<sizeof(ScoreBoard::fileNames)/sizeof(ScoreBoard::fileNames[0]);i++) {
        QString fileName(ScoreBoard::fileNames[i]);
        QFile file(this->rootPath + "/" + fileName + ".txt");
        if (file.open(QIODevice::ReadWrite))
        {
            fileData = file.readAll();
            QString text(fileData);
            this->updateField(fileName, text);
            file.close();

        }
    }

}

void ScoreBoard::setRootPath(QString value)
{
    this->rootPath = value;
}

void ScoreBoard::updateField(QString fileName, QString value)
{
    if (fileName == "HomeHT1")
    {
        this->gameScore->HomeHT1Score = value;
        ui->firstHTHome->setValue(value.toInt());
    } else if (fileName == "HomeHT2")
    {
        this->gameScore->HomeHT2Score = value;
        ui->secondHTHome->setValue(value.toInt());
    } else if (fileName == "HomeOT")
    {
        this->gameScore->HomeOTScore = value;
        ui->OTHome->setValue(value.toInt());
    } else if (fileName == "HomeLive")
    {
        this->gameScore->HomeLiveScore = value;
        ui->liveHome->setValue(value.toInt());
    } else if (fileName == "HomePoints")
    {
        this->gameScore->HomePoints = value;
        ui->HomePoints->setValue(value.toInt());
    } else if (fileName == "GuestHT1")
    {
        this->gameScore->GuestHT1Score = value;
        ui->firstHTGuest->setValue(value.toInt());
    } else if (fileName == "GuestHT2")
    {
        this->gameScore->GuestHT2Score = value;
        ui->secondHTGuest->setValue(value.toInt());
    } else if (fileName == "GuestOT")
    {
        this->gameScore->GuestOTScore = value;
        ui->otGuest->setValue(value.toInt());
    } else if (fileName == "GuestLive")
    {
        this->gameScore->GuestLiveScore = value;
        ui->liveGuest->setValue(value.toInt());
    } else if (fileName == "GuestPoints")
    {
        this->gameScore->GuestPoints = value;
        ui->GuestPoints->setValue(value.toInt());
    } else if (fileName == "Timer")
    {
        QStringList splits = value.split(" : ");
        this->time = QTime(0,splits[0].toInt(), splits[1].toInt());
        ui->timerMinutes->setText(splits[0]);
        ui->timerSeconds->setText(splits[1]);
    } else if (fileName == "GuestName")
    {
        this->guestName = value;
        ui->teamNameGuest->setText(value);
    } else if (fileName == "HomeName")
    {
        this->homeName = value;
        ui->teamNameHome->setText(value);
    } else if (fileName == "TournamentName")
    {
        this->tournamentName =value;
        ui->tournamentName->setText(value);
    }
}

void ScoreBoard::setSOG1(QString value) {
  this->gameScore->GuestSOGoals[0] = value;
}
void ScoreBoard::setSOG2(QString value) {
  this->gameScore->GuestSOGoals[1] = value;
}
void ScoreBoard::setSOG3(QString value) {
  this->gameScore->GuestSOGoals[2] = value;
}
void ScoreBoard::setSOG4(QString value) {
  this->gameScore->GuestSOGoals[3] = value;
}
void ScoreBoard::setSOG5(QString value) {
  this->gameScore->GuestSOGoals[4] = value;
}
void ScoreBoard::setSOG6(QString value) {
  this->gameScore->GuestSOGoals[5] = value;
}
void ScoreBoard::setSOG7(QString value) {
  this->gameScore->GuestSOGoals[6] = value;
}
void ScoreBoard::setSOG8(QString value) {
  this->gameScore->GuestSOGoals[7] = value;
}
void ScoreBoard::setSOG9(QString value) {
  this->gameScore->GuestSOGoals[8] = value;
}
void ScoreBoard::setSOG10(QString value) {
  this->gameScore->GuestSOGoals[9] = value;
}

void ScoreBoard::setSOH1(QString value) {
  this->gameScore->HomeSOGoals[0] = value;
}
void ScoreBoard::setSOH2(QString value) {
  this->gameScore->HomeSOGoals[1] = value;
}
void ScoreBoard::setSOH3(QString value) {
  this->gameScore->HomeSOGoals[2] = value;
}
void ScoreBoard::setSOH4(QString value) {
  this->gameScore->HomeSOGoals[3] = value;
}
void ScoreBoard::setSOH5(QString value) {
  this->gameScore->HomeSOGoals[4] = value;
}
void ScoreBoard::setSOH6(QString value) {
  this->gameScore->HomeSOGoals[5] = value;
}
void ScoreBoard::setSOH7(QString value) {
  this->gameScore->HomeSOGoals[6] = value;
}
void ScoreBoard::setSOH8(QString value) {
  this->gameScore->HomeSOGoals[7] = value;
}
void ScoreBoard::setSOH9(QString value) {
  this->gameScore->HomeSOGoals[8] = value;
}
void ScoreBoard::setSOH10(QString value) {
    this->gameScore->HomeSOGoals[9] = value;
}

void ScoreBoard::copyLiveScoreToSO()
{
    this->gameScore->HomeOTScore = QString::number(ui->liveHome->value());
    this->gameScore->GuestOTScore = QString::number(ui->liveGuest->value());
    ui->OTHome->setValue(this->gameScore->HomeOTScore.toInt());
    ui->otGuest->setValue(this->gameScore->GuestOTScore.toInt());
}

void ScoreBoard::copyLiveScoreToHT1()
{
    this->gameScore->HomeHT1Score = QString::number(ui->liveHome->value());
    this->gameScore->GuestHT1Score = QString::number(ui->liveGuest->value());
    ui->firstHTHome->setValue(this->gameScore->HomeHT1Score.toInt());
    ui->firstHTGuest->setValue(this->gameScore->GuestHT1Score.toInt());
}

void ScoreBoard::copyLiveScoreToHT2()
{
    this->gameScore->HomeHT2Score = QString::number(ui->liveHome->value());
    this->gameScore->GuestHT2Score = QString::number(ui->liveGuest->value());
    ui->secondHTHome->setValue(this->gameScore->HomeHT2Score.toInt());
    ui->secondHTGuest->setValue(this->gameScore->GuestHT2Score.toInt());
}
