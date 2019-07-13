这个工程主要用于imu数据标定，标定参数分别为：陀螺仪的测量噪声和bias噪声，加速度计的测量噪声和bias噪声
工程参考代码分别来自于与另外两位大佬的代码：参考链接
(1)https://github.com/rpng/kalibr_allan
(2)https://github.com/HeYijia/

参考(1)的代码编译和运行需要在linux下面安装matlab，考虑到本人linux下空间不够，且据说linux下安装matlab很复杂，所以我对里面源码进行了部分修改。
修改的内容主要是/bagconvert/src/main.cpp和/bagconvert/CMakeLists.txt两个文件
main.cpp文件原始的功能是将.bag文件转换为.mat文件，先将其修改为将.bag文件转换为.txt文件。
CMakeLists.txt 主要是屏蔽了和matlab相关的依赖

工程运行步骤：
打开终端分别执行以下命令
1、roscd
2、cd ../src
3、git clone 
4、cd ..
5、catkin_make
6、rosrun vio_data_simulation vio_data_simulation_node
7、rosrun bagconvert bagconvert /home/xiongwy/catkin_ws/src/kalibr_allan/data/imu.bag imu 

将/home/xiongwy/catkin_ws/src/kalibr_allan/data 和/kalibr_allan/matalab 两个文件夹复制到Windows盘符下面
切换道Windows系统下面，打开matlab，先后执行以下两个脚本
SCRIPT_allan_matparallel.m (该脚本执行完成需要花费一定时间，计算产生结果并保存)
SCRIPT_process_results.m (绘制allan曲线图)




