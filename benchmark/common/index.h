#pragma once

#include <cstdlib>

class Index {
public:
	virtual void load(const char*) = 0;
	virtual size_t size() const = 0;
};
