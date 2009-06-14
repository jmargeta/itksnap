/*=========================================================================

  Program:   ITK-SNAP
  Module:    $RCSfile: GenericImageData.h,v $
  Language:  C++
  Date:      $Date: 2009/06/14 05:55:42 $
  Version:   $Revision: 1.7 $
  Copyright (c) 2007 Paul A. Yushkevich
  
  This file is part of ITK-SNAP 

  ITK-SNAP is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details. 
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  -----

  Copyright (c) 2003 Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

  -----

  Copyright (c) 2003 Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information. 

=========================================================================*/
#ifndef __GenericImageData_h_
#define __GenericImageData_h_

#include "SNAPCommon.h"
#include "IRISException.h"
#include "LabelImageWrapper.h"
#include "GreyImageWrapper.h"
#include "RGBImageWrapper.h"
#include "GlobalState.h"
#include "ImageCoordinateGeometry.h"

class IRISApplication;

/**
 * \class GenericImageData
 * \brief This class encapsulates the image data used by 
 * the IRIS component of SnAP.  
 *
 * This data consists of a grey image [gi] and a segmentation image [si].
 * The following rules must be satisfied by this class:
 *  + exists(si) ==> exists(gi)
 *  + if exists(si) then size(si) == size(gi)
 */
class GenericImageData 
{
public:
  // Image type definitions
  typedef GreyImageWrapper::ImageType GreyImageType;
  typedef RGBImageWrapper::ImageType RGBImageType;
  typedef LabelImageWrapper::ImageType LabelImageType;
  typedef itk::ImageRegion<3> RegionType;

  typedef std::list<ImageWrapperBase *> WrapperList;
  typedef WrapperList::iterator WrapperIterator;
  typedef WrapperList::const_iterator WrapperConstIterator;

  GenericImageData(IRISApplication *parent);
  virtual ~GenericImageData() {};

  /** 
   * Access the 'main' image, either grey or RGB. The main image is the
   * one that all other images must mimic
   */
  ImageWrapperBase* GetMain()
    {
    assert(m_MainImage->IsInitialized());
    return m_MainImage;
    }

  bool IsMainLoaded()
    {
    return m_MainImage->IsInitialized();
    }

  /**
   * Access the greyscale image (read only access is allowed)
   */
  GreyImageWrapper* GetGrey() {
    assert(m_GreyWrapper.IsInitialized());
    return &m_GreyWrapper;
  }

  /**
   * Access the greyscale overlay image (read only access is allowed)
   */
  WrapperList* GetGreyOverlays() {
    assert(m_GreyOverlayWrappers.size() > 0);
    return &m_GreyOverlayWrappers;
  }

  GreyImageWrapper* GetGreyOverlayLast() {
    assert(m_GreyOverlayWrappers.size() > 0);
    return static_cast<GreyImageWrapper *>(m_GreyOverlayWrappers.back());
  }

  /**
   * Access the RGB image (read only access is allowed)
   */
  RGBImageWrapper* GetRGB() {
    assert(m_RGBWrapper.IsInitialized());
    return &m_RGBWrapper;
  }

  /**
   * Access the segmentation image (read only access allowed 
   * to preserve state)
   */
  LabelImageWrapper* GetSegmentation() {
    assert(m_MainImage->IsInitialized() && m_LabelWrapper.IsInitialized());
    return &m_LabelWrapper;
  }

  /** 
   * Get the extents of the image volume
   */
  Vector3ui GetVolumeExtents() const {
    assert(m_MainImage->IsInitialized());
    assert(m_MainImage->GetSize() == m_Size);
    return m_Size;
  }

  /** 
   * Get the ImageRegion (largest possible region of all the images)
   */
  RegionType GetImageRegion() const;

  /**
   * Get the spacing of the gray scale image (and all the associated images) 
   */
  Vector3d GetImageSpacing();

  /**
   * Get the origin of the gray scale image (and all the associated images) 
   */
  Vector3d GetImageOrigin();

  /**
   * Set the grey image (read important note).
   * 
   * Note: this method replaces the internal pointer to the grey image
   * by the pointer that is passed in.  That means that the caller should relinquish
   * control of this pointer and that the GenericImageData class will dispose of the
   * pointer properly. 
   *
   * The second parameter to this method is the new geometry object, which depends
   * on the size of the grey image and will be updated.
   */
  virtual void SetGreyImage(
    GreyImageType *newGreyImage,
    const ImageCoordinateGeometry &newGeometry,
    const GreyTypeToNativeFunctor &native);

  virtual void SetGreyOverlay(
    GreyImageType *newGreyImage,
    const GreyTypeToNativeFunctor &native);

  virtual void UnloadGreyOverlays();
  virtual void UnloadGreyOverlayLast();

  virtual void SetRGBImage(RGBImageType *newRGBImage,
                    const ImageCoordinateGeometry &newGeometry);
  
  virtual void SetRGBImageAsOverlay(RGBImageType *newRGBImage);
  
  /**
   * This method sets the segmentation image (see note for SetGrey).
   */
  virtual void SetSegmentationImage(LabelImageType *newLabelImage);

  /**
   * Set voxel in segmentation image
   */
  void SetSegmentationVoxel(const Vector3ui &index, LabelType value);

  /**
   * Check validity of greyscale image
   */
  bool IsGreyLoaded();

  /**
   * Check validity of greyscale image
   */
  bool IsGreyOverlayLoaded();

  /**
   * Check validity of RGB image
   */
  bool IsRGBLoaded();

  /**
   * Check validity of segmentation image
   */
  bool IsSegmentationLoaded();

  /**
   * Set the cursor (crosshairs) position, in pixel coordinates
   */
  virtual void SetCrosshairs(const Vector3ui &crosshairs);

  /**
   * Set the image coordinate geometry for this image set.  Propagates
   * the transform to the internal image wrappers
   */
  virtual void SetImageGeometry(const ImageCoordinateGeometry &geometry);

  /** Get the image coordinate geometry */
  irisGetMacro(ImageGeometry,ImageCoordinateGeometry);

protected:
  // Wrapper around the grey-scale image
  GreyImageWrapper m_GreyWrapper;

  // Wrapper around the RGB image
  RGBImageWrapper m_RGBWrapper;

  // Wrapper around the segmentatoin image
  LabelImageWrapper m_LabelWrapper;

  // A list of linked wrappers, whose cursor position and image geometry
  // are updated concurrently
  WrapperList m_LinkedWrappers;
  WrapperList m_GreyOverlayWrappers;

  // A pointer to the 'main' image, i.e., the image that is treated as the 
  // reference for all other images. It is typically the grey image, but 
  // since we now allow for RGB images, it can point to the RGB image too
  ImageWrapperBase *m_MainImage;

  // Dimensions of the images (must match) 
  Vector3ui m_Size;

  // Parent object
  IRISApplication *m_Parent;

  // Image coordinate geometry (it's placed here because the transform depends
  // on image size)
  ImageCoordinateGeometry m_ImageGeometry;

};

#endif
