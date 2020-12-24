“构建”就是指创建事务中动手的那部分。

软件开发中的活动：

1.定义问题（problem definition）

2.需求分析（requirements developerment）

3.规划构建（construction planning）

4.软件架构（software architecture）

5.详细设计(detailed design)

6.编码与调试(coding and debugging)

7.单元测试(unit testing)

8.集成测试(intergration testing)

9.集成(intergration)

10.系统测试(system tesing)

11保障维护(correctting maintenance)

构建活动中的具体任务（task）

1.验证有关的基础工作已经完成，因此构建活动可以顺利地进行下去。

2.确定如何测试所有代码。

3.设计和编写类（class）和子程序(routine)。

4.创建并命名变量（variable）和具名常量(named constant)。

5.选择控制结构（control structure）,组织语句块。

6.对你的代码进行单元测试和集成测试，并排除其中的错误。

7.评审开发团队其他成员的底层设计和代码，并让他们评审你的工作。

8.润饰点，仔细进行代码的格式化和注释。

9.将单独开发的多个软件组件集成一体。

10.调整代码（tuning code）,让它更快、更省资源。


### Autotools

Autotools的重要之处在于它们不是一个通用的构建系统 - 它们实现了GNU编码标准，没有其他任何东西。 如果你想制作一个遵循所有GNU标准的软件包，那么Autotools就是一个很好的工具。 如果你不这样做，那么你应该使用Scons或CMake。

### CMake
CMake，这是目前唯一一个能正确支持C++11的build system。一些著名的C++项目如LLVM已经在用CMake，Boost也正在转向CMake。
配合CPack，CMake可以直接生成各种OS上的安装包比如deb、rpm、NSIS等。
相比其他的工具 CMake 有两个巨大的优势，一个是通用性，跨平台跨编译器。另一个是有图形界面，修改参数很方便。

### 比较一
从开发人员的角度来看，cmake目前最容易使用，从用户角度来看，autotools有一大优势
autotools生成一个单独的文件配置脚本，并且生成它的所有文件随发行版一起提供。在grep / sed / awk / vi的帮助下很容易理解和修复。将此与Cmake进行比较，其中在\/usr \/share \/cmak\* \/Modules中找到大量文件，除非用户拥有管理员权限，否则这些文件无法由用户修复。
因此，如果某些东西不起作用，通常可以通过使用标准Unix工具（grep / sed / awk / vi等）以大锤方式轻松“固定”，而无需了解构建系统。



### 示例
Autotools的示例

Quagga 动态路由协议软件
Quagga是一个开源的基于Zebra实现了RIP, OSPF, BGP的动态路由软件。它提供的CLI命令和Cisco IOS类似 ，可以使用 quagga 将linux 机器打造成一台功能完备的路由器


Cmake的示例

OpenCV  计算机视觉和机器学习软件库
OpenCV是一个基于BSD许可（开源）发行的跨平台计算机视觉和机器学习软件库，可以运行在Linux、Windows、Android和Mac OS操作系统上。
它轻量级而且高效——由一系列 C 函数和少量 C++ 类构成，同时提供了Python、Ruby、MATLAB等语言的接口，
实现了图像处理和计算机视觉方面的很多通用算法。

