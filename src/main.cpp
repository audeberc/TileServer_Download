#include <stdio.h>
#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <stdexcept>
#include <sys/time.h>
#include <unistd.h>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/progress.hpp>
#include "string_utils.hpp"
#include "map_utils.hpp"
#include "download_utils.hpp"

namespace fs = boost::filesystem;
namespace po = boost::program_options;


int main(int argc, char *argv[])
{
    int opt{0};
    std::string opt2{""};
    po::options_description desc("Allowed options");
    desc.add_options()("help", "produce help message")("zmin", po::value<int>(&opt)->default_value(0), "Min zoom level (default 0)")("zmax", po::value<int>(&opt)->default_value(18), "Max zoom level (default 18)")("bounds", po::value<std::string>(&opt2)->default_value(""), "Bounding box. comma-separated values 'lonMin,latMin,lonMax,latMax'")("url", po::value<std::string>(), "Tile server URL")("out", po::value<std::string>(), "Out folder path");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 1;
    }

    if (vm.count("url") && vm.count("out"))
    {
        std::string tile_server_url = vm["url"].as<std::string>();
        std::string bounds = vm["bounds"].as<std::string>();
        std::string out = vm["out"].as<std::string>();
        int zmin = (vm["zmin"]).as<int>();
        int zmax = (vm["zmax"]).as<int>();
        std::cout << "Scapping tile server: "
                  << tile_server_url << " " << zmin << " " << zmax << " " << bounds << std::endl;

        if (tile_server_url.find("{x}") != std::string::npos && tile_server_url.find("{y}") != std::string::npos && tile_server_url.find("{z}") != std::string::npos)
        {
            boost::progress_display progress(zmax - zmin + 1);
            std::vector<float> bounds_val;
            if (bounds == "")
            {
                std::cout << "No bounds specified" << std::endl;
            }
            else
            {
                string_utils::parse_bounds(bounds, bounds_val);
            }

            for (int z = zmin; z < zmax + 1; z++)
            {
                int n = (1 << z);
                int xtile_min = 0;
                int xtile_max = n + 1;
                int ytile_min = 0;
                int ytile_max = n + 1;
                if (bounds_val.size() == 4)
                {
                    xtile_min = map_utils::lon_to_xtile(bounds_val[0], n);
                    xtile_max = map_utils::lon_to_xtile(bounds_val[2], n) + 1;
                    int ytile_a = map_utils::lat_to_ytile(bounds_val[1], n); 
                    int ytile_b = map_utils::lat_to_ytile(bounds_val[3], n);
                    ytile_min = std::min(ytile_a, ytile_b);
                    ytile_max = std::max(ytile_a, ytile_b) + 1;
                }

                for (int x = xtile_min; x < xtile_max; x++)
                {

                    for (int y = ytile_min; y < ytile_max; y++)
                    {

                        std::string tile_server_url = vm["url"].as<std::string>();
                        string_utils::replace(tile_server_url, "{z}", std::to_string(z));
                        string_utils::replace(tile_server_url, "{x}", std::to_string(x));
                        string_utils::replace(tile_server_url, "{y}", std::to_string(y));
                        download_utils::create_dirs(out, z, x);
                        fs::path dir0(out);
                        fs::path dir1(std::to_string(z));
                        fs::path dir2(std::to_string(x));
                        fs::path dir3(std::to_string(y));
                        download_utils::downloadFile(tile_server_url.c_str(), (dir0 / dir1 / dir2 / dir3).c_str());
                        if (x + y + z % 15 == 0)
                        {
                            usleep(500 * 1000);
                        }
                    }
                }
                ++progress;
            }
        }
    }

    else
    {
        std::cout << desc << std::endl;
    }

    return 0;
}