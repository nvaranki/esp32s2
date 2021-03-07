//

#ifndef H_MicroControllerUnit
#define H_MicroControllerUnit

#include "PowerManagementUnit.hpp"

class MicroControllerUnit
{
private:
    /* data */
    PowerManagementUnit* pmu;
public:
    MicroControllerUnit(/* args */);
    ~MicroControllerUnit();
public:
    PowerManagementUnit* getPowerManagementUnit();
};

#endif