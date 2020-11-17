#include <stdio.h> 
#include <iostream> 
#include <cstring>
#include <cmath>
#include<vector>
#include <stdexcept>

#include <curl/curl.h>
#include <sys/time.h>
#include <unistd.h>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/progress.hpp>


#define MAX_PARALLEL 10 
namespace fs = boost::filesystem;
namespace po = boost::program_options;

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}


void downloadFile(const char* url, const char* fname) {
    CURL *curl;
    FILE *fp;
    CURLcode res;
    curl = curl_easy_init();
    if (curl){
        fp = fopen(fname, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
}



int create_dirs(std::string out_folder, int z, int x){
    fs::path dir0 (out_folder);
    fs::path dir1 (std::to_string(z));
    fs::path dir2 (std::to_string(x));

    if (!fs::exists( dir0 ) ){
        fs::create_directory(dir0);
    }
    if (!fs::exists( dir0/dir1 ) ){
        fs::create_directory(dir0/dir1);
    }
    if (!fs::exists( dir0/dir1/dir2 ) ){
        fs::create_directory(dir0/dir1/dir2);
    }
}

  
int main(int argc, char *argv[]){
    
    int opt;
    std::string opt2;
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help", "produce help message")    
    ("zmin", po::value<int>(&opt)->default_value(0), "Min zoom level (default 0)")
    ("zmax", po::value<int>(&opt)->default_value(18), "Max zoom level (default 18)")
    ("bounds", po::value<std::string>(&opt2)->default_value(""), "Bounding box. comma-separated values 'lonMin,latMin,lonMax,latMax'")
    ("url", po::value<std::string>(), "Tile server URL")
    ("out", po::value<std::string>(), "Out folder path");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);    

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }

    if (vm.count("url") && vm.count("out")) {
        std::string tile_server_url = vm["url"].as<std::string>();
        std::string bounds = vm["bounds"].as<std::string>();
        std::string out = vm["out"].as<std::string>();
        int zmin = (vm["zmin"]).as<int>();
        int zmax = (vm["zmax"]).as<int>();
        std::cout << "Scapping tile server: " 
    << tile_server_url << " " << zmin  << " " << zmax  << " " << bounds  << std::endl;

    
        if (tile_server_url.find("{x}") != std::string::npos && tile_server_url.find("{y}") != std::string::npos && tile_server_url.find("{z}") != std::string::npos) {
            //int number_of_combination =   (pow(2, (zmax - zmin)+1)-zmax-2);
            boost::progress_display progress( zmax - zmin + 1  );
            std::vector<float> bounds_val;
            if (bounds == ""){
                std::cout << "No bounds specified"  << std::endl;
            }
            else {                                    
                  
                  std::stringstream ss(bounds);

                   while( ss.good() )
                    {
                        std::string substr;
                        std::getline( ss, substr, ',' );
                        bounds_val.push_back( std::stof(substr ));
                    }   
                if (bounds_val.size() != 4) {
                    throw std::invalid_argument( "You must provide 4 comma separated bounds" );
                }
            }

            for (int z = zmin; z < zmax + 1; z++){
                int n =  (1 << z); 
                int xtile_min = 0;
                int xtile_max = n;
                int ytile_min = 0;
                int ytile_max = n;
                if (bounds != ""){                    
                    xtile_min = (int)(floor((bounds_val[0] + 180.0) / 360.0 * n)); 
                    xtile_max = (int)(floor((bounds_val[2] + 180.0) / 360.0 * n)) + 1; 
                    
                    int ytile_a =(int)(floor((1.0 - asinh(tan(bounds_val[1]* M_PI/180.0 )) / M_PI) / 2.0 * n));
                    int ytile_b =(int)(floor((1.0 - asinh(tan(bounds_val[3]* M_PI/180.0 )) / M_PI) / 2.0 * n));
                    ytile_min = std::min(ytile_a, ytile_b);
                    ytile_max = std::max(ytile_a, ytile_b)+1;
                }
                 
                for (int x = xtile_min; x < xtile_max; x++){

                    

                    for (int y = ytile_min; y < ytile_max; y++){
                        
                        std::string tile_server_url = vm["url"].as<std::string>();
                        replace(tile_server_url, "{z}", std::to_string(z));
                        replace(tile_server_url, "{x}", std::to_string(x));
                        replace(tile_server_url, "{y}", std::to_string(y));                         
                        create_dirs(out, z, x);
                        fs::path dir0 (out);
                        fs::path dir1 (std::to_string(z));
                        fs::path dir2 (std::to_string(x));
                        fs::path dir3 (std::to_string(y));
                        downloadFile(tile_server_url.c_str(), (dir0/dir1/dir2/dir3).c_str());
                        if (x+y+z % 15 == 0){
                            usleep(500 * 1000);}
                        }

                   
                }
                ++progress ;        
            }

        }

    }
    
    else {
        std::cout << desc << std::endl;
    }

    return 0;

}