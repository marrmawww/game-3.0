#pragma once
#include <string>
#include "Base.h"

class ResourceSystem {
public:
    explicit ResourceSystem(Base& base) : base_(base) {}
    std::string getInfo() const { return base_.resourcesInfo(); }
private:
    Base& base_;
};
