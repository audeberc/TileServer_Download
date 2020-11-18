#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include <cmath>

namespace geo_utils
{
    int lon_to_xtile(float lon, int n)
    {
        return (int)(floor((lon + 180.0) / 360.0 * n));
    }

    int lat_to_ytile(float lat, int n)
    {
        return (int)(floor((1.0 - asinh(tan(lat * M_PI / 180.0)) / M_PI) / 2.0 * n));
    }

    double tilex_to_long(int x, int z)
    {
        return x / (double)(1 << z) * 360.0 - 180;
    }

    double tiley_to_lat(int y, int z)
    {
        double n = M_PI - 2.0 * M_PI * y / (double)(1 << z);
        return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
    }

    void georeference_image(int x, int y, int z, const char *path)
    {
        GDALDataset *poDataset;
        GDALAllRegister();
        poDataset = (GDALDataset *)GDALOpen(path, GA_ReadOnly);

        double lat_1 = tiley_to_lat(y, z);
        double lon_1 = tilex_to_long(x, z);
        double lat_2 = tiley_to_lat(y + 1, z);
        double lon_2 = tilex_to_long(x + 1, z);

        double adfGeoTransform[6] = { lon_1, (lon_2 - lon_1)/255.0, 0, lat_1, 0, (lat_2 - lat_1)/255.0 };
        OGRSpatialReference oSRS;
        char *pszSRS_WKT = NULL;
        
        poDataset->SetGeoTransform( adfGeoTransform );
        //oSRS.SetUTM( 11, TRUE );
        oSRS.SetWellKnownGeogCS( "EPSG:4326" );
        oSRS.exportToWkt( &pszSRS_WKT );
        poDataset->SetProjection( pszSRS_WKT );
        CPLFree( pszSRS_WKT );
        
        GDALClose( (GDALDatasetH) poDataset );
    }

} // namespace geo_utils