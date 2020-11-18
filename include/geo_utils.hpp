namespace geo_utils
{
    int lon_to_xtile(float lon, int n);
    int lat_to_ytile(float lat, int n);
    double tilex_to_long(int x, int z);
    double tiley_to_lat(int x, int z);
     void georeference_image(int x, int y, int z, const char *path);

} // namespace geo_utils