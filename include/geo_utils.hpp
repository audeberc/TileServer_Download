namespace geo_utils {
     int lon_to_xtile(float lon, int n);
    int lat_to_ytile(float lat, int n);
    void georeference_image(int x, int y, int z, const char* path );

}