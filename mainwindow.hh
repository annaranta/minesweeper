/*
#############################################################################
# COMP.CS.110 Programming 2: Autumn 2021                                    #
# Project4: Minesweeper                                                     #
# File: mainwindow.hh                                                       #
# Description: Handles the graphic user interface of game. Works with       #
#            Gameboard and Square which allowes the gameplay. Mainwindow    #
#            contains a grid of pushbuttons, reset, start and flagbuttons,  #
#            timer and spin box that gives the seed value.                  #
#                                                                           #
#############################################################################
*/

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "gameboard.hh"
#include "square.hh"
#include <QMainWindow>
#include <QTextBrowser>
#include <QPushButton>
#include <vector>
#include <QTimer>
#include <QString>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    // Handles the timer.
    void onSecTimer();

    // Takes the seed value that is inputted by user.
    void on_seedSpinBox_valueChanged(int arg1);

    // Starts the timer and sets the board ready for playing.
    void onstartButtonClicked();

    // Cleares the board and stops the timer.
    void onresetButtonClicked();

    // Determines if the square will be opened or
    // if a flag or question mark is added on it.
    void onflagPushButtonClicked();


private:
    Ui::MainWindow *ui;

    QTimer* timer;

    // Measurements for the gameboard that consists of buttons.
    const int margin = 0;
    const int button_width = 300/BOARD_SIDE;
    const int button_height = 300/BOARD_SIDE;

    int seed_value_ = 0;

    QPixmap mine_image_;
    QPixmap flag_image_;
    QPixmap questionmark_image_;
    QPixmap click_image_;

    // Bool that keeps track of when user wants to place
    // flags on the board.
    bool flag_on = false;

    // Bool that keeps track of when user wants to place
    // question marks on the board.
    bool question_mark_on = false;

    // Pointers to the button gameboard with the correct coordinates.
    std::vector<std::vector<QPushButton*>> buttons_;

    // Creates the grid of buttons in wanted size.
    void initGrid();

    // Handles the users clics on the gameboard. Directs the program
    // to the correct function depending on if the user wants to place
    // flags, question marks or just open the button. Calls for
    // the function that checks if the game is won.
    void buttonClick();

    // Checks if mine is clicked and calls for the function that
    // handles the lost game, if necessary. If not counts the adjament
    // mines. If an empty button is clicked opens all empty adjament
    // squares.
    void gameplay(int x, int y, Square board_square);

    // Sets a flag on the wanted button. Informs the gameplay
    // that a square is flagged.
    void flagButton(int x, int y, Square board_square);

    // Sets a question mark on the wanted button.
    void questionMarkButton(int x, int y, Square board_square);

    // Cheks if all squares that have mines in them are flagged.
    // If yes, the game ends and a text that informs the user of
    // won game appears. Timer is also stopped
    void checkForWin();

    // If mine is clicked the game is lost and over. Shows the user
    // where the other mines were and timer stops.
    void gameLost();

    // Handles the images and adds them to the
    void images();

    GameBoard board;

};
#endif // MAINWINDOW_HH

