#include "Grid.h"

#include <fstream>
#include <stdexcept>
#include <cmath>

Grid::Grid(const std::string& fileName)
{
    std::string path = "../sudoku_grids/" + fileName;
    std::ifstream file(path);

    if (!file.is_open())
        throw std::runtime_error("Failed to open file: " + fileName);

    // načtení velikosti sudoku
    file >> m_size;

    if (m_size == 0)
        throw std::runtime_error("Invalid sudoku size.");

    // např.:
    // 9  -> 3
    // 16 -> 4
    m_blockSize = static_cast<size_t>(std::sqrt(m_size));

    if (m_blockSize * m_blockSize != m_size)
        throw std::runtime_error("Sudoku size must have integer square root.");

    // alokace polí
    m_grid.resize(m_size, std::vector<int>(m_size, 0));
    m_fixed.resize(m_size, std::vector<bool>(m_size, false));

    // načtení gridu
    for (size_t row = 0; row < m_size; ++row)
    {
        for (size_t col = 0; col < m_size; ++col)
        {
            if (!(file >> m_grid[row][col]))
                throw std::runtime_error("Invalid sudoku format.");

            // nenulové hodnoty jsou fixní
            if (m_grid[row][col] != 0)
                m_fixed[row][col] = true;
        }
    }
}

bool Grid::isFixed(const Coord& coord) const
{
    return m_fixed.at(coord.row).at(coord.col);
}

void Grid::setNumber(const Coord& coord, int num)
{
    m_grid.at(coord.row).at(coord.col) = num;
}

int Grid::getNumber(const Coord& coord) const
{
    return m_grid.at(coord.row).at(coord.col);
}

size_t Grid::getSize() const
{
    return  m_size;
}

size_t Grid::getBlockSize() const
{
    return m_blockSize;
}