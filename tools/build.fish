# usage
# ./build.fish 
# -o /path/to/output 
# -s /path/to/source
# --open [/path/to/open] # auto open output.tga or arg path
set out (pwd)/build/main
set src (pwd)/main.cpp

# parse -o -s
set -l i 1
set out_set 0
while test $i -le (count $argv)
    if test $argv[$i] = "-o"
        set out_set 1
        if test (echo $argv[(math $i + 1)] | cut -c 1) = "/"
            set out $argv[(math $i + 1)]
        else
            set out (pwd)/$argv[(math $i + 1)]
        end
    else if test $argv[$i] = "-s"
        if test (echo $argv[(math $i + 1)] | cut -c 1) = "/"
            set src $argv[(math $i + 1)]
        else
            set src (pwd)/$argv[(math $i + 1)]
        end
        set exec_basename_without_ext (basename $src | cut -d. -f1)
        if test $out_set = 0
            set out (dirname $out)/$exec_basename_without_ext
        end
    else if test $argv[$i] = "--open"
        set open 1
        if test (echo $argv[(math $i + 1)] | cut -c 1) = "/"
            set open_path $argv[(math $i + 1)]
        else
            set open_path (pwd)/$argv[(math $i + 1)]
        end
    end

    set i (math $i + 1)
end

mkdir -p (dirname $out)

g++ --std=c++11 -g -o $out $src -lm

if test $open = 1
    # run and open
    $out
    if test -z $open_path
        open $out.tga
    else
        open $open_path
    end
end