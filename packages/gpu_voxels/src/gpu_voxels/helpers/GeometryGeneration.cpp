// this is for emacs file handling -*- mode: c++; indent-tabs-mode: nil -*-

// -- BEGIN LICENSE BLOCK ----------------------------------------------
// This file is part of the GPU Voxels Software Library.
//
// This program is free software licensed under the CDDL
// (COMMON DEVELOPMENT AND DISTRIBUTION LICENSE Version 1.0).
// You can find a copy of this license in LICENSE.txt in the top
// directory of the source code.
//
// © Copyright 2014 FZI Forschungszentrum Informatik, Karlsruhe, Germany
//
// -- END LICENSE BLOCK ------------------------------------------------

//----------------------------------------------------------------------
/*!\file
 *
 * \author  Andreas Hermann
 * \date    2016-01-09
 *
 */
//----------------------------------------------------------------------

#include <gpu_voxels/helpers/GeometryGeneration.h>

namespace gpu_voxels
{
namespace geometry_generation
{


void createOrientedBoxEdges(const OrientedBoxParams& params, float spacing, PointCloud &ret)
{
    std::vector<Vector3f> cloud;

    for(float x_dim = -params.dim.x; x_dim <= params.dim.x; x_dim += spacing)
    {
      cloud.push_back(Vector3f(x_dim,  params.dim.y,  params.dim.z));
      cloud.push_back(Vector3f(x_dim,  params.dim.y, -params.dim.z));
      cloud.push_back(Vector3f(x_dim, -params.dim.y,  params.dim.z));
      cloud.push_back(Vector3f(x_dim, -params.dim.y, -params.dim.z));
    }
    for(float y_dim =-params.dim.y; y_dim <= params.dim.y; y_dim += spacing)
    {
      cloud.push_back(Vector3f( params.dim.x, y_dim,  params.dim.z));
      cloud.push_back(Vector3f( params.dim.x, y_dim, -params.dim.z));
      cloud.push_back(Vector3f(-params.dim.x, y_dim,  params.dim.z));
      cloud.push_back(Vector3f(-params.dim.x, y_dim, -params.dim.z));
    }
    for(float z_dim = -params.dim.z; z_dim <= params.dim.z; z_dim += spacing)
    {
      cloud.push_back(Vector3f( params.dim.x,  params.dim.y, z_dim));
      cloud.push_back(Vector3f( params.dim.x, -params.dim.y, z_dim));
      cloud.push_back(Vector3f(-params.dim.x,  params.dim.y, z_dim));
      cloud.push_back(Vector3f(-params.dim.x, -params.dim.y, z_dim));
    }

    ret.update(cloud);

    Matrix4f transformation = Matrix4f::createFromRotationAndTranslation(Matrix3f::createFromRPY(params.rot), params.center);
    ret.transformSelf(&transformation);

    return;
}

void createOrientedBox(const OrientedBoxParams& params, float spacing, PointCloud &ret)
{
  std::vector<Vector3f> cloud;

  for(float x_dim = -params.dim.x; x_dim <= params.dim.x; x_dim += spacing)
  {
    for(float y_dim = -params.dim.y; y_dim <= params.dim.y; y_dim += spacing)
    {
      for(float z_dim = -params.dim.z; z_dim <= params.dim.z; z_dim += spacing)
      {
        Vector3f point(x_dim, y_dim, z_dim);
        cloud.push_back(point);
      }
    }
  }

  ret.update(cloud);
  Matrix4f transformation = Matrix4f::createFromRotationAndTranslation(Matrix3f::createFromRPY(params.rot), params.center);
  ret.transformSelf(&transformation);

  return;
}


std::vector<Vector3f> createBoxOfPoints(Vector3f min, Vector3f max, float delta)
{
  std::vector<Vector3f> box_cloud;
  for(float x_dim = min.x; x_dim <= max.x; x_dim += delta)
  {
    for(float y_dim = min.y; y_dim <= max.y; y_dim += delta)
    {
      for(float z_dim = min.z; z_dim <= max.z; z_dim += delta)
      {
        Vector3f point(x_dim, y_dim, z_dim);
        box_cloud.push_back(point);
      }
    }
  }
  return box_cloud;
}


void createEquidistantPointsInBox(const size_t max_nr_points,
                                  const Vector3ui max_coords,
                                  const float side_length,
                                  std::vector<Vector3f> &points)
{
  uint32_t num_points = 0;
  float x, y, z;
  for (uint32_t i = 0; i < (max_coords.x - 1) / 2; i++)
  {
    for (uint32_t j = 0; j < (max_coords.y - 1) / 2; j++)
    {
      for (uint32_t k = 0; k < (max_coords.z - 1) / 2; k++)
      {
        if (num_points >= max_nr_points)
        {
          goto OUT_OF_LOOP;
        }
        x = i * 2 * side_length + side_length / 2.0;
        y = j * 2 * side_length + side_length / 2.0;
        z = k * 2 * side_length + side_length / 2.0;
        points.push_back(Vector3f(x, y, z));
        num_points++;
      }
    }
  }
  OUT_OF_LOOP:
  return;
}


void createNonOverlapping3dCheckerboard(const size_t max_nr_points,
                                        const Vector3ui max_coords,
                                        const float side_length,
                                        std::vector<Vector3f> &black_points,
                                        std::vector<Vector3f> &white_points)
{
  uint32_t num_points = 0;
  float x, y, z;
  for (uint32_t i = 0; i < (max_coords.x - 1) / 2; i++)
  {
    for (uint32_t j = 0; j < (max_coords.y - 1) / 2; j++)
    {
      for (uint32_t k = 0; k < (max_coords.z - 1) / 2; k++)
      {
        if (num_points >= max_nr_points)
        {
          goto OUT_OF_LOOP;
        }
        x = i * 2 * side_length + side_length / 2.0;
        y = j * 2 * side_length + side_length / 2.0;
        z = k * 2 * side_length + side_length / 2.0;
        black_points.push_back(Vector3f(x, y, z));
        x = (i * 2 + 1) * side_length + side_length / 2.0;
        y = (j * 2 + 1) * side_length + side_length / 2.0;
        z = (k * 2 + 1) * side_length + side_length / 2.0;
        white_points.push_back(Vector3f(x, y, z));
        num_points++;
      }
    }
  }
  OUT_OF_LOOP:
  return;
}


} // END OF NS gpu_voxels
} // END OF NS geometry_generation
