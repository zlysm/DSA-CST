# CST LAB2 Zuma

## 01

- Runtime Error

- 当在第0位插入并发生消除时，left指针为0，此时执行`play(left - 1)`会造成数组越界

- 构造出初始发生消除的情况

## 02

- Runtime Error

- 当消除后数组为空时进行访问，会发生数组越界

- 构造出消除后数组为空的情况

## 03

- Runtime Error

- 插入的rank大于数组长度时，会发生数组越界

- 构造出非法的插入下标

## 04

- Wrong Answer

- left应为需要删除的部分的起点，while循环多减了一次left，早成多删除一个珠子

- 构造消除发生在非开头的情况

## 05

- Wrong Answer
- 使用cin读入字符串，会导致当初始序列为空时，无法被正常读入
- 构造初始为空的字符串