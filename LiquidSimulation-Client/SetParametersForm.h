#ifndef SETPARAMETERSFORM_H
#define SETPARAMETERSFORM_H

#include <QWidget>
#include "CellularAutomata.h"
#include "LiquidSceneCreator.h"
#include "LiquidSimulatorGUI.h"
#include "QMessageBox"
#include <QScreen>

namespace Ui {
class Form;
}

class SetParameterForm : public QWidget
{
    Q_OBJECT

public:
    explicit SetParameterForm(QWidget *parent = 0);
    ~SetParameterForm();

private:
    void initAutomata(CellularAutomata<float> *automata);
    Ui::Form *ui;
    LiquidSceneCreator* scene;
    CellularAutomata<float>* automata;
    LiquidSimulatorGUI simulatorGUI;
public slots:
    void submitParameters();

    void updateAutomata();
    void updateGrid();
    void setAddingType();
};

#endif // SETPARAMETERSFORM_H
