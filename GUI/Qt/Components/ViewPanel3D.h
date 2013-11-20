#ifndef VIEWPANEL3D_H
#define VIEWPANEL3D_H

#include <SNAPComponent.h>

namespace Ui {
  class ViewPanel3D;
}

class Generic3DModel;
class GenericView3D;

class ViewPanel3D : public SNAPComponent
{
  Q_OBJECT

public:
  explicit ViewPanel3D(QWidget *parent = 0);
  ~ViewPanel3D();

  // Register with the global model
  void Initialize(GlobalUIModel *model);

  GenericView3D *Get3DView();

private slots:
  virtual void onModelUpdate(const EventBucket &bucket);

  void on_btnUpdateMesh_clicked();

  void on_btnScreenshot_clicked();

  void on_btnResetView_clicked();

  void on_btnAccept_clicked();

  void on_btnCancel_clicked();

  void on_btnExpand_clicked();



private:
  Ui::ViewPanel3D *ui;

  GlobalUIModel *m_GlobalUI;

  Generic3DModel *m_Model;

  void UpdateExpandViewButton();
};

#endif // VIEWPANEL3D_H
