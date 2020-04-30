/*
42. 接雨水
给定 n 个非负整数表示每个宽度为 1 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水。
 

       H
   HXXXHHXH
_HXHHXHHHHHH

上面是由数组 [0,1,0,2,1,0,1,3,2,1,2,1] 表示的高度图，在这种情况下，可以接 6 个单位的雨水（蓝色部分表示雨水）。 感谢 Marcos 贡献此图。

示例:

输入: [0,1,0,2,1,0,1,3,2,1,2,1]
输出: 6

分析：双指针法

求每一列的水，我们只需要关注当前列，以及左边最高的墙，右边最高的墙就够了。
装水的多少，当然根据木桶效应，我们只需要看左边最高的墙和右边最高的墙中较矮的一个就够了。
所以，根据较矮的那个墙和当前列的墙的高度可以分为两种种情况。

较矮的墙的高度大于当前列的墙的高度。当前列的注水为 max-height。
较矮的墙的高度小于等于当前列的墙的高度。当前列的注水为零。

我们不从左和从右分开计算，我们想办法一次完成遍历。
我们注意到，只要 right_max[i]>left_max[i]，积水高度将由 left_max 决定，类似的left_max[i]>right_max[i]，积水高度将由 left_max 决定。 所以我们可以认为如果一端有更高的条形块（例如右端），积水的高度依赖于当前方向的高度（从左到右）。当我们发现另一侧（右侧）的条形块高度不是最高的，我们则开始从相反的方向遍历（从右到左）。
我们必须在遍历时维护left_max 和right_max ，但是我们现在可以使用两个指针交替进行，实现 1 次遍历即可完成。

*/

#include <stdio.h>

int trap(int* height, int heightSize)
{
	int left = 0, right = heightSize - 1;
	int ans = 0;
	int left_max = 0, right_max = 0;
	
	//不计算left==right是因为最后的right点肯定是最高的列，最高的列不会有注水。
	while (left < right)
	{
		if (height[left] < height[right])// height[right]大于height[left]点及其左边的列值，说明left点的left_max<right_max。
		{ 
			height[left] >= left_max ? (left_max = height[left]) : (ans += (left_max - height[left]));
			++left;
		}
		else// height[left]大于height[right]点及其右边的列值，说明right点的left_max>=right_max。
		{ 
			height[right] >= right_max ? (right_max = height[right]) : (ans += (right_max - height[right]));
			--right;
		}
	}
	return ans;
}

int main(int argc, char* argv[])
{
    int nums[] = {0,1,0,2,1,0,1,3,2,1,2,1};
    int size = sizeof(nums)/sizeof(int);
	
    int ret  = 0;
	ret = trap(nums, size);
	printf("ret = %d\n", ret);//6

    return 0;
}
