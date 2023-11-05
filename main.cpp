/* Minesweeper_gui
 *
 * Desc:
 *
 *   This program contains an minesweeper game with graphic user interface.
 *   Class mainwindow handles the board wich consists of grid that is made out
 *   of QPushbuttons, start and reset buttons, a button that allows user to
 *   place flags and question marks, spin box that contains the seed
 *   number and a timer.
 *   Before starting the game user can select a seed number that affects
 *   the places the mines are placed. When game is on user clicks buttons
 *   on the grid or flags them. A placed flag means that user thinks
 *   a mine is hidden under that box. If button doesent contain a mine
 *   it opens and reveals a number that says how many mines are in the buttons
 *   adjacent squares. With some logic thinking user can evaluate the buttons
 *   containing mines.
 *   Game ends if user presses a button that has mine in it. This means the game
 *   is lost. The game is won if user puts flags over every mine.
 *
 * Program author
 *
 * Name: Anna Rantanen
 * Student number: 50746217
 * UserID: mpanra
 * E-Mail: annainga.rantanen@tuni.fi
 *
 * */

#include "mainwindow.hh"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
