/* AI performance test: Расчет ПТЭ на float32.
 *
 * Author © 2021 Nikolai Varankine
*/

#include <cmath>
#include <stdio.h>
#include "DataBuilderFloat2D.hpp"

DataBuilderFloat2D::DataBuilderFloat2D( long n, int sizeInp, int sizeOut ) :
    n( n ), sizeInp( sizeInp ), sizeOut( sizeOut ),
    inpTest( new float[n*sizeInp] ),
    outTest( new float[n*sizeOut] ),
    matrixTest( new float[n*sizeOut*sizeInp*2] )
{
}

DataBuilderFloat2D::~DataBuilderFloat2D()
{
    delete inpTest;
    delete outTest;
    delete matrixTest;
}

void DataBuilderFloat2D::computeTestInput()
{
    // create test data
    float factor = 1.f / ( n * sizeInp );
    for( int layer = 0; layer < n; layer++ )
        for( int i = 0; i < sizeInp; i++ )
        {
            int pos = layer * sizeInp + i;
            inpTest[pos] = pos * factor; // [layer][i]
        }
    printf( "Computed test input\n" );
}
    
void DataBuilderFloat2D::computeTestMatrix()
{
    //float[][][][] matrixTest = new float[(int)n][sizeOut][sizeInp][2];
    float factor = 1.f / ( n * sizeOut * sizeInp * 2 );
    for( int layer = 0; layer < n; layer++ )
        for( int o = 0; o < sizeOut; o++ )
            for( int i = 0; i < sizeInp; i++ )
                for( int k = 0; k < 2; k++ )
                {
                    int pos = layer * sizeOut * sizeInp * 2
                            + o * sizeInp * 2
                            + i * 2
                            + k;
                    //matrixTest[layer][o][i][k] = pos * factor;
                    matrixTest[pos] = pos * factor;
                }
    printf( "Computed test matrix\n" );
}
    
uint64_t DataBuilderFloat2D::computeTestOutput( SystemTimer* const stm )
{
    double const LN2M = -std::log( 2. ), SIR = 1. / sizeInp;
    uint64_t t0 = stm->getValidValue( TIME_ATTEMPTS );
    for( int layer = 0; layer < n; layer++ )
        for( int o = 0; o < sizeOut; o++ )
        {
            float sum2 = 0.f;
            for( int i = 0; i < sizeInp; i++ )
            {
                int pos = layer * sizeOut * sizeInp * 2
                        + o * sizeInp * 2
                        + i * 2; // 10.3% 13768/5/64/64=0.7 ns
                float v = inpTest[layer * sizeInp + i];
                //v = ( v - matrixTest[layer][o][i][0] ) / matrixTest[layer][o][i][1];
                v = ( v - matrixTest[pos+0] ) / matrixTest[pos+1]; // 70.2% 94044/5/64/64=4.6 ns
                //v = ( v - matrixTest[pos+0] ) * _matrixTest[pos+1]; // 46.6% 59351/5/64/64=2.9 ns
                sum2 += v * v;
            }
            outTest[layer * sizeOut + o] = (float) std::exp( LN2M * ( sum2 * SIR ) ); // 8.2% 11000/5/64=34 ns
        }
    uint64_t t1 = stm->getValidValue( TIME_ATTEMPTS );
    printf( "Computed test output\n" );
    return t1-t0;
}
    
uint64_t DataBuilderFloat2D::fill( SystemTimer* const stm )
{
    computeTestInput();
    computeTestMatrix();
    return computeTestOutput( stm );
}
