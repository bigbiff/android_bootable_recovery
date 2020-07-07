#!/bin/bash -x

process_file()
{
    skip_libs=("libcgrouprc.so libcrypto_utils.so libbootloader_message.so libsquashfs_utils.so libfec.so libunwindstack.so libjsoncpp.so")
    dst=$1/$(basename $2)
    src=$2

    [[ -e $src ]] || return 0

    if [ $dst == $src ]; then
      cp -f -p $src $src.tmp
      src=$2.tmp
    fi

    sed "s|/system/bin/linker64\x0|/sbin/linker64\x0\x0\x0\x0\x0\x0\x0|g" $src | sed "s|/system/bin/linker\x0|/sbin/linker\x0\x0\x0\x0\x0\x0\x0|g" | sed "s|/system/bin/sh\x0|/sbin/sh\x0\x0\x0\x0\x0\x0\x0|g" > $dst
    file=`basename $src`
    dir=`dirname $src`
    [[ $src == $2.tmp ]] && rm $src
    # rm $src
    [[ $dir != 'out/target/product/flame/recovery/root/system/lib64' ]] && rm $src
    [[ "${skip_libs[*]}" == *"$file"* ]] && return 0 || rm -f $src
}


dest=$1
shift 1
for ARG in $*
do
    process_file $dest $ARG
done
