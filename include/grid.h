#pragma once
#include <array>
#include <bitset>
#include <memory>
#include <list>
#include <vector>

template <typename T, unsigned M, unsigned N = M>
class Grid {
  public:
    struct Cell;
    struct Element
    {
      std::shared_ptr<T> data;
      Cell *ref;
    };

    using index_type = typename std::vector<Element>::size_type;
    struct Cell
    {
      std::list<index_type> elements;
    };


    std::vector<std::shared_ptr<T>> query(int x1, int y1, int x2, int y2) const;
    std::shared_ptr<T> get(index_type) const;
    index_type insert(T t, int x, int y);
    index_type insert(std::shared_ptr<T> t, int x, int y);
    std::shared_ptr<T> remove(index_type id);
    std::shared_ptr<T> move(index_type id, int x, int y);
    void clear();
    void clear(std::vector<std::shared_ptr<T>>& list);

  private:
    std::array<Cell, M*N> cells;
    std::bitset<(M*M-1)*N/(3*M)> mip;
    /// @todo Use a free list structure for basic memory reallocation.
    std::vector<Element> elements;
};

#include <iostream>
#include <cassert>
template <typename T, unsigned M, unsigned N>
typename Grid<T,M,N>::index_type Grid<T,M,N>::insert(T t, int x, int y) { return insert(std::make_shared<T>(t), x, y); }
template <typename T, unsigned M, unsigned N>
typename Grid<T,M,N>::index_type Grid<T,M,N>::insert(std::shared_ptr<T> t, int x, int y) {
  assert(x >= 0 && x < (int)M);
  assert(y >= 0 && y < (int)N);
  // retrieve the cell indexed by coordinates
  auto& cell = cells[x + y * M];
  // create a new Element
  elements.push_back({ t, &cell });
  // add new element's index to cell's list
  cell.elements.push_back(elements.size() - 1);
  // return new element index
  return elements.size() - 1;
}

template <typename T, unsigned M, unsigned N>
std::shared_ptr<T> Grid<T,M,N>::remove(index_type id) {
  assert(id < elements.size() && elements[id].data);
  // retrieve the element indexed by id
  auto& e = elements.at(id);
  // remove this element from its referenced cell
  for (auto it = e.ref->elements.begin(); it != e.ref->elements.end(); /* empty */) {
    if (*it == id) it = e.ref->elements.erase(it);
    else ++it;
  }
  // save the element data for return value
  std::shared_ptr<T> out = e.data;
  // unset element data
  /// @todo: Use a free list structure so this index can be reclaimed.
  e.data = nullptr;
  e.ref = nullptr;
  return out;
}

template <typename T, unsigned M, unsigned N>
std::shared_ptr<T> Grid<T,M,N>::move(index_type id, int x, int y) {
  assert(id < elements.size() && elements[id].data);
  assert(x >= 0 && x < (int)M);
  assert(y >= 0 && y < (int)N);
  // retrieve the element indexed by id
  auto& e = elements.at(id);
  // remove this element from its referenced cell
  /// @todo: Combine this with Grid::remove to reduce duplicated code.
  for (auto it = e.ref->elements.begin(); it != e.ref->elements.end(); /* empty */) {
    if (*it == id)
      it = e.ref->elements.erase(it);
    else ++it;
  }
  // retrive the cell indexed by coordinate
  auto& cell = cells[x + y * M];
  // add element to cell's list
  cell.elements.push_back(id);
  // add cell to element's reference
  e.ref = &cell;
  return e.data;
}

template <typename T, unsigned M, unsigned N>
std::vector<std::shared_ptr<T>> Grid<T,M,N>::query(int x1, int y1, int x2, int y2) const {
  std::vector<std::shared_ptr<T>> data = {};
  // loop over all coordinates in region
  for (int x = x1; x < x2; ++x) {
    for (int y = y1; y < y2; ++y) {
      // clip on grid boundaries
      if (x < 0 || x >= (int)M) continue;
      if (y < 0 || y >= (int)N) continue;
      // add data from all elements in cell to output
      for (auto& e : cells[x + y * M].elements) {
        auto& i = elements[e].data;
        data.push_back(i);
      }
    }
  }
  return data;
}

#if 0
#include <array>
#include <bitset>
#include <cmath>
#include <iterator>
#include <list>
#include <memory>
#include <numeric>
#include <vector>
#include "util.h"
#include <glm/glm.hpp>

template <class T, unsigned M, unsigned N = M>
class Grid {
  static_assert(is_power_of_2(M));
  static_assert(is_power_of_2(N));
  public:
    Grid();
    std::vector<T> query(glm::vec4 rect) const;
    void insert(T&, glm::vec2);
  private:
    struct Cell {
      std::list<T> elements;
    };
    constexpr static unsigned depth = std::log2(std::min(M,N));
    constexpr static auto mip_indices = generate_mip_indices<depth, M, N>();
    constexpr static auto mip_sizes = generate_mip_sizes<depth, M, N>();
    std::bitset<std::accumulate(mip_sizes.begin(), mip_sizes.end(), 0)> mip;
    std::array<Cell, M*N> cells;
};

#include <spdlog/spdlog.h>
#include "random.h"

//#define DEBUG_QUERY
template <class T, unsigned M, unsigned N>
std::vector<T> Grid<T,M,N>::query(glm::vec4 rect) const {
#ifdef DEBUG_QUERY
  spdlog::info("void Grid<{},{},{}>::query(glm::vec4 rect) const", "T", M, N);
  spdlog::info("Searching ({:.1f}, {:.1f}) ({:.1f}, {:.1f})", rect.x, rect.y, rect.z, rect.w);
  unsigned cell_count = 0;
  unsigned mip_count = 0;
#endif

  std::vector<T> elements;

  // clip rect to grid bounds
  /// @todo Grid position and size: need to update clipping
  rect.x = std::max(0.0f, rect.x);
  rect.y = std::max(0.0f, rect.y);
  rect.z = std::min(M-1.0f, rect.z);
  rect.w = std::min(N-1.0f, rect.w);

  // find cells which overlap rect
  // start by traversing the mip from 0 depth
  for (unsigned i = 0; i < depth; ++i) {
    // get the mip index
    auto idx = std::ranges::views::reverse(mip_indices)[i];
    // divide rect by the current depth
    auto r = rect / static_cast<float>(std::pow(2, depth - i));

#ifdef DEBUG_QUERY
    unsigned mip_count_depth = 0;
#endif

    bool found = false;

    // search through cells within r
    for (unsigned x = r.x; x <= r.z; ++x) {
      for (unsigned y = r.y; y <= r.w; ++y) {
        // convert the x,y to an index k
        /// @todo Grid position and size: need to update indexing
        auto k = index(x, y, std::pow(2, i), std::pow(2, i));
        // mark that we have found a cell at this depth
        found |= mip.test(idx + k);

#ifdef DEBUG_QUERY
        mip_count_depth++;
#endif

        if (i == depth - 1) {
          unsigned dM = M/std::pow(2, depth-1);
          unsigned dN = N/std::pow(2, depth-1);
          for (unsigned m = 0; m < dM; ++m) {
            for (unsigned n = 0; n < dN; ++n) {
              auto xx = 2*x+m;
              auto yy = 2*y+n;
              unsigned k;
              try {
                k = index(xx, yy, M, N);
              } catch (...) { spdlog::warn("out of range:"); }
              auto& l = cells[k].elements;
              elements.insert(elements.begin(), l.begin(), l.end());
            }
          }
#ifdef DEBUG_QUERY
          cell_count++;
#endif
        }
      }
    }

#ifdef DEBUG_QUERY
    mip_count += mip_count_depth;
    spdlog::info("Examined {} mips ({},{}) to ({},{}) at depth {} (÷{})",
        mip_count_depth,
        (unsigned)r.x, (unsigned)r.y, (unsigned)r.z, (unsigned)r.w,
        i, std::pow(2, depth-i));
    if (!found) { spdlog::info("Abandoning search at depth {} after examining {} mips", i, mip_count); }
#endif

    // cancel the search if no cells were found at this depth
    if (!found) break;
  }

#ifdef DEBUG_QUERY
  spdlog::info("Examined {} cells and {} mips. Found {} elements", cell_count, mip_count, elements.size());
  spdlog::info("------------");
#endif

  return elements;

#if 0
  unsigned total_count = 0, total_expected = 0;
  for (unsigned i = 0; i < depth; ++i) {
    auto idx = std::ranges::views::reverse(mip_indices)[i];
    glm::vec4 miprect = rect / (float)std::pow(2, depth - i);
    glm::uvec2 top{ miprect.x, miprect.y };
    glm::uvec2 bot{ miprect.z, miprect.w };

    unsigned count = 0;
    bool found = false;
    unsigned expected = (bot.y - top.y + 1) * (bot.x - top.x + 1);
    for (unsigned x = top.x; x <= bot.x; ++x) {
      for (unsigned y = top.y; y <= bot.y; ++y) {
        unsigned k;
        try { k = index(x, y, std::pow(2, i), std::pow(2, i)); }
        catch (...) { continue; }
        if (found) continue;
        found |= mip.test(idx + k);
        count++;
      }
    }
    auto f = found ? "" : "not ";
    total_count += count;
    total_expected += expected;
    float percent = 100.0f * (float)count / (float)expected;
    spdlog::info("{:4}found after {:>7} {:<7} {:6.2f}% tests at depth {}", f, count, expected, percent, i);
    if (!found) break;
  }
  float percent = 100.0f * (float)total_count / (float)total_expected;
  spdlog::info("searched {} of {} mipcells: {:6.2f}%", total_count, total_expected, percent);
#endif
}

static int colors[] = { 31, 33, 35, 32, 34, 36, 31, 33, 35, 32, 34, 36 };
template <class T, unsigned M, unsigned N>
Grid<T,M,N>::Grid() {
#if 0
  auto printBits = [&]() {
    unsigned i = 0;
    unsigned k = 0;
    unsigned count = 0;
    while (k < mip.size()) {
      for (int n = 0; n < 9; ++n) {
        for (int m = 0; m < 16; ++m) {
          if (!(k < mip.size())) break;
          k += 4;
          count++;
          char buf[32];
          snprintf(buf, sizeof(buf), "\E[%dm", colors[i]);
          printf("%s%s\E[0m", buf, "0");
          if (k == mip_indices[i+1]) ++i;
        }
        printf(" ");
      }
      printf("\n");
    }
    printf("printed %d chars (%d)\n", count, k);
  };
  printBits();
#endif
  spdlog::info("dimension: {}x{}: {}", M, N, M*N);
  spdlog::info("depth: {}", depth);
  int w = std::ceil(std::log10(sizeof(*this)));
  spdlog::info("sizeof(mip_indices):  {:{}} ({})", sizeof(mip_indices), w, mip_indices.size());
  spdlog::info("sizeof(mip_sizes):    {:{}} ({})", sizeof(mip_sizes), w, mip_sizes.size());
  spdlog::info("sizeof(*this):        {:{}} ({})", sizeof(*this), w, sizeof(*this) - sizeof(cells) - sizeof(mip));
  spdlog::info("sizeof(cells):        {:{}} ({})", sizeof(cells), w, cells.size());
  spdlog::info("sizeof(mip):          {:{}} ({})", sizeof(mip), w, mip.size());
  static_assert(mip_indices.size() == mip_sizes.size());
  for (unsigned i = 0; i < mip_indices.size(); ++i) {
    char buf[32];
    snprintf(buf, sizeof(buf), "\E[%dm", colors[i]);
    spdlog::info(" {:2}: {:7} {}(+{})\E[0m", i, mip_indices[i], buf, mip_sizes[i]);
  }
}

template <class T, unsigned M, unsigned N>
void Grid<T,M,N>::insert(T& value, glm::vec2 pos) {
  spdlog::info("void Grid<{},{},{}>::insert(T&, glm::vec2))", "T", M, N);

  for (unsigned i = 0; i < depth; ++i) {
    auto idx = mip_indices[depth - i - 1];

    unsigned divisor = std::pow(2, depth - i);

    auto width = M / divisor;
    auto height = N / divisor;
    glm::uvec2 smol_p = pos / (float)divisor;
    auto k = index(smol_p.x, smol_p.y, width, height);
#if 0
    spdlog::info("{}: {}x{}", i, width, height);
    spdlog::info("    ({}, {})", smol_p.x, smol_p.y);
    spdlog::info("    k: {}", k);
    spdlog::info("    divisor: {}", divisor);
    spdlog::info("    idx: {}", idx);
#endif

    try { mip.set(idx + k); }
    catch (std::exception& e) {
      spdlog::error("{}", e.what());
    }
  }

  auto k = index(pos.x, pos.y, M, N);
  cells[k].elements.push_back(value);
}

template <class T>
class Grid64 : public Grid<T,64> {
  private:
    std::bitset<32*32> cells2;
    std::bitset<16*16> cells4;
    std::bitset<8*8> cells8;
    std::bitset<4*4> cells16;
    std::bitset<2*2> cells32;
    std::bitset<1*1> cells64;
};
#endif
