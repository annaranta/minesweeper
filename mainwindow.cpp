#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "gameboard.hh"
#include "gameboard.cpp"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    images();

    initGrid();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onSecTimer);

    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::onresetButtonClicked);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onstartButtonClicked);
    connect(ui->flagPushButton, &QPushButton::clicked, this, &MainWindow::onflagPushButtonClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
    for (auto& button_line : buttons_)
    {
        for (auto& button : button_line)
        {
            delete button;
        }
    }
    delete timer;
}

void MainWindow::buttonClick()
{

    for (int y = 0; y < BOARD_SIDE; y++)
    {
        for (int x = 0; x < BOARD_SIDE; x++)
        {
            // Finds the button that was clicked.
            if (buttons_.at(y).at(x) == sender())
            {
                // Gets the correspondin square from the backend.
                Square board_square = board.getSquare(x,y);

                if (flag_on)
                {
                    // Calls for the function that handles the button flagging.
                    flagButton(x, y, board_square);
                    return;
                }
                else if (question_mark_on)
                {
                    // Calls for the function that handles putting question marks.
                    questionMarkButton(x, y, board_square);
                    return;
                }
                else
                {
                    // A flagged square cannot be opened.
                    if (board_square.hasFlag())
                    {
                        return;
                    }
                    // Calls for function that handles opening the square.
                    gameplay(x, y, board_square);

                    return;
                }
                // After every click the board checks if the game is won.
                checkForWin();

                return;
            }
        }
    }

}

void MainWindow::gameplay(int x, int y, Square board_square)
{
    QPushButton* sender_button = buttons_.at(y).at(x);

    board_square.open();

    board.setSquare(board_square,x,y);

    // An opened button cannot be touched.
    sender_button->setCheckable(true);
    sender_button->setChecked(true);
    sender_button->setDisabled(true);


    if (board_square.hasMine())
    {
        // When a mine is clicked the game is lost.
        gameLost();

    }
    else
    {
        for (int y = 0 ; y < BOARD_SIDE; ++y)
        {
            for (int x = 0 ; x < BOARD_SIDE ; ++x )
            {
                if (board.getSquare(x,y).isOpen() and !board.getSquare(x,y).hasMine())
                {
                    // If clicked button doesn't contain mine it reveals a number
                    // that tells how many mines are in its adjacent buttons.
                    // Opens empty adjacent squares.
                    int adjacent = board.getSquare(x,y).countAdjacent();

                    if (adjacent > 0)
                    {
                        buttons_.at(y).at(x)->setText(QString::number(adjacent));
                    }
                    buttons_.at(y).at(x)->setCheckable(true);
                    buttons_.at(y).at(x)->setChecked(true);
                    buttons_.at(y).at(x)->setDisabled(true);
                 }
            }
        }
    }

}

void MainWindow::flagButton(int x, int y, Square board_square)
{
    // Flag can be removed when the flag button is on.
    if (board_square.hasFlag())
    {
        buttons_.at(y).at(x)->setIcon(QIcon());

        board_square.removeFlag();
        board.setSquare(board_square, x, y);
        return;
    }
    // Adds the flag to the wanted button.
    buttons_.at(y).at(x)->setIcon(flag_image_);

    board_square.addFlag();
    board.setSquare(board_square, x, y);

}

void MainWindow::questionMarkButton(int x, int y, Square board_square)
{
    //Question mark works similar to flag.
    if (board_square.hasQuestionMark())
    {
        buttons_.at(y).at(x)->setIcon(QIcon());

        board_square.removeQuestionMark();
        board.setSquare(board_square, x, y);
        return;
    }
    buttons_.at(y).at(x)->setIcon(questionmark_image_);

    board_square.addQuestionMark();
    board.setSquare(board_square, x, y);
}


void MainWindow::initGrid()
{
    ui->flagPushButton->setIcon(click_image_);
    for (int y = 0; y < BOARD_SIDE; y++)
    {
        std::vector<QPushButton*> line;
        for (int x = 0; x < BOARD_SIDE; x++)
        {

            QPushButton* pushButton = new QPushButton(this);

            line.push_back(pushButton);

            // Pushbuttons are added in the wanted area.
            pushButton->setGeometry(2 * margin + x * (button_width + margin),
                                    2 * margin + y * (button_height + margin),
                                    button_width,
                                    button_height);

            pushButton->setCheckable(true);
            pushButton->setChecked(true);

            pushButton->setStyleSheet("color: black;");

            // Pushbuttons are connected to clicks.
            connect(pushButton, &QPushButton::clicked,
                    this, &MainWindow::buttonClick);
        }

        buttons_.push_back(line);
    }
}

void MainWindow::onSecTimer()
{

    int seconds = ui->lcdNumberSec->intValue();
    int minutes = ui->lcdNumberMin->intValue();

    ++seconds;

    if (seconds == 60)
    {
        seconds = 0;
        ++minutes;
    }

    ui->lcdNumberSec->display(seconds);
    ui->lcdNumberMin->display(minutes);
}


void MainWindow::on_seedSpinBox_valueChanged(int arg1)
{
    if (arg1 == 0)
    {
        seed_value_ = time(NULL);
    }
    else
    {
        seed_value_ = arg1;
    }
}

void MainWindow::onstartButtonClicked()
{
    timer->start(1000);

    // The back end board is created with the given seed number.
    board.init(seed_value_);

    for (auto& line : buttons_)
    {
        // Enables user to click on the gameboard.
        for (auto& button : line)
        {
            button->setChecked(false);
            button->setCheckable(false);
            button->setDisabled(false);
        }
    }
    // Disables the start button and seed spinbox while game is on.
    ui->startButton->setCheckable(true);
    ui->startButton->setChecked(true);
    ui->startButton->setDisabled(true);
    ui->seedSpinBox->setDisabled(true);
}

void MainWindow::onresetButtonClicked()
{
    // Clears the back end board for a new game.
    board.new_game();

    // Stops and resets the clock until start button is pressed.
    timer->stop();
    ui->lcdNumberMin->display(0);
    ui->lcdNumberSec->display(0);

    for (auto& line : buttons_)
    {
        for (auto& button : line)
        {
            // Disables and resets the gameboard until start button is pressed.
            button->setCheckable(true);
            button->setChecked(true);
            button->setDisabled(true);
            button->setIcon(QIcon());
            button->setText("");
        }
    }
    // Enables the use of start button and seed spin box.
    ui->textBrowser->setText("");
    ui->startButton->setChecked(false);
    ui->startButton->setCheckable(false);
    ui->startButton->setDisabled(false);
    ui->seedSpinBox->setDisabled(false);

}

void MainWindow::onflagPushButtonClicked()
{
    // By clicking the same button user can deside is they
    // want to put flag or question mark or just to click the button.
    if (question_mark_on)
    {
        ui->flagPushButton->setIcon(flag_image_);
        question_mark_on = false;
        flag_on = true;


    }
    else if (flag_on)
    {
        ui->flagPushButton->setIcon(click_image_);
        flag_on = false;
    }
    else
    {
        ui->flagPushButton->setIcon(questionmark_image_);
        question_mark_on = true;
    }
}

void MainWindow::checkForWin()
{
    bool game_end = true;
    for (int y = 0 ; y < BOARD_SIDE; ++y)
    {
        for (int x = 0 ; x < BOARD_SIDE ; ++x )
        {
            // If every square that contains a mine is flagged the game is won.
            if(board.getSquare(x,y).hasMine())
            {
                if(!board.getSquare(x,y).hasFlag())
                {
                    game_end = false;
                }
            }
        }
    }
    if (game_end)
    {
        ui->textBrowser->setText("Gongratulations! You have won the game!");
        for (auto& line : buttons_)
        {
            for (auto& button : line)
            {

                button->setCheckable(true);
                button->setChecked(true);
                button->setDisabled(true);
            }
        }
        timer->stop();
    }

}

void MainWindow::gameLost()
{
    timer->stop();

    for (int y = 0 ; y < BOARD_SIDE; ++y)
    {
        for (int x = 0 ; x < BOARD_SIDE ; ++x )
        {
            if(board.getSquare(x,y).hasMine())
            {
                buttons_.at(y).at(x)->setIcon(mine_image_);

            }
            buttons_.at(y).at(x)->setCheckable(true);
            buttons_.at(y).at(x)->setChecked(true);
            buttons_.at(y).at(x)->setDisabled(true);

        }
    }
    ui->textBrowser->setText("Too bad, you lost the game!");
}

void MainWindow::images()
{
    // Defining where the images can be found and what kind of images they are
       const std::string PREFIX(":/new/prefix1/resources/");
       const std::string SUFFIX(".png");


       // Converting image (png) to a pixmap
       std::string mine_file = PREFIX + "mine" + SUFFIX;
       QPixmap mine_image(QString::fromStdString(mine_file));

       std::string flag_file = PREFIX + "flag" + SUFFIX;
       QPixmap flag_image(QString::fromStdString(flag_file));

       std::string question_file = PREFIX + "questionmark" + SUFFIX;
       QPixmap questionmark_image(QString::fromStdString(question_file));

       std::string click_file = PREFIX + "click" + SUFFIX;
       QPixmap click_image(QString::fromStdString(click_file));

       // Scaling the pixmap
       mine_image_ = mine_image.scaled(button_width*30, button_height*30);
       flag_image_ = flag_image.scaled(button_width, button_height);
       questionmark_image_ = questionmark_image.scaled(button_width, button_height);
       click_image_ = click_image.scaled(button_width, button_height);
}

