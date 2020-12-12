g++ -std=c++14 core.cc -o core
chmod +x main.py
mkfifo fifo
./main.py <fifo | ./core >fifo
# ./main.py  <fifo | tee >(./core >fifo | tee fifo)
rm -r fifo core __pycache__
