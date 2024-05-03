/**     @file ModelPart.h
 *
 *     EEEE2076 - Software Engineering & VR Project
 *
 *     Template for model parts that will be added as treeview items
 *
 *     P Evans 2022
 */

#ifndef VIEWER_MODELPART_H
#define VIEWER_MODELPART_H

#include "Filters/FilterData.h"
#include "Filters/FilterType.h"
#include <QList>
#include <QString>
#include <QVariant>

/* VTK headers - will be needed when VTK used in next worksheet,
 * commented out for now
 *
 * Note that there are a few function definitions and variables
 * commented out below - this is because you haven't yet installed
 * the VTK library which is needed.
 */
#include <vtkActor.h>
#include <vtkColor.h>
#include <vtkDataSetMapper.h>
#include <vtkMapper.h>
#include <vtkSTLReader.h>
#include <vtkSmartPointer.h>
#include <vtkWeakPointer.h>

/**
 * ModelPart stores data related to each part. It stores the actor for GUI views
 * as well as a weak reference to the actor for RenderThread.
 */
class ModelPart {
public:
  /** Constructor
   * @param data is a List (array) of strings for each property of this item
   * (part name and visiblity in our case
   * @param parent is the parent of this item (one level up in tree)
   */
  ModelPart(const QList<QVariant> &data, ModelPart *parent = nullptr);

  /** Destructor
   *
   * Needs to free array of child items
   */
  ~ModelPart();

  /** Add a child to this item.
   * @param item Pointer to child object (must already be allocated using new)
   */
  void appendChild(ModelPart *item);

  /** Return child at position 'row' below this item
   * @param row is the row number (below this item)
   * @return pointer to the item requested.
   */
  ModelPart *child(int row);

  /** Return number of children to this item
   * @return number of children
   */
  int childCount() const;

  /** Get number of data items (2 - part name and visibility string) in this
   * case.
   * @return number of visible data columns
   */
  int columnCount() const;

  /** Return the data item at a particular column for this item.
   * i.e. either part name of visibility
   * used by Qt when displaying tree
   * @param column is column index
   * @return the QVariant (represents string)
   */
  QVariant data(int column) const;

  /** Default function required by Qt to allow setting of part
   * properties within treeview.
   * @param column is the index of the property to set
   * @param value is the value to apply
   */
  void set(int column, const QVariant &value);

  /** Get pointer to parent item
   * @return pointer to parent item
   */
  ModelPart *parentItem();

  /** Get row index of item, relative to parent item
   * @return row index
   */
  int row() const;

  /** Set colour
   * (0-255 RGB values as ints)
   * @param R is the red channel of the colour being set.
   * @param G is the green channel of the colour being set.
   * @param B is the blue channel of the colour being set.
   */
  void setColour(const unsigned char R, const unsigned char G,
                 const unsigned char B);

  /**
   * Function that returns the red channel of vtkColour.
   * @return Red channel of vtkColour.
   */
  unsigned char getColourR();

  /**
   * Function that returns the green channel of vtkColour.
   * @return Green channel of vtkColour.
   */
  unsigned char getColourG();

  /**
   * Function that returns the blue channel of vtkColour.
   * @return Blue channel of vtkColour.
   */
  unsigned char getColourB();

  /**
   * Function that set the metallic property of the actor.
   * @param M is the new metallic property.
   */
  void setMetallic(const float M);

  /**
   * Function that set the roughness property of the actor.
   * @param R is the new roughness property.
   */
  void setRoughness(const float R);

  /**
   * Function that set the anisotropy property of the actor.
   * @param A is the new anisotropy property.
   */
  void setAnisotropy(const float A);

  /**
   * Function that set the anisotropy rotation property of the actor.
   * @param AR is the new anisotropy rotation property.
   */
  void setAnisotropyRotation(const float AR);

  /**
   * Function that returns the metallic value of the object.
   * @return Metallic value.
   */
  float getMetallic();

  /**
   * Function that returns the roughness value of the object.
   * @return Roughness value.
   */
  float getRoughness();

  /**
   * Function that returns the anisotropy value of the object.
   * @return Anisotropy value.
   */
  float getAnisotropy();

  /**
   * Function that returns the anisotropy rotation of the object.
   * @return Anisotropy rotation.
   */
  float getAnisotropyRotation();

  /**
   * Function that will search through the child and deletes it.
   * @param child is the pointer to the child that is being deleted.
   */
  void removeChild(ModelPart *child);

  /** Set visible flag
   * @param isVisible sets visible/non-visible
   */
  void setVisible(bool isVisible);

  /** Get visible flag
   * @return visible flag as boolean
   */
  bool visible() const;

  /** Load STL file
   * @param fileName
   */
  void loadSTL(QString fileName);

  /** Return actor
   * @return pointer to default actor for GUI rendering
   */
  vtkSmartPointer<vtkActor> getActor();

  /** Return new actor for use in VR
   * @return pointer to new actor
   */
  vtkSmartPointer<vtkActor> getNewActor();

  /**
   * Function that return colour
   * @return colour for this part
   */
  vtkColor3<unsigned char> getColour() const;

  /**
   * Function that returns weak pointer to VR actor.
   * @return Weak pointer to VR actor.
   */
  vtkWeakPointer<vtkActor> getVRActor() const;

  /**
   * Returns a copy of filter list.
   * @return Copy of filter list.
   */
  std::vector<Filter::FilterData> getFilterList() const;

  /**
   * Set filterList in this part to modified version.
   * @param _filterList is the modified filter list.
   */
  void setFilterList(const std::vector<Filter::FilterData> &_filterList);

  /**
   * Set filter chain in this part.
   */
  void setFilterFromList();

  /**
   * Get file reader from this part.
   * @return File reader of this part.
   */
  vtkSmartPointer<vtkSTLReader> getFileReader() const;

  /**
   * Set vrPolyData to the given vtkPolyData.
   * @param newPolyData is the new polyData that will be set to vrPolyData.
   */
  void setVRPolyData(vtkSmartPointer<vtkPolyData> newPolyData);

  /**
   * Get vrMapper of the current part.
   * @return Smart pointer to vrMapper of current part.
   */
  vtkSmartPointer<vtkMapper> getVRMapper() const;

  /**
   * Set filter list of the VR rendering chain.
   * @param newFilterList is the new filter list to be used in VR rendering
   * chain.
   */
  void setVRFilterList(const std::vector<Filter::FilterData> &newFilterList);

private:
  QList<ModelPart *> m_childItems; /**< List (array) of child items */
  QList<QVariant> m_itemData;      /**< List (array of column data for item */
  ModelPart *m_parentItem;         /**< Pointer to parent */

  bool isVisible; /**< True/false to indicate if should be visible in model
                     rendering */

  std::string fileName;               /**< File name when loading STL file*/
  vtkSmartPointer<vtkSTLReader> file; /**< Datafile from which part loaded */
  vtkSmartPointer<vtkMapper> mapper;  /**< Mapper for rendering */
  vtkSmartPointer<vtkActor> actor;    /**< Actor for rendering */
  vtkColor3<unsigned char> colour;    /**< User defineable colour */
  float metallic;                     /**< Metallic property of actor */
  float roughness;                    /**< Roughness property of actor */
  float anisotropy;                   /**< Anisotropy property of actor */
  float anisotropyrotation; /**< Anisotropy rotation property of actor */

  std::vector<Filter::FilterData>
      vrFilterList; /**< List of filter that should be applied to this part */

  std::vector<Filter::FilterData>
      filterList; /**< List of filter that should be applied to this part */

  vtkSmartPointer<vtkPolyData> vrPolyData; /**< Poly data for VR renderer */
  vtkSmartPointer<vtkMapper> vrMapper;     /**< Mapper for VR renderer */
  vtkWeakPointer<vtkActor> vrActor;        /**< Actor for VR renderer */
};

#endif
