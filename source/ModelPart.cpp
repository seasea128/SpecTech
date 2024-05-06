/**     @file ModelPart.cpp
 *
 *     EEEE2076 - Software Engineering & VR Project
 *
 *     Template for model parts that will be added as treeview items
 *
 *     P Evans 2022
 */

#include "ModelPart.h"

/* Commented out for now, will be uncommented later when you have
 * installed the VTK library
 */
#include <vtkActor.h>
#include <vtkClipDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkPlane.h>
#include <vtkProperty.h>
#include <vtkShrinkFilter.h>
#include <vtkSmartPointer.h>
#include <vtkWeakPointerBase.h>

#include "Utils.h"

ModelPart::ModelPart(const QList<QVariant> &data, ModelPart *parent)
    : m_itemData(data), m_parentItem(parent), file(nullptr), fileName(""),
      filterList({}), vrFilterList({}) {
  /* You probably want to give the item a default colour */
  isVisible = true;
  setColour(255, 255, 255);
  metallic = 0.5;
  anisotropy = 0.5;
  anisotropyrotation = 0.5;
  roughness = 0.5;
}

ModelPart::~ModelPart() { qDeleteAll(m_childItems); }

void ModelPart::appendChild(ModelPart *item) {
  /* Add another model part as a child of this part
   * (it will appear as a sub-branch in the treeview)
   */
  item->m_parentItem = this;
  m_childItems.append(item);
}

ModelPart *ModelPart::child(int row) {
  /* Return pointer to child item in row below this item.
   */
  if (row < 0 || row >= m_childItems.size())
    return nullptr;
  return m_childItems.at(row);
}

int ModelPart::childCount() const {
  /* Count number of child items
   */
  return m_childItems.count();
}

int ModelPart::columnCount() const {
  /* Count number of columns (properties) that this item has.
   */
  return m_itemData.count();
}

QVariant ModelPart::data(int column) const {
  /* Return the data associated with a column of this item
   *  Note on the QVariant type - it is a generic placeholder type
   *  that can take on the type of most Qt classes. It allows each
   *  column or property to store data of an arbitrary type.
   */
  if (column < 0 || column >= m_itemData.size())
    return QVariant();
  return m_itemData.at(column);
}

void ModelPart::set(int column, const QVariant &value) {
  /* Set the data associated with a column of this item
   */
  if (column < 0 || column >= m_itemData.size())
    return;

  m_itemData.replace(column, value);
}

ModelPart *ModelPart::parentItem() { return m_parentItem; }

int ModelPart::row() const {
  /* Return the row index of this item, relative to it's parent.
   */
  if (m_parentItem)
    return m_parentItem->m_childItems.indexOf(const_cast<ModelPart *>(this));
  return 0;
}

void ModelPart::setColour(const unsigned char R, const unsigned char G,
                          const unsigned char B) {
  // Set colour recursively, also set it to property inside actor
  colour.Set(R, G, B);

  for (const auto &child : m_childItems) {
    child->setColour(R, G, B);
  }

  if (actor != nullptr) {
    actor->GetProperty()->SetColor(double(colour.GetRed()) / 255,
                                   double(colour.GetGreen()) / 255,
                                   double(colour.GetBlue()) / 255);
  }
}

vtkWeakPointer<vtkActor> ModelPart::getVRActor() const { return vrActor; }

void ModelPart::removeChild(ModelPart *child) {
  // Delete given child in the list of child
  auto index = m_childItems.indexOf(child);
  m_childItems.removeAt(index);
  delete child;
}

unsigned char ModelPart::getColourR() {
  /* This is a placeholder function that will be used in the next worksheet
   */

  /* As the name suggests ... */
  return colour.GetRed(); // needs updating
}

unsigned char ModelPart::getColourG() {
  /* This is a placeholder function that will be used in the next worksheet
   */

  /* As the name suggests ... */
  return colour.GetGreen(); // needs updating
}

unsigned char ModelPart::getColourB() {
  /* This is a placeholder function that will be used in the next worksheet
   */

  /* As the name suggests ... */
  return colour.GetBlue(); // needs updating
}

float ModelPart::getMetallic() { return metallic; }

float ModelPart::getRoughness() { return roughness; }

float ModelPart::getAnisotropy() { return anisotropy; }

float ModelPart::getAnisotropyRotation() { return anisotropyrotation; }

void ModelPart::setMetallic(const float M) {
  // Set metallic recursively
  metallic = M;
  for (const auto &child : m_childItems) {
    child->setMetallic(M);
  }

  if (actor != nullptr) {
    actor->GetProperty()->SetMetallic(metallic);
  }
}
void ModelPart::setRoughness(const float R) {
  // Set roughness recursively
  roughness = R;
  for (const auto &child : m_childItems) {
    child->setRoughness(R);
  }

  if (actor != nullptr) {
    actor->GetProperty()->SetRoughness(roughness);
  }
}
void ModelPart::setAnisotropy(const float A) {
  // Set anisotropy recursively
  anisotropy = A;
  for (const auto &child : m_childItems) {
    child->setAnisotropy(A);
  }

  if (actor != nullptr) {
    actor->GetProperty()->SetAnisotropy(anisotropy);
  }
}
void ModelPart::setAnisotropyRotation(const float AR) {
  // Set anisotropy rotation recursively
  anisotropyrotation = AR;
  for (const auto &child : m_childItems) {
    child->setAnisotropyRotation(AR);
  }

  if (actor != nullptr) {
    actor->GetProperty()->SetAnisotropyRotation(anisotropyrotation);
  }
}

void ModelPart::setVisible(bool isVisible) {
  // Set visibility recursively
  this->isVisible = isVisible;
  for (const auto &child : m_childItems) {
    child->setVisible(isVisible);
  }
  if (isVisible) {
    set(1, QString("true"));
  } else {
    set(1, QString("false"));
  }
  if (actor != nullptr) {
    actor->GetProperty()->SetOpacity(isVisible ? 1.0 : 0.0);
  }
}

bool ModelPart::visible() const {
  /* This is a placeholder function that will be used in the next worksheet
   */

  /* As the name suggests ... */
  return isVisible;
}

void ModelPart::loadSTL(QString fileName) {
  /* This is a placeholder function that will be used in the next worksheet
   */

  /* 1. Use the vtkSTLReader class to load the STL file
   *     https://vtk.org/doc/nightly/html/classvtkSTLReader.html
   */
  file = vtkSmartPointer<vtkSTLReader>::New(); /**< Datafile from which part
                                                  loaded */

  this->fileName = fileName.toStdString();
  qDebug() << "File pointer: " << file;
  qDebug() << "File name: " << this->fileName.c_str();
  file->SetFileName(this->fileName.c_str());
  file->Update();

  /* 2. Initialise the part's vtkMapper */
  mapper =
      vtkSmartPointer<vtkDataSetMapper>::New(); /**< Mapper for rendering */

  /* 3. Initialise the part's vtkActor and link to the mapper */
  actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  setColour(colour.GetRed(), colour.GetGreen(), colour.GetBlue());

  // PBR related settings
  actor->GetProperty()->SetInterpolationToPBR();
  actor->GetProperty()->SetRoughness(roughness);
  actor->GetProperty()->SetMetallic(metallic);
  actor->GetProperty()->SetAnisotropy(anisotropy);
  actor->GetProperty()->SetAnisotropyRotation(anisotropyrotation);

  Utils::setFilterChainFromListWithFile(filterList, file, mapper);

  // Rotate the actor so that it matches up with the ground
  double *ac = actor->GetOrigin();

  actor->RotateX(-90);
  actor->AddPosition(-ac[0] + 0, -ac[1] - 100, -ac[2] - 200);
}

vtkSmartPointer<vtkActor> ModelPart::getActor() {
  /* Needs to return a smart pointer to the vtkActor to allow
   * part to be rendered.
   */
  return actor;
}

vtkColor3<unsigned char> ModelPart::getColour() const { return colour; }

vtkSmartPointer<vtkActor> ModelPart::getNewActor() {
  /* 1. Create new mapper */
  qDebug() << "Creating new mapper";
  vrMapper = vtkSmartPointer<vtkDataSetMapper>::New();
  qDebug() << "Setting connection";
  qDebug() << "File pointer: " << file;
  if (file == nullptr) {
    qDebug() << "File is null pointer, stopping";
    return nullptr;
  }

  // Create new polyData and copy the data from file reader so that another
  // filter chain can be applied to the VR actor.
  vtkSmartPointer<vtkPolyData> vrPolyData = vtkSmartPointer<vtkPolyData>::New();

  vrPolyData->DeepCopy(file->GetOutputDataObject(0));

  vrFilterList = Utils::copyFilterList(filterList);

  Utils::setFilterChainFromListWithPolyData(vrFilterList, vrPolyData, vrMapper);

  qDebug() << "Created new mapper";

  /* 2. Create new actor and link to mapper */
  auto localVRActor = vtkSmartPointer<vtkActor>::New();
  localVRActor->SetMapper(vrMapper);

  /* 3. Link the vtkProperties of the original actor to the new actor. This
   * means if you change properties of the original part (colour, position,
   * etc), the changes will be reflected in the GUI AND VR rendering.
   *
   * See the vtkActor documentation, particularly the GetProperty() and
   * SetProperty() functions.
   */

  // Copy actor's property to newActor, vtkProperty is not 100% threadsafe if it
  // is being access too often from another thread.
  localVRActor->GetProperty()->DeepCopy(actor->GetProperty());

  vrActor = localVRActor; // Assign to a weak pointer stored inside this class

  return localVRActor;
}

std::vector<Filter::FilterData> ModelPart::getFilterList() const {
  return filterList;
}

void ModelPart::setFilterChainFromList() {
  // Set filter chain only if file reader and mapper exist.
  if (file != nullptr && mapper != nullptr) {
    Utils::setFilterChainFromListWithFile(filterList, file, mapper);
  }
  for (auto &child : m_childItems) {
    child->setFilterChainFromList();
  }
}

void ModelPart::setFilterList(
    const std::vector<Filter::FilterData> &_filterList) {
  // Set filter list recursively.
  filterList = _filterList;
  for (auto &child : m_childItems) {
    child->setFilterList(Utils::copyFilterList(_filterList));
  }
}

vtkSmartPointer<vtkSTLReader> ModelPart::getFileReader() const { return file; }

void ModelPart::setVRPolyData(vtkSmartPointer<vtkPolyData> newPolyData) {
  vrPolyData = newPolyData;
}

vtkSmartPointer<vtkMapper> ModelPart::getVRMapper() const { return vrMapper; }

void ModelPart::setVRFilterList(
    const std::vector<Filter::FilterData> &newfilterList) {
  vrFilterList = newfilterList;
}
