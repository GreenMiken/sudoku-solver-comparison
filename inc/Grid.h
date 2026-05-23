#pragma once
#include <array>
#include <string>
#include <cstddef>
#include <vector>

struct Coord
{
    Coord(int x, int y) : row(x), col(y) {}
    int row;
    int col;
};

class Grid
{
public:
    Grid(const std::string& fileName);
    size_t getSize() const;
    size_t getBlockSize() const;
    bool isFixed(const Coord& coord) const;
    void setNumber(const Coord& coord, int num);
    int getNumber(const Coord& coord) const;

private:
    size_t m_size;
    size_t m_blockSize;

    std::vector<std::vector<int>> m_grid;
    std::vector<std::vector<bool>> m_fixed;
};
