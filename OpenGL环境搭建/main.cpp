//
//  main.cpp
//  OpenGL环境搭建
//
//  Created by 杨杨鹏 on 2020/8/28.
//  Copyright © 2020 杨杨鹏. All rights reserved.
//

// ------------------------ 三角形 -------------------------------------

#include "GLShaderManager.h"
/*
 `#include<GLShaderManager.h>` 移入了GLTool 着色器管理器（shader Mananger）类。没有着色器，我们就不能在OpenGL（核心框架）进行着色。着色器管理器不仅允许我们创建并管理着色器，还提供一组“存储着色器”，他们能够进行一些初步䄦基本的渲染操作。
 */

#include "GLTools.h"
/*
 `#include<GLTools.h>`  GLTool.h头文件包含了大部分GLTool中类似C语言的独立函数
*/

 
#include <GLUT/GLUT.h>
/*
 在Mac 系统下，`#include<glut/glut.h>`
 在Windows 和 Linux上，我们使用freeglut的静态库版本并且需要添加一个宏
*/

//定义一个，着色管理器
GLShaderManager shaderManager;

//简单的批次容器，是GLTools的一个简单的容器类。
GLBatch triangleBatch;

/*
 在窗口大小改变时，接收新的宽度&高度。
 */
void changeSize(int w,int h)
{
    /*
      x,y 参数代表窗口中视图的左下角坐标，而宽度、高度是像素为表示，通常x,y 都是为0
     */
    glViewport(0, 0, w, h);
    
}

void RenderScene(void)
{

    //1.清除一个或者一组特定的缓存区
    /*
     缓冲区是一块存在图像信息的储存空间，红色、绿色、蓝色和alpha分量通常一起分量通常一起作为颜色缓存区或像素缓存区引用。
     OpenGL 中不止一种缓冲区（颜色缓存区、深度缓存区和模板缓存区）
      清除缓存区对数值进行预置
     参数：指定将要清除的缓存的
     GL_COLOR_BUFFER_BIT :指示当前激活的用来进行颜色写入缓冲区
     GL_DEPTH_BUFFER_BIT :指示深度缓存区
     GL_STENCIL_BUFFER_BIT:指示模板缓冲区
     */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    
    
    //2.设置一组浮点数来表示红色
    GLfloat vRed[] = {1.0,0.0,0.0,1.0f};
    
    //传递到存储着色器，即GLT_SHADER_IDENTITY着色器，这个着色器只是使用指定颜色以默认笛卡尔坐标第在屏幕上渲染几何图形
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vRed);
    
    //提交着色器
    triangleBatch.Draw();
    
    //在开始的设置openGL 窗口的时候，我们指定要一个双缓冲区的渲染环境。这就意味着将在后台缓冲区进行渲染，渲染结束后交换给前台。这种方式可以防止观察者看到可能伴随着动画帧与动画帧之间的闪烁的渲染过程。缓冲区交换平台将以平台特定的方式进行。
    //将后台缓冲区进行渲染，然后结束后交换给前台
    glutSwapBuffers();
    
}

void setupRC()
{
    //设置清屏颜色（背景颜色）
    glClearColor(0.98f, 0.40f, 0.7f, 1);
    
    
    //没有着色器，在OpenGL 核心框架中是无法进行任何渲染的。初始化一个渲染管理器。
    //在前面的课程，我们会采用固管线渲染，后面会学着用OpenGL着色语言来写着色器
    shaderManager.InitializeStockShaders();
    
    
    //指定顶点
    //在OpenGL中，三角形是一种基本的3D图元绘图原素。
    GLfloat vVerts[] = {
        -0.5f,0.0f,0.0f,
        0.5f,0.0f,0.0f,
        0.0f,0.5f,0.0f
    };
    
    triangleBatch.Begin(GL_TRIANGLES, 3);
    triangleBatch.CopyVertexData3f(vVerts);
    triangleBatch.End();
    
}

int main(int argc,char *argv[])
{

    //初始化GLUT库,这个函数只是传说命令参数并且初始化glut库
    glutInit(&argc, argv);
    
    /*
     初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区
     
     --GLUT_DOUBLE`：双缓存窗口，是指绘图命令实际上是离屏缓存区执行的，然后迅速转换成窗口视图，这种方式，经常用来生成动画效果；
     --GLUT_DEPTH`：标志将一个深度缓存区分配为显示的一部分，因此我们能够执行深度测试；
     --GLUT_STENCIL`：确保我们也会有一个可用的模板缓存区。
     深度、模板测试后面会细致讲到
     */
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
    
    //GLUT窗口大小、窗口标题
    glutInitWindowSize(800, 600);
    glutCreateWindow("Triangle");
    
    /*
     GLUT 内部运行一个本地消息循环，拦截适当的消息。然后调用我们不同时间注册的回调函数。我们一共注册2个回调函数：
     1）为窗口改变大小而设置的一个回调函数
     2）包含OpenGL 渲染的回调函数
     */
    //注册重塑函数
    glutReshapeFunc(changeSize);
    //注册显示函数
    glutDisplayFunc(RenderScene);

    /*
     初始化一个GLEW库,确保OpenGL API对程序完全可用。
     在试图做任何渲染之前，要检查确定驱动程序的初始化过程中没有任何问题
     */
    GLenum status = glewInit();
    if (GLEW_OK != status) {
        
        printf("GLEW Error:%s\n",glewGetErrorString(status));
        return 1;
        
    }
    
    //设置我们的渲染环境
    setupRC();
    
    glutMainLoop();
    
    return  0;
    
}

// ------------------------ 可移动正方形 -------------------------------------

////定义一个，着色管理器
//GLShaderManager shaderManager;
//
////简单的批次容器，是GLTools的一个简单的容器类。
//GLBatch triangleBatch;
//
//
////blockSize 边长
//GLfloat blockSize = 0.1f;
//
////正方形的4个点坐标
//GLfloat vVerts[] = {
//        -blockSize,-blockSize,0.0f,
//        blockSize,-blockSize,0.0f,
//        blockSize,blockSize,0.0f,
//        -blockSize,blockSize,0.0f
//};
//
//
//
///*
// 在窗口大小改变时，接收新的宽度&高度。
// */
//void changeSize(int w,int h)
//{
//    /*
//      x,y 参数代表窗口中视图的左下角坐标，而宽度、高度是像素为表示，通常x,y 都是为0
//     */
//    glViewport(0, 0, w, h);
//
//}
//
//void RenderScene(void)
//{
//
//    //1.清除一个或者一组特定的缓存区
//    /*
//     缓冲区是一块存在图像信息的储存空间，红色、绿色、蓝色和alpha分量通常一起分量通常一起作为颜色缓存区或像素缓存区引用。
//     OpenGL 中不止一种缓冲区（颜色缓存区、深度缓存区和模板缓存区）
//      清除缓存区对数值进行预置
//     参数：指定将要清除的缓存的
//     GL_COLOR_BUFFER_BIT :指示当前激活的用来进行颜色写入缓冲区
//     GL_DEPTH_BUFFER_BIT :指示深度缓存区
//     GL_STENCIL_BUFFER_BIT:指示模板缓冲区
//     */
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
//
//
//    //2.设置一组浮点数来表示红色
//    GLfloat vRed[] = {1.0,0.0,0.0,1.0f};
//
//    //传递到存储着色器，即GLT_SHADER_IDENTITY着色器，这个着色器只是使用指定颜色以默认笛卡尔坐标第在屏幕上渲染几何图形
//    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vRed);
//
//    //提交着色器
//    triangleBatch.Draw();
//
//    //在开始的设置openGL 窗口的时候，我们指定要一个双缓冲区的渲染环境。这就意味着将在后台缓冲区进行渲染，渲染结束后交换给前台。这种方式可以防止观察者看到可能伴随着动画帧与动画帧之间的闪烁的渲染过程。缓冲区交换平台将以平台特定的方式进行。
//    //将后台缓冲区进行渲染，然后结束后交换给前台
//    glutSwapBuffers();
//
//}
//
//void setupRC()
//{
//    //设置清屏颜色（背景颜色）
//    glClearColor(0.98f, 0.40f, 0.7f, 1);
//
//
//    //没有着色器，在OpenGL 核心框架中是无法进行任何渲染的。初始化一个渲染管理器。
//    //在前面的课程，我们会采用固管线渲染，后面会学着用OpenGL着色语言来写着色器
//    shaderManager.InitializeStockShaders();
//
//
////    //指定顶点
////    //在OpenGL中，三角形是一种基本的3D图元绘图原素。
////    GLfloat vVerts[] = {
////        -0.5f,0.0f,0.0f,
////        0.5f,0.0f,0.0f,
////        0.0f,0.5f,0.0f
////    };
//
//    //修改为GL_TRIANGLE_FAN ，4个顶点
//    triangleBatch.Begin(GL_TRIANGLE_FAN, 4);
//    triangleBatch.CopyVertexData3f(vVerts);
//    triangleBatch.End();
//
//}
//
//void SpecialKeys(int key, int x, int y){
//
//    GLfloat stepSize = 0.1f;
//
//    GLfloat blockX = vVerts[0];
//    GLfloat blockY = vVerts[10];
//
//    printf("v[0] = %f\n",blockX);
//    printf("v[10] = %f\n",blockY);
//
//
//    if (key == GLUT_KEY_UP) {
//
//        blockY += stepSize;
//    }
//
//    if (key == GLUT_KEY_DOWN) {
//
//        blockY -= stepSize;
//    }
//
//    if (key == GLUT_KEY_LEFT) {
//        blockX -= stepSize;
//    }
//
//    if (key == GLUT_KEY_RIGHT) {
//        blockX += stepSize;
//    }
//
//    //触碰到边界（4个边界）的处理
//
//    //当正方形移动超过最左边的时候
//    if (blockX < -1.0f) {
//        blockX = -1.0f;
//    }
//
//    //当正方形移动到最右边时
//    //1.0 - blockSize * 2 = 总边长 - 正方形的边长 = 最左边点的位置
//    if (blockX > (1.0 - blockSize * 2)) {
//        blockX = 1.0f - blockSize * 2;
//    }
//
//    //当正方形移动到最下面时
//    //-1.0 - blockSize * 2 = Y（负轴边界） - 正方形边长 = 最下面点的位置
//    if (blockY < -1.0f + blockSize * 2 ) {
//
//        blockY = -1.0f + blockSize * 2;
//    }
//
//    //当正方形移动到最上面时
//    if (blockY > 1.0f) {
//
//        blockY = 1.0f;
//
//    }
//
//    printf("blockX = %f\n",blockX);
//    printf("blockY = %f\n",blockY);
//
//    // Recalculate vertex positions
//    vVerts[0] = blockX;
//    vVerts[1] = blockY - blockSize*2;
//
//    printf("(vVerts[0] == %f,vVerts[1] == %f,vVerts[2] == %f)\n",vVerts[0],vVerts[1],vVerts[2]);
//
//
//    vVerts[3] = blockX + blockSize*2;
//    vVerts[4] = blockY - blockSize*2;
//    printf("(vVerts[3] == %f,vVerts[4] == %f,vVerts[5] == %f)\n",vVerts[3],vVerts[4],vVerts[5]);
//
//    vVerts[6] = blockX + blockSize*2;
//    vVerts[7] = blockY;
//    printf("(vVerts[6] == %f,vVerts[7] == %f,vVerts[8] == %f)\n",vVerts[6],vVerts[7],vVerts[8]);
//
//    vVerts[9] = blockX;
//    vVerts[10] = blockY;
//    printf("(vVerts[9] == %f,vVerts[10] == %f)\n",vVerts[9],vVerts[10]);
//
//    triangleBatch.CopyVertexData3f(vVerts);
//
//    glutPostRedisplay();
//}
//
//int main(int argc,char *argv[])
//{
//    //设置当前工作目录，针对MAC OS X
//    /*
//     `GLTools`函数`glSetWorkingDrectory`用来设置当前工作目录。实际上在Windows中是不必要的，因为工作目录默认就是与程序可执行执行程序相同的目录。但是在Mac OS X中，这个程序将当前工作文件夹改为应用程序捆绑包中的`/Resource`文件夹。`GLUT`的优先设定自动进行了这个中设置，但是这样中方法更加安全。
//     */
//    gltSetWorkingDirectory(argv[0]);
//
//
//    //初始化GLUT库,这个函数只是传说命令参数并且初始化glut库
//    glutInit(&argc, argv);
//
//    /*
//     初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
//     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区
//
//     --GLUT_DOUBLE`：双缓存窗口，是指绘图命令实际上是离屏缓存区执行的，然后迅速转换成窗口视图，这种方式，经常用来生成动画效果；
//     --GLUT_DEPTH`：标志将一个深度缓存区分配为显示的一部分，因此我们能够执行深度测试；
//     --GLUT_STENCIL`：确保我们也会有一个可用的模板缓存区。
//     深度、模板测试后面会细致讲到
//     */
//    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
//
//    //GLUT窗口大小、窗口标题
//    glutInitWindowSize(800, 600);
//    glutCreateWindow("Triangle");
//
//    /*
//     GLUT 内部运行一个本地消息循环，拦截适当的消息。然后调用我们不同时间注册的回调函数。我们一共注册2个回调函数：
//     1）为窗口改变大小而设置的一个回调函数
//     2）包含OpenGL 渲染的回调函数
//     */
//    //注册重塑函数
//    glutReshapeFunc(changeSize);
//    //注册显示函数
//    glutDisplayFunc(RenderScene);
//
//    //注册特殊函数
//   glutSpecialFunc(SpecialKeys);
//
//
//
//    /*
//     初始化一个GLEW库,确保OpenGL API对程序完全可用。
//     在试图做任何渲染之前，要检查确定驱动程序的初始化过程中没有任何问题
//     */
//    GLenum status = glewInit();
//    if (GLEW_OK != status) {
//
//        printf("GLEW Error:%s\n",glewGetErrorString(status));
//        return 1;
//
//    }
//
//    //设置我们的渲染环境
//    setupRC();
//
//    glutMainLoop();
//
//
//
//
//    return  0;
//
//}


//void draw()
//{
////----------------------画圆---------------------------------
//    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    //设置颜色
//    glColor3f(1.0f, 0.0f, 0.0f);
//
//    //开始渲染
//    glBegin(GL_POLYGON);
//
//    const int n = 55;//当n为3时为三角形；n为4时是四边形，n为5时为五边形。。。。。
//    const GLfloat R = 0.5f;//圆的半径
//    const GLfloat pi = 3.1415926f;
//    for (int i = 0; i < n; i++)
//    {
//        glVertex2f(R*cos(2 * pi / n*i), R*sin(2 * pi / n*i));
//    }
//    //结束渲染
//    glEnd();
//
//    //强制刷新缓存区，保证绘制命令得以执行
//    glFlush();
//
//
//
//
//
//
////// ------------------------五角形-----------------------------
////
/////*
//// 设五角星的五个顶点分布位置关系如下：
//// A
//// E       B
////
//// D   C
//// 首先，根据余弦定理列方程，计算五角星的中心到顶点的距离a
//// （假设五角星对应正五边形的边长为.0）
//// a = 1 / (2-2*cos(72*Pi/180));
//// 然后，根据正弦和余弦的定义，计算B的x坐标bx和y坐标by，以及C的y坐标
//// （假设五角星的中心在坐标原点）
//// bx = a * cos(18 * Pi/180);
//// by = a * sin(18 * Pi/180);
//// cy = -a * cos(18 * Pi/180);
//// 五个点的坐标就可以通过以上四个量和一些常数简单的表示出来
//// */
//    const GLfloat Pi = 3.1415926536f;
//     GLfloat a = 1 / (2-2*cos(72*Pi/180));
//     GLfloat bx = a * cos(18 * Pi/180);
//     GLfloat by = a * sin(18 * Pi/180);
//     GLfloat cy = -a * cos(18 * Pi/180);
//     GLfloat
//     PointA[2] = { 0, a },
//     PointB[2] = { bx, by },
//     PointC[2] = { 0.5, cy },
//     PointD[2] = { -0.5, cy },
//     PointE[2] = { -bx, by };
//
//     glClear(GL_COLOR_BUFFER_BIT);
//     // 按照A->C->E->B->D->A的顺序，可以一笔将五角星画出
//     glBegin(GL_LINE_LOOP);
//     glVertex2fv(PointA);
//     glVertex2fv(PointC);
//     glVertex2fv(PointE);
//     glVertex2fv(PointB);
//     glVertex2fv(PointD);
//     glEnd();
//     glFlush();
//
////----------- 画出正弦函数的图形---------------
//    /*
//     由于OpenGL默认坐标值只能从-1到1，（可以修改，但方法留到以后讲）
//     所以我们设置一个因子factor，把所有的坐标值等比例缩小，
//     这样就可以画出更多个正弦周期
//     试修改factor的值，观察变化情况
//     */
////    const GLfloat factor = 0.1f;
////    GLfloat x;
////    glClear(GL_COLOR_BUFFER_BIT);
////    glBegin(GL_LINES);
////    glVertex2f(-1.0f, 0.0f);
////    glVertex2f(1.0f, 0.0f);        // 以上两个点可以画x轴
////    glVertex2f(0.0f, -1.0f);
////    glVertex2f(0.0f, 1.0f);        // 以上两个点可以画y轴
////    glEnd();
////    glBegin(GL_LINE_STRIP);
////    for(x=-1.0f/factor; x<1.0f/factor; x+=0.01f)
////    {
////        glVertex2f(x*factor, sin(x)*factor);
////    }
////    glEnd();
////    glFlush();
//
//}
//
//int main(int argc,const char *argv[])
//{
//
//    //1.初始化一个GLUT库
//    glutInit(&argc, (char **)argv);
//
//    //2.创建一个窗口并制定窗口名
//    glutCreateWindow("CC_Window");
//
//    //3.注册一个绘图函数，操作系统在必要时刻就会对窗体进行重绘制操作
//    //它设置了一个显示回调（diplay callback），即GLUT在每次更新窗口内容的时候回自动调用该例程
//    glutDisplayFunc(draw);
//
//    //这是一个无限执行的循环，它会负责一直处理窗口和操作系统的用户输入等操作。（注意：不会执行在glutMainLoop()之后的所有命令。）
//    glutMainLoop();
//
//    return 0;
//}
