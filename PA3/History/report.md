# CST 3-1-1 History

张益铭  车13  2021010552

## 算法思路和实现要点

这道题要求处理提交记录，并实时回答查询结果。每个学生的实际 ID 需要用当前提交记录的 ID 与上一次的答案进行异或操作

可以用一个哈希表记录每个学生 ID 的最后提交时刻。如果学生之前提交过，直接输出其上次提交的时间；如果是该学生的第一次提交，输出当前时刻。最后再更新该学生的最后提交时间为当前时刻

## 渐进时间复杂度的分析，包含过程

数组查找和更新均为O(1)，需处理 n 条提交记录，总体复杂度为 O(n)

## 渐进空间复杂度的分析，包含过程

本题实际上不需要处理数据冲突，只需要开一个足够大的数组，用每个学生的id当作key进行访问即可

空间限制256mb，可以开的int数组大小为256M/4=64*10^6，可设置`MAX_ID = 63000000`


## 遇到的困难和解决方法

无

## 估计完成本题所用时间

1小时

## 关于本题的更多感想

无
