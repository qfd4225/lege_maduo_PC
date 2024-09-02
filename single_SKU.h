#ifndef MYCLASS_H
#define MYCLASS_H

#include <QVector>
#include <QDebug>

class SingleSKUBpp {
public:
	struct Tray {
		double length;
		double width;
		double max_height;
		double max_weight;
		int Layout_max_num;
	};
	struct Box {
		double length;
		double width;
		double height;
		double weight;
	};

private:
	Tray TrayMember;
	Box BoxMember;
	double overLiftRate;
	double overLiftLen, overLiftWid, overLiftBase;
	int valid_level = 0;

	// 判断货物是否可以放置在托盘上
	bool ifPlaced() const;


public:
	// 定义返回数据类型
	// 定义新的数据类型 LayoutResult
	struct LayoutResult {
		double x; // 横纵坐标
		double y;
		double z;
		double x_len; // 箱子在xy方向上的跨度
		double y_len;
	};

	struct LayoutResultTotal {
		double x; // x y z坐标(中心)
		double y;
		double z;
		int box_ori; // 箱子朝向(横纵放置)
		int enter_dir; // 箱子进入方向
	};

	struct TrayMsql {
        float tray_length;
        float tray_width;
        float tray_MaxHeight;
        float tray_MaxWeight;
        int tray_MaxNumber;
        float box_length;
        float box_width;
        float box_height;
        float box_weight;
	};

	struct LayoutResultMsql {
		int floor_num;
		double x; // x y z坐标(中心)
		double y;
		double z;
		int box_ori; // 箱子朝向(横纵放置)
		int enter_dir; // 箱子进入方向
	};


	enum Layout {
		horizontal_vertical,
		cross_crossmirror,
		rectangular_rectangularmirror
	};

	enum BPtype {
		none_type,
		overhorizontal_type,
		cross_type,
		rectangular_type,
		horizontal_type,
		vertical_type,
		hor_ver_type
	};

	//方块与方块的相对位置
	enum Direction {
		NONE,
		LEFT,
		RIGHT,
		ABOVE,
		BELOW
	};

	// 码垛进入方向设定
	enum EnterDir {
		ENTERVERTICAL,	// 垂直进入
		ENTERLEFT,
		ENTERRIGHT,
		ENTERABOVE,
		ENTERBELOW,
		ENTERUPLEFT,
		ENTERUPRIGHT,
		ENTERDOWNLEFT,
		ENTERDOWNRIGHT
	};

	// 点坐标数据类型
	struct Point {
		double x, y;
	};
	// 方块矩形的数据类型
	struct Rectangle {
		Point bottomLeft;  // 矩形左下角坐标
		double width, height; // 宽度和高度
	};

	// 定义码垛最终结果
	QVector<LayoutResultTotal> layout_total_ori;

	// 定义传输给sql的结果
	QVector<LayoutResultMsql> layout_total_sql;
	TrayMsql tray_sql_data;

    SingleSKUBpp(double length_Tray, double width_Tray, double max_height_Tray, double max_weight_Tray,
                 double length_Box, double width_Box, double height_Box, double weight_Box,
                 double overLiftRate = 0, int layout_max_num = 999);

	// 横向放置
	QVector<LayoutResult> HorizontalLayout(const double base_lenght, const double base_width, const int level_num, const int box_num = 999);
	// 纵向放置
	QVector<LayoutResult> VerticalLayout(const double base_lenght, const double base_width, const int level_num, const int box_num = 999);
	// 横纵交替放置
	QVector<LayoutResult> HorCrossVerLayout(const double base_lenght, const double base_width, const int level_num, const int box_num = 999);
	// 交叉放置
	QVector<LayoutResult> CrossLayout(const double base_lenght, const double base_width, const int level_num, const int box_num = 999);
	// 交叉放置镜像
	QVector<LayoutResult> CrossLayoutMirro(const double base_lenght, const double base_width, const int level_num, const int box_num = 999);
	// 单回字形放置
	QVector<LayoutResult> SingleRectangularLayout(const double base_lenght, const double base_width, const  int level_num, const int box_num = 999);
	// 多边回字形放置
	QVector<LayoutResult> MutiRectangularLayout(const double base_lenght, const double base_base_widthwith, const  int level_num, const int box_num = 999);
	// 多变回字形放置（镜像）
	QVector<LayoutResult> MutiRectangularLayoutMirror(const double base_lenght, const double base_width, const  int level_num, const int box_num = 999);
	// 横向放置 （超长型）
	QVector<LayoutResult> OverHorizontalLayout(const double base_lenght, const double base_width, const int level_num, const int box_num = 999);

	// 整体放置布局（不含限重）
	QVector<LayoutResult> LayoutTotal(Layout layout_method);


	// 分析方块的相对位置关系
	bool isPointInsideRectangle(Point point, Rectangle rect) {
		// 判断点是否在矩形内部或边上
		if (point.x >= rect.bottomLeft.x && point.x <= rect.bottomLeft.x + rect.width &&
			point.y >= rect.bottomLeft.y && point.y <= rect.bottomLeft.y + rect.height) {
			return true;
		}
		return false;
	}
	int isPointInsideRectangleCorner(Rectangle rect1, Rectangle rect2) {
		int corner = 0;
		// 计算rect1和rect2的对角顶点
		Point rect1_top_right = { rect1.bottomLeft.x + rect1.width, rect1.bottomLeft.y + rect1.height };
		Point rect2_top_right = { rect2.bottomLeft.x + rect2.width, rect2.bottomLeft.y + rect2.height };
		Point rect1_top_left = { rect1.bottomLeft.x, rect1.bottomLeft.y + rect1.height };
		Point rect2_top_left = { rect2.bottomLeft.x, rect2.bottomLeft.y + rect2.height };
		Point rect1_bottom_right = { rect1.bottomLeft.x + rect1.width, rect1.bottomLeft.y };
		Point rect2_bottom_right = { rect2.bottomLeft.x + rect2.width, rect2.bottomLeft.y };
		// 判断rect1的右上角与rect2的左下角是否重叠
		if (rect1_top_right.x == rect2.bottomLeft.x && rect1_top_right.y == rect2.bottomLeft.y) {
			corner++;
		}
		// 判断rect1的左上角与rect2的右下角是否重叠
		if (rect1_top_left.x == rect2_bottom_right.x && rect1_top_left.y == rect2_bottom_right.y) {
			corner++;
		}
		// 判断rect1的左下角与rect2的右上角是否重叠
		if (rect1.bottomLeft.x == rect2_top_right.x && rect1.bottomLeft.y == rect2_top_right.y) {
			corner++;
		}
		// 判断rect1的右下角与rect2的左上角是否重叠
		if (rect1_bottom_right.x == rect2_top_left.x && rect1_bottom_right.y == rect2_top_left.y) {
			corner++;
		}
		return corner;
	}
	bool doOverlap(Rectangle rect1, Rectangle rect2, QVector<int>& directions);

	// 整体放置布局，自动分析单层布局形式
	QVector<LayoutResult> LayoutTotalAuto();

	Tray GetTrayMember() { return TrayMember; }
	Box GetBoxMember() { return BoxMember; }

};

#endif // MYCLASS_H
