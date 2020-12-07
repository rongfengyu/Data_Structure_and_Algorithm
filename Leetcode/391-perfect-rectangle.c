/*
391. 完美矩形
我们有 N 个与坐标轴对齐的矩形, 其中 N > 0, 判断它们是否能精确地覆盖一个矩形区域。

每个矩形用左下角的点和右上角的点的坐标来表示。例如， 一个单位正方形可以表示为 [1,1,2,2]。 ( 左下角的点的坐标为 (1, 1) 以及右上角的点的坐标为 (2, 2) )。



示例 1:

rectangles = [
  [1,1,3,3],
  [3,1,4,2],
  [3,2,4,4],
  [1,3,2,4],
  [2,3,3,4]
]

返回 true。5个矩形一起可以精确地覆盖一个矩形区域。
 



示例 2:

rectangles = [
  [1,1,2,3],
  [1,3,2,4],
  [3,1,4,2],
  [3,2,4,4]
]

返回 false。两个矩形之间有间隔，无法覆盖成一个矩形。
 



示例 3:

rectangles = [
  [1,1,3,3],
  [3,1,4,2],
  [1,3,2,4],
  [3,2,4,4]
]

返回 false。图形顶端留有间隔，无法覆盖成一个矩形。
 



示例 4:

rectangles = [
  [1,1,3,3],
  [3,1,4,2],
  [1,3,2,4],
  [2,2,4,4]
]

返回 false。因为中间有相交区域，虽然形成了矩形，但不是精确覆盖。

*/


def isRectangleCover(rectangles: List[List[int]]) -> bool:
    X1, Y1 = float('inf'), float('inf')
    X2, Y2 = -float('inf'), -float('inf')

    points = set()
    actual_area = 0
    for x1, y1, x2, y2 in rectangles:
        # 计算完美矩形的理论顶点坐标
        X1, Y1 = min(X1, x1), min(Y1, y1)
        X2, Y2 = max(X2, x2), max(Y2, y2)
        # 累加小矩形的面积
        actual_area += (x2 - x1) * (y2 - y1)
        # 记录最终形成的图形中的顶点
        p1, p2 = (x1, y1), (x1, y2)
        p3, p4 = (x2, y1), (x2, y2)
        for p in [p1, p2, p3, p4]:
            if p in points: points.remove(p)
            else:           points.add(p)
    # 判断面积是否相同
    expected_area = (X2 - X1) * (Y2 - Y1)
    if actual_area != expected_area:
        return False
    # 判断最终留下的顶点个数是否为 4
    if len(points) != 4:       return False
    # 判断留下的 4 个顶点是否是完美矩形的顶点
    if (X1, Y1) not in points: return False
    if (X1, Y2) not in points: return False
    if (X2, Y1) not in points: return False
    if (X2, Y2) not in points: return False
    # 面积和顶点都对应，说明矩形符合题意
    return True
	
	