#include "SetParametersForm.h"
#include "ui_SetParametersForm.h"

SetParameterForm::SetParameterForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    this->automata= new CellularAutomata<float>(ui->rowsBox->value(),ui->rowsBox->value());
    initAutomata(automata);
    this->scene = new LiquidSceneCreator(*automata,ui->cellSizeBox->value());
    this->ui->graphicsView->setScene(scene);
    this->scene->drawGrid(*automata);
    this->setAddingType();
}
SetParameterForm::~SetParameterForm()
{
    delete ui;
}

void SetParameterForm::initAutomata(CellularAutomata<float> * automata){
    for(int i=0;i<automata->getSize();i++ )
        automata->state(i)=0;
    for(unsigned long i = 0;i<automata->getDim1();i++)
    {
        automata->state(i,0)=LiquidSimulation::SOLID;
        automata->state(i,automata->getDim2()-1)=LiquidSimulation::SOLID;
    }

    for(unsigned long j = 0;j<automata->getDim2();j++)
    {
        automata->state(0,j)=LiquidSimulation::SOLID;
        automata->state(automata->getDim1()-1,j)=LiquidSimulation::SOLID;
    }
}

void SetParameterForm::submitParameters()
{
    simulatorGUI.setNumProcesses(ui->nProcBox->currentText().toInt()+1);

    simulatorGUI.setNumColumns(ui->rowsBox->value());
    simulatorGUI.setNumRows(ui->rowsBox->value());

    simulatorGUI.setNumSteps(ui->stepsBox->value());
    simulatorGUI.setUpdateSteps(ui->updateBox->value());
    if(!simulatorGUI.validateParamenter())
    {

        QMessageBox::critical(this,"Error","Some parameters is not correct");
        return;
    }

    simulatorGUI.setCellSize(ui->cellSizeBox->value());
    simulatorGUI.setFixedSize(QSize(QGuiApplication::screens()[0]->size()*0.8));
    simulatorGUI.initScene(*automata);
    simulatorGUI.show();
    this->hide();

    if(!simulatorGUI.startSimulation())
    {
        QMessageBox::critical(&simulatorGUI,"Connection failed","Error in connection");
        return;
    }
}
void SetParameterForm::updateAutomata()
{
    delete automata;
    this->automata= new CellularAutomata<float>(ui->rowsBox->value(),ui->rowsBox->value());
    initAutomata(automata);

    scene->setAutomata(*automata);
    scene->recreateGrid(*automata,ui->cellSizeBox->value());
    scene->drawGrid(*automata);
}

void SetParameterForm::updateGrid()
{
    scene->recreateGrid(*automata,ui->cellSizeBox->value());
    scene->drawGrid(*automata);
}

void SetParameterForm::setAddingType()
{
    QString text = ui->comboBox->currentText();
    if(text == "Water")
        scene->setAddingType(LiquidSimulation::AddWater);
    else if (text == "Block")
        scene->setAddingType(LiquidSimulation::SOLID);
    else
        scene->setAddingType(LiquidSimulation::SOURCE_WATER);
}
