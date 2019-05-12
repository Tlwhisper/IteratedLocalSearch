# 更多精彩尽在微信公众号【程序猿声】
![](http://upload-images.jianshu.io/upload_images/10386940-80101f05ccc77525.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 迭代局部搜索（Iterated Local Search, ILS）

## 00 目录
- 局部搜索算法
- 简单局部搜索
- 迭代局部搜索

## 01 局部搜索算法

### 1.1 什么是局部搜索算法？
局部搜索是解决最优化问题的一种启发式算法。因为对于很多复杂的问题，求解最优解的时间可能是极其长的。因此诞生了各种启发式算法来退而求其次寻找次优解，局部搜索就是其中一种。它是一种近似算法（Approximate algorithms）。

局部搜索算法是从爬山法改进而来的。简单来说，局部搜索算法是一种简单的贪心搜索算法，该算法每次从当前解的临近解空间中选择一个最优解作为当前解，直到达到一个局部最优解。局部搜索从一个初始解出发，然后搜索解的邻域，如有更优的解则移动至该解并继续执行搜索，否则返回当前解。

### 1.2 算法思想过程
局部搜索会先从一个初始解开始，通过邻域动作。产生初始解的邻居解，然后根据某种策略选择邻居解。一直重复以上过程，直到达到终止条件。

不同局部搜索算法的区别就在于：邻域动作的定义以及选择邻居解的策略。这也是决定算法好坏的关键之处。

### 1.3 什么又是邻域动作？
其实邻域动作就是一个函数。那么，通过这个函数，对当前的最优解s，产生s对应的邻居解的一个集合。比如：
>对于一个bool型问题，其当前解为：s = 1001，当将邻域动作定义为翻转其中一个bit时，得到的邻居解的集合N(s)={0001,1101,1011,1000}，其中N(s) ∈ S。同理，当将邻域动作定义为互换相邻bit时，得到的邻居解的集合N(s)={0101,1001,1010}.

## 02 简单局部搜索
在开始我们的迭代局部搜索之前，还是先来给大家科普几个简单局部搜索算法。他们也是基于个体的启发式算法（Single solution）。

### 2.1 爬山法（HILL-CLIMBING）
[干货 | 用模拟退火(SA, Simulated Annealing)算法解决旅行商问题](https://mp.weixin.qq.com/s?__biz=MzU0NzgyMjgwNg==&mid=2247484661&amp;idx=1&amp;sn=b96e68355fda7b374291c6ef9a4eb6a9&source=41#wechat_redirect)
### 2.2 模拟退火（SIMULATED ANNEALING）
[干货 | 用模拟退火(SA, Simulated Annealing)算法解决旅行商问题](https://mp.weixin.qq.com/s?__biz=MzU0NzgyMjgwNg==&mid=2247484661&amp;idx=1&amp;sn=b96e68355fda7b374291c6ef9a4eb6a9&source=41#wechat_redirect)
### 2.3 模拟退火（SIMULATED ANNEALING）
[干货|十分钟快速复习禁忌搜索(c++版)](https://mp.weixin.qq.com/s?src=11&timestamp=1522523991&ver=788&signature=tJkG-ToIDfCNeCQ-O*FPGAXOjlLpCpBxYslv0O7CQuBDKmhqiPrr-VzfjWhYpyUBQmbwbriPeHfYVxKX1EK0*MOxDZNHcbNbzVYxh4st5TvP3S6rRwsZnC*fB8nlrVRm&new=1)

[干货 | 十分钟掌握禁忌搜索算法求解带时间窗的车辆路径问题(附C++代码和详细代码注释)](https://mp.weixin.qq.com/s?src=11&timestamp=1522523991&ver=788&signature=tJkG-ToIDfCNeCQ-O*FPGAXOjlLpCpBxYslv0O7CQuBVaLWApOjXmOZFSES23wBIEXh4VWCl4Mr7kuq7FUT8ACijL4pquGa-6uHpdM*HHpReon2hHWiTaMHPJ5fd-5rR&new=1)

## 03 迭代局部搜索（Iterated Local Search, ILS）
### 3.1 介绍
迭代局部搜索属于探索性局部搜索方法（EXPLORATIVE LOCAL SEARCH METHODS）的一种。它在局部搜索得到的局部最优解上，加入了扰动，然后再重新进行局部搜索。

### 3.2 过程描述
注：下文的局部搜索(或者LocalSearch)指定都是简单局部搜索。指上文介绍的三种中的任意一种。

1. 从初始解s中进行局部搜索，找到一个局部最优解s1。
2. 扰动s1，获得新的解s2。
3. 从新解s2中进行局部搜索，再次找到一个局部最优解s3。
4. 基于判断策略，对s3好坏进行判断。选择接受s3作为新解或者回退s2。
5. 直到找到满足条件的最优解，不然跳回第二步。

其图解如下：

![image](http://upload-images.jianshu.io/upload_images/10386940-46c9526b58cdb56c.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

伪代码如下：

![image](http://upload-images.jianshu.io/upload_images/10386940-d96920b368263ab3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

关于其中的接受判断准则，这里采用了模拟退火中的概率函数：

![image](http://upload-images.jianshu.io/upload_images/10386940-a059ecf11eba7faa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 04 代码时间

以下C++代码还是用于求解TSP旅行商问题。至于什么是旅行商问题，读者可以从爬山算法那篇文章了解。

欲获取代码，请关注我们的微信公众号【程序猿声】，在后台回复：**ILS代码**。即可获取。

![微信公众号](http://upload-images.jianshu.io/upload_images/10386940-546ac15b9d7add56.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

