# TileServer_Download


(Very) Simple tool to download xyz tile servers for local use.


## Build

> make 

## Usage

> ./xyz_tilescrapper --url {tile server url, including x,y,z in brackets} --out {path to out folder} [--zmin {minimum zoom level} --zmax {maximum zoom level} --bounds {comma-separated values 'lonMin,latMin,lonMax,latMax'}]

Example:

> ./xyz_tilescrapper --url http://a.tile.stamen.com/toner/{z}/{x}/{y}.png  --out ./paris --zmin 6 --zmax 16 --bounds 2.224731,48.807090,2.451668,48.910993
