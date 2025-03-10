# CST LAB4-1 BBST

张益铭 车13 2021010552

## 实现过程及复杂度分析

我首先在`BST.hpp`中实现了BST虚基类，然后分别在`AVL.hpp`和`Splay.hpp`中继承BST，实现AVL树和Splay树。

在`BST.hpp`中，我实现了Node节点，包含指向父节点 (parent)、左子节点 (lc)、右子节点 (rc) 的指针，以及键值 (key) 和高度 (height)。还提供 updateHeight 方法动态维护节点高度，attachLc 和 attachRc 方法用于连接左右子树。

### AVL

使用平衡因子 BalFac(x) = stature(x->lc) - stature(x->rc) 来衡量左右子树高度差，平衡因子在 [-1, 1] 范围内，树被认为是平衡的。

- 插入操作
  - 插入节点后，自下而上更新祖先节点高度。遇到不平衡节点，调用 rotateAt 进行单旋转或双旋转恢复平衡。
  - 插入分为查找和调整平衡，二者的复杂度均为O(log n)，因此插入操作的时间复杂度为O(log n)。

- 删除操作
  - 删除节点后，类似插入，自下而上更新祖先节点高度并恢复平衡。
  - 删除分为查找、删除节点、调整平衡，查找和调整平衡的复杂度均为O(log n)，删除节点的复杂度为O(1)，因此删除操作的时间复杂度为O(log n)。
- 查找操作
  - 继承BST中的二分查找。
  - 时间复杂度为 O(log n)

### Splay

核心是通过伸展操作，通过一系列的单旋转（zig）或双旋转（zig-zig 和 zig-zag）将目标节点提升为根节点。使用 splay 方法实现伸展，重复对目标节点进行旋转直到成为根节点。

- 插入操作
  - 查找目标键值后，将插入的新节点伸展至根节点。

- 删除操作
  - 删除目标节点后，将其左右子树分离。
  - 通过 splay 将右子树的最小节点伸展至根，并连接原左子树。
- 查找操作
  - 继承BST中的二分查找。
  - 查找完毕后，对找到的节点进行splay操作。

基于势能分析，任何序列的 m 次操作总时间复杂度为 O(m log n)。在均摊意义下，Splay树的插入删除和查找操作复杂度均为O(log n)

## 测例

测例使用`generator.cpp`生成至`results/`目录下，所有测例的总操作数均为1,000,000。

- 1.in
  - 测例1为大规模随机数据测试
  - 首先随机插入3e5个随机数，然后随机进行2e5次删除，再进行1e5次查找
  - 然后插入2e5个随机数，随机进行1e5次删除，再进行1e5次查找
  - 每次查找范围为$[0,8500000]$
- 2.in
  - 测例2为顺序递增插入
  - 首先顺序插入3e5个数，即$0,1,\dots,299999$，然后随机进行2e5次删除，再进行1e5次查找
  - 然后顺序插入2e5个数，即$300000,300001,\dots,499999$，随机进行1e5次删除，再进行1e5次查找
  - 每次查找范围为$[0,8500000]$
- 3.in
  - 测例3为大规模局部查找
  - 首先顺序插入3e5个数，即$0,1,\dots,299999$，然后随机进行1e5次删除，再进行2e5次查找
  - 然后顺序插入2e5个数，即$300000,300001,\dots,499999$，随机进行1e5次删除，再进行1e5次查找
  - 每次查找范围为$[0,10000]$

## 性能分析

|           |       1.in       |            2.in            |           3.in           |
| :-------: | :--------------: | :------------------------: | :----------------------: |
| 测例说明  | 随机插入删除查找 | 插入递增数列，随机删除查找 | 插入递增数列，小批量查找 |
| 总操作数  |     1000000      |          1000000           |         1000000          |
|  插入数   |      500000      |           500000           |          500000          |
|  删除数   |      300000      |           300000           |          200000          |
|  查找数   |      200000      |           200000           |          300000          |
|  ALV耗时  |    377.567ms     |         260.914ms          |        242.043ms         |
| Splay耗时 |    571.428ms     |         267.369ms          |        224.110ms         |

- 测例 1: 随机插入删除查找

  - AVL 树性能优于 Splay 树。

  - AVL 树严格维护平衡，插入和删除的代价较稳定，操作耗时为 O(log n)。Splay 树虽然分摊复杂度为 O(log n)，但单次操作（如伸展）在随机分布的情况下可能退化为 O(n)，导致性能波动。
  - AVL 树通过平衡性保证较低的树高，随机查找性能接近 O(log n)。Splay 树在每次查找后将目标节点提升到根，虽然分摊性能好，但随机访问模式下带来的频繁伸展增加了开销。

- 测例 2: 插入递增数列，随机删除查找

  - 两种数据结构性能接近。

  - AVL 树严格维护平衡，尽管输入是递增序列，树结构保持 O(log n) 的高度。Splay 树在递增插入时形成链表样式，单次插入代价 O(n)，但后续操作通过伸展逐渐优化树形。

  - AVL 树操作平稳，耗时更低。Splay 树在删除操作中会因伸展增加额外开销，但随机访问后的伸展也改善了局部性，整体耗时接近 AVL。

- 测例 3: 插入递增数列，小批量查找

  - Splay 树性能优于 AVL 树。

  - 与测例 2 相同，AVL 树保持平衡，树高为 O(log n)。Splay 树链表式结构在插入阶段表现不佳，但树的局部性在查找阶段得以利用。
  - Splay 树在每次查找后将节点提升为根，优化了后续查找的代价，特别是对重复或接近节点的访问性能优于 AVL。AVL 树查找代价固定，无法充分利用批量查找的局部性优势。
