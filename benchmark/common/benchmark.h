#pragma once

#include <vector>

#include "index.h"

class Benchmark {
public:
	virtual void setIndex(const Index *const) = 0;
	virtual void query(const std::vector<uint32_t> &, const size_t = 20) = 0;
};
