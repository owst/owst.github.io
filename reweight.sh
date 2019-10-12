#!/usr/bin/env bash

num=10

while IFS= read -r line
do
    filename=${line%%:*}
    current_weight=${line##*: }

    sed -i "s/weight: $current_weight/weight: $num/" "$filename"

    num=$((num + 10))
done < <(grep '^weight:' _oss_contributions/* | sort -k2 -n)
