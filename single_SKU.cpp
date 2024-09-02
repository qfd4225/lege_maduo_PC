// single_SKU.cpp
#include "single_SKU.h"
#include <QElapsedTimer>


SingleSKUBpp::SingleSKUBpp(double length_Tray, double width_Tray, double max_height_Tray, double max_weight_Tray, double length_Box, double width_Box, double height_Box, double weight_Box, double overLiftRate, int layout_max_num)
    : TrayMember({ length_Tray, width_Tray, max_height_Tray, max_weight_Tray, layout_max_num }), BoxMember({ length_Box, width_Box, height_Box, weight_Box }), overLiftRate(overLiftRate) {
    overLiftLen = BoxMember.length * overLiftRate;
    overLiftWid = BoxMember.width * overLiftRate;
    overLiftBase = BoxMember.width * overLiftRate;
    qDebug() << "overLiftLen" << overLiftLen;
    qDebug() << "overLiftWid" << overLiftWid;
    qDebug() << "overLiftBase" << overLiftBase;
    // 最大可放置层数
    valid_level = max_height_Tray / height_Box;
    qDebug() << "valid_level" << valid_level;

}

bool SingleSKUBpp::ifPlaced() const {
    // 实现判断逻辑，返回是否可以放置
    return true;
}

QVector<SingleSKUBpp::LayoutResult> SingleSKUBpp::HorizontalLayout(const double base_lenght, const double base_width, const int level_num, const int box_num) {
    QVector<LayoutResult> layoutResult;
    qDebug() << "enter";

    //设置居中放置时x和y方向上需要平移的移动量
    double x_remain = 0, y_remain = 0;

    double availableLength = base_lenght + overLiftBase * 2;
    double availableWidth = base_width + overLiftBase * 2;
    qDebug() << "availableLength" << availableLength;
    qDebug() << "availableWidth" << availableWidth;

    int x_num = availableLength / BoxMember.length;
    int y_num = availableWidth / BoxMember.width;
    qDebug() << "x_num" << x_num;
    qDebug() << "y_num" << y_num;

    // 优先居中放置
    // 计算起始位置x和y
    double x_init = (base_lenght - (x_num * BoxMember.length)) / 2;
    double y_init = (base_width - (y_num * BoxMember.width)) / 2;
    qDebug() << "x_init" << x_init;
    qDebug() << "y_init" << y_init;

    for (int i_y = 0; i_y < y_num; i_y++) {
        for (int i_x = 0; i_x < x_num; i_x++) {
            //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.length, y_init + i_y * BoxMember.width));
            layoutResult.append({ x_init + i_x * BoxMember.length, y_init + i_y * BoxMember.width,(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
            if (layoutResult.size() >= box_num) {
                return layoutResult;
            }
        }
    }

    //// 分散型放置
    //// 计算x、y方向偏移量
    //if (x_num > 1 && y_num > 1) {  // x和y方向上的箱子数量均大于一个 即可均匀分布
    //	x_remain = (base_lenght - BoxMember.length * x_num) / (x_num - 1);
    //	y_remain = (base_width - BoxMember.width * y_num) / (y_num - 1);
    //	double x_init = 0;
    //	double y_init = 0;
    //	qDebug() << "x_init" << x_init;
    //	qDebug() << "y_init" << y_init;

    //	for (int i_y = 0; i_y < y_num; i_y++) {
    //		for (int i_x = 0; i_x < x_num; i_x++) {
    //			//layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.length, y_init + i_y * BoxMember.width));
    //			layoutResult.append({ x_init + i_x * (BoxMember.length + x_remain), y_init + i_y * (BoxMember.width + y_remain),(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
    //			if (layoutResult.size() >= box_num) {
    //				return layoutResult;
    //			}
    //		}
    //	}
    //}
    //else if (y_num > 1) {
    //	y_remain = (base_width - BoxMember.width * y_num) / (y_num - 1);
    //	double x_init = (base_lenght - (x_num * BoxMember.length)) / 2;
    //	double y_init = 0;
    //	qDebug() << "x_init" << x_init;
    //	qDebug() << "y_init" << y_init;

    //	for (int i_y = 0; i_y < y_num; i_y++) {
    //		for (int i_x = 0; i_x < x_num; i_x++) {
    //			//layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.length, y_init + i_y * BoxMember.width));
    //			layoutResult.append({ x_init + i_x * (BoxMember.length + x_remain), y_init + i_y * (BoxMember.width + y_remain),(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
    //			if (layoutResult.size() >= box_num) {
    //				return layoutResult;
    //			}
    //		}
    //	}
    //}
    //else if (x_num > 1) {
    //	x_remain = (base_lenght - BoxMember.length * x_num) / (x_num - 1);
    //	double x_init = 0;
    //	double y_init = (base_width - (y_num * BoxMember.width)) / 2;
    //	qDebug() << "x_init" << x_init;
    //	qDebug() << "y_init" << y_init;

    //	for (int i_y = 0; i_y < y_num; i_y++) {
    //		for (int i_x = 0; i_x < x_num; i_x++) {
    //			//layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.length, y_init + i_y * BoxMember.width));
    //			layoutResult.append({ x_init + i_x * (BoxMember.length + x_remain), y_init + i_y * (BoxMember.width + y_remain),(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
    //			if (layoutResult.size() >= box_num) {
    //				return layoutResult;
    //			}
    //		}
    //	}
    //}
    //else {
    //	// 居中放置
    //	double x_init = (base_lenght - (x_num * BoxMember.length)) / 2;
    //	double y_init = (base_width - (y_num * BoxMember.width)) / 2;
    //	qDebug() << "x_init" << x_init;
    //	qDebug() << "y_init" << y_init;
    //	for (int i_y = 0; i_y < y_num; i_y++) {
    //		for (int i_x = 0; i_x < x_num; i_x++) {
    //			//layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.length, y_init + i_y * BoxMember.width));
    //			layoutResult.append({ x_init + i_x * (BoxMember.length + x_remain), y_init + i_y * (BoxMember.width + y_remain),(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
    //			if (layoutResult.size() >= box_num) {
    //				return layoutResult;
    //			}
    //		}
    //	}
    //}

    return layoutResult;
}

QVector<SingleSKUBpp::LayoutResult> SingleSKUBpp::VerticalLayout(const double base_lenght, const double base_width, const int level_num, const int box_num) {
    // 实现垂直布局逻辑，可以使用 overLiftNum
    QVector<LayoutResult> layoutResult;
    qDebug() << "enter";

    //设置居中放置时x和y方向上需要平移的移动量
    double x_remain = 0, y_remain = 0;

    double availableLength = base_lenght + overLiftBase * 2;
    double availableWidth = base_width + overLiftBase * 2;
    qDebug() << "availableLength" << availableLength;
    qDebug() << "availableWidth" << availableWidth;

    int x_num = availableLength / BoxMember.width;
    int y_num = availableWidth / BoxMember.length;
    qDebug() << "x_num" << x_num;
    qDebug() << "y_num" << y_num;

    // 优先居中放置
    // 计算起始位置x和y
    double x_init = (base_lenght - (x_num * BoxMember.width)) / 2;
    double y_init = (base_width - (y_num * BoxMember.length)) / 2;
    qDebug() << "x_init" << x_init;
    qDebug() << "y_init" << y_init;

    for (int i_y = 0; i_y < y_num; i_y++) {
        for (int i_x = 0; i_x < x_num; i_x++) {
            //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
            layoutResult.append({ x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length,(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
            if (layoutResult.size() >= box_num) {
                return layoutResult;
            }
        }
    }

    //// 分散型放置
    //// 计算x、y方向偏移量
    //if (x_num > 1 && y_num > 1) {  // x和y方向上的箱子数量均大于一个 即可均匀分布
    //	x_remain = (base_lenght - BoxMember.width * x_num) / (x_num - 1);
    //	y_remain = (base_width - BoxMember.length * y_num) / (y_num - 1);
    //	double x_init = 0;
    //	double y_init = 0;
    //	qDebug() << "x_init" << x_init;
    //	qDebug() << "y_init" << y_init;

    //	for (int i_y = 0; i_y < y_num; i_y++) {
    //		for (int i_x = 0; i_x < x_num; i_x++) {
    //			//layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.length, y_init + i_y * BoxMember.width));
    //			layoutResult.append({ x_init + i_x * (BoxMember.width + x_remain), y_init + i_y * (BoxMember.length + y_remain),(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
    //			if (layoutResult.size() >= box_num) {
    //				return layoutResult;
    //			}
    //		}
    //	}
    //}
    //else if (y_num > 1) {
    //	y_remain = (base_width - BoxMember.length * y_num) / (y_num - 1);
    //	double x_init = (base_lenght - (x_num * BoxMember.width)) / 2;
    //	double y_init = 0;
    //	qDebug() << "x_init" << x_init;
    //	qDebug() << "y_init" << y_init;

    //	for (int i_y = 0; i_y < y_num; i_y++) {
    //		for (int i_x = 0; i_x < x_num; i_x++) {
    //			//layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.length, y_init + i_y * BoxMember.width));
    //			layoutResult.append({ x_init + i_x * (BoxMember.width + x_remain), y_init + i_y * (BoxMember.length + y_remain),(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
    //			if (layoutResult.size() >= box_num) {
    //				return layoutResult;
    //			}
    //		}
    //	}
    //}
    //else if (x_num > 1) {
    //	x_remain = (base_lenght - BoxMember.width * x_num) / (x_num - 1);
    //	double x_init = 0;
    //	double y_init = (base_width - (y_num * BoxMember.length)) / 2;
    //	qDebug() << "x_init" << x_init;
    //	qDebug() << "y_init" << y_init;

    //	for (int i_y = 0; i_y < y_num; i_y++) {
    //		for (int i_x = 0; i_x < x_num; i_x++) {
    //			//layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.length, y_init + i_y * BoxMember.width));
    //			layoutResult.append({ x_init + i_x * (BoxMember.width + x_remain), y_init + i_y * (BoxMember.length + y_remain),(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
    //			if (layoutResult.size() >= box_num) {
    //				return layoutResult;
    //			}
    //		}
    //	}
    //}
    //else {
    //	double x_init = (base_lenght - (x_num * BoxMember.width)) / 2;
    //	double y_init = (base_width - (y_num * BoxMember.length)) / 2;
    //	qDebug() << "x_init" << x_init;
    //	qDebug() << "y_init" << y_init;

    //	for (int i_y = 0; i_y < y_num; i_y++) {
    //		for (int i_x = 0; i_x < x_num; i_x++) {
    //			//layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.length, y_init + i_y * BoxMember.width));
    //			layoutResult.append({ x_init + i_x * (BoxMember.width + x_remain), y_init + i_y * (BoxMember.length + y_remain),(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
    //			if (layoutResult.size() >= box_num) {
    //				return layoutResult;
    //			}
    //		}
    //	}
    //}

    return layoutResult;
}

QVector<SingleSKUBpp::LayoutResult> SingleSKUBpp::HorCrossVerLayout(const double base_lenght, const double base_width, const int level_num, const int box_num)
{
    QVector<LayoutResult> layoutResult;
    qDebug() << "enter";

    double availableLength = base_lenght + overLiftBase * 2;
    double availableWidth = base_width + overLiftBase * 2;
    qDebug() << "availableLength" << availableLength;
    qDebug() << "availableWidth" << availableWidth;

    // 计算横向放置数量
    int x_num_hor = availableLength / BoxMember.length;
    int y_num_hor = availableWidth / BoxMember.width;
    qDebug() << "x_num_hor" << x_num_hor;
    qDebug() << "y_num_hor" << y_num_hor;

    // 计算纵向放置数量
    int x_num_ver = availableLength / BoxMember.width;
    int y_num_ver = availableWidth / BoxMember.length;
    qDebug() << "x_num_ver" << x_num_ver;
    qDebug() << "y_num_ver" << y_num_ver;
    // 定义最大拉伸量
    float over_rate_len = 1.0 / 3.0 * BoxMember.width; // len方向
    //float over_rate_wid = 0.5 * BoxMember.width; // wid方向

    // 当横向y方向数量和纵向x方向数量均大于2时进行该模式放置
    if (y_num_hor >= 2 && x_num_ver >= 2) {
        // 横向放置时紧凑状态下矩形长宽
        float hor_len_temp = x_num_hor * BoxMember.length;
        float hor_wid_temp = y_num_hor * BoxMember.width;
        // 纵向放置时紧凑状态下矩形长宽
        float ver_len_temp = x_num_ver * BoxMember.width;
        float ver_wid_temp = y_num_ver * BoxMember.length;
        // 将小的len和wid作为最大目标矩形的len和wid
        float len_temp = hor_len_temp + over_rate_len;
        if (availableLength < len_temp) {
            len_temp = availableLength;
        }
        float wid_temp = ver_wid_temp + over_rate_len;
        if (availableWidth < wid_temp) {
            wid_temp = availableWidth;
        }

        // 基于新的len和wid 重新计算可放置数量
        // 计算横向放置数量
        x_num_hor = len_temp / BoxMember.length;
        y_num_hor = wid_temp / BoxMember.width;
        // 计算纵向放置数量
        x_num_ver = len_temp / BoxMember.width;
        y_num_ver = wid_temp / BoxMember.length;
        // 放置数量过少
        if (y_num_hor < 2 || x_num_ver < 2) {
            return layoutResult;
        }

        // 重新计算紧凑状态下的长宽并取大值作为新长宽
        // 横向放置时紧凑状态下矩形长宽
        hor_len_temp = x_num_hor * BoxMember.length;
        hor_wid_temp = y_num_hor * BoxMember.width;
        // 纵向放置时紧凑状态下矩形长宽
        ver_len_temp = x_num_ver * BoxMember.width;
        ver_wid_temp = y_num_ver * BoxMember.length;
        // 将大的len和wid作为最终矩形的len和wid
        len_temp = hor_len_temp;
        if (ver_len_temp > len_temp) {
            len_temp = ver_len_temp;
        }
        wid_temp = hor_wid_temp;
        if (ver_wid_temp > wid_temp) {
            wid_temp = ver_wid_temp;
        }

        // 计算横向放置xy方向的空隙
        float hor_x_gap = (len_temp - hor_len_temp) / x_num_hor;
        float hor_y_gap = (wid_temp - hor_wid_temp) / (y_num_hor - 1);
        // 计算纵向放置xy方向的空隙
        float ver_x_gap = (len_temp - ver_len_temp) / (x_num_ver - 1);
        float ver_y_gap = (wid_temp - ver_wid_temp) / y_num_ver;

        // 奇数层横向放置
        if (level_num % 2 == 1) {
            // 居中放置
            // 计算起始位置x和y
            double x_init = (base_lenght - len_temp) / 2;
            double y_init = (base_width - wid_temp) / 2;
            qDebug() << "x_init" << x_init;
            qDebug() << "y_init" << y_init;
            // x方向放置优先
            for (int i_y = 0; i_y < y_num_hor; i_y++) {
                for (int i_x = 0; i_x < x_num_hor; i_x++) {
                    //奇偶列区分
                    if (i_y % 2 == 0) {
                        layoutResult.append({ x_init + i_x * BoxMember.length + i_x * hor_x_gap, y_init + i_y * BoxMember.width + i_y * hor_y_gap,(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
                        if (layoutResult.size() >= box_num) {
                            return layoutResult;
                        }
                    }
                    else {
                        layoutResult.append({ x_init + i_x * BoxMember.length + (i_x + 1) * hor_x_gap, y_init + i_y * BoxMember.width + i_y * hor_y_gap,(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
                        if (layoutResult.size() >= box_num) {
                            return layoutResult;
                        }
                    }
                }
            }
        }
        // 偶数层纵向放置
        else {
            // 居中放置
            // 计算起始位置x和y
            double x_init = (base_lenght - len_temp) / 2;
            double y_init = (base_width - wid_temp) / 2;
            qDebug() << "x_init" << x_init;
            qDebug() << "y_init" << y_init;
            // y方向放置优先
            for (int i_x = 0; i_x < x_num_ver; i_x++) {
                for (int i_y = 0; i_y < y_num_ver; i_y++) {
                    //奇偶列区分
                    if (i_x % 2 == 0) {
                        layoutResult.append({ x_init + i_x * BoxMember.width + i_x * ver_x_gap, y_init + i_y * BoxMember.length + i_y * ver_y_gap,(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
                        if (layoutResult.size() >= box_num) {
                            return layoutResult;
                        }
                    }
                    else {
                        layoutResult.append({ x_init + i_x * BoxMember.width + i_x * ver_x_gap, y_init + i_y * BoxMember.length + (i_y + 1) * ver_y_gap,(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
                        if (layoutResult.size() >= box_num) {
                            return layoutResult;
                        }
                    }
                }
            }
        }

    }
    else {
        return layoutResult;
    }
    return layoutResult;
}

QVector<SingleSKUBpp::LayoutResult> SingleSKUBpp::CrossLayout(const double base_lenght, const double base_width, const int level_num, const int box_num) {
    // 实现交叉布局逻辑，可以使用 overLiftNum
    QVector<LayoutResult> layoutResult;
    qDebug() << "enter CrossLayout";

    double over_rate = 0.25;// 可空出比例
    double over_len = BoxMember.width * over_rate;

    int num_length_length = (base_lenght + overLiftBase * 2) / BoxMember.length;
    int num_width_width = (base_width + overLiftBase * 2) / BoxMember.width;
    int num_length_width = (base_lenght + overLiftBase * 2) / BoxMember.width;
    int num_width_length = (base_width + overLiftBase * 2) / BoxMember.length;
    double overLift = overLiftBase;

    qDebug() << "num_lenght_length" << num_length_length;
    qDebug() << "num_width_width" << num_width_width;
    qDebug() << "num_length_width" << num_length_width;
    qDebug() << "num_width_length" << num_width_length;

    if ((BoxMember.length + BoxMember.width) > base_lenght) {
        // 箱子数不够横纵摆放
        qDebug() << "Not enough boxes";
        return layoutResult;
    }

    int place_type = 1; // 摆放方式
    int box_total = 0; // 当前方式的最大箱子摆放数量

    int num_h_x = 0, num_h_y = 0; // 水平放置箱子两个方向的数量
    int num_v_x = 0, num_v_y = 0; // 垂直放置箱子两个方向的数量
    // 箱子数量 box_total = num_h_x * num_h_y + num_v_x * num_v_y

    int box_num_temp;
    // 放置方式1 横向与纵向垂直 宽去对齐长
    for (int i_x = 1; i_x <= num_length_length; i_x++) {
        for (int i_y = 1; i_y <= num_width_width; i_y++) {
            int num_v_x_temp = ((i_x * BoxMember.length) / BoxMember.width);
            int num_v_y_temp = (base_width + overLiftBase * 2 - i_y * BoxMember.width) / BoxMember.length;
            box_num_temp = i_x * i_y + num_v_x_temp * num_v_y_temp;
            if (box_num_temp > box_total && num_v_x_temp > 0 && num_v_y_temp > 0) {
                box_total = box_num_temp;
                num_h_x = i_x;
                num_h_y = i_y;
                num_v_x = num_v_x_temp;
                num_v_y = num_v_y_temp;
                place_type = 1;
            }
        }
    }

    // 放置方式2 横向与纵向水平 宽去对齐长
    for (int i_x = 1; i_x <= num_length_width; i_x++) {
        for (int i_y = 1; i_y <= num_width_length; i_y++) {
            int num_h_x_temp = (base_lenght + overLiftBase * 2 - i_x * BoxMember.width) / BoxMember.length;
            int num_h_y_temp = (i_y * BoxMember.length) / BoxMember.width;
            box_num_temp = i_x * i_y + num_h_x_temp * num_h_y_temp;
            if (box_num_temp > box_total && num_h_x_temp > 0 && num_h_y_temp > 0) {
                box_total = box_num_temp;
                num_v_x = i_x;
                num_v_y = i_y;
                num_h_x = num_h_x_temp;
                num_h_y = num_h_y_temp;
                place_type = 2;
            }
        }
    }

    // 放置方式3 横向与纵向垂直 长去对齐宽
    for (int i_x = 1; i_x <= num_length_width; i_x++) {
        for (int i_y = 1; i_y <= num_width_length; i_y++) {
            int num_h_x_temp = i_x * BoxMember.width / BoxMember.length;
            int num_h_y_temp = (base_width + overLiftBase * 2 - BoxMember.length * i_y) / BoxMember.width;
            box_num_temp = i_x * i_y + num_h_x_temp * num_h_y_temp;
            if (box_num_temp > box_total && num_h_x_temp > 0 && num_h_y_temp > 0 && (i_x * BoxMember.width - num_h_x_temp * BoxMember.length) <= BoxMember.width * over_rate) {
                box_total = box_num_temp;
                num_h_x = num_h_x_temp;
                num_h_y = num_h_y_temp;
                num_v_x = i_x;
                num_v_y = i_y;
                place_type = 3;
            }
        }
    }

    // 放置方式4 横向与纵向水平 长去对齐宽
    for (int i_x = 1; i_x <= num_length_length; i_x++) {
        for (int i_y = 1; i_y <= num_width_width; i_y++) {
            int num_v_x_temp = (base_lenght + overLiftBase * 2 - i_x * BoxMember.length) / BoxMember.width;
            int num_v_y_temp = i_y * BoxMember.width / BoxMember.length;
            box_num_temp = i_x * i_y + num_v_x_temp * num_v_x_temp;
            if (box_num_temp > box_total && num_v_x_temp > 0 && num_v_y_temp > 0 && (i_y * BoxMember.width - num_v_y_temp * BoxMember.length) <= (BoxMember.width * over_rate)) {
                box_total = box_num_temp;
                num_v_x = num_v_x_temp;
                num_v_y = num_v_y_temp;
                num_h_x = i_x;
                num_h_y = i_y;
                place_type = 4;
            }
        }
    }

    // 按方案放置
    if (place_type == 1) {
        qDebug() << "place type 1";
        qDebug() << "num_h_x" << num_h_x;
        qDebug() << "num_h_y" << num_h_y;
        qDebug() << "num_v_x" << num_v_x;
        qDebug() << "num_v_y" << num_v_y;
        if (num_v_x == 1) {
            return layoutResult;
        }
        // 横向居中放置 纵向以横向为宽分开放置
        // 纵向放置是否居中或偏移
        // if (num_h_x > 1) {
        if (num_h_x * BoxMember.length > num_v_x * BoxMember.width) {
            // 横向放置的x起始、终止坐标
            double x_init = (base_lenght - (num_h_x * BoxMember.length)) / 2;
            double x_end = x_init + num_h_x * BoxMember.length;
            // 偏移
            double x_remain = (num_h_x * BoxMember.length - num_v_x * BoxMember.width) / (num_v_x - 1);
            //double y_remain = (base_width + overLift * 2 - num_h_y * BoxMember.width - num_v_y * BoxMember.length) / (num_h_y + num_v_y - 1);
            double y_remain = 0;
            // 先放置垂直箱子
            //double y_init = -overLift;
            double y_init = (base_width - (num_h_y * BoxMember.width + num_v_y * BoxMember.length)) / 2;
            for (int i_x = 0; i_x < num_v_x; i_x++) {
                for (int i_y = 0; i_y < num_v_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.width + x_remain), y_init + i_y * (BoxMember.length + y_remain),(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
            // 放置水平箱子
            y_init = y_init + (BoxMember.length + y_remain) * num_v_y;
            for (int i_x = 0; i_x < num_h_x; i_x++) {
                for (int i_y = 0; i_y < num_h_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.length + 0), y_init + i_y * (BoxMember.width + y_remain),(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
        }
        else {
            // 居中
            double x_init = (base_lenght - (num_v_x * BoxMember.width)) / 2;
            double x_remain = 0;
            //double y_remain = (base_width + overLift * 2 - num_h_y * BoxMember.width - num_v_y * BoxMember.length) / (num_h_y + num_v_y - 1);
            double y_remain = 0;
            // 先放置垂直箱子
            //double y_init = -overLift;
            double y_init = (base_width - (num_h_y * BoxMember.width + num_v_y * BoxMember.length)) / 2;
            for (int i_x = 0; i_x < num_v_x; i_x++) {
                for (int i_y = 0; i_y < num_v_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.width + x_remain), y_init + i_y * (BoxMember.length + y_remain),(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
            // 放置水平箱子
            x_init = (base_lenght - (num_h_x * BoxMember.length)) / 2;
            y_init = y_init + (BoxMember.length + y_remain) * num_v_y;
            for (int i_x = 0; i_x < num_h_x; i_x++) {
                for (int i_y = 0; i_y < num_h_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.length + x_remain), y_init + i_y * (BoxMember.width + y_remain),(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
        }
    }
    else if (place_type == 2) {
        qDebug() << "place type 2";
        qDebug() << "num_v_x" << num_v_x;
        qDebug() << "num_v_y" << num_v_y;
        qDebug() << "num_h_x" << num_h_x;
        qDebug() << "num_h_y" << num_h_y;
        if (num_h_y == 1) {
            return layoutResult;
        }
        // 纵向居中放置 横向以纵向为宽分开放置
        // 横向放置是否居中或偏移
        //if (num_v_y > 1) {
        if (num_v_y * BoxMember.length > num_v_x * BoxMember.width) {
            // 偏移放置
            double y_init = (base_width - (num_v_y * BoxMember.length)) / 2;
            // 计算偏移量
            double y_remain = (num_v_y * BoxMember.length - num_h_y * BoxMember.width) / (num_h_y - 1);
            //double x_remain = (base_lenght + overLift * 2 - num_v_x * BoxMember.width - num_h_x * BoxMember.length) / (num_v_x + num_h_x - 1);
            double x_remain = 0;
            // 放置垂直箱子
            //double x_init = -overLift;
            double x_init = (base_lenght - num_v_x * BoxMember.width - num_h_x * BoxMember.length) / 2;
            for (int i_x = 0; i_x < num_v_x; i_x++) {
                for (int i_y = 0; i_y < num_v_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.width + x_remain), y_init + i_y * (BoxMember.length + 0),(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
            // 放置水平箱子
            x_init = x_init + num_v_x * (BoxMember.width + x_remain);
            for (int i_x = 0; i_x < num_h_x; i_x++) {
                for (int i_y = 0; i_y < num_h_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.length + x_remain), y_init + i_y * (BoxMember.width + y_remain),(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
        }
        else {
            // 居中放置
            double y_init = (base_width - (num_v_y * BoxMember.length)) / 2;
            // 计算偏移量
            double y_remain = 0;
            //double x_remain = (base_lenght + overLift * 2 - num_v_x * BoxMember.width - num_h_x * BoxMember.length) / (num_v_x + num_h_x - 1);
            double x_remain = 0;
            // 放置垂直箱子
            //double x_init = -overLift;
            double x_init = (base_lenght - num_v_x * BoxMember.width - num_h_x * BoxMember.length) / 2;
            for (int i_x = 0; i_x < num_v_x; i_x++) {
                for (int i_y = 0; i_y < num_v_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.width + x_remain), y_init + i_y * (BoxMember.length + 0),(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
            // 放置水平箱子
            y_init = (base_width - (num_h_y * BoxMember.width)) / 2;
            x_init = x_init + (BoxMember.width + x_remain) * num_v_x;
            for (int i_x = 0; i_x < num_h_x; i_x++) {
                for (int i_y = 0; i_y < num_h_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.length + x_remain), y_init + i_y * (BoxMember.width + y_remain),(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
        }
    }
    else if (place_type == 3) {
        qDebug() << "place type 3";
        qDebug() << "num_h_x" << num_h_x;
        qDebug() << "num_h_y" << num_h_y;
        qDebug() << "num_v_x" << num_v_x;
        qDebug() << "num_v_y" << num_v_y;
        if (num_v_x <= 1) {
            return layoutResult;
        }
        // 先排列纵向、再排列横向
        // 横向只有一个时，居中放置横向，纵向紧凑排列
        if (num_h_x == 1) {
            // 纵向放置的x起始、y起始坐标
            double x_init = (base_lenght - (num_v_x * BoxMember.width)) / 2;
            double y_init = (base_width - num_v_y * BoxMember.length - num_h_y * BoxMember.width) / 2;
            // 放置垂直箱子
            for (int i_x = 0; i_x < num_v_x; i_x++) {
                for (int i_y = 0; i_y < num_v_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.width), y_init + i_y * (BoxMember.length),(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
            // 放置水平箱子
            y_init = y_init + (BoxMember.length) * num_v_y;
            x_init = (base_lenght - BoxMember.length) / 2;
            for (int i_x = 0; i_x < num_h_x; i_x++) {
                for (int i_y = 0; i_y < num_h_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.length + 0), y_init + i_y * (BoxMember.width),(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
        }
        // 横向两端对齐，纵向紧凑排列
        else {
            // 纵向放置的x起始、y起始坐标
            double x_init = (base_lenght - (num_v_x * BoxMember.width)) / 2;
            double y_init = (base_width - num_v_y * BoxMember.length - num_h_y * BoxMember.width) / 2;
            // 放置垂直箱子
            for (int i_x = 0; i_x < num_v_x; i_x++) {
                for (int i_y = 0; i_y < num_v_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.width), y_init + i_y * (BoxMember.length),(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
            double x_remain = (num_v_x * BoxMember.width - num_h_x * BoxMember.length) / (num_h_x - 1);
            y_init = y_init + num_v_y * BoxMember.length;
            for (int i_x = 0; i_x < num_h_x; i_x++) {
                for (int i_y = 0; i_y < num_h_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.length + x_remain), y_init + i_y * (BoxMember.width),(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
        }
    }
    else if (place_type == 4) {
        qDebug() << "place type 4";
        qDebug() << "num_v_x" << num_v_x;
        qDebug() << "num_v_y" << num_v_y;
        qDebug() << "num_h_x" << num_h_x;
        qDebug() << "num_h_y" << num_h_y;
        if (num_h_y <= 1) {
            return layoutResult;
        }
        // 先排列纵向 再排列横向
        // 纵向一个时，居中放置
        if (num_v_y == 1) {
            // 偏移放置
            double y_init = (base_width - BoxMember.length) / 2;
            // 计算偏移量
            double y_remain = 0;
            //double x_remain = (base_lenght + overLift * 2 - num_v_x * BoxMember.width - num_h_x * BoxMember.length) / (num_v_x + num_h_x - 1);
            double x_remain = 0;
            // 放置垂直箱子
            //double x_init = -overLift;
            double x_init = (base_lenght - num_v_x * BoxMember.width - num_h_x * BoxMember.length) / 2;
            for (int i_x = 0; i_x < num_v_x; i_x++) {
                for (int i_y = 0; i_y < num_v_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.width + x_remain), y_init + i_y * (BoxMember.length + y_remain),(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
            // 放置水平箱子
            x_init = x_init + num_v_x * (BoxMember.width + x_remain);
            y_init = (base_width - (num_h_y * BoxMember.width)) / 2;
            for (int i_x = 0; i_x < num_h_x; i_x++) {
                for (int i_y = 0; i_y < num_h_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.length + x_remain), y_init + i_y * (BoxMember.width + 0),(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
        }
        else {
            // 偏移放置
            double y_init = (base_width - (num_h_y * BoxMember.width)) / 2;
            // 计算偏移量
            double y_remain = (num_h_y * BoxMember.width - num_v_y * BoxMember.length) / (num_v_y - 1);
            //double x_remain = (base_lenght + overLift * 2 - num_v_x * BoxMember.width - num_h_x * BoxMember.length) / (num_v_x + num_h_x - 1);
            double x_remain = 0;
            // 放置垂直箱子
            //double x_init = -overLift;
            double x_init = (base_lenght - num_v_x * BoxMember.width - num_h_x * BoxMember.length) / 2;
            for (int i_x = 0; i_x < num_v_x; i_x++) {
                for (int i_y = 0; i_y < num_v_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.width + x_remain), y_init + i_y * (BoxMember.length + y_remain),(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
            // 放置水平箱子
            x_init = x_init + num_v_x * (BoxMember.width + x_remain);
            for (int i_x = 0; i_x < num_h_x; i_x++) {
                for (int i_y = 0; i_y < num_h_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.length + x_remain), y_init + i_y * (BoxMember.width + 0),(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
        }
    }
    else {
        qDebug() << "place type error";
        return layoutResult;
    }
    return layoutResult;
}

QVector<SingleSKUBpp::LayoutResult> SingleSKUBpp::CrossLayoutMirro(const double base_lenght, const double base_width, const int level_num, const int box_num) {
    // 实现交叉布局逻辑，可以使用 overLiftNum
    QVector<LayoutResult> layoutResult;
    qDebug() << "enter CrossLayoutMirro";

    double over_rate = 0.25;// 可空出比例
    double over_len = BoxMember.width * over_rate;

    int num_length_length = (base_lenght + overLiftBase * 2) / BoxMember.length;
    int num_width_width = (base_width + overLiftBase * 2) / BoxMember.width;
    int num_length_width = (base_lenght + overLiftBase * 2) / BoxMember.width;
    int num_width_length = (base_width + overLiftBase * 2) / BoxMember.length;
    double overLift = overLiftBase;

    qDebug() << "num_lenght_length" << num_length_length;
    qDebug() << "num_width_width" << num_width_width;
    qDebug() << "num_length_width" << num_length_width;
    qDebug() << "num_width_length" << num_width_length;

    if ((BoxMember.length + BoxMember.width) > base_lenght) {
        // 箱子数不够横纵摆放
        qDebug() << "Not enough boxes";
        return layoutResult;
    }

    int place_type = 1; // 摆放方式
    int box_total = 0; // 当前方式的最大箱子摆放数量

    int num_h_x = 0, num_h_y = 0; // 水平放置箱子两个方向的数量
    int num_v_x = 0, num_v_y = 0; // 垂直放置箱子两个方向的数量
    // 箱子数量 box_total = num_h_x * num_h_y + num_v_x * num_v_y

    int box_num_temp;
    // 放置方式1 横向与纵向垂直 宽去对齐长
    for (int i_x = 1; i_x <= num_length_length; i_x++) {
        for (int i_y = 1; i_y <= num_width_width; i_y++) {
            int num_v_x_temp = ((i_x * BoxMember.length) / BoxMember.width);
            int num_v_y_temp = (base_width + overLiftBase * 2 - i_y * BoxMember.width) / BoxMember.length;
            box_num_temp = i_x * i_y + num_v_x_temp * num_v_y_temp;
            if (box_num_temp > box_total && num_v_x_temp > 0 && num_v_y_temp > 0) {
                box_total = box_num_temp;
                num_h_x = i_x;
                num_h_y = i_y;
                num_v_x = num_v_x_temp;
                num_v_y = num_v_y_temp;
                place_type = 1;
            }
        }
    }

    // 放置方式2 横向与纵向水平 宽去对齐长
    for (int i_x = 1; i_x <= num_length_width; i_x++) {
        for (int i_y = 1; i_y <= num_width_length; i_y++) {
            int num_h_x_temp = (base_lenght + overLiftBase * 2 - i_x * BoxMember.width) / BoxMember.length;
            int num_h_y_temp = (i_y * BoxMember.length) / BoxMember.width;
            box_num_temp = i_x * i_y + num_h_x_temp * num_h_y_temp;
            if (box_num_temp > box_total && num_h_x_temp > 0 && num_h_y_temp > 0) {
                box_total = box_num_temp;
                num_v_x = i_x;
                num_v_y = i_y;
                num_h_x = num_h_x_temp;
                num_h_y = num_h_y_temp;
                place_type = 2;
            }
        }
    }

    // 放置方式3 横向与纵向垂直 长去对齐宽
    for (int i_x = 1; i_x <= num_length_width; i_x++) {
        for (int i_y = 1; i_y <= num_width_length; i_y++) {
            int num_h_x_temp = i_x * BoxMember.width / BoxMember.length;
            int num_h_y_temp = (base_width + overLiftBase * 2 - BoxMember.length * i_y) / BoxMember.width;
            box_num_temp = i_x * i_y + num_h_x_temp * num_h_y_temp;
            if (box_num_temp > box_total && num_h_x_temp > 0 && num_h_y_temp > 0 && (i_x * BoxMember.width - num_h_x_temp * BoxMember.length) <= BoxMember.width * over_rate) {
                box_total = box_num_temp;
                num_h_x = num_h_x_temp;
                num_h_y = num_h_y_temp;
                num_v_x = i_x;
                num_v_y = i_y;
                place_type = 3;
            }
        }
    }

    // 放置方式4 横向与纵向水平 长去对齐宽
    for (int i_x = 1; i_x <= num_length_length; i_x++) {
        for (int i_y = 1; i_y <= num_width_width; i_y++) {
            int num_v_x_temp = (base_lenght + overLiftBase * 2 - i_x * BoxMember.length) / BoxMember.width;
            int num_v_y_temp = i_y * BoxMember.width / BoxMember.length;
            box_num_temp = i_x * i_y + num_v_x_temp * num_v_x_temp;
            if (box_num_temp > box_total && num_v_x_temp > 0 && num_v_y_temp > 0 && (i_y * BoxMember.width - num_v_y_temp * BoxMember.length) <= (BoxMember.width * over_rate)) {
                box_total = box_num_temp;
                num_v_x = num_v_x_temp;
                num_v_y = num_v_y_temp;
                num_h_x = i_x;
                num_h_y = i_y;
                place_type = 4;
            }
        }
    }

    // 按方案放置
    if (place_type == 1) {
        qDebug() << "place type 1";
        qDebug() << "num_h_x" << num_h_x;
        qDebug() << "num_h_y" << num_h_y;
        qDebug() << "num_v_x" << num_v_x;
        qDebug() << "num_v_y" << num_v_y;
        if (num_v_x == 1) {
            return layoutResult;
        }
        // 横向居中放置 纵向以横向为宽分开放置
        // 纵向放置是否居中或偏移
        if (num_h_x > 1) {
            // 横向放置的x起始、终止坐标
            double x_init = (base_lenght - (num_h_x * BoxMember.length)) / 2;
            double x_end = x_init + num_h_x * BoxMember.length;
            // 偏移
            double x_remain = (num_h_x * BoxMember.length - num_v_x * BoxMember.width) / (num_v_x - 1);
            //double y_remain = (base_width + overLift * 2 - num_h_y * BoxMember.width - num_v_y * BoxMember.length) / (num_h_y + num_v_y - 1);
            double y_remain = 0;
            // 放置水平箱子
            //double y_init = -overLift;
            double y_init = (base_width - (num_h_y * BoxMember.width + num_v_y * BoxMember.length)) / 2;
            for (int i_x = 0; i_x < num_h_x; i_x++) {
                for (int i_y = 0; i_y < num_h_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.length + 0), y_init + i_y * (BoxMember.width + y_remain),(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
            // 放置垂直箱子
            y_init = y_init + (BoxMember.width + y_remain) * num_h_y;
            for (int i_x = 0; i_x < num_v_x; i_x++) {
                for (int i_y = 0; i_y < num_v_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.width + x_remain), y_init + i_y * (BoxMember.length + y_remain),(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
        }
        else {
            // 居中
            double x_remain = 0;
            //double y_remain = (base_width + overLift * 2 - num_h_y * BoxMember.width - num_v_y * BoxMember.length) / (num_h_y + num_v_y - 1);
            double y_remain = 0;
            // 放置水平箱子
            double x_init = (base_lenght - (num_h_x * BoxMember.length)) / 2;
            //double y_init = -overLift;
            double y_init = (base_width - (num_h_y * BoxMember.width + num_v_y * BoxMember.length)) / 2;
            for (int i_x = 0; i_x < num_h_x; i_x++) {
                for (int i_y = 0; i_y < num_h_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.length + x_remain), y_init + i_y * (BoxMember.width + y_remain),(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
            // 放置垂直箱子
            x_init = (base_lenght - (num_v_x * BoxMember.width)) / 2;
            y_init = y_init + (BoxMember.width + y_remain) * num_h_y;
            for (int i_x = 0; i_x < num_v_x; i_x++) {
                for (int i_y = 0; i_y < num_v_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.width + x_remain), y_init + i_y * (BoxMember.length + y_remain),(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
        }
    }
    else if (place_type == 2) {
        qDebug() << "place type 2";
        qDebug() << "num_v_x" << num_v_x;
        qDebug() << "num_v_y" << num_v_y;
        qDebug() << "num_h_x" << num_h_x;
        qDebug() << "num_h_y" << num_h_y;
        if (num_h_y == 1) {
            return layoutResult;
        }
        // 纵向居中放置 横向以纵向为宽分开放置
        // 横向放置是否居中或偏移
        if (num_v_y > 1) {
            // 偏移放置
            double y_init = (base_width - (num_v_y * BoxMember.length)) / 2;
            // 计算偏移量
            double y_remain = (num_v_y * BoxMember.length - num_h_y * BoxMember.width) / (num_h_y - 1);
            // double x_remain = (base_lenght + overLift * 2 - num_v_x * BoxMember.width - num_h_x * BoxMember.length) / (num_v_x + num_h_x - 1);
            double x_remain = 0;
            // 放置水平箱子
            //double x_init = -overLift;
            double x_init = (base_lenght - num_v_x * BoxMember.width - num_h_x * BoxMember.length) / 2;
            for (int i_x = 0; i_x < num_h_x; i_x++) {
                for (int i_y = 0; i_y < num_h_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.length + x_remain), y_init + i_y * (BoxMember.width + y_remain),(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
            // 放置垂直箱子
            x_init = x_init + num_h_x * (BoxMember.length + x_remain);
            for (int i_x = 0; i_x < num_v_x; i_x++) {
                for (int i_y = 0; i_y < num_v_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.width + x_remain), y_init + i_y * (BoxMember.length + 0),(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }

        }
        else {
            // 居中放置
            // 计算偏移量
            double y_remain = 0;
            //double x_remain = (base_lenght + overLift * 2 - num_v_x * BoxMember.width - num_h_x * BoxMember.length) / (num_v_x + num_h_x - 1);
            double x_remain = 0;
            // 放置水平箱子
            double y_init = (base_width - (num_h_y * BoxMember.width)) / 2;
            //double x_init = -overLift;
            double x_init = (base_lenght - num_v_x * BoxMember.width - num_h_x * BoxMember.length) / 2;
            for (int i_x = 0; i_x < num_h_x; i_x++) {
                for (int i_y = 0; i_y < num_h_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.length + x_remain), y_init + i_y * (BoxMember.width + y_remain),(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
            // 放置垂直箱子
            y_init = (base_width - (num_v_y * BoxMember.length)) / 2;
            x_init = x_init + (BoxMember.length + x_remain) * num_h_x;
            for (int i_x = 0; i_x < num_v_x; i_x++) {
                for (int i_y = 0; i_y < num_v_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.width + x_remain), y_init + i_y * (BoxMember.length + 0),(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
        }
    }
    else if (place_type == 3) {
        qDebug() << "place type 3";
        qDebug() << "num_h_x" << num_h_x;
        qDebug() << "num_h_y" << num_h_y;
        qDebug() << "num_v_x" << num_v_x;
        qDebug() << "num_v_y" << num_v_y;
        if (num_v_x <= 1) {
            return layoutResult;
        }
        // 先排列横向、再排列纵向
        // 横向只有一个时，居中放置横向，纵向紧凑排列
        if (num_h_x == 1) {
            // 横向放置的x起始、y起始坐标
            double x_init = (base_lenght - BoxMember.length) / 2;
            double y_init = (base_width - num_v_y * BoxMember.length - num_h_y * BoxMember.width) / 2;
            for (int i_x = 0; i_x < num_h_x; i_x++) {
                for (int i_y = 0; i_y < num_h_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.length + 0), y_init + i_y * (BoxMember.width),(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
            // 纵向放置的x起始、y起始坐标
            x_init = (base_lenght - (num_v_x * BoxMember.width)) / 2;
            y_init = y_init + num_h_y * BoxMember.width;
            // 放置垂直箱子
            for (int i_x = 0; i_x < num_v_x; i_x++) {
                for (int i_y = 0; i_y < num_v_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.width), y_init + i_y * (BoxMember.length),(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
        }
        // 横向两端对齐，纵向紧凑排列
        else {
            // 横向放置的x起始、y起始坐标
            double x_init = (base_lenght - (num_v_x * BoxMember.width)) / 2;
            double y_init = (base_width - num_v_y * BoxMember.length - num_h_y * BoxMember.width) / 2;
            double x_remain = (num_v_x * BoxMember.width - num_h_x * BoxMember.length) / (num_h_x - 1);
            //放置水平箱子
            for (int i_x = 0; i_x < num_h_x; i_x++) {
                for (int i_y = 0; i_y < num_h_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.length + x_remain), y_init + i_y * (BoxMember.width),(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
            // 放置垂直箱子
            y_init = y_init + num_h_y * BoxMember.width;
            for (int i_x = 0; i_x < num_v_x; i_x++) {
                for (int i_y = 0; i_y < num_v_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.width), y_init + i_y * (BoxMember.length),(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }

        }
    }
    else if (place_type == 4) {
        qDebug() << "place type 4";
        qDebug() << "num_v_x" << num_v_x;
        qDebug() << "num_v_y" << num_v_y;
        qDebug() << "num_h_x" << num_h_x;
        qDebug() << "num_h_y" << num_h_y;
        if (num_h_y <= 1) {
            return layoutResult;
        }
        // 先排列纵向 再排列横向
        // 纵向一个时，居中放置
        if (num_v_y == 1) {
            // 放置水平箱子
            double x_init = (base_lenght - num_v_x * BoxMember.width - num_h_x * BoxMember.length) / 2;
            double y_init = (base_width - (num_h_y * BoxMember.width)) / 2;
            for (int i_x = 0; i_x < num_h_x; i_x++) {
                for (int i_y = 0; i_y < num_h_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.length + 0), y_init + i_y * (BoxMember.width + 0),(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
            // 放置垂直箱子
            x_init = x_init + num_h_x * BoxMember.length;
            y_init = (base_width - BoxMember.length) / 2;
            double y_remain = 0;
            //double x_remain = (base_lenght + overLift * 2 - num_v_x * BoxMember.width - num_h_x * BoxMember.length) / (num_v_x + num_h_x - 1);
            double x_remain = 0;
            // 放置垂直箱子
            for (int i_x = 0; i_x < num_v_x; i_x++) {
                for (int i_y = 0; i_y < num_v_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.width + x_remain), y_init + i_y * (BoxMember.length + y_remain),(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
        }
        else {
            // 放置水平箱子
            double x_init = (base_lenght - num_v_x * BoxMember.width - num_h_x * BoxMember.length) / 2;
            double y_init = (base_width - (num_h_y * BoxMember.width)) / 2;
            for (int i_x = 0; i_x < num_h_x; i_x++) {
                for (int i_y = 0; i_y < num_h_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.length + 0), y_init + i_y * (BoxMember.width + 0),(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
            // 放置垂直箱子
            x_init = x_init + num_h_x * BoxMember.length;
            double y_remain = (num_h_y * BoxMember.width - num_v_y * BoxMember.length) / (num_v_y - 1);
            //double x_remain = (base_lenght + overLift * 2 - num_v_x * BoxMember.width - num_h_x * BoxMember.length) / (num_v_x + num_h_x - 1);
            double x_remain = 0;
            // 放置垂直箱子
            for (int i_x = 0; i_x < num_v_x; i_x++) {
                for (int i_y = 0; i_y < num_v_y; i_y++) {
                    //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
                    layoutResult.append({ x_init + i_x * (BoxMember.width + x_remain), y_init + i_y * (BoxMember.length + y_remain),(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
                    if (layoutResult.size() >= box_num) {
                        return layoutResult;
                    }
                }
            }
        }
    }
    else {
        qDebug() << "place type error";
        return layoutResult;
    }
    return layoutResult;
}

QVector<SingleSKUBpp::LayoutResult> SingleSKUBpp::SingleRectangularLayout(const double base_lenght, const double base_width, const int level_num, const int box_num) {
    // 实现矩形布局逻辑，可以使用 overLiftNum
    QVector<LayoutResult> layoutResult;
    // 以短边作为越界标准
    double availableEdge = base_width + overLiftBase;

    // 最多放置数量
    // 长边可接纳短边数量n
    int num_temp_1 = BoxMember.length / BoxMember.width;

    // 回字形放置 方式按len + n * wid 求 n
    int n_wid = (availableEdge - BoxMember.length) / BoxMember.width;

    // 以小值作为放置数量依据
    int place_num;
    if (num_temp_1 < n_wid) { // 完全按回字形放置
        qDebug() << "place by box";
        place_num = num_temp_1;
    }
    else { // 以托盘边界作为回字变成放置
        qDebug() << "place by tray";
        place_num = n_wid;
    }

    // 回字形左下角
    double x_init = 0;
    double y_init = 0;
    qDebug() << "x_init" << x_init;
    qDebug() << "y_init" << y_init;
    for (int i_x = 0; i_x < 1; i_x++) {
        for (int i_y = 0; i_y < place_num; i_y++) {
            //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
            layoutResult.append({ x_init + i_x * BoxMember.length, y_init + i_y * BoxMember.width,(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
            if (layoutResult.size() >= box_num) {
                return layoutResult;
            }
        }
    }
    // 回字形右下角
    x_init = BoxMember.length;
    y_init = 0;
    qDebug() << "x_init" << x_init;
    qDebug() << "y_init" << y_init;
    for (int i_x = 0; i_x < place_num; i_x++) {
        for (int i_y = 0; i_y < 1; i_y++) {
            //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
            layoutResult.append({ x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length,(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
            if (layoutResult.size() >= box_num) {
                return layoutResult;
            }
        }
    }
    // 回字形左上角
    x_init = 0;
    y_init = place_num * BoxMember.width;
    qDebug() << "x_init" << x_init;
    qDebug() << "y_init" << y_init;
    for (int i_x = 0; i_x < place_num; i_x++) {
        for (int i_y = 0; i_y < 1; i_y++) {
            //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
            layoutResult.append({ x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length,(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
            if (layoutResult.size() >= box_num) {
                return layoutResult;
            }
        }
    }
    // 回字形右上角
    x_init = place_num * BoxMember.width;
    y_init = BoxMember.length;
    qDebug() << "x_init" << x_init;
    qDebug() << "y_init" << y_init;
    for (int i_x = 0; i_x < 1; i_x++) {
        for (int i_y = 0; i_y < place_num; i_y++) {
            //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
            layoutResult.append({ x_init + i_x * BoxMember.length, y_init + i_y * BoxMember.width,(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
            if (layoutResult.size() >= box_num) {
                return layoutResult;
            }
        }
    }

    return layoutResult;
}

QVector<SingleSKUBpp::LayoutResult> SingleSKUBpp::MutiRectangularLayout(const double base_lenght, const double base_width, const int level_num, const int box_num) {
    // 实现矩形布局逻辑，可以使用 overLiftNum
    QVector<LayoutResult> layoutResult;

    // 短边越界标准 计算摆放个数
    double availableEdge_width = base_width + overLiftBase;
    // 计算矩形长边最大放置数进行遍历
    int max_len_num_width = availableEdge_width / BoxMember.length;
    // 定义短边可放置的 n1_length, m1_width, max_box_num1
    int n1_length = 0, m1_width = 0, max_box_num1 = 0;
    // 遍历短边下最大放置n1_length和m1_width
    for (int i_length = 1; i_length <= max_len_num_width; i_length++) {
        // 以长边为准的最大短边数
        int max_width_num_1 = i_length * BoxMember.length / BoxMember.width;
        // 以托盘为准的最大短边数
        int max_width_num_2 = (availableEdge_width - i_length * BoxMember.length) / BoxMember.width;
        // 以较小值为准
        int max_width_num = max_width_num_1;
        if (max_width_num_1 > max_width_num_2) {
            max_width_num = max_width_num_2;
        }
        int num_temp = i_length * max_width_num * 4;
        if (num_temp > max_box_num1) {
            max_box_num1 = num_temp;
            n1_length = i_length;
            m1_width = max_width_num;
        }
    }

    // 长边越界标准 计算摆放个数
    double availableEdge_length = base_lenght + overLiftBase;
    // 计算矩形长边最大放置数进行遍历
    int max_len_num_length = availableEdge_length / BoxMember.length;
    // 定义短边可放置的 n1_length, m1_width, max_box_num1
    int n2_length = 0, m2_width = 0, max_box_num2 = 0;
    // 遍历短边下最大放置n1_length和m1_width
    for (int i_length = 1; i_length <= max_len_num_length; i_length++) {
        // 以长边为准的最大短边数
        int max_width_num_1 = i_length * BoxMember.length / BoxMember.width;
        // 以托盘为准的最大短边数
        int max_width_num_2 = (availableEdge_length - i_length * BoxMember.length) / BoxMember.width;
        // 以较小值为准
        int max_width_num = max_width_num_1;
        if (max_width_num_1 > max_width_num_2) {
            max_width_num = max_width_num_2;
        }
        int num_temp = i_length * max_width_num * 4;
        if (num_temp > max_box_num2) {
            max_box_num2 = num_temp;
            n2_length = i_length;
            m2_width = max_width_num;
        }
    }
    // 回字形宽信息
    qDebug() << "n1_length" << n1_length;
    qDebug() << "m1_width" << m1_width;
    qDebug() << "max_box_num1" << max_box_num1;
    // 回字形长信息
    qDebug() << "n2_length" << n2_length;
    qDebug() << "m2_width" << m2_width;
    qDebug() << "max_box_num2" << max_box_num2;

    // 计算回字形 长宽
    double rec_wid = n1_length * BoxMember.length + m1_width * BoxMember.width;
    double rec_len = n2_length * BoxMember.length + m2_width * BoxMember.width;
    // 计算两个方向的偏移量
    double x_remain = (TrayMember.length - rec_len) / 2;
    double y_remain = (TrayMember.width - rec_wid) / 2;
    qDebug() << "x_remain" << x_remain;
    qDebug() << "y_remain" << y_remain;

    // 回字形左下角
    double x_init = 0 + x_remain;
    double y_init = 0 + y_remain;
    qDebug() << "x_init" << x_init;
    qDebug() << "y_init" << y_init;
    for (int i_x = 0; i_x < n2_length; i_x++) {
        for (int i_y = 0; i_y < m1_width; i_y++) {
            //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
            layoutResult.append({ x_init + i_x * BoxMember.length, y_init + i_y * BoxMember.width,(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
            if (layoutResult.size() >= box_num) {
                return layoutResult;
            }
        }
    }
    // 回字形右下角
    x_init = BoxMember.length * n2_length + x_remain;
    y_init = 0 + y_remain;
    qDebug() << "x_init" << x_init;
    qDebug() << "y_init" << y_init;
    for (int i_x = 0; i_x < m2_width; i_x++) {
        for (int i_y = 0; i_y < n1_length; i_y++) {
            //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
            layoutResult.append({ x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length,(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
            if (layoutResult.size() >= box_num) {
                return layoutResult;
            }
        }
    }
    // 回字形左上角
    x_init = 0 + x_remain;
    y_init = m1_width * BoxMember.width + y_remain;
    qDebug() << "x_init" << x_init;
    qDebug() << "y_init" << y_init;
    for (int i_x = 0; i_x < m2_width; i_x++) {
        for (int i_y = 0; i_y < n1_length; i_y++) {
            //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
            layoutResult.append({ x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length,(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
            if (layoutResult.size() >= box_num) {
                return layoutResult;
            }
        }
    }
    // 回字形右上角
    x_init = m2_width * BoxMember.width + x_remain;
    y_init = BoxMember.length * n1_length + y_remain;
    qDebug() << "x_init" << x_init;
    qDebug() << "y_init" << y_init;
    for (int i_x = 0; i_x < n2_length; i_x++) {
        for (int i_y = 0; i_y < m1_width; i_y++) {
            //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
            layoutResult.append({ x_init + i_x * BoxMember.length, y_init + i_y * BoxMember.width,(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
            if (layoutResult.size() >= box_num) {
                return layoutResult;
            }
        }
    }

    return layoutResult;
}

QVector<SingleSKUBpp::LayoutResult> SingleSKUBpp::MutiRectangularLayoutMirror(const double base_lenght, const double base_width, const int level_num, const int box_num) {
    // 实现矩形布局逻辑，可以使用 overLiftNum
    QVector<LayoutResult> layoutResult;

    // 短边越界标准 计算摆放个数
    double availableEdge_width = base_width + overLiftBase;
    // 计算矩形长边最大放置数进行遍历
    int max_len_num_width = availableEdge_width / BoxMember.length;
    // 定义短边可放置的 n1_length, m1_width, max_box_num1
    int n1_length = 0, m1_width = 0, max_box_num1 = 0;
    // 遍历短边下最大放置n1_length和m1_width
    for (int i_length = 1; i_length <= max_len_num_width; i_length++) {
        // 以长边为准的最大短边数
        int max_width_num_1 = i_length * BoxMember.length / BoxMember.width;
        // 以托盘为准的最大短边数
        int max_width_num_2 = (availableEdge_width - i_length * BoxMember.length) / BoxMember.width;
        // 以较小值为准
        int max_width_num = max_width_num_1;
        if (max_width_num_1 > max_width_num_2) {
            max_width_num = max_width_num_2;
        }
        int num_temp = i_length * max_width_num * 4;
        if (num_temp > max_box_num1) {
            max_box_num1 = num_temp;
            n1_length = i_length;
            m1_width = max_width_num;
        }
    }

    // 长边越界标准 计算摆放个数
    double availableEdge_length = base_lenght + overLiftBase;
    // 计算矩形长边最大放置数进行遍历
    int max_len_num_length = availableEdge_length / BoxMember.length;
    // 定义短边可放置的 n1_length, m1_width, max_box_num1
    int n2_length = 0, m2_width = 0, max_box_num2 = 0;
    // 遍历短边下最大放置n1_length和m1_width
    for (int i_length = 1; i_length <= max_len_num_length; i_length++) {
        // 以长边为准的最大短边数
        int max_width_num_1 = i_length * BoxMember.length / BoxMember.width;
        // 以托盘为准的最大短边数
        int max_width_num_2 = (availableEdge_length - i_length * BoxMember.length) / BoxMember.width;
        // 以较小值为准
        int max_width_num = max_width_num_1;
        if (max_width_num_1 > max_width_num_2) {
            max_width_num = max_width_num_2;
        }
        int num_temp = i_length * max_width_num * 4;
        if (num_temp > max_box_num2) {
            max_box_num2 = num_temp;
            n2_length = i_length;
            m2_width = max_width_num;
        }
    }
    // 回字形宽信息
    qDebug() << "n1_length" << n1_length;
    qDebug() << "m1_width" << m1_width;
    qDebug() << "max_box_num1" << max_box_num1;
    // 回字形长信息
    qDebug() << "n2_length" << n2_length;
    qDebug() << "m2_width" << m2_width;
    qDebug() << "max_box_num2" << max_box_num2;

    // 计算回字形 长宽
    double rec_wid = n1_length * BoxMember.length + m1_width * BoxMember.width;
    double rec_len = n2_length * BoxMember.length + m2_width * BoxMember.width;
    // 计算两个方向的偏移量
    double x_remain = (TrayMember.length - rec_len) / 2;
    double y_remain = (TrayMember.width - rec_wid) / 2;
    qDebug() << "x_remain" << x_remain;
    qDebug() << "y_remain" << y_remain;

    // 回字形左下角
    double x_init = 0 + x_remain;
    double y_init = 0 + y_remain;
    qDebug() << "x_init" << x_init;
    qDebug() << "y_init" << y_init;
    for (int i_x = 0; i_x < m2_width; i_x++) {
        for (int i_y = 0; i_y < n1_length; i_y++) {
            //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
            layoutResult.append({ x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length,(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
            if (layoutResult.size() >= box_num) {
                return layoutResult;
            }
        }
    }
    // 回字形右下角
    x_init = m2_width * BoxMember.width + x_remain;
    y_init = 0 + y_remain;
    qDebug() << "x_init" << x_init;
    qDebug() << "y_init" << y_init;
    for (int i_x = 0; i_x < n2_length; i_x++) {
        for (int i_y = 0; i_y < m1_width; i_y++) {
            //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
            layoutResult.append({ x_init + i_x * BoxMember.length, y_init + i_y * BoxMember.width,(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
            if (layoutResult.size() >= box_num) {
                return layoutResult;
            }
        }
    }
    // 回字形左上角
    x_init = 0 + x_remain;
    y_init = BoxMember.length * n1_length + y_remain;
    qDebug() << "x_init" << x_init;
    qDebug() << "y_init" << y_init;
    for (int i_x = 0; i_x < n2_length; i_x++) {
        for (int i_y = 0; i_y < m1_width; i_y++) {
            //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
            layoutResult.append({ x_init + i_x * BoxMember.length, y_init + i_y * BoxMember.width,(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
            if (layoutResult.size() >= box_num) {
                return layoutResult;
            }
        }
    }
    // 回字形右上角
    x_init = BoxMember.length * n2_length + x_remain;
    y_init = m1_width * BoxMember.width + y_remain;
    qDebug() << "x_init" << x_init;
    qDebug() << "y_init" << y_init;
    for (int i_x = 0; i_x < m2_width; i_x++) {
        for (int i_y = 0; i_y < n1_length; i_y++) {
            //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length));
            layoutResult.append({ x_init + i_x * BoxMember.width, y_init + i_y * BoxMember.length,(level_num - 1) * BoxMember.height, BoxMember.width, BoxMember.length });
            if (layoutResult.size() >= box_num) {
                return layoutResult;
            }
        }
    }

    return layoutResult;
}

QVector<SingleSKUBpp::LayoutResult> SingleSKUBpp::OverHorizontalLayout(const double base_lenght, const double base_width, const int level_num, const int box_num) {
    QVector<LayoutResult> layoutResult;
    qDebug() << "enter";

    //设置居中放置时x和y方向上需要平移的移动量
    double x_remain = 0, y_remain = 0;

    //if (BoxMember.length <= base_lenght) {
    //	qDebug() << "box length is too short.";
    //	return layoutResult;
    //}

    double availableLength = base_lenght + overLiftBase * 2;
    double availableWidth = base_width + overLiftBase * 2;
    qDebug() << "availableLength" << availableLength;
    qDebug() << "availableWidth" << availableWidth;

    int x_num = 1;
    int y_num = availableWidth / BoxMember.width;
    qDebug() << "x_num" << x_num;
    qDebug() << "y_num" << y_num;

    // 计算y方向偏移量
    //y_remain = (base_width - BoxMember.width * y_num) / (y_num - 1);

    // 优先居中放置
    // 计算起始位置x和y
    double x_init = (base_lenght - (x_num * BoxMember.length)) / 2;
    double y_init = (base_width - (y_num * BoxMember.width)) / 2;
    qDebug() << "x_init" << x_init;
    qDebug() << "y_init" << y_init;

    for (int i_y = 0; i_y < y_num; i_y++) {
        for (int i_x = 0; i_x < x_num; i_x++) {
            //layoutResult.append(QPair<double, double>(x_init + i_x * BoxMember.length, y_init + i_y * BoxMember.width));
            layoutResult.append({ x_init + i_x * (BoxMember.length + x_remain), y_init + i_y * (BoxMember.width + y_remain),(level_num - 1) * BoxMember.height, BoxMember.length, BoxMember.width });
            if (layoutResult.size() >= box_num) {
                return layoutResult;
            }
        }
    }
    return layoutResult;
}

QVector<SingleSKUBpp::LayoutResult> SingleSKUBpp::LayoutTotal(Layout layout_method)
{
    // 最大可放置层数valid_level
    QVector<LayoutResult> layout_total;
    // 不同交替放置下
    qDebug() << "valid_level" << valid_level;
    if (layout_method == horizontal_vertical) {
        for (size_t i = 0; i < valid_level; i++) {
            // 奇数层横向放置 偶数层纵向放置
            if (i % 2 == 1) {
                layout_total.append(HorizontalLayout(TrayMember.length, TrayMember.width, i + 1));
            }
            else {
                layout_total.append(VerticalLayout(TrayMember.length, TrayMember.width, i + 1));
            }
        }
    }
    else if (layout_method == cross_crossmirror) {
        for (size_t i = 0; i < valid_level; i++) {
            // 奇数层横向放置 偶数层纵向放置
            if (i % 2 == 1) {
                layout_total.append(CrossLayout(TrayMember.length, TrayMember.width, i + 1));
            }
            else {
                layout_total.append(CrossLayoutMirro(TrayMember.length, TrayMember.width, i + 1));
            }
        }
    }
    else if (layout_method == rectangular_rectangularmirror) {
        for (size_t i = 0; i < valid_level; i++) {
            // 奇数层横向放置 偶数层纵向放置
            if (i % 2 == 1) {
                layout_total.append(MutiRectangularLayout(TrayMember.length, TrayMember.width, i + 1));
            }
            else {
                layout_total.append(MutiRectangularLayoutMirror(TrayMember.length, TrayMember.width, i + 1));
            }
        }
    }
    // 其他情况统一按回字形放置
    else {
        for (size_t i = 0; i < valid_level; i++) {
            // 奇数层横向放置 偶数层纵向放置
            if (i % 2 == 1) {
                layout_total.append(HorizontalLayout(TrayMember.length, TrayMember.width, i + 1));
            }
            else {
                layout_total.append(VerticalLayout(TrayMember.length, TrayMember.width, i + 1));
            }
        }
    }
    return layout_total;
}


bool SingleSKUBpp::doOverlap(Rectangle rect1, Rectangle rect2, QVector<int>& directions)
{
    // 计算两个矩形的右上角坐标
    Point rect1TopRight = { rect1.bottomLeft.x + rect1.width, rect1.bottomLeft.y + rect1.height };
    Point rect2TopRight = { rect2.bottomLeft.x + rect2.width, rect2.bottomLeft.y + rect2.height };

    // 判断两个矩形是否有边与边重叠
    int inside_temp = 0;
    int corner_temp = isPointInsideRectangleCorner(rect2, rect1);
    if (isPointInsideRectangle(rect2.bottomLeft, rect1)) {
        inside_temp++;
    }
    if (isPointInsideRectangle({ rect2.bottomLeft.x + rect2.width, rect2.bottomLeft.y }, rect1)) {
        inside_temp++;
    }
    if (isPointInsideRectangle({ rect2.bottomLeft.x, rect2.bottomLeft.y + rect2.height }, rect1)) {
        inside_temp++;
    }
    if (isPointInsideRectangle({ rect2.bottomLeft.x + rect2.width, rect2.bottomLeft.y + rect2.height }, rect1)) {
        inside_temp++;
    }

    if (corner_temp == 1 || inside_temp == 0) {
        return false;
    }
    if (rect1TopRight.x <= rect2.bottomLeft.x)
        directions[LEFT]++;
    else if (rect1.bottomLeft.x >= rect2TopRight.x)
        directions[RIGHT]++;
    else if (rect1TopRight.y <= rect2.bottomLeft.y)
        directions[BELOW]++;
    else if (rect1.bottomLeft.y >= rect2TopRight.y)
        directions[ABOVE]++;
    return true;
}


QVector<SingleSKUBpp::LayoutResult> SingleSKUBpp::LayoutTotalAuto()
{
    QElapsedTimer timer;
    timer.start();

    QVector<LayoutResult> layout_total;
    //QVector<LayoutResultTotal> layout_total_ori;
    // 新码垛开始时将layout_total_ori置为空
    layout_total_ori.clear();
    qDebug() << "enter LayoutTotalAuto";

    // 受重量限制的最大可放置数量
    int max_box_num_wieght = TrayMember.max_weight / BoxMember.weight;

    int bp_type = none_type;
    int single_level_num = 0;

    // 判断是否为扁平件（只需要单纯横纵放置）
    bool if_flat = false;

    // 加入扁平件逻辑
    //if () {
    //	if_flat = true;
    //}

    // 如果长边超托 则每层均按超托放置
    // 横纵交替模式下两层数量可能不同
    auto layout_level_single_HorCrossVerLayout_first = HorCrossVerLayout(TrayMember.length, TrayMember.width, 1).size();
    auto layout_level_single_HorCrossVerLayout_second = HorCrossVerLayout(TrayMember.length, TrayMember.width, 2).size();
    if (BoxMember.length > TrayMember.width + overLiftBase * 2) {
        auto layout_level_single = OverHorizontalLayout(TrayMember.length, TrayMember.width, 1);
        auto level_single_num = layout_level_single.size();
        qDebug() << "level_single_num" << level_single_num;
        bp_type = overhorizontal_type;
        single_level_num = level_single_num;
    }
    else {
        // 计算回字模式下可摆放个数
        auto layout_level_single_MutiRectangularLayout = MutiRectangularLayout(TrayMember.length, TrayMember.width, 1);
        auto level_single_num_MutiRectangularLayout = layout_level_single_MutiRectangularLayout.size();
        if (level_single_num_MutiRectangularLayout > single_level_num && !if_flat) {
            qDebug() << "enter MutiRectangularLayout";
            bp_type = rectangular_type;
            single_level_num = level_single_num_MutiRectangularLayout;
        }

        // 计算横纵模式下可摆放个数
        auto layout_level_single_CrossLayout = CrossLayout(TrayMember.length, TrayMember.width, 1);
        auto level_single_num_CrossLayout = layout_level_single_CrossLayout.size();
        if (level_single_num_CrossLayout > single_level_num && !if_flat) {
            qDebug() << "enter CrossLayout";
            bp_type = cross_type;
            single_level_num = level_single_num_CrossLayout;
        }

        // 计算横纵交替模式下可摆放个数
        auto level_single_num_HorCrossVerLayout = (layout_level_single_HorCrossVerLayout_first + layout_level_single_HorCrossVerLayout_second) / 2;
        if (level_single_num_HorCrossVerLayout > single_level_num && !if_flat) {
            qDebug() << "enter HorCrossVerLayout";
            bp_type = hor_ver_type;
            single_level_num = level_single_num_HorCrossVerLayout;
        }

        // 计算横向模式下可摆放个数
        auto layout_level_single_HorizontalLayout = HorizontalLayout(TrayMember.length, TrayMember.width, 1);
        auto level_single_num_HorizontalLayout = layout_level_single_HorizontalLayout.size();
        if (bp_type == none_type && level_single_num_HorizontalLayout > single_level_num) {
            qDebug() << "enter HorizontalLayout";
            bp_type = horizontal_type;
            single_level_num = level_single_num_HorizontalLayout;
        }
        // 计算纵向模式下可摆放个数
        auto layout_level_single_VerticalLayout = VerticalLayout(TrayMember.length, TrayMember.width, 1);
        auto level_single_num_VerticalLayout = layout_level_single_VerticalLayout.size();
        if ((bp_type == horizontal_type || bp_type == none_type) && level_single_num_VerticalLayout > single_level_num) {
            qDebug() << "enter VerticalLayout";
            bp_type = vertical_type;
            single_level_num = level_single_num_VerticalLayout;
        }
        qDebug() << "enter single_level_num" << single_level_num;

        qDebug() << "level_single_num_MutiRectangularLayout" << level_single_num_MutiRectangularLayout;
        qDebug() << "level_single_num_CrossLayout" << level_single_num_CrossLayout;
        qDebug() << "level_single_num_HorizontalLayout" << level_single_num_HorizontalLayout;
        qDebug() << "level_single_num_VerticalLayout" << level_single_num_VerticalLayout;
    }

    qDebug() << "bp_type====================================" << bp_type;
    if (bp_type == none_type) {
        qDebug() << "type none";
        return layout_total;
    }
    else if (bp_type == overhorizontal_type) {
        for (int i = 0; i < valid_level; i++) {
            max_box_num_wieght -= single_level_num;
            if (max_box_num_wieght < 0) {
                // 最后一层时数量按上一次留下来箱子数量摆放
                single_level_num = max_box_num_wieght + single_level_num;
            }
            layout_total.append(OverHorizontalLayout(TrayMember.length, TrayMember.width, i + 1, single_level_num));
            if (max_box_num_wieght <= 0) {
                // 超重停止摆放
                break;
            }
        }
    }
    else if (bp_type == horizontal_type) {
        for (int i = 0; i < valid_level; i++) {
            max_box_num_wieght -= single_level_num;
            if (max_box_num_wieght < 0) {
                // 最后一层时数量按上一次留下来箱子数量摆放
                single_level_num = max_box_num_wieght + single_level_num;
            }
            layout_total.append(HorizontalLayout(TrayMember.length, TrayMember.width, i + 1, single_level_num));
            if (max_box_num_wieght <= 0) {
                // 超重停止摆放
                break;
            }
        }

    }
    else if (bp_type == vertical_type) {
        for (int i = 0; i < valid_level; i++) {
            max_box_num_wieght -= single_level_num;
            if (max_box_num_wieght < 0) {
                // 最后一层时数量按上一次留下来箱子数量摆放
                single_level_num = max_box_num_wieght + single_level_num;
            }
            layout_total.append(VerticalLayout(TrayMember.length, TrayMember.width, i + 1, single_level_num));
            if (max_box_num_wieght <= 0) {
                // 超重停止摆放
                break;
            }
        }
    }
    else if (bp_type == cross_type) {
        for (size_t i = 0; i < valid_level; i++) {
            max_box_num_wieght -= single_level_num;
            if (max_box_num_wieght < 0) {
                // 最后一层时数量按上一次留下来箱子数量摆放
                single_level_num = max_box_num_wieght + single_level_num;
            }
            // 奇数层横向放置 偶数层纵向放置
            if (i % 2 == 1) {
                layout_total.append(CrossLayout(TrayMember.length, TrayMember.width, i + 1, single_level_num));
            }
            else {
                layout_total.append(CrossLayoutMirro(TrayMember.length, TrayMember.width, i + 1, single_level_num));
            }
            if (max_box_num_wieght <= 0) {
                // 超重停止摆放
                break;
            }
        }
    }
    else if (bp_type == rectangular_type) {
        for (size_t i = 0; i < valid_level; i++) {
            max_box_num_wieght -= single_level_num;
            if (max_box_num_wieght < 0) {
                // 最后一层时数量按上一次留下来箱子数量摆放
                single_level_num = max_box_num_wieght + single_level_num;
            }
            // 奇数层横向放置 偶数层纵向放置
            if (i % 2 == 1) {
                layout_total.append(MutiRectangularLayout(TrayMember.length, TrayMember.width, i + 1, single_level_num));
            }
            else {
                layout_total.append(MutiRectangularLayoutMirror(TrayMember.length, TrayMember.width, i + 1, single_level_num));
            }
            if (max_box_num_wieght <= 0) {

                // 超重停止摆放
                break;
            }
        }
    }
    else if (bp_type == hor_ver_type) {
        for (size_t i = 0; i < valid_level; i++) {
            if (i % 2 == 0) {
                single_level_num = layout_level_single_HorCrossVerLayout_first;
            }
            else {
                single_level_num = layout_level_single_HorCrossVerLayout_second;
            }
            max_box_num_wieght -= single_level_num;
            if (max_box_num_wieght < 0) {
                // 最后一层时数量按上一次留下来箱子数量摆放
                single_level_num = max_box_num_wieght + single_level_num;
            }
            layout_total.append(HorCrossVerLayout(TrayMember.length, TrayMember.width, i + 1, single_level_num));
            if (max_box_num_wieght <= 0) {
                // 超重停止摆放
                break;
            }
        }
    }
    else {
        qDebug() << "type error";
        return layout_total;
    }

    // 对layout_total进行遍历，同时遍历当前箱子前的箱子，分析当前箱子周围箱子情况并将结果append入layout_total_ori
    for (int i = 0; i < layout_total.size(); ++i) {
        const auto& result = layout_total[i];
        int box_ori_temp = 0;
        if (result.x_len == BoxMember.length && result.y_len == BoxMember.width) {
            // 正向放置
            box_ori_temp = 0;
        }
        else if (result.y_len == BoxMember.length && result.x_len == BoxMember.width) {
            // 旋转90°放置
            box_ori_temp = 90;
        }
        else {
            // 放置错误
            box_ori_temp = 0;
        }

        qDebug() << "now box=================== " << result.x << "," << result.y << "," << result.z;
        // 判断当前方块前方方块的方向方向数量
        QVector<int> directions(5, 0);
        int box_dir = 0;
        // 考虑前面的数据
        for (int j = 0; j < i; ++j) {
            const auto& previous_result = layout_total[j];
            // 考虑箱子是否在同一层，同一层则进行碰撞检测
            if (previous_result.z == result.z) {
                Rectangle rect_temp = { {previous_result.x, previous_result.y}, previous_result.x_len, previous_result.y_len }; // 矩形1：左下角坐标 + 宽度 + 高度
                Rectangle rect_obj = { {result.x, result.y}, result.x_len, result.y_len }; // 矩形2：左下角坐标 + 宽度 + 高度
                qDebug() << "rect_temp " << previous_result.x << "," << previous_result.y << "," << previous_result.z << "," << previous_result.x_len << "," << previous_result.y_len;
                qDebug() << "rect_obj " << result.x << "," << result.y << "," << result.z << "," << result.x_len << "," << result.y_len;
                qDebug() << "-------------------- ";
                doOverlap(rect_temp, rect_obj, directions);
            }
        }
        qDebug() << "Left: " << directions[LEFT];
        qDebug() << "Right: " << directions[RIGHT];
        qDebug() << "Above: " << directions[ABOVE];
        qDebug() << "Below: " << directions[BELOW];
        if (directions[LEFT] == 0 && directions[RIGHT] == 0 && directions[ABOVE] == 0 && directions[BELOW] == 0) {
            box_dir = ENTERVERTICAL;
        }
        else if (directions[LEFT] == 0 && directions[RIGHT] == 1 && directions[ABOVE] == 0 && directions[BELOW] == 0) {
            box_dir = ENTERLEFT;
        }
        else if (directions[LEFT] == 1 && directions[RIGHT] == 0 && directions[ABOVE] == 0 && directions[BELOW] == 0) {
            box_dir = ENTERRIGHT;
        }
        else if (directions[LEFT] == 0 && directions[RIGHT] == 0 && directions[ABOVE] == 0 && directions[BELOW] == 1) {
            box_dir = ENTERABOVE;
        }
        else if (directions[LEFT] == 0 && directions[RIGHT] == 0 && directions[ABOVE] == 1 && directions[BELOW] == 0) {
            box_dir = ENTERBELOW;
        }
        else if (directions[LEFT] == 0 && directions[RIGHT] == 1 && directions[ABOVE] == 0 && directions[BELOW] == 1) {
            box_dir = ENTERUPLEFT;
        }
        else if (directions[LEFT] == 1 && directions[RIGHT] == 0 && directions[ABOVE] == 0 && directions[BELOW] == 1) {
            box_dir = ENTERUPRIGHT;
        }
        else if (directions[LEFT] == 0 && directions[RIGHT] == 1 && directions[ABOVE] == 1 && directions[BELOW] == 0) {
            box_dir = ENTERDOWNLEFT;
        }
        else if (directions[LEFT] == 1 && directions[RIGHT] == 0 && directions[ABOVE] == 1 && directions[BELOW] == 0) {
            box_dir = ENTERDOWNRIGHT;
        }
        else {
            box_dir = ENTERVERTICAL;
        }
        layout_total_ori.append({ (result.x + result.x_len * 0.5),(result.y + result.y_len * 0.5), result.z ,box_ori_temp ,box_dir });
        // 给sql数据
        layout_total_sql.append({ int(result.z / BoxMember.height), (result.x + result.x_len * 0.5),(result.y + result.y_len * 0.5), result.z,box_ori_temp ,box_dir });
    }

    // 给sql托盘数据
    tray_sql_data.tray_length= TrayMember.length;
    tray_sql_data.tray_width= TrayMember.width;
    tray_sql_data.tray_MaxHeight = TrayMember.max_height;
    tray_sql_data.tray_MaxWeight = TrayMember.max_weight;
    tray_sql_data.tray_MaxNumber = TrayMember.Layout_max_num;
    tray_sql_data.box_length= BoxMember.length;
    tray_sql_data.box_width = BoxMember.width;
    tray_sql_data.box_height = BoxMember.height;
    tray_sql_data.box_weight = BoxMember.weight;

    qint64 elapsed = timer.elapsed();
    qDebug() << "auto bpp run time total：" << elapsed / 1000.0 << "s";
    return layout_total;
}
