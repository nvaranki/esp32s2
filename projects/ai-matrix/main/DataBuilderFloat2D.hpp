/* AI performance test: Расчет ПТЭ на float32.
 *
 * Author © 2021 Nikolai Varankine
*/

#ifndef H_DataBuilderFloat2D
#define H_DataBuilderFloat2D

#include <stdint.h>
#include "SystemTimer.hpp"

class DataBuilderFloat2D final
{
public:
    const long n;
    const int sizeInp, sizeOut;
private:
    float* const inpTest; // [n][sizeInp]
    float* const outTest; // [n][sizeOut]
    float* const matrixTest; // [n][sizeOut][sizeInp]
public:
    DataBuilderFloat2D( long n, int sizeInp, int sizeOut );
    virtual ~DataBuilderFloat2D();
public:
    static constexpr uint32_t TIME_ATTEMPTS = 3;
    uint64_t fill( SystemTimer* const stm );
    uint64_t fill_nf( SystemTimer* const stm );
private:
    void computeTestInput();
    void computeTestMatrix();
    uint64_t computeTestOutput( SystemTimer* const stm );
};

#endif