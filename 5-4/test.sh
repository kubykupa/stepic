#!/bin/sh

echo '6' > ./input.txt
echo '2' >> ./input.txt
echo '1' >> ./input.txt
echo '5' >> ./input.txt
echo '0' >> ./input.txt
echo '7' >> ./input.txt
echo '4' >> ./input.txt
echo '9' >> ./input.txt
echo '3' >> ./input.txt
echo '8' >> ./input.txt
echo 'cat ./input.txt | sort | head -n 1' | ./shell

#cat ./result.out
