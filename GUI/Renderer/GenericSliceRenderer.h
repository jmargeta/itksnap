/*=========================================================================

  Program:   ITK-SNAP
  Module:    $RCSfile: Filename.cxx,v $
  Language:  C++
  Date:      $Date: 2010/10/18 11:25:44 $
  Version:   $Revision: 1.12 $
  Copyright (c) 2011 Paul A. Yushkevich

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

=========================================================================*/

#ifndef GENERICSLICERENDERER_H
#define GENERICSLICERENDERER_H

#include <GenericSliceModel.h>
#include <ImageWrapper.h>
#include <OpenGLSliceTexture.h>
#include <SNAPOpenGL.h>
#include <list>

class GenericSliceRenderer;

class SliceRendererDelegate
{
public:
  virtual ~SliceRendererDelegate() {}
  virtual void paintGL(GenericSliceRenderer *parent) = 0;
};

class GenericSliceRenderer : public AbstractModel
{
public:

  irisITKObjectMacro(GenericSliceRenderer, AbstractModel)

  FIRES(ModelUpdateEvent)

  void SetModel(GenericSliceModel *model);

  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();

  irisGetMacro(Model, GenericSliceModel *)
  irisIsMacro(ThumbnailDrawing)

  typedef std::list<SliceRendererDelegate *> RendererDelegateList;

  // Get a reference to the list of overlays stored in here
  const RendererDelegateList &GetOverlays() const
    { return m_Overlays; }

  RendererDelegateList &GetOverlays()
    { return m_Overlays; }

  // A callback for when the model is reinitialized
  // void OnModelReinitialize();

protected:

  GenericSliceRenderer();
  virtual ~GenericSliceRenderer() {}

  void OnUpdate();

  void DrawMainTexture();
  void DrawSegmentationTexture();
  void DrawOverlayTexture();
  void DrawThumbnail();
  void DrawOverlays();

  GenericSliceModel *m_Model;

  // Whether rendering to thumbnail or not
  bool m_ThumbnailDrawing;

  // A dynamic association between various image layers and texture objects
  typedef OpenGLSliceTexture<ImageWrapperBase::DisplayPixelType> Texture;
  typedef std::map<ImageWrapperBase *, Texture *> TextureMap;
  TextureMap m_Texture;

  // A list of overlays that the user can configure
  RendererDelegateList m_Overlays;

  // Internal method used by UpdateTextureMap()
  void AssociateTexture(ImageWrapperBase *iw, TextureMap &src, TextureMap &trg);

  // Update the texture map to mirror the current images in the model
  void UpdateTextureMap();

};

#endif // GENERICSLICERENDERER_H