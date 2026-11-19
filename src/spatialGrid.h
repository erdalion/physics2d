#ifndef spatialgrid_h
#define spatialgrid_h

#include "vec2.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <iostream>
#include <cmath>
#include <span>

class SpatialGrid
{
public:
    int cols;
    int rows;
    float cellSize;
    std::vector<int> cellCounts;
    std::vector<int> cellParticles;
    int maxPerCell = 16;

    SpatialGrid(float width, float height, float radius)
    {
        cellSize = 2.0f * radius;
        cols = static_cast<int>(std::ceil(width / cellSize));
        rows = static_cast<int>(std::ceil(height / cellSize));
        cellCounts.resize(cols * rows, 0);
        cellParticles.resize(cols * rows * maxPerCell, 0);
    }

    void clear()
    {
        std::ranges::fill(cellCounts, 0);
    }

    void insert(int particleIndex, Vec2 pos)
    {
        int cx = static_cast<int>(pos.x / cellSize);
        int cy = static_cast<int>(pos.y / cellSize);
        cx = std::clamp(cx, 0, cols - 1);
        cy = std::clamp(cy, 0, rows - 1);
        int index = cx + cy * cols;
        int count = cellCounts[index];
        if (count < maxPerCell)
        {
            cellParticles[index * maxPerCell + count] = particleIndex;
            cellCounts[index]++;
        }
    }

    std::span<const int> getCellParticles(int cellIndex) const
    {
        const int count = cellCounts[cellIndex];
        const std::size_t offset =
            static_cast<std::size_t>(cellIndex) * static_cast<std::size_t>(maxPerCell);
        return {&cellParticles[0] + offset, static_cast<std::size_t>(count)};
    }

    std::pair<int, int> getCellCoords(Vec2 pos) const
    {
        int cx = std::clamp(static_cast<int>(pos.x / cellSize), 0, cols - 1);
        int cy = std::clamp(static_cast<int>(pos.y / cellSize), 0, rows - 1);
        return {cx, cy};
    }
};

#endif