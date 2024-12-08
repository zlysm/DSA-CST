# CST LAB3 HashFun

张益铭 车13 2021010552

## 哈希策略的实现

记字符串的长度为$L$，$S_i$表示字符串$S$的第$i$个字符，$N$为哈希表的大小

1. “坏”的哈希函数
   $$
   Hash(S)=\left(\sum_{i=0}^{L-1}S_i\right)\bmod N
   $$

2. “好”的哈希函数
   $$
   Hash(S)=\left(\sum_{i=0}^{L-1}p^{L-i-1}\times S_i\right)\bmod N
   $$
   其中$p=31$

3. 双向平方试探策略

   ```c++
   int double_square::operator()(hash_entry* Table, int table_size, int last_choice) {
       if (!inited) {
           begin = last_choice;
           inited = true;
       }
       int factor = (int)ceil((double)i / 2);
       int next_choice = (begin + direction * factor * factor) % table_size;
       direction = -direction;
       ++i;
       return next_choice < 0 ? next_choice + table_size : next_choice;
   }
   ```

   第一次试探时，会记录初始值为last_choice，然后交替进行正负两个方向的平方探测

4. 公共溢出区策略

   ```c++
   int overflow_area::operator()(hash_entry* Table, int table_size, int last_choice){
       return int(0.8 * table_size) + overflow_count++;
   }
   ```

   选择试探策略为公共溢出区时，会将哈希表的前80%当作正常的哈希表，后20%当作公共溢出区，并通过`overflow_count`来维护冲突插入的位置

## 测试

使用`grade.py`进行测试。该文件通过调用`generator.cpp`生成数据，并使用python的dict生成标准答案，最终与程序运行结果进行比对。可以在`generate_input()`函数中自定义插入、查询的个数

在`generator.cpp`中，为了避免生成的随机样本过于分散，导致插入和查询的样本重叠太小，我设置每次从`poj.txt`的前 $\min(2*\max(\text{insert},\text{query}), N)$ 个样本采样

- test1.in: insert_num=100, query_num=100

  第一个测试点为小批量测试，插入数据的value各不相同

- test2.in: insert_num=50000, query_num=50000

  中等程度的数据量，部分entry的value值存在相同的情况

- test3.in: insert_num=200000, query_num=300000

  使用大批量随机数据进行测试，且包含大量value为0的entry，提高了value相同但是key不同的概率，造成更多冲突

测试结果如下，单位为秒：

test1.in:

|                | 线性试探 | 双向平方试探 | 公共溢出区 |
| :------------: | :------: | :----------: | :--------: |
| “坏”的哈希函数 |  0.179   |    0.014     |   0.012    |
| “好”的哈希函数 |  0.012   |    0.011     |   0.010    |

test2.in:

|                | 线性试探 | 双向平方试探 | 公共溢出区 |
| :------------: | :------: | :----------: | :--------: |
| “坏”的哈希函数 |  12.770  |    0.167     |   7.759    |
| “好”的哈希函数 |  0.029   |    0.028     |   0.080    |

test3.in:

|                | 线性试探 | 双向平方试探 | 公共溢出区 |
| :------------: | :------: | :----------: | :--------: |
| “坏”的哈希函数 |   >30    |    1.793     |    >30     |
| “好”的哈希函数 |  0.110   |    0.109     |    >30     |

## 分析结果

1. 好的哈希函数的性能大幅领先坏的哈希函数。好的哈希函数能将key均匀分布在哈希表中，减少冲突，提高查找和插入效率；而坏的哈希函数分布不均，导致大量冲突，降低性能
2. 线性试探冲突时容易形成聚集，增加后续冲突概率；双向平方试探跳跃更随机，冲突更少，性能更优
3. 封闭散列占优势。开放散列适合低冲突率场景，插入和删除操作更灵活；封闭散列更适合高负载因子场景，节省内存
4. 可能导致哈希函数分布不均，增加冲突率，降低哈希表性能，尤其在字符分布偏斜的数据集中表现明显，影响查找和插入效率
5. 可以通过监控负载因子，当超出上限时扩展哈希表容量并重新哈希现有元素；当低于下限时缩小容量，同样重新哈希
