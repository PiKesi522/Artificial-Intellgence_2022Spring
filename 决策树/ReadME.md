1. **[决策树] 基于信息增益，对下述数据集进行决策树构建，描述过程一个关于配眼镜的一个决策分类所需要的数据，数据集包含4属性：age, astigmatism, tear-prod-rate为输入特征，contact-lenses为决策属性。**

<img src="C:\Users\86008\AppData\Roaming\Typora\typora-user-images\image-20220422091932974.png" alt="image-20220422091932974" style="zoom: 175%;" />

​			决策属性contact-lenses有soft，hard和none三种情况，分别数量为2，3，7。计算总体信息熵：

​				$H(D)=-(\frac{2}{12}log_2\frac{2}{12}+\frac{3}{12}log_2\frac{3}{12}+\frac{7}{12}log_2\frac{7}{12}) = 1.3844$			

​			划分属性包含年龄（Age）, 散光（Astigmatism）, 分泌眼泪的频率（Tear Production Rate）。按照各自	的样本，可以得到以下划分：

​			1.**按年龄划分**：青年（Young），中年（Pre presbyopic）和老年（Presbyopic）

|      Age       | Soft | Hard | None | Count |
| :------------: | :--: | :--: | :--: | :---: |
|     Young      |  1   |  1   |  1   |   3   |
| Pre presbyopic |  1   |  1   |  3   |   5   |
|   Presbyopic   |  0   |  1   |  3   |   4   |

​			可以得到信息熵和信息增益分别为：

​				$H(D^1) = -(\frac{1}{3}log_2\frac{1}{3} + \frac{1}{3}log_2\frac{1}{3} + \frac{1}{3}log_2\frac{1}{3}) = 1.5850$

​				$H(D^2) = -(\frac{1}{5}log_2\frac{1}{5} + \frac{1}{5}log_2\frac{1}{5} + \frac{3}{5}log_2\frac{3}{5}) = 1.4988$

​				$H(D^3) = -(\frac{0}{4}log_2\frac{0}{4} + \frac{1}{4}log_2\frac{1}{4} + \frac{3}{4}log_2\frac{3}{4}) = 0.8113$

​				$G(D,Age) = 1.3844 - (\frac{3}{12} * 1.5850 + \frac{5}{12}*1.4988 + \frac{4}{12} * 0.8113) = 0.0932 $



​			2. **按散光度划分**：有散光（Yes），无散光（No）

| Astigmatism | Soft | Hard | None | Count |
| :---------: | :--: | :--: | :--: | :---: |
|     Yes     |  0   |  3   |  3   |   6   |
|     No      |  2   |  0   |  4   |   6   |

​			可以得到信息熵和信息增益分别为：

​				$H(D^1) = -(\frac{0}{6}log_2\frac{0}{6} + \frac{3}{6}log_2\frac{3}{6} + \frac{3}{6}log_2\frac{3}{6}) = 1$

​				$H(D^2) = -(\frac{2}{6}log_2\frac{2}{6} + \frac{0}{6}log_2\frac{0}{6} + \frac{4}{6}log_2\frac{4}{6}) = 0.9183$

​				$G(D,Astigmatism) = 1.3844 - (\frac{6}{12} * 1 + \frac{6}{12}* 0.9183) = 0.4253 $



​			3. **按分泌的频率划分**：正常频率（Normal），频率较少（Reduced）

| Tear Production Rate | Soft | Hard | None | Count |
| :------------------: | :--: | :--: | :--: | :---: |
|        Normal        |  2   |  3   |  3   |   8   |
|       Reduced        |  0   |  0   |  4   |   4   |

​			可以得到信息熵和信息增益分别为：

​				$H(D^1) = -(\frac{2}{8}log_2\frac{2}{8} + \frac{3}{8}log_2\frac{3}{8} + \frac{3}{8}log_2\frac{3}{8}) = 1.5613$

​				$H(D^2) = -(\frac{0}{4}log_2\frac{0}{4} + \frac{0}{4}log_2\frac{0}{4} + \frac{4}{4}log_2\frac{4}{4}) = 0$

​				$G(D,Tear Production Rate) = 1.3844 - (\frac{8}{12} * 1.5613 + \frac{4}{12}* 0) = 0.3435$

​			

​			三种划分的信息增益分别是：

​				$G(D,Age) = 0.0932$   	 $G(D,Astigmatism)= 0.4253 $      $G(D,Tear) = 0.3435$

​			其中散光这一属性的信息增益最大，所以第一步划分选择散光（Astigmatism）

<img src="C:\Users\86008\AppData\Roaming\Typora\typora-user-images\image-20220422161900826.png" alt="image-20220422161900826" style="zoom:67%;" />



​			得到两个分组，两个分组再次进行计算总体信息熵，分类结果分别是（0，3，3）和（2，0，4）：

​						$H(D_{yes}) = 1$			$H(D_{no}) = 0.9183$

​			先处理$H(D_{yes})$，此时还有两种划分：年龄和眼泪频率

|      Age       | Soft | Hard | None | Count |
| :------------: | :--: | :--: | :--: | :---: |
|     Young      |  0   |  1   |  1   |   2   |
| Pre presbyopic |  1   |  1   |  2   |   3   |
|   Presbyopic   |  0   |  1   |  1   |   2   |

​				$H(D^1) = -(\frac{0}{2}log_2\frac{0}{2} + \frac{1}{2}log_2\frac{1}{2} + \frac{1}{2}log_2\frac{1}{2}) = 1$

​				$H(D^2) = -(\frac{0}{3}log_2\frac{0}{3} + \frac{1}{3}log_2\frac{1}{3} + \frac{2}{3}log_2\frac{2}{3}) = 0.9183$

​				$H(D^3) = -(\frac{0}{2}log_2\frac{0}{2} + \frac{1}{2}log_2\frac{1}{2} + \frac{1}{2}log_2\frac{1}{2}) = 1$

​				$G(D_{yes},Age) = 1 - (\frac{2}{7} * 1 + \frac{3}{7}*0.9183 + \frac{2}{7} * 1) = 0.0350 $



| Tear Production Rate | Soft | Hard | None | Count |
| :------------------: | :--: | :--: | :--: | :---: |
|        Normal        |  0   |  3   |  2   |   5   |
|       Reduced        |  0   |  0   |  2   |   2   |

​			可以得到信息熵和信息增益分别为：

​				$H(D^1) = -(\frac{0}{5}log_2\frac{0}{5} + \frac{3}{5}log_2\frac{3}{5} + \frac{2}{5}log_2\frac{2}{5}) = 0.9710$

​				$H(D^2) = -(\frac{0}{2}log_2\frac{0}{2} + \frac{0}{2}log_2\frac{0}{2} + \frac{2}{2}log_2\frac{2}{2}) = 0$

​				$G(D,Tear Production Rate) = 1 - (\frac{5}{7} * 0.9710 + \frac{2}{7}* 0) = 0.3064$

​		此时下一次划分选择眼泪分泌频率（Tear Production Rate），最后一次划分选择年龄（Age）

<img src="C:\Users\86008\AppData\Roaming\Typora\typora-user-images\image-20220422164451123.png" alt="image-20220422164451123" style="zoom: 67%;" />





​			再处理$H(D_{no})$，此时还有两种划分：年龄和眼泪频率

|      Age       | Soft | Hard | None | Count |
| :------------: | :--: | :--: | :--: | :---: |
|     Young      |  1   |  0   |  0   |   1   |
| Pre presbyopic |  1   |  0   |  1   |   2   |
|   Presbyopic   |  0   |  0   |  2   |   2   |

​				$H(D^1) = -(\frac{1}{1}log_2\frac{1}{1} + \frac{0}{0}log_2\frac{0}{0} + \frac{0}{0}log_2\frac{0}{0}) = 0$

​				$H(D^2) = -(\frac{0}{2}log_2\frac{0}{2} + \frac{1}{2}log_2\frac{1}{2} + \frac{1}{2}log_2\frac{1}{2}) = 1$

​				$H(D^3) = -(\frac{0}{2}log_2\frac{0}{2} + \frac{0}{2}log_2\frac{0}{2} + \frac{2}{2}log_2\frac{2}{2}) = 0$

​				$G(D_{no},Age) = 0.9183 - (\frac{1}{5} * 0 + \frac{2}{5}*1 + \frac{2}{5} * 0) = 0.5183 $



| Tear Production Rate | Soft | Hard | None | Count |
| :------------------: | :--: | :--: | :--: | :---: |
|        Normal        |  2   |  0   |  1   |   3   |
|       Reduced        |  0   |  0   |  2   |   2   |

​			可以得到信息熵和信息增益分别为：

​				$H(D^1) = -(\frac{2}{3}log_2\frac{2}{3} + \frac{0}{3}log_2\frac{0}{3} + \frac{1}{3}log_2\frac{1}{3}) = 0.9183$

​				$H(D^2) = -(\frac{0}{2}log_2\frac{0}{2} + \frac{0}{2}log_2\frac{0}{2} + \frac{2}{2}log_2\frac{2}{2}) = 0$

​				$G(D,Tear Production Rate) = 0.9183 - (\frac{3}{5} * 0.9183 + \frac{2}{5}* 0) = 0.3673$

​			此时下一次划分选择年龄（Age），最后一次划分选择眼泪分泌频率（Tear Production Rate）

<img src="C:\Users\86008\AppData\Roaming\Typora\typora-user-images\image-20220422170227398.png" alt="image-20220422170227398" style="zoom:67%;" />



#### **得到完全分类结果：**

![image-20220422170247451](C:\Users\86008\AppData\Roaming\Typora\typora-user-images\image-20220422170247451.png)


