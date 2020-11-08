# fastcsv2json:  convert csv to json array

Usage: fastcsv2json [OPTION]

Convert csv to json array from STDIN to STDOUT

Options:

-c, --max-columns       maximum columns, default 200

-d, --delimiter         delimiter as pipe, comma, semicolumn, column or tab

-h, --help              give this help

-s, --max-column-size   maximum column size, default 500

-v, --version           version information, license and copyright

**example:** 

fastcsv2json -d pipe -c 10 -s 20 < myfile.csv > myfile.json

fastjson2csv is written in ISO C90 and will compile easily under any platform.





 
