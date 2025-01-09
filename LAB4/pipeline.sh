clang++ -std=c++11 generator.cpp -o generator
./generator
clang++ -std=c++11 main.cpp -o main
./main AVL < data/1.in > data/1AVL.ans
./main Splay < data/1.in > data/1Splay.ans
./main AVL < data/2.in > data/2AVL.ans
./main Splay < data/2.in > data/2Splay.ans
./main AVL < data/3.in > data/3AVL.ans
./main Splay < data/3.in > data/3Splay.ans
echo "AVL 1:"
cat data/1AVL.ans | tail -n 1
echo "Splay 1:"
cat data/1Splay.ans | tail -n 1
echo "AVL 2:"
cat data/2AVL.ans | tail -n 1
echo "Splay 2:"
cat data/2Splay.ans | tail -n 1
echo "AVL 3:"
cat data/3AVL.ans | tail -n 1
echo "Splay 3:"
cat data/3Splay.ans | tail -n 1