# fastcsv2json:  convert csv to json array

Usage: fastcsv2json [OPTION]

Convert csv to json array from STDIN to STDOUT

Options:

-c, --max-columns&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;maximum columns, default 200

-d, --delimiter&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;delimiter as pipe, comma, semicolumn, column or tab

-h, --help&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;give this help

-s, --max-column-size&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;maximum column size, default 500

-v, --version&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;version information, license and copyright

**example:** 

fastcsv2json -d pipe -c 10 -s 20 < myfile.csv > myfile.json

fastjson2csv is written in ISO C90 and will compile easily under any platform.





 
