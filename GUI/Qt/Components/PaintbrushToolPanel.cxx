#include "PaintbrushToolPanel.h"
#include "ui_PaintbrushToolPanel.h"

#include "PaintbrushSettingsModel.h"
#include "QtRadioButtonCoupling.h"
#include "QtCheckBoxCoupling.h"
#include "QtDoubleSpinBoxCoupling.h"
#include "QtSpinBoxCoupling.h"
#include "QtSliderCoupling.h"

PaintbrushToolPanel::PaintbrushToolPanel(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::PaintbrushToolPanel)
{
  ui->setupUi(this);
}

PaintbrushToolPanel::~PaintbrushToolPanel()
{
  delete ui;
}

void PaintbrushToolPanel::SetModel(PaintbrushSettingsModel *model)
{
  m_Model = model;

  // Couple the radio buttons
  QObjectList lModeButtons;
  lModeButtons.push_back(ui->btnSquare);
  lModeButtons.push_back(ui->btnRound);
  lModeButtons.push_back(ui->btnWatershed);
  makeRadioGroupCoupling(ui->grpBrushStyle,
                         m_Model->GetPaintbrushModeModel(),
                         lModeButtons);

  // Couple the other controls
  makeCoupling(ui->chkVolumetric, model->GetVolumetricBrushModel());
  makeCoupling(ui->chkIsotropic, model->GetIsotropicBrushModel());
  makeCoupling(ui->chkChase, model->GetChaseCursorModel());

  makeCoupling(ui->inBrushSizeSlider, model->GetBrushSizeModel());
  makeCoupling(ui->inBrushSizeSpinbox, model->GetBrushSizeModel());

  // Couple the visibility of the adaptive widget
  makeWidgetVisibilityCoupling(ui->grpAdaptive, model->GetAdaptiveModeModel());

  makeCoupling(ui->inGranularity, model->GetThresholdLevelModel());
  makeCoupling(ui->inSmoothness, model->GetSmoothingIterationsModel());
}