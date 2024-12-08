# LAB1 Embrace DSA

张益铭  车13  2021010552

## 2A

n需满足
$$
2\times10^{-8}\times 10n^2 \leq 1
$$
解得$n\leq2236$，即最大应该准备$n=2236$的黑盒数据。

## 2B

n需满足
$$
2\times10^{-8}\times 20n\log_2n \leq 1
$$
解得$n\leq 145746$，即最大应该准备$n= 145746$​的黑盒数据。

## 3A

### 找到的bug

- solution_1
  - 当m或n为2000时数组越界
  - 每次询问后q没有置零

- solution_2
  - 当m或n为2000时数组越界
  - 每次询问后q没有置零
  - 通过打印sum加的数发现，计算sum时rowsum矩阵的y坐标多加了1

### AIGC工具的回答

- solution_1
  - 在你提供的代码中，有几个潜在的问题：
    1. **变量 `sum` 未在每次查询时重置**：`sum` 是累加子矩阵元素的变量，但是在每次处理新的查询时，`sum` 没有被清零，这会导致结果错误。你需要在每次查询开始前将 `sum` 置为 0。
    2. **数组下标越界**：数组 `matrix` 在代码中从 `1` 开始填充，但是通常 C++ 的数组是 0 基的。如果输入的数组下标 `x` 和 `y` 是 0 基的，需要在访问时调整。如果输入的下标是从 `1` 开始的，还需要确保 `matrix` 数组的定义和填充方式与输入数据的下标匹配。
    3. **初始化问题**：在声明 `int matrix[2000][2000]` 时，`matrix` 的值未初始化。如果数组中有未填充的值，可能会导致意外行为。
- solution_2
  - 在这段代码中，主要的问题有：
    1. **`sum` 未在每次查询时重置**：与之前的问题类似，`sum` 在每次查询之后没有重置为 0，导致累积了前面查询的结果。
    2. **计算子矩阵的边界错误**：在 `rowsum[x + j][y + b] - rowsum[x + j][y]` 中，当查询范围包括第 `y` 列时，`rowsum[x + j][y]` 应该是包含 `y` 列之前的所有元素的和（不包含 `y`），因此应该改为 `rowsum[x + j][y - 1]`。
    3. **数组下标的处理**：矩阵和 `rowsum` 使用的是从 `1` 开始的下标，而输入的 `x`、`y` 可能是从 1 开始的，需要确保下标访问时的一致性。这里需要小心处理边界的访问，特别是 `y` 为 1 的情况，避免下标越界。

## 3B

使用gdb调试时，编译应加上-g选项，然后使用`g++ -g main.cpp -o main`进行编译，然后使用`gdb ./main`进入调试。之后可用break，print，next等命令调试

## 3C

`srand(time(0))` 的作用是以当前时间作为随机数种子，目的是避免每次生成相同的随机数据

## 3D

system命令相当于在命令行中执行括号内的指令，`battle.cpp`的目的是比较两个不同程序在相同输入下的结果是否一致

编译随机输入生成器、输入验证程序和两个程序；然后进入循环，每次生成一组随机输入，检查输入是否有效，如果无效，停止运行；使用两个不同的程序分别计算输出，再比较两个解决方案的输出，如果不同，则报告并停止。

## 3E

代码如下：

```c++
#include <iostream>

void foo() {
    std::cout << "recursive call" << std::endl;
    foo();
}

int main() {
    foo();
    return 0;
}
```

使用gdb进行调试，输入run后，会一直打印recursive call，直到栈溢出，此时会输出

```
Program received signal SIGSEGV, Segmentation fault.
0x00007ffff788b660 in _IO_new_file_xsputn (n=14, data=0x555555556004, f=<optimized out>) at ./libio/fileops.c:1235
1235	./libio/fileops.c: No such file or directory.
```

## 4A

可以先计算每行元素的累加和，再按列进行累加，这样在(x,y)和(a,b)之间的元素和就可以在O(1)时间内得出，计算公式如下

```c++
int sum = rowsum[x + a - 1][y + b - 1] - rowsum[x + a - 1][y - 1] - rowsum[x - 1][y + b - 1] + rowsum[x - 1][y - 1];
```

## 4B

当m=n=200，q=100时，solution_3没有明显优势，具体耗时如下

```
./solution_1 < rand.in > 1.out  0.00s user 0.00s system 90% cpu 0.004 total
./solution_2 < rand.in > 2.out  0.00s user 0.00s system 93% cpu 0.005 total
./solution_3 < rand.in > 3.out  0.00s user 0.00s system 93% cpu 0.005 total
```

当m=n=2000，q=1000时，solution_3运行最快，具体耗时如下

```
./solution_1 < rand.in > 1.out  3.56s user 0.02s system 99% cpu 3.577 total
./solution_2 < rand.in > 2.out  0.21s user 0.02s system 99% cpu 0.231 total
./solution_3 < rand.in > 3.out  0.19s user 0.02s system 99% cpu 0.217 total
```

## 4C

当二维数组的大小足够大，且查询次数足够多时，`solution_3.cpp` 相比另外两个程序会展现出明显优势。