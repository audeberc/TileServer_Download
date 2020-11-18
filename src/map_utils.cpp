#include <cmath>

namespace map_utils
{

    int lon_to_xtile(float lon, int n)
    {
        return (int)(floor((lon + 180.0) / 360.0 * n));
    }

    int lat_to_ytile(float lat, int n)
    {
        return (int)(floor((1.0 - asinh(tan(lat * M_PI / 180.0)) / M_PI) / 2.0 * n));
    }
    
}