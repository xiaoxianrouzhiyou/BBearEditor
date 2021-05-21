#include "BBUIMainWindow.h"
#include "ui_BBUIMainWindow.h"
#include "BBUtils.h"

BBUIMainWindow::BBUIMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_pUi(new Ui::BBUIMainWindow)
{
    m_pUi->setupUi(this);
    setWindowLayout();
    setGameObjectDockWidget();
    setConnect();
}

BBUIMainWindow::~BBUIMainWindow()
{
    BB_SAFE_DELETE(m_pUi);
}

void BBUIMainWindow::setWindowLayout()
{
    // Allow nesting of docks
    setDockNestingEnabled(true);

    // dock layout
    addDockWidget(Qt::LeftDockWidgetArea, m_pUi->dockGameObject);
    splitDockWidget(m_pUi->dockGameObject, m_pUi->dockEditview, Qt::Horizontal);
    splitDockWidget(m_pUi->dockEditview, m_pUi->dockInspector, Qt::Horizontal);
    splitDockWidget(m_pUi->dockGameObject, m_pUi->dockHierarchy, Qt::Vertical);
    splitDockWidget(m_pUi->dockEditview, m_pUi->dockProject, Qt::Vertical);
//    tabifyDockWidget(m_pUi->dockPreview, m_pUi->dockEditview);
//    tabifyDockWidget(m_pUi->dockConsole, m_pUi->dockProject);
//    splitDockWidget(m_pUi->dockProject, m_pUi->dockConsole, Qt::Horizontal);
//    m_pUi->dockPreview->setVisible(false);
//    m_pUi->dockConsole->setVisible(false);

    // resize
    m_pUi->dockGameObjectContents->updateSizeHint(QSize(350, 150));
    m_pUi->dockInspectorContents->updateSizeHint(QSize(300, 500));
    m_pUi->dockProjectContents->updateSizeHint(QSize(300, 250));

    // Stretch the width of the file list in the project view
    m_pUi->splitterProject->setStretchFactor(1, 1);
}

void BBUIMainWindow::setGameObjectDockWidget()
{
    // Load contents for list
    m_pUi->listBaseGameObject->loadListItems(BB_PATH_BASEOBJECTLIST);
    m_pUi->listBaseGameObject->setMimeType(BB_MIMETYPE_BASEOBJECT);
}

void BBUIMainWindow::setConnect()
{
    // The handling of key events in the EditView in OpenGL
    QObject::connect(m_pUi->dockEditview, SIGNAL(pressMoveKeySignal(char)),
                     m_pUi->openGLWidget, SLOT(pressMoveKeySlot(char)));
    QObject::connect(m_pUi->dockEditview, SIGNAL(releaseMoveKeySignal(char)),
                     m_pUi->openGLWidget, SLOT(releaseMoveKeySlot(char)));
    QObject::connect(m_pUi->dockEditview, SIGNAL(pressTransformSignal(char)),
                     m_pUi->openGLWidget, SLOT(pressTransformSlot(char)));
    QObject::connect(m_pUi->dockEditview, SIGNAL(pressESCSignal()), m_pUi->openGLWidget, SLOT(pressESCSlot()));
    QObject::connect(m_pUi->dockEditview, SIGNAL(keyPress(QKeyEvent*)), m_pUi->openGLWidget, SLOT(onKeyPress(QKeyEvent*)));
//    QObject::connect(this, SIGNAL(sceneMultipleSelectKey(bool)), ui->openGLWidget, SLOT(multipleSelectKey(bool)));
    // Drag into the model, and add item in the Hierarchy tree
    QObject::connect(m_pUi->openGLWidget, SIGNAL(addGameObject(BBGameObject*)),
                     m_pUi->treeHierarchy, SLOT(addGameObject(BBGameObject*)));
    // select item in treeHierarchy, use coordinate system to select object in OpenGL view
    QObject::connect(m_pUi->treeHierarchy, SIGNAL(setCoordinateSystemSelectedObject(BBGameObject*)),
                     m_pUi->openGLWidget, SLOT(setCoordinateSystemSelectedObject(BBGameObject*)));
    // pick object in OpenGL view, select corresponding item in treeHierarchy
    QObject::connect(m_pUi->openGLWidget, SIGNAL(pickObject(BBGameObject*)),
                     m_pUi->treeHierarchy, SLOT(selectPickedObject(BBGameObject*)));
    QObject::connect(m_pUi->openGLWidget, SIGNAL(pickObjects(QList<BBGameObject*>)),
                     m_pUi->treeHierarchy, SLOT(selectPickedObjects(QList<BBGameObject*>)));


//    //点击层级视图对象 在属性栏中显示属性
//    QObject::connect(ui->treeHierarchy, SIGNAL(showGameObjectProperty(GameObject*)),
//                     ui->propertyManager, SLOT(showHierarchyTreeItemProperty(GameObject*)));
//    //对整个项目操作的菜单事件
//    QObject::connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));

//    //更换坐标系的模式
//    QObject::connect(this, SIGNAL(pressTransformSignal(char)),
//                     ui->openGLWidget, SLOT(pressTransformSlot(char)));

//    //双击item 摄像机以其对应的gameobject为焦点
//    QObject::connect(ui->treeHierarchy, SIGNAL(lookAtGameObjectSignal(GameObject*)),
//                     ui->openGLWidget, SLOT(lookAtGameObjectSlot(GameObject*)));
//    //操作层级视图 删除场景中的对象
//    QObject::connect(ui->treeHierarchy, SIGNAL(deleteSceneGameObject(GameObject*)),
//                     ui->openGLWidget, SLOT(deleteSceneGameObject(GameObject*)));
//    //点击文件夹控件上的按钮
//    QObject::connect(ui->buttonRootProject, SIGNAL(clicked()), ui->treeProject, SLOT(pressRootButton()));
//    QObject::connect(ui->buttonMoreProject, SIGNAL(clicked()), ui->treeProject, SLOT(pressSettingButton()));
//    //文件夹与文件视图
//    QObject::connect(ui->treeProject, SIGNAL(showFolderContent(QString)),
//                     ui->listFile, SLOT(showFolderContent(QString)));

//    //对场景中的对象进行transform操作 属性栏相应值改变
//    QObject::connect(ui->openGLWidget, SIGNAL(updateTransform(GameObject*, char)),
//                     ui->propertyManager, SLOT(updateTransform(GameObject*, char)));
//    //修改属性栏transform 场景中对应坐标系也要进行位置旋转的修改
//    QObject::connect(ui->propertyManager, SIGNAL(updateCoordinateSignal()),
//                     ui->openGLWidget, SLOT(updateCoordinate()));
//    //文件视图改变 需要刷新文件夹树
//    QObject::connect(ui->listFile, SIGNAL(updateProjectTree()), ui->treeProject, SLOT(loadProject()));
//    //显示文件夹的内容 上方显示文件夹的路径
//    QObject::connect(ui->listFile, SIGNAL(showFolderPath(QString)),
//                     ui->filePathWidget, SLOT(showCurrentFolderPath(QString)));
//    //点击文件夹路径内的一项 跳转显示该文件夹
//    QObject::connect(ui->filePathWidget, SIGNAL(showFolderContent(QString)),
//                     ui->listFile, SLOT(showFolderContent(QString)));
//    //文件列表跳转之后 文件夹树的currentShowFolderContentItem需要更新
//    QObject::connect(ui->listFile, SIGNAL(updateCurrentShowFolderContentItem(QString)),
//                     ui->treeProject, SLOT(updateCurrentShowFolderContentItem(QString)));
//    //在文件夹树中修改文件夹名字 当前文件夹路径更新 至文件列表
//    QObject::connect(ui->treeProject, SIGNAL(updateFolderName(QString, QString)),
//                     ui->listFile, SLOT(updateFolderName(QString, QString)));
//    //在文件列表中修改文件夹名字 树中相应项更新名字
//    QObject::connect(ui->listFile, SIGNAL(updateFolderNameInvert(QString, QString)),
//                     ui->treeProject, SLOT(updateFolderNameInvert(QString, QString)));
//    //在文件列表中创建文件夹 文件夹树添加相应项
//    QObject::connect(ui->listFile, SIGNAL(addItemInProjectTree(QString, QString)),
//                     ui->treeProject, SLOT(addFolderItemInTree(QString, QString)));
//    //在文件列表中删除文件夹 文件夹树相应项删去
//    QObject::connect(ui->listFile, SIGNAL(deleteItemInProjectTree(QString)),
//                     ui->treeProject, SLOT(deleteFolderItemInTree(QString)));
//    //在文件列表中移动文件夹 文件夹树相应项移动
//    QObject::connect(ui->listFile, SIGNAL(moveItemInProjectTree(QString, QString)),
//                     ui->treeProject, SLOT(moveFolderItemInTree(QString, QString)));
//    //从文件列表移动文件到文件夹树 删去移走项
//    QObject::connect(ui->treeProject, SIGNAL(deleteFileItemInList()), ui->listFile, SLOT(deleteItem()));
//    //文件列表菜单按钮 弹出菜单
//    QObject::connect(ui->buttonSettingFile, SIGNAL(clicked()), ui->listFile, SLOT(popupMenu()));
//    //主菜单的菜单项 根据控件的焦点调用不同控件的菜单项
//    QObject::connect(ui->actionCopy, SIGNAL(triggered()), this, SLOT(actionCopyByFocus()));
//    QObject::connect(ui->actionPaste, SIGNAL(triggered()), this, SLOT(actionPasteByFocus()));
//    QObject::connect(ui->actionDelete, SIGNAL(triggered()), this, SLOT(actionDeleteByFocus()));
//    //在文件列表中粘贴文件夹 文件夹树相应结点也要粘贴
//    QObject::connect(ui->listFile, SIGNAL(pasteItemInProjectTree(QList<QString>)),
//                     ui->treeProject, SLOT(pasteItemWithoutPasteFile(QList<QString>)));
//    //在文件夹树中复制 文件列表的剪贴板也改变
//    QObject::connect(ui->treeProject, SIGNAL(copyToFileList(QList<QString>)),
//                     ui->listFile, SLOT(copyByProjectTree(QList<QString>)));
//    //在文件列表中复制 文件夹树的剪贴板也改变
//    QObject::connect(ui->listFile, SIGNAL(copyToProjectTree(QList<QString>)),
//                     ui->treeProject, SLOT(copyByFileList(QList<QString>)));
//    //在文件夹树中粘贴 剪贴板上的非文件夹文件交给文件列表处理
//    QObject::connect(ui->treeProject, SIGNAL(pasteFile(QList<QString>, QString, QList<QString>)),
//                     ui->listFile, SLOT(pasteFileFromProjectTree(QList<QString>, QString, QList<QString>)));
//    //在文件列表中重命名 文件夹树对应的结点如果在剪贴板中 需要移除
//    QObject::connect(ui->listFile, SIGNAL(removeRenameItemInTreeClipBoard(QString)),
//                     ui->treeProject, SLOT(removeClipBoardRenameItem(QString)));
//    //属性栏根按钮切换
//    QObject::connect(ui->buttonRootInspector, SIGNAL(clicked()), this, SLOT(switchInspectorRootButton()));
//    //在层级视图中重命名 属性栏对应更新名字
//    QObject::connect(ui->treeHierarchy, SIGNAL(updateNameInInspector(GameObject*, QString)),
//                     ui->propertyManager, SLOT(renameGameObject(GameObject*, QString)));
//    //在属性栏更新名字 层级视图对应树节点重命名
//    QObject::connect(ui->propertyManager, SIGNAL(renameGameObjectInHierarchySignal(GameObject*)),
//                     ui->treeHierarchy, SLOT(renameItemName(GameObject*)));
//    //在属性栏勾选激活性 层级视图做相应处理
//    QObject::connect(ui->propertyManager, SIGNAL(changeActivationSignal(GameObject*, bool)),
//                     ui->treeHierarchy, SLOT(changeGameObjectActivation(GameObject*, bool)));
//    //层级视图修改激活性 属性栏对应图标更新
//    QObject::connect(ui->treeHierarchy, SIGNAL(changeButtonActiveCheckStateInInspector(GameObject*, bool)),
//                     ui->propertyManager, SLOT(changeButtonActiveCheckState(GameObject*, bool)));
//    //文件或预制体拖入层级视图 在场景中创建模型
//    QObject::connect(ui->treeHierarchy, SIGNAL(createModel(QString)),
//                     ui->openGLWidget, SLOT(createModelDependParent(QString)));
//    //灯光拖入层级视图 在场景中创建灯光
//    QObject::connect(ui->treeHierarchy, SIGNAL(createLight(QString)),
//                     ui->openGLWidget, SLOT(createLightDependParent(QString)));
//    //在层级视图中拷贝树节点 场景中拷贝对象并与拷贝的结点对应起来
//    QObject::connect(ui->treeHierarchy, SIGNAL(copyGameObject(GameObject*, QTreeWidgetItem*, QVector3D)),
//                     ui->openGLWidget, SLOT(copyGameObject(GameObject*, QTreeWidgetItem*, QVector3D)));
//    //在场景中拷贝的对象 需要将层级视图树节点和拷贝的对象插入层级视图映射中
//    QObject::connect(ui->openGLWidget, SIGNAL(copyGameObjectInsertMap(QTreeWidgetItem*, GameObject*)),
//                     ui->treeHierarchy, SLOT(copyGameObjectInsertMap(QTreeWidgetItem*, GameObject*)));
//    //在层级视图中选择多项 场景中坐标系选择多项的重心
//    QObject::connect(ui->treeHierarchy, SIGNAL(setCoordinateSelectedObjects(QList<GameObject*>, CenterPoint*)),
//                     ui->openGLWidget, SLOT(setCoordinateSelectedObjects(QList<GameObject*>, CenterPoint*)));
//    //在层级视图中选择多项 属性栏显示集合的属性
//    QObject::connect(ui->treeHierarchy, SIGNAL(showSetProperty(QList<GameObject*>, CenterPoint*)),
//                     ui->propertyManager, SLOT(showHierarchyTreeItemsProperty(QList<GameObject*>, CenterPoint*)));
//    //在集合的属性栏中修改可见性 最后 重新选中所有对象 否则会选中最后一项
//    QObject::connect(ui->propertyManager, SIGNAL(setSelectedObjects(QList<GameObject*>)),
//                     ui->treeHierarchy, SLOT(selectedObjects(QList<GameObject*>)));
//    //在场景中按下shift或者cmd进行多选 层级视图也进行多选
//    QObject::connect(ui->openGLWidget, SIGNAL(updateMultipleSelectObjects(GameObject*)),
//                     ui->treeHierarchy, SLOT(updateMultipleSelectObjects(GameObject*)));
//    //在场景中框选的对象 在层级视图中选中
//    QObject::connect(ui->openGLWidget, SIGNAL(regionSelectObjects(QList<GameObject*>)),
//                     ui->treeHierarchy, SLOT(selectedObjects(QList<GameObject*>)));
//    //文件列表中选择 属性栏需要显示文件的属性 层级视图 场景 去掉选中项
//    QObject::connect(ui->listFile, SIGNAL(cancelHierarchyTreeSelectedItems()),
//                     ui->treeHierarchy, SLOT(cancelSelectedItems()));
//    //层级视图 场景 去掉选中项
//    QObject::connect(ui->treeProject, SIGNAL(cancelHierarchyTreeSelectedItems()),
//                     ui->treeHierarchy, SLOT(cancelSelectedItems()));
//    //选中场景 或者 选中层级视图 或者拖预制体 文件列表去掉选中项
//    QObject::connect(ui->dockEditview, SIGNAL(cancelFileListSelectedItems()),
//                     ui->listFile, SLOT(cancelSelectedItems()));
//    QObject::connect(ui->treeHierarchy, SIGNAL(cancelFileListSelectedItems()),
//                     ui->listFile, SLOT(cancelSelectedItems()));
//    QObject::connect(ui->listBaseGameObject, SIGNAL(cancelFileListSelectedItems()),
//                     ui->listFile, SLOT(cancelSelectedItems()));
//    QObject::connect(ui->listLightGameObject, SIGNAL(cancelFileListSelectedItems()),
//                     ui->listFile, SLOT(cancelSelectedItems()));
//    //选中文件 在属性栏中显示文件的属性
//    QObject::connect(ui->listFile, SIGNAL(showMaterialProperty(QString)),
//                     ui->propertyManager, SLOT(showMaterialProperty(QString)));
//    QObject::connect(ui->listFile, SIGNAL(showFbxProperty(QString)),
//                     ui->propertyManager, SLOT(showFbxProperty(QString)));
//    //清空属性栏
//    QObject::connect(ui->listFile, SIGNAL(clearPropertyWidget()), ui->propertyManager, SLOT(clear()));
//    QObject::connect(ui->treeProject, SIGNAL(clearPropertyWidget()), ui->propertyManager, SLOT(clear()));
//    //修改材质 文件列表的材质文件图标改变
//    QObject::connect(ui->propertyManager, SIGNAL(updateMaterialPreviewInOtherWidgetSignal(QString)),
//                     ui->listFile, SLOT(updateMaterialFileIcon(QString)));
//    //拖动材质 到场景中的对象 属性栏 对象的材质属性要更新
//    QObject::connect(ui->openGLWidget, SIGNAL(updateMaterialProperty(Model*)),
//                     ui->propertyManager, SLOT(updateMaterialFactory(Model*)));
//    //点击场景根按钮 属性栏显示场景属性
//    QObject::connect(ui->buttonRootHierarchy, SIGNAL(clicked()), this, SLOT(clickedButtonRootHierarchy()));

}
