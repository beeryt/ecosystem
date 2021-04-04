#pragma once

constexpr unsigned index(unsigned x, unsigned y, unsigned width, unsigned height) {
  if (x >= width) throw std::runtime_error("hi");
  if (y >= height) throw std::runtime_error("hi");
  return x + y * width;
}

template <unsigned W, unsigned H = W>
constexpr unsigned index(unsigned x, unsigned y) { return index(x, y, W, H); }

template <unsigned W>
constexpr void coord(unsigned idx, unsigned& x, unsigned& y)
{
  x = idx / W;
  y = idx % W;
}

constexpr bool is_power_of_2(int v) { return v && ((v & (v - 1)) == 0); }

template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& v) {
  os << "[";
  for (unsigned i = 0; i < v.size(); ++i) {
    if (i != 0) os << " ";
    os << v[i];
    if (i < v.size() - 1) os << ",";
  }
  return os << "]";
}

template <unsigned M, unsigned N>
static constexpr unsigned inline f(const unsigned k) {
  unsigned m = log2(M);
  unsigned n = log2(N);
  return std::pow(2, m + n - 2*k - 2);
}

template <unsigned M, unsigned N>
static constexpr unsigned inline g(const unsigned k) {
  return 4*f<M,N>(k) * (std::pow(4, k) - 1) / 3;
}

template <unsigned M, unsigned N>
static constexpr unsigned inline h() {
  unsigned m = log2(M);
  unsigned n = log2(N);
  return std::pow(2, m-n) * (std::pow(4, m) - 1) / 3;
}

template <unsigned depth, unsigned M, unsigned N>
constexpr auto generate_mip() {
  std::array<unsigned, depth> sizes = {};
  std::array<unsigned, depth> indices = {};
  for (unsigned i = 0; i < depth; ++i) {
    sizes[i] = f<M,N>(i);
    indices[i] = g<M,N>(i);
  }
  return std::tie(indices, sizes);
}

template <unsigned depth, unsigned M, unsigned N>
constexpr auto generate_mip_sizes() {
  std::array<unsigned, depth> sizes = {};
  for (unsigned i = 0; i < depth; ++i) {
    sizes[i] = f<M,N>(i);
  }
  return sizes;
}

template <unsigned depth, unsigned M, unsigned N>
constexpr auto generate_mip_indices()
{
  std::array<unsigned, depth> indices = {};
  for (unsigned i = 0; i < depth; ++i) {
    indices[i] = g<M,N>(i);
  }
  return indices;
}
