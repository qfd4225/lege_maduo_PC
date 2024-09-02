#include "viewer_boxes_3d.h"
#include "ui_viewer_boxes_3d.h"
double stack_length = 12, stack_width = 10, height_limit = 18, goods_length = -1.0, goods_width = -1.0, goods_height = -1.0, out_ratio = 0, weight = -1.0, total_weight = -1.0;
int max_layers = -1;
QImage generateTextureImage(int number, const QSize& size)
{
	QImage image(size, QImage::Format_ARGB32);
	image.fill(Qt::transparent);

	QPainter painter(&image);
	painter.setPen(Qt::black);
	painter.setFont(QFont("Arial", size.height() / 4));
	painter.drawText(image.rect(), Qt::AlignCenter, QString::number(number));
	return image;
}

void ini_scene(Qt3DCore::QEntity* rootEntity, Qt3DExtras::Qt3DWindow* view)
{
	// Camera
	// 6、在显示3D图形的过程当中，摄像机是必不可少的，只有摄像机摆放的合适人眼才能看到3D建模的样子
	Qt3DRender::QCamera* cameraEntity = view->camera();
	cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	cameraEntity->setPosition(QVector3D(-20, 20, -20.0f));
	cameraEntity->setUpVector(QVector3D(0, 1, 0));
	cameraEntity->setViewCenter(QVector3D(6, 5, 20));

	Qt3DRender::QRenderStateSet* renderStateSet = new Qt3DRender::QRenderStateSet(rootEntity);
	Qt3DRender::QDepthTest* depthTest = new Qt3DRender::QDepthTest();
	renderStateSet->addRenderState(depthTest);

	// 摄像机控制配置（这里配置以第一人称视角控制场景摄影机）
	// For camera controls
	Qt3DExtras::QFirstPersonCameraController* camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
	camController->setCamera(cameraEntity);
	// 设置线性速度
	camController->setLinearSpeed(6.0f); // 设置线性速度为10（可以根据需要调整）
	camController->setLookSpeed(30.0f);
	// 设置滚轮缩放速度
	camController->setLookSpeed(100.0f); // 设置视角旋转速度为100（可以根据需要调整）
	// 7、在“画布”中加入子实体“光照light”
	Qt3DCore::QEntity* lightEntity = new Qt3DCore::QEntity(rootEntity);
	// 定义一种光源对象，这里定义的是“点光”，此外还有“定向光”以及“聚集光”可选
	Qt3DRender::QPointLight* light = new Qt3DRender::QPointLight(lightEntity);
	light->setColor("white");
	light->setIntensity(1);

	lightEntity->addComponent(light);
	// 定义光照实体的变换（设置光照显示位置等）
	Qt3DCore::QTransform* lightTransform = new Qt3DCore::QTransform(lightEntity);
	lightTransform->setTranslation(QVector3D(0, 20, 0));
	lightEntity->addComponent(lightTransform);

	// 创建第二个点光源实体
	Qt3DCore::QEntity* lightEntity2 = new Qt3DCore::QEntity(rootEntity);
	Qt3DRender::QPointLight* light2 = new Qt3DRender::QPointLight(lightEntity2);
	light2->setColor("white");
	light2->setIntensity(1);
	lightEntity2->addComponent(light2);
	Qt3DCore::QTransform* lightTransform2 = new Qt3DCore::QTransform(lightEntity2);
	lightTransform2->setTranslation(QVector3D(40, 0, 30)); // 设置点光源位置
	lightEntity2->addComponent(lightTransform2);

	// 创建第三个点光源实体
	Qt3DCore::QEntity* lightEntity3 = new Qt3DCore::QEntity(rootEntity);
	Qt3DRender::QPointLight* light3 = new Qt3DRender::QPointLight(lightEntity3);
	light3->setColor("white");
	light3->setIntensity(1);
	lightEntity3->addComponent(light3);
	Qt3DCore::QTransform* lightTransform3 = new Qt3DCore::QTransform(lightEntity3);
	lightTransform3->setTranslation(QVector3D(-20, 0, 40)); // 设置点光源位置
	lightEntity3->addComponent(lightTransform3);

	// 创建第四个点光源实体
	Qt3DCore::QEntity* lightEntity4 = new Qt3DCore::QEntity(rootEntity);
	Qt3DRender::QPointLight* light4 = new Qt3DRender::QPointLight(lightEntity4);
	light4->setColor("white");
	light4->setIntensity(1);
	lightEntity4->addComponent(light4);
	Qt3DCore::QTransform* lightTransform4 = new Qt3DCore::QTransform(lightEntity4);
	lightTransform4->setTranslation(QVector3D(40, 0, -20)); // 设置点光源位置
	lightEntity4->addComponent(lightTransform4);
	//创建第五个点光源实体
	/*
	Qt3DCore::QEntity *lightEntity5 = new Qt3DCore::QEntity(rootEntity);
	Qt3DRender::QPointLight *light5 = new Qt3DRender::QPointLight(lightEntity5);
	light5->setColor("white");
	light5->setIntensity(1);
	lightEntity5->addComponent(light5);
	Qt3DCore::QTransform *lightTransform5 = new Qt3DCore::QTransform(lightEntity5);
	lightTransform5->setTranslation(QVector3D(20, 0, 10)); // 设置点光源位置
	lightEntity5->addComponent(lightTransform5);*/
}

void createCube(Qt3DCore::QEntity* rootEntity, double length, double width, double height, double x, double y, double z, int number)
{
	// Cuboid shape data
	QColor boxColor = QColor::fromHsv((rand() * 10) % 359, 120, 120);

	if (y == -1)
	{
		boxColor = QColor::fromHsv(27, 120, 120);
	}
	/*
	else
	{
		if(boxcolor==27)
		{
			boxcolor++;
		}
		boxColor=QColor::fromHsv(boxcolor, 120, 120);
		boxcolor= (boxcolor+40) % 359;
	}*/

	Qt3DExtras::QCuboidMesh* cuboid = new Qt3DExtras::QCuboidMesh();
	cuboid->setXExtent(length); // 设置长
	cuboid->setYExtent(width);  // 设置宽
	cuboid->setZExtent(height); // 设置高

	double halfLength = length / 2.0f;
	double halfWidth = width / 2.0f;
	double halfHeight = height / 2.0f;
	double xPosition = x + halfLength;
	double yPosition = y + halfWidth;
	double zPosition = z + halfHeight;

	// CuboidMesh Transform
	Qt3DCore::QTransform* cuboidTransform = new Qt3DCore::QTransform();
	cuboidTransform->setTranslation(QVector3D(xPosition, yPosition, zPosition));

    qDebug() << "height_limit="<<height_limit;
	if (y == height_limit) {
		// Create semi-transparent material
		Qt3DExtras::QPhongMaterial* semiTransparentMaterial = new Qt3DExtras::QPhongMaterial();
		QColor semiTransparentColor = boxColor;
		semiTransparentColor.setAlpha(5000); // Set alpha value for transparency
		semiTransparentMaterial->setDiffuse(semiTransparentColor);

		// Create cube at height limit
		Qt3DCore::QEntity* heightLimitCube = new Qt3DCore::QEntity(rootEntity);
		heightLimitCube->addComponent(cuboid);
		heightLimitCube->addComponent(cuboidTransform);
		heightLimitCube->addComponent(semiTransparentMaterial);
	}
	else
	{
		Qt3DExtras::QPhongMaterial* cuboidMaterial = new Qt3DExtras::QPhongMaterial();
		cuboidMaterial->setShininess(0);
		cuboidMaterial->setDiffuse(boxColor);

		//Cuboid

		Qt3DCore::QEntity* m_cuboidEntity = new Qt3DCore::QEntity(rootEntity);
		m_cuboidEntity->addComponent(cuboid);
		m_cuboidEntity->addComponent(cuboidTransform);
		m_cuboidEntity->addComponent(cuboidMaterial);
	}
}

void createList(Qt3DCore::QEntity* rootEntity, QVector<SingleSKUBpp::LayoutResult> result, int selectlayers, double gweight, double height)
{
	for (const auto& pair : result)
    {
		if (selectlayers == 0 || pair.z <= height * (selectlayers - 1))
        {
            createCube(rootEntity, pair.x_len, height, pair.y_len, pair.x, pair.z, pair.y, total_weight / gweight + 1);
			if (pair.z / height > max_layers)
			{
				max_layers = pair.z / goods_height;
			}
		}
		total_weight += gweight;
    }
}

bool parameterCheck1()
{
	if (goods_length < goods_height || goods_width < goods_height) {
		QMessageBox::critical(nullptr, "Error", "Goods'height should smaller than their width or length.");
		return false;
	}
	if (stack_length == -1.0 || stack_width == -1.0 || height_limit == -1.0 || goods_length == -1.0 || goods_width == -1.0 || goods_height == -1.0 || out_ratio == -1.0) {
		QMessageBox::critical(nullptr, "Error", "Please fullfill all parameters then click this button.");
		return false;
	}
	if (goods_length > 16.5)
	{
		QMessageBox::critical(nullptr, "Error", "Goods'length out of limit.");
		return false;
	}
	if (goods_width > 9)
	{
		QMessageBox::critical(nullptr, "Error", "Goods'width out of limit.");
		return false;
	}
	if (goods_height > 7)
	{
		QMessageBox::critical(nullptr, "Error", "Goods'height out of limit.");
		return false;
	}
	if (goods_length < 2.25)
	{
		QMessageBox::critical(nullptr, "Error", "Goods'length less than the minimum limit.");
		return false;
	}
	if (goods_width < 1.25)
	{
		QMessageBox::critical(nullptr, "Error", "Goods'width less than the minimum limit.");
		return false;
	}
	if (goods_height < 0.1)
	{
		QMessageBox::critical(nullptr, "Error", "Goods'height less than the minimum limit.");
		return false;
	}
	if (weight > 66)
	{
		QMessageBox::critical(nullptr, "Error", "Goods'weight out of limit.");
		return false;
	}
	if (weight < 5)
	{
		QMessageBox::critical(nullptr, "Error", "Goods'weight less than the minimum limit.");
		return false;
	}
	total_weight = 0;
	return true;
}

void viewerBoxes3D::onReadButtonClicked(QWidget* container, Qt3DCore::QEntity* rootEntity, Qt3DExtras::Qt3DWindow* view)
{
	QString comboBoxText = ui->comboBox->currentText();
	QString filePath = QFileDialog::getOpenFileName(nullptr, "Select Excel File", "", "Excel Files (*.xlsx *.xls)");
	QAxObject excel("Excel.Application");
	if (excel.isNull()) {
		qDebug() << "Failed to create Excel application.";
		return;
	}

	excel.setProperty("Visible", false);  // 如果不想显示Excel界面，可以设为false
	QAxObject* workbooks = excel.querySubObject("Workbooks");
	if (workbooks == nullptr) {
		qDebug() << "Failed to get Workbooks.";
		return;
	}

	QAxObject* workbook = workbooks->querySubObject("Open(const QString&)", filePath);
	if (workbook == nullptr) {
		qDebug() << "Failed to open workbook.";
		return;
	}
	QFileInfo fileInfo(filePath);
	QString directory = fileInfo.absolutePath();

	// 获取当前日期和时间
	QDateTime currentDateTime = QDateTime::currentDateTime();
	QString folderName = currentDateTime.toString("yyyyMMdd_hhmmss");

	// 创建新文件夹
	QDir dir(directory);
	if (dir.mkdir(folderName)) {
		qDebug() << "Folder created:" << folderName;
	}
	else {
		qDebug() << "Failed to create folder:" << folderName;
	}

	// 假设我们读取第一个工作表
	QAxObject* worksheet = workbook->querySubObject("Worksheets(int)", 1);
	if (worksheet == nullptr) {
		qDebug() << "Failed to get worksheet.";
		return;
	}

	QAxObject* usedRange = worksheet->querySubObject("UsedRange");
	if (usedRange == nullptr) {
		qDebug() << "Failed to get used range.";
		return;
	}

	QAxObject* rows = usedRange->querySubObject("Rows");
	int rowCount = rows->property("Count").toInt();
	QAxObject* columns = usedRange->querySubObject("Columns");

	// 指定列号
	int lengthIndex = 3;  // 假设Length在第一列
	int widthIndex = 4;   // 假设Width在第二列
	int heightIndex = 5;  // 假设Height在第三列
	int weightIndex = 6;  // 假设Weight在第四列
	QVector<double> lengths, widths, heights, weights;
	// 遍历每一行并获取值
	for (int row = 2; row <= rowCount; ++row) {  // 从第二行开始遍历
		QAxObject* lengthCell = worksheet->querySubObject("Cells(int,int)", row, lengthIndex);
		QAxObject* widthCell = worksheet->querySubObject("Cells(int,int)", row, widthIndex);
		QAxObject* heightCell = worksheet->querySubObject("Cells(int,int)", row, heightIndex);
		QAxObject* weightCell = worksheet->querySubObject("Cells(int,int)", row, weightIndex);

		if (lengthCell == nullptr || widthCell == nullptr || heightCell == nullptr || weightCell == nullptr) {
			qDebug() << "Failed to get cell at row" << row;
			continue;
		}

		// 尝试使用 dynamicCall 代替 property
		QVariant lengthValue = lengthCell->dynamicCall("Value()");
		QVariant widthValue = widthCell->dynamicCall("Value()");
		QVariant heightValue = heightCell->dynamicCall("Value()");
		QVariant weightValue = weightCell->dynamicCall("Value()");

		double length = lengthValue.toDouble() * 0.254;
		double width = widthValue.toDouble() * 0.254;
		double height = heightValue.toDouble() * 0.254;
		double weight = weightValue.toDouble() * 0.45359237;

		lengths.append(length);
		widths.append(width);
		heights.append(height);
		weights.append(weight);
		qDebug() << "row:" << row
			<< "length:" << lengths[row]
			<< "width:" << widths[row]
			<< "height:" << heights[row]
			<< "weight:" << weights[row];
	}
	workbook->dynamicCall("Close()");
	excel.dynamicCall("Quit()");
	QScreen* screen = QGuiApplication::primaryScreen();
	QRect screenGeometry = screen->geometry();
	int screenWidth = screenGeometry.width();
	int screenHeight = screenGeometry.height();

	// 假设任务栏高度为40（你可以根据实际情况调整）
	int taskbarHeight = 0;
	int captureHeight = screenHeight - taskbarHeight;

	for (int row = 0; row <= rowCount - 2; row++)
	{
		if (lengths[row] < heights[row] || widths[row] < heights[row]) {
			QString txtFileName = QString("%1_H_biggerthan_W_or_L.txt").arg(row + 2);
			QString txtFilePath = QDir(directory).absoluteFilePath(folderName + "/" + txtFileName);
			QFile txtFile(txtFilePath);
			if (txtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				QTextStream out(&txtFile);
				out << "";  // 写入空内容
				txtFile.close();
				qDebug() << "Created file:" << txtFileName;
			}
			else {
				qDebug() << "Failed to create file:" << txtFileName;
			}
			continue;
		}
		if (stack_length == -1.0 || stack_width == -1.0 || height_limit == -1.0 || lengths[row] == -1.0 || widths[row] == -1.0 || heights[row] == -1.0 || out_ratio == -1.0) {
			QString txtFileName = QString("%1_Not_fullfill_all.txt").arg(row + 2);
			QString txtFilePath = QDir(directory).absoluteFilePath(folderName + "/" + txtFileName);
			QFile txtFile(txtFilePath);
			if (txtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				QTextStream out(&txtFile);
				out << "";  // 写入空内容
				txtFile.close();
				qDebug() << "Created file:" << txtFileName;
			}
			else {
				qDebug() << "Failed to create file:" << txtFileName;
			}
			continue;
		}
		if (lengths[row] > 16.5)
		{
			QString txtFileName = QString("%1_L_outof_limit.txt").arg(row + 2);
			QString txtFilePath = QDir(directory).absoluteFilePath(folderName + "/" + txtFileName);
			QFile txtFile(txtFilePath);
			if (txtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				QTextStream out(&txtFile);
				out << "";  // 写入空内容
				txtFile.close();
				qDebug() << "Created file:" << txtFileName;
			}
			else {
				qDebug() << "Failed to create file:" << txtFileName;
			}
			continue;
		}
		if (widths[row] > 9)
		{
			QString txtFileName = QString("%1_W_outof_limit.txt").arg(row + 2);
			QString txtFilePath = QDir(directory).absoluteFilePath(folderName + "/" + txtFileName);
			QFile txtFile(txtFilePath);
			if (txtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				QTextStream out(&txtFile);
				out << "";  // 写入空内容
				txtFile.close();
				qDebug() << "Created file:" << txtFileName;
			}
			else {
				qDebug() << "Failed to create file:" << txtFileName;
			}
			continue;
		}
		if (heights[row] > 7)
		{
			QString txtFileName = QString("%1_H_outof_limit.txt").arg(row + 2);
			QString txtFilePath = QDir(directory).absoluteFilePath(folderName + "/" + txtFileName);
			QFile txtFile(txtFilePath);
			if (txtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				QTextStream out(&txtFile);
				out << "";  // 写入空内容
				txtFile.close();
				qDebug() << "Created file:" << txtFileName;
			}
			else {
				qDebug() << "Failed to create file:" << txtFileName;
			}
			continue;
		}
		if (lengths[row] < 2.25)
		{
			QString txtFileName = QString("%1_L_too_small.txt").arg(row + 2);
			QString txtFilePath = QDir(directory).absoluteFilePath(folderName + "/" + txtFileName);
			QFile txtFile(txtFilePath);
			if (txtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				QTextStream out(&txtFile);
				out << "";  // 写入空内容
				txtFile.close();
				qDebug() << "Created file:" << txtFileName;
			}
			else {
				qDebug() << "Failed to create file:" << txtFileName;
			}
			continue;
		}
		if (widths[row] < 1.25)
		{
			QString txtFileName = QString("%1_W_too_small.txt").arg(row + 2);
			QString txtFilePath = QDir(directory).absoluteFilePath(folderName + "/" + txtFileName);
			QFile txtFile(txtFilePath);
			if (txtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				QTextStream out(&txtFile);
				out << "";  // 写入空内容
				txtFile.close();
				qDebug() << "Created file:" << txtFileName;
			}
			else {
				qDebug() << "Failed to create file:" << txtFileName;
			}
			continue;
		}
		if (heights[row] < 0.1)
		{
			QString txtFileName = QString("%1_H_too_small.txt").arg(row + 2);
			QString txtFilePath = QDir(directory).absoluteFilePath(folderName + "/" + txtFileName);
			QFile txtFile(txtFilePath);
			if (txtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				QTextStream out(&txtFile);
				out << "";  // 写入空内容
				txtFile.close();
				qDebug() << "Created file:" << txtFileName;
			}
			else {
				qDebug() << "Failed to create file:" << txtFileName;
			}
			continue;
		}
		if (weights[row] > 66)
		{
			QString txtFileName = QString("%1_Weight_outof_limit.txt").arg(row + 2);
			QString txtFilePath = QDir(directory).absoluteFilePath(folderName + "/" + txtFileName);
			QFile txtFile(txtFilePath);
			if (txtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				QTextStream out(&txtFile);
				out << "";  // 写入空内容
				txtFile.close();
				qDebug() << "Created file:" << txtFileName;
			}
			else {
				qDebug() << "Failed to create file:" << txtFileName;
			}
			continue;
		}

		if (weights[row] < 5)
		{
			QString txtFileName = QString("%1_Weight_too_small.txt").arg(row + 2);
			QString txtFilePath = QDir(directory).absoluteFilePath(folderName + "/" + txtFileName);
			QFile txtFile(txtFilePath);
			if (txtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				QTextStream out(&txtFile);
				out << "";  // 写入空内容
				txtFile.close();
				qDebug() << "Created file:" << txtFileName;
			}
			else {
				qDebug() << "Failed to create file:" << txtFileName;
			}
			continue;
		}

		total_weight = 0;
		/*
				QList<Qt3DCore::QEntity *> childEntities = rootEntity->findChildren<Qt3DCore::QEntity *>();
				for (Qt3DCore::QEntity *childEntity : childEntities) {
					delete childEntity;
				}
				ini_scene(rootEntity,view);
				createCube(rootEntity,stack_length,1,stack_width,0,-1,0,0);
				//createCube(rootEntity,stack_length*1.5,0.1,stack_width*1.5,-stack_length*0.5,height_limit,-stack_width*0.5);
				// 调用 createCube 函数生成长方体
				//createCube(goods_length, goods_width,goods_height,0, 0, 0);
				SingleSKUBpp myObject(stack_length, stack_width, height_limit, lengths[row], widths[row], heights[row],weights[row], out_ratio);
				QVector<SingleSKUBpp::LayoutResult> result = myObject.LayoutTotalAuto();
				createList(rootEntity,result,0);
				totalweightLineEdit->setText(QString::number(total_weight));
		*/
		onButtonClicked(rootEntity, view, 1, lengths[row], widths[row], heights[row], weights[row]);

		QEventLoop loop;
		QTimer::singleShot(1000, &loop, &QEventLoop::quit);
		loop.exec();

		QString screenshotFileName = QString("%1_TotalWeight_%2.png").arg(row + 2).arg(total_weight);
		QString screenshotFilePath = QDir(directory).absoluteFilePath(folderName + "/" + screenshotFileName);
		QString nircmdPath = "./nircmd.exe";
		QStringList arguments;
		arguments << "savescreenshot" << screenshotFilePath << "0" << "0" << QString::number(screenWidth) << QString::number(captureHeight);
		QProcess process;
		process.start(nircmdPath, arguments);

		if (!process.waitForFinished()) {
			qDebug() << "Failed to execute NirCmd:" << process.errorString();
			continue;
		}

		qDebug() << "Screenshot saved to:" << screenshotFilePath;

	}


}
void viewerBoxes3D::onLineEditEditingFinished(double& parameter, QLineEdit* lineEdit) {
	bool ok;
	double value = lineEdit->text().toDouble(&ok);
	if (ok) {
		parameter = value;
	}
	else {
		// Handle invalid input, you can choose to show a message box or take other actions
	}
}

void viewerBoxes3D::setSelectedData(QList<QStringList> result_list, QList<QStringList> box_list)
{

    if(result_list.size()<=0 || box_list.size()<=0)
    {
        QMessageBox::information(this,"提示","未查询到该sku信息!");
        return;
    }

    // 重新初始化界面
    QList<Qt3DCore::QEntity*> childEntities = rootEntity->findChildren<Qt3DCore::QEntity*>();
    for (Qt3DCore::QEntity* childEntity : childEntities) {
        delete childEntity;
    }

    ini_scene(rootEntity, view);
    createCube(rootEntity, stack_length, 1, stack_width, 0, -1, 0, 0);

    auto b_result = box_list[0];

    QString sku_code = b_result.at(0);
    double box_length = b_result.at(1).toDouble();
    double box_width = b_result.at(2).toDouble();
    double box_height = b_result.at(3).toDouble();
    double box_weight = b_result.at(4).toDouble();

    QVector<SingleSKUBpp::LayoutResult> layout_result;

    for(auto p_result : result_list)
    {
        SingleSKUBpp::LayoutResult l_r;

        QString sku_code = p_result.at(0);      
        double x = p_result.at(7).toDouble();
        double y = p_result.at(8).toDouble();
        double z = p_result.at(9).toDouble();
        int box_ori = p_result.at(10).toInt();     

        if(box_ori == 0)
        {
            l_r = {x,y,z,box_length,box_width};
        }
        else
        {
            l_r = {x,y,z,box_width,box_length};
        }
        qDebug()<<"layout_P = " << l_r.x<<l_r.y<<l_r.z<<l_r.x_len<<l_r.y_len;
        layout_result.append(l_r);

    }

    createList(rootEntity, layout_result, 0, box_weight, box_height);
    return;

}

viewerBoxes3D::viewerBoxes3D(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::viewerBoxes3D)
{
	ui->setupUi(this);
	// 1、创建3D场景视图窗口view
    view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
    // 2、创建放置3D场景视图窗口的容器，场景view需要先放在一个容器中
    QWidget* container = QWidget::createWindowContainer(view);
    QSize screenSize = view->screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(screenSize);
    // 3、创建一个主窗口Widget，进行适当布局操作

    ui->widget_3DViewer->setLayout(new QVBoxLayout());
    ui->widget_3DViewer->layout()->addWidget(container);


	// 给应用程序主窗口设置一个标题
	ui->widget_3DViewer->setWindowTitle(QStringLiteral("Basic shapes"));
	// 5、创建根实体（Root Entity）对象，即所谓的“画布”，并将其设置到3D场景视图中去
	// Root entity
    rootEntity = new Qt3DCore::QEntity();
	view->setRootEntity(rootEntity);


	ui->comboBox->addItem("horizontal_vertical");
	ui->comboBox->addItem("cross_crossmirror");
	ui->comboBox->addItem("rectangular_rectangularmirror");


	ui->createCubeButton->setText(QStringLiteral("输入数据")); // 按钮文本
	// ui->createCubeButton->setDescription(QString::fromLatin1("Please input stack's length,width,height, height limit and goods' length,width,height")); // 按钮描述
	ui->createCubeButton->setIconSize(QSize(0, 0));

    ui->readSqlButton->setText(QStringLiteral("读取SQL数据")); // 按钮文本
    ui->readSqlButton->setIconSize(QSize(0, 0));

	ui->readFileButton->setText(QStringLiteral("读取数据")); // 按钮文本
	//	ui->readFileButton->setDescription(QString::fromLatin1("Please input stack's length,width,height, height limit and then I will read goods' length,width,height from your selected xls/xlsx file")); // 按钮描述
	ui->readFileButton->setIconSize(QSize(0, 0));


	ui->lengthLineEdit->setText(QString::number(stack_length));
	ui->widthLineEdit->setText(QString::number(stack_width));
	ui->heightLineEdit->setText(QString::number(height_limit));
	ui->outRatioLineEdit->setText(QString::number(out_ratio));

	QObject::connect(ui->lengthLineEdit, &QLineEdit::editingFinished, [&]() {
		onLineEditEditingFinished(stack_length, ui->lengthLineEdit);
		});
	QObject::connect(ui->widthLineEdit, &QLineEdit::editingFinished, [&]() {
		onLineEditEditingFinished(stack_width, ui->widthLineEdit);
		});
	QObject::connect(ui->heightLineEdit, &QLineEdit::editingFinished, [&]() {
		onLineEditEditingFinished(height_limit, ui->heightLineEdit);
		});
	QObject::connect(ui->goods_length_LineEdit, &QLineEdit::editingFinished, [&]() {
		onLineEditEditingFinished(goods_length, ui->goods_length_LineEdit);
		});
	QObject::connect(ui->goods_width_LineEdit, &QLineEdit::editingFinished, [&]() {
		onLineEditEditingFinished(goods_width, ui->goods_width_LineEdit);
		});
	QObject::connect(ui->goods_height_LineEdit, &QLineEdit::editingFinished, [&]() {
		onLineEditEditingFinished(goods_height, ui->goods_height_LineEdit);
		});
	QObject::connect(ui->outRatioLineEdit, &QLineEdit::editingFinished, [&]() {
		onLineEditEditingFinished(out_ratio, ui->outRatioLineEdit);
		});
	QObject::connect(ui->weightLineEdit, &QLineEdit::editingFinished, [&]() {
		onLineEditEditingFinished(weight, ui->weightLineEdit);
		});
    QObject::connect(this,&viewerBoxes3D::showLayout,[=](QVector<SingleSKUBpp::LayoutResult> layout,double goods_weight,double goods_height){
        showLayOutResult(rootEntity, view,layout,goods_weight,goods_height);
    });

    key_edit_dialog = new sqlKeyEditDialog(this);
    connect(key_edit_dialog,&sqlKeyEditDialog::confirmSignal,[=](QStringList combo_info)
        {
        if(combo_info.size() != 6)
        {
            QMessageBox::warning(NULL, "提示！", "combo_info.size() != 6",
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::Yes);
            return;
        }
        QString str_skuCode = combo_info[0];
        QString str_TrayLength = combo_info[1];
        QString str_TrayWidth = combo_info[2];
        QString str_TrayMaxHeight = combo_info[3];
        QString str_TrayMaxWeight = combo_info[4];
        QString str_TrayMaxNumber = combo_info[5];

        if (str_skuCode.isEmpty() ||
            str_TrayLength.isEmpty() ||
            str_TrayWidth.isEmpty() ||
            str_TrayMaxHeight.isEmpty() ||
            str_TrayMaxWeight.isEmpty() ||
            str_TrayMaxNumber.isEmpty()) {
            QMessageBox::warning(NULL, "提示！", "请完成信息填写!",
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::Yes);
            return;
        }

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "确认", "请确认操作!",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            key_edit_dialog->close();
            QString Sku_Code = str_skuCode;
            int Tray_Length = str_TrayLength.toInt();
            int Tray_Width = str_TrayWidth.toInt();
            int Tray_MaxHeight = str_TrayMaxHeight.toInt();
            int Tray_MaxWeight = str_TrayMaxWeight.toInt();
            int Tray_MaxNumber = str_TrayMaxNumber.toInt();

            emit getDataSignal(Sku_Code,Tray_Length,Tray_Width,Tray_MaxHeight,Tray_MaxWeight,Tray_MaxNumber);
        }
    });
	// 连接按钮点击事件到槽函数
    connect(ui->readSqlButton,&QCommandLinkButton::clicked, [=]() {
        key_edit_dialog->clearForm();
        key_edit_dialog->exec();
    });

	QObject::connect(ui->createCubeButton, &QCommandLinkButton::clicked, [=]() {
		onButtonClicked(rootEntity, view, 0, 0, 0, 0, 0);
		});
	QObject::connect(ui->readFileButton, &QCommandLinkButton::clicked, [=]() {
        emit readSqlSignal();
		});

	QObject::connect(ui->pushButton_returnChooseMode, &QPushButton::clicked, this, &viewerBoxes3D::returnChooseMode);
	QObject::connect(ui->pushButton_returnHome, &QPushButton::clicked, this, &viewerBoxes3D::returnHome);
	this->setFixedSize(1200, 800);
}

void viewerBoxes3D::showLayOutResult(Qt3DCore::QEntity* rootEntity, Qt3DExtras::Qt3DWindow* view, QVector<SingleSKUBpp::LayoutResult> layout,double goods_weight,double goods_height)
{
    if(this->isHidden()==true)
    {return;}
    for(auto lay : layout) {
        qDebug()<<"lay.x="<<lay.x;
        qDebug()<<"lay.y="<<lay.y;
        qDebug()<<"lay.x_len="<<lay.x_len;
        qDebug()<<"lay.y_len="<<lay.y_len;
        qDebug()<<"lay.z="<<lay.z;
    }
    QList<Qt3DCore::QEntity*> childEntities = rootEntity->findChildren<Qt3DCore::QEntity*>();
    for (Qt3DCore::QEntity* childEntity : childEntities) {
        delete childEntity;
    }
    ini_scene(rootEntity, view);
    createCube(rootEntity, 12, 1, 10, 0, -1, 0, 0);
    createList(rootEntity, layout, 0, goods_weight, goods_height);
}
void viewerBoxes3D::onButtonClicked(Qt3DCore::QEntity* rootEntity, Qt3DExtras::Qt3DWindow* view, int model, double length, double width, double height, double gweight)
{
	QString comboBoxText = ui->comboBox->currentText();
	if (model == 0)
	{
		if (parameterCheck1() == false) { return; }
		length = goods_length;
		width = goods_width;
		height = goods_height;
		gweight = weight;
	}
	QList<Qt3DCore::QEntity*> childEntities = rootEntity->findChildren<Qt3DCore::QEntity*>();
	for (Qt3DCore::QEntity* childEntity : childEntities) {
		delete childEntity;
	}

    ini_scene(rootEntity, view);
	createCube(rootEntity, stack_length, 1, stack_width, 0, -1, 0, 0);
	//createCube(rootEntity,stack_length*1.5,0.1,stack_width*1.5,-stack_length*0.5,height_limit,-stack_width*0.5);
	// 调用 createCube 函数生成长方体
	//createCube(goods_length, goods_width,goods_height,0, 0, 0);
    double maxWeight = QString(ui->totalweightLineEdit->text()).toDouble();
    SingleSKUBpp myObject(stack_length, stack_width, height_limit,maxWeight, length, width, height, gweight, out_ratio);

	QVector<SingleSKUBpp::LayoutResult> result = myObject.LayoutTotalAuto();
    for (auto p_result:result)
    {
        qDebug()<<"p_result="<<p_result.x<<p_result.y<<p_result.z<<p_result.x_len<<p_result.y_len;
    }
	createList(rootEntity, result, 0, gweight, height);
    qDebug()<<"gweight="<<gweight;
    qDebug()<<"height="<<height;
	ui->totalweightLineEdit->setText(QString::number(total_weight));

	return;
}

viewerBoxes3D::~viewerBoxes3D()
{
	delete ui;
}
